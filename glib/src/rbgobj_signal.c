/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_signal.c -

  $Author: mutoh $
  $Date: 2003/06/26 15:14:47 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE cSignal;
VALUE rbgobj_signal_wrap(guint sig_id);

/**********************************************************************/

static VALUE signal_func_table;

void
rbgobj_set_signal_func(klass, sig_name, func)
    VALUE klass;
    gchar* sig_name; 
    GValToRValSignalFunc func;
{
    VALUE obj = Data_Wrap_Struct(rb_cData, NULL, NULL, func);
    guint signal_id = g_signal_lookup(sig_name, CLASS2GTYPE(klass));
    rb_hash_aset(signal_func_table, UINT2NUM(signal_id), obj);
}

static GValToRValSignalFunc
rbgobj_get_signal_func(guint signal_id)
{
    GValToRValSignalFunc func = NULL;
    VALUE func_obj = rb_hash_aref(signal_func_table, UINT2NUM(signal_id));
    if (!NIL_P(func_obj))
        Data_Get_Struct(func_obj, void, func);
    return func;
}

/**********************************************************************/

static guint
to_signal_id(signal, gtype)
     VALUE signal;
     GType gtype;
{
    if (rb_obj_is_kind_of(signal, cSignal)) {
        GSignalQuery* query;
        Data_Get_Struct(signal, GSignalQuery, query);
        return query->signal_id;
    } else if (rb_respond_to(signal, rb_intern("to_str"))){
        StringValue(signal);
        return g_signal_lookup(StringValuePtr(signal), gtype);
    } else {
        return NUM2UINT(signal);
    }
}

static GQuark
to_gquark(obj)
     VALUE obj;
{
    if (NIL_P(obj)) {
        return 0;
    } else if (rb_respond_to(obj, rb_intern("to_str"))){
        StringValue(obj);
        return g_quark_from_string(StringValuePtr(obj));
    } else {
        return NUM2UINT(obj);
    }
}

/**********************************************************************/

#ifdef RBGLIB_ENABLE_EXPERIMENTAL

static ID id_send;

typedef struct _DispatchClosure DispatchClosure;

struct _DispatchClosure
{
    GClosure closure;
    ID method_id;
};

static void
dispatch_closure_marshal(GClosure*       closure,
                         GValue*         return_value,
                         guint           n_param_values,
                         const GValue*   param_values,
                         gpointer        invocation_hint,
                         gpointer        marshal_data)
{
    int i;
    VALUE instance;
    VALUE args = rb_ary_new2(n_param_values);
    VALUE ret;

    instance = GVAL2RVAL(param_values);
    
    rb_ary_store(args, 0, ID2SYM(((DispatchClosure*)closure)->method_id));
    for (i = 1; i < n_param_values; i++)
        rb_ary_store(args, i, GVAL2RVAL(&param_values[i]));

    ret = rb_apply(instance, id_send, args);

    if (return_value)
        rbgobj_rvalue_to_gvalue(ret, return_value);
}

static GClosure*
dispatch_closure_new(ID method_id)
{
    DispatchClosure* closure;

    closure = (DispatchClosure*)g_closure_new_simple(sizeof(DispatchClosure), NULL);
    closure->method_id = method_id;
    g_closure_set_marshal((GClosure*)closure, &dispatch_closure_marshal);

    return (GClosure*)closure;    
}

#endif /* RBGLIB_ENABLE_EXPERIMENTAL */

/**********************************************************************/

#ifdef RBGLIB_ENABLE_EXPERIMENTAL

static gboolean
accumulator_func(GSignalInvocationHint* ihint,
                 GValue*                return_accu,
                 const GValue*          handler_return,
                 gpointer               data)
{
    VALUE proc = (VALUE)data;
    VALUE val = GVAL2RVAL(return_accu);
    VALUE new = GVAL2RVAL(handler_return);
    VALUE hint = Qnil; // FIXME
    VALUE tmp;
    gboolean continue_emission = TRUE;

    tmp = rb_funcall(proc, rb_intern("call"), 3, hint, val, new);
    /* FIXME */
    if (TYPE(tmp) == T_ARRAY) {
        continue_emission = RTEST(rb_ary_entry(tmp, 0));
        val = rb_ary_entry(tmp, 1);
    } else {
        val = tmp;        
    }
    rbgobj_rvalue_to_gvalue(val, return_accu);

    return continue_emission;
}

static VALUE
gobj_s_signal_new(int argc, VALUE* argv, VALUE self)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    VALUE signal_name, signal_flags, accumulator, return_type, params;
    GClosure* class_closure;
    GType* param_types;
    guint n_params;
    int i;
    guint sig;

    rb_scan_args(argc, argv, "50", &signal_name, &signal_flags,
                 &accumulator, &return_type, &params);

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registerd class",
                 rb_class2name(self));

    StringValue(signal_name);

    {
        ID method_id;
        method_id = rb_to_id(rb_str_concat(rb_str_new2("do_"), signal_name));
        class_closure = dispatch_closure_new(method_id);
    }

    if (NIL_P(params)) {
        n_params = 0;
        param_types = NULL;
    } else {
        n_params = RARRAY(params)->len;
        param_types = ALLOCA_N(GType, n_params);
        for (i = 0; i < n_params; i++)
            param_types[i] = rbgobj_gtype_get(RARRAY(params)->ptr[i]);
    }

    sig = g_signal_newv(StringValuePtr(signal_name),
                        cinfo->gtype,
                        NUM2INT(signal_flags),
                        class_closure,
                        NIL_P(accumulator) ? NULL : accumulator_func,
                        NIL_P(accumulator) ? NULL : (gpointer)accumulator,
                        NULL, /* c_marshaller */
                        rbgobj_gtype_get(return_type),
                        n_params,
                        param_types);

    if (!sig)
        rb_raise(rb_eRuntimeError, "g_signal_newv failed");

    if (accumulator)
        G_RELATIVE(self, accumulator); /* FIXME */

    return rbgobj_signal_wrap(sig);
}

#endif /* RBGLIB_ENABLE_EXPERIMENTAL */

void
_signal_list(VALUE result, GType gtype)
{
    guint n_ids, i;
    guint* ids = g_signal_list_ids(gtype, &n_ids);
    for (i = 0; i < n_ids; i++)
        rb_ary_push(result, rb_str_new2(g_signal_name(ids[i])));
}

static VALUE
gobj_s_signals(int argc, VALUE* argv, VALUE self)
{
    GType gtype;
    VALUE inherited_too, result;

    rb_scan_args(argc, argv, "01", &inherited_too);
    gtype = CLASS2GTYPE(self);
    result = rb_ary_new();

    if (RTEST(inherited_too)){
        guint n_interfaces, i;
        GType* interfaces = g_type_interfaces(gtype, &n_interfaces);
        for (i = 0; i < n_interfaces; i++)
            _signal_list(result, interfaces[i]);
        g_free(interfaces);

        for (; gtype; gtype = g_type_parent(gtype))
            _signal_list(result, gtype);
    } else if (GTYPE2CLASS(gtype) == self) {
        _signal_list(result, gtype);
    }

    return result;
}

static VALUE
gobj_s_signal(VALUE self, VALUE name)
{
    guint sig_id;
    StringValue(name);
    sig_id = g_signal_lookup(StringValuePtr(name), CLASS2GTYPE(self));
    if (!sig_id)
        rb_raise(rb_eNameError, "no such signal: %s", StringValuePtr(name));
    return rbgobj_signal_wrap(sig_id);
}

static VALUE
gobj_sig_connect(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig, rest;
    int i;
    GClosure* rclosure;
    const char* sig_name;
    guint signal_id;
    GQuark detail;

    rb_scan_args(argc, argv, "1*", &sig, &rest);

    if (SYMBOL_P(sig)){
        sig_name = rb_id2name(SYM2ID(sig));
    } else {
        StringValue(sig);
        sig_name = StringValuePtr(sig);
    }

    if (!g_signal_parse_name(sig_name, CLASS2GTYPE(CLASS_OF(self)), &signal_id, &detail, TRUE))
        rb_raise(rb_eNameError, "no such signal: %s", sig_name);

    rclosure = g_rclosure_new(G_BLOCK_PROC(), rest, 
                              rbgobj_get_signal_func(signal_id));
    i = g_signal_connect_closure_by_id(RVAL2GOBJ(self), signal_id, detail, rclosure, FALSE);

    return INT2FIX(i);
}

static VALUE
gobj_sig_connect_after(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig, rest;
    int i;
    GClosure* rclosure;
    const char* sig_name;
    guint signal_id;
    GQuark detail;

    rb_scan_args(argc, argv, "1*", &sig, &rest);

    if (SYMBOL_P(sig))
        sig_name = rb_id2name(SYM2ID(sig));
    else {
        StringValue(sig);
        sig_name = StringValuePtr(sig);
    }

    if (!g_signal_parse_name(sig_name, CLASS2GTYPE(CLASS_OF(self)), &signal_id, &detail, TRUE))
        rb_raise(rb_eNameError, "no such signal: %s", sig_name);

    rclosure = g_rclosure_new(G_BLOCK_PROC(), rest, 
                              rbgobj_get_signal_func(signal_id));
    i = g_signal_connect_closure_by_id(RVAL2GOBJ(self), signal_id, detail, rclosure, TRUE);

    return INT2FIX(i);
}

static VALUE
emit_impl(self, signal_id, detail, args)
    VALUE self;
    guint signal_id;
    GQuark detail;
    VALUE args;
{
    GSignalQuery query;
    GValueArray* instance_and_params;
    GValue* params;
    GValue return_value = {0,};
    int i;
    gboolean use_ret;

    g_signal_query(signal_id, &query);

    instance_and_params = g_value_array_new(1 + query.n_params);
    params = instance_and_params->values + 1;

    g_value_init(instance_and_params->values, G_TYPE_OBJECT);
    rbgobj_rvalue_to_gvalue(self, instance_and_params->values);
    for (i = 0; i < query.n_params; i++){
        g_value_init(params + i, query.param_types[i]);
        rbgobj_rvalue_to_gvalue(rb_ary_entry(args, i), params + i);
    }

    /* Should we use G_TYPE_IS_VALUE_TYPE(query.return_type)? */
    use_ret = (query.return_type != G_TYPE_NONE);

    if (use_ret)
        g_value_init(&return_value, query.return_type);

    g_signal_emitv(instance_and_params->values,
                   signal_id, detail,
                   (use_ret) ? &return_value : NULL);

    g_value_array_free(instance_and_params);

    if (use_ret){
        VALUE result = GVAL2RVAL(&return_value);
        g_value_unset(&return_value);
        return result;
    } else {
        return Qnil;
    }
}

static VALUE
gobj_sig_emit(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE detailed_signal, params;
    guint signal_id;
    GQuark detail;

    rb_scan_args(argc, argv, "1*", &detailed_signal, &params);
    StringValue(detailed_signal);

    if (!g_signal_parse_name(StringValuePtr(detailed_signal),
                             CLASS2GTYPE(CLASS_OF(self)),
                             &signal_id, &detail, FALSE))        
        rb_raise(rb_eArgError, "invalid signal \"%s\"",
                 StringValuePtr(detailed_signal));

    return emit_impl(self, signal_id, detail, params);
}

static VALUE
gobj_sig_emit_stop(self, detailed_signal)
    VALUE self, detailed_signal;
{
    gpointer instance = RVAL2GOBJ(self);
    guint signal_id;
    GQuark detail;

    StringValue(detailed_signal);

    if (!g_signal_parse_name(StringValuePtr(detailed_signal),
                             CLASS2GTYPE(CLASS_OF(self)),
                             &signal_id, &detail, FALSE))        
        rb_raise(rb_eArgError, "invalid signal \"%s\"",
                 StringValuePtr(detailed_signal));

    g_signal_stop_emission(instance, signal_id, detail);
    return self;
}

static VALUE gobj_sig_handler_unblock(VALUE self, VALUE id);

static VALUE
_sig_handler_block_ensure(arg)
    VALUE arg;
{
    VALUE self = RARRAY(arg)->ptr[0];
    VALUE id   = RARRAY(arg)->ptr[1];
    gobj_sig_handler_unblock(self, id);
    return Qnil;
}

static VALUE
gobj_sig_handler_block(self, id)
    VALUE self, id;
{
    g_signal_handler_block(RVAL2GOBJ(self), NUM2INT(id));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, _sig_handler_block_ensure,
                  rb_ary_new3(2, self, id));
    return self;
}

static VALUE
gobj_sig_handler_unblock(self, id)
    VALUE self, id;
{
    g_signal_handler_unblock(RVAL2GOBJ(self), NUM2INT(id));
    return self;
}

static VALUE
gobj_sig_handler_disconnect(self, id)
    VALUE self, id;
{
    g_signal_handler_disconnect(RVAL2GOBJ(self), NUM2INT(id));
    return self;
}

static VALUE
gobj_sig_handler_is_connected(self, id)
     VALUE self, id;
{
    return g_signal_handler_is_connected(RVAL2GOBJ(self), NUM2ULONG(id)) ? Qtrue : Qfalse;
}

#if 0
gulong	 g_signal_handler_find		      (gpointer		  instance,
					       GSignalMatchType	  mask,
					       guint		  signal_id,
					       GQuark		  detail,
					       GClosure		 *closure,
					       gpointer		  func,
					       gpointer		  data);
guint	 g_signal_handlers_block_matched      (gpointer		  instance,
					       GSignalMatchType	  mask,
					       guint		  signal_id,
					       GQuark		  detail,
					       GClosure		 *closure,
					       gpointer		  func,
					       gpointer		  data);
guint	 g_signal_handlers_unblock_matched    (gpointer		  instance,
					       GSignalMatchType	  mask,
					       guint		  signal_id,
					       GQuark		  detail,
					       GClosure		 *closure,
					       gpointer		  func,
					       gpointer		  data);
guint	 g_signal_handlers_disconnect_matched (gpointer		  instance,
					       GSignalMatchType	  mask,
					       guint		  signal_id,
					       GQuark		  detail,
					       GClosure		 *closure,
					       gpointer		  func,
					       gpointer		  data);
#endif

#if 0
/* --- chaining for language bindings --- */
void	g_signal_override_class_closure	      (guint		  signal_id,
					       GType		  instance_type,
					       GClosure		 *class_closure);
void	g_signal_chain_from_overridden	      (const GValue      *instance_and_params,
					       GValue            *return_value);
#endif

static void
Init_signal_misc()
{
#ifdef RBGLIB_ENABLE_EXPERIMENTAL
    id_send = rb_intern("__send__");
#endif
    signal_func_table = rb_hash_new();
    rb_global_variable(&signal_func_table);

#ifdef RBGLIB_ENABLE_EXPERIMENTAL
    rb_define_method(mMetaInterface, "signal_new", gobj_s_signal_new, -1);
#endif
    rb_define_method(mMetaInterface, "signals", gobj_s_signals, -1);
    rb_define_method(mMetaInterface, "signal", gobj_s_signal, 1);

    rb_define_method(cInstantiatable, "signal_connect", gobj_sig_connect, -1);
    rb_define_method(cInstantiatable, "signal_connect_after",
                     gobj_sig_connect_after, -1);

    rb_define_method(cInstantiatable, "signal_emit",
                     gobj_sig_emit, -1);
    rb_define_method(cInstantiatable, "signal_emit_stop",
                     gobj_sig_emit_stop, 1);
    rb_define_method(cInstantiatable, "signal_handler_block",
                     gobj_sig_handler_block, 1);
    rb_define_method(cInstantiatable, "signal_handler_unblock",
                     gobj_sig_handler_unblock, 1);
    rb_define_method(cInstantiatable, "signal_handler_disconnect",
                     gobj_sig_handler_disconnect, 1);

    rb_define_method(cInstantiatable, "signal_handler_is_connected?",
                     gobj_sig_handler_is_connected, 1);
}

/**********************************************************************/

VALUE
rbgobj_signal_wrap(sig_id)
    guint sig_id;
{
    VALUE result;
    GSignalQuery* query;

    result = Data_Make_Struct(cSignal, GSignalQuery, NULL, free, query);
    g_signal_query(sig_id, query);
    return result;
}

static VALUE
query_signal_id(self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return UINT2NUM(query->signal_id);
}

static VALUE
query_signal_name(self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return rb_str_new2(query->signal_name);
}

static VALUE
query_itype(self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return rbgobj_gtype_new(query->itype);
}

static VALUE
query_return_type(self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return rbgobj_gtype_new(query->return_type);
}

static VALUE
query_signal_flags(self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return UINT2NUM(query->signal_flags);
}

static VALUE
query_param_types(self)
{
    GSignalQuery* query;
    VALUE result;
    int i;
    Data_Get_Struct(self, GSignalQuery, query);

    result = rb_ary_new2(query->n_params);
    for (i = 0; i < query->n_params; i++)
        rb_ary_store(result, i, rbgobj_gtype_new(query->param_types[i]));

    return result;
}

#define query_is_flag(flag) \
    static VALUE \
    query_is_##flag(self) \
    { \
        GSignalQuery* query; \
        Data_Get_Struct(self, GSignalQuery, query); \
        return (query->signal_flags & flag) ? Qtrue : Qfalse; \
    }

query_is_flag(G_SIGNAL_RUN_FIRST)
query_is_flag(G_SIGNAL_RUN_LAST)
query_is_flag(G_SIGNAL_RUN_CLEANUP)
query_is_flag(G_SIGNAL_NO_RECURSE)
query_is_flag(G_SIGNAL_DETAILED)
query_is_flag(G_SIGNAL_ACTION)
query_is_flag(G_SIGNAL_NO_HOOKS)


static gboolean
hook_func(GSignalInvocationHint* ihint,
          guint                  n_param_values,
          const GValue*          param_values,
          gpointer               data)
{
    GClosure* closure = data;
    GValue ret_val ={0,};
    gboolean ret;

    g_value_init(&ret_val, G_TYPE_BOOLEAN);
    g_closure_invoke(closure, &ret_val, n_param_values, param_values, ihint);
    ret = g_value_get_boolean(&ret_val);
    g_value_unset(&ret_val);

    return ret;
}

static gulong
g_signal_add_emission_hook_closure (guint     signal_id,
                                    GQuark    detail,
                                    GClosure* closure)
{
    guint hook_id;
    g_closure_ref(closure);
    g_closure_sink(closure);
    hook_id = g_signal_add_emission_hook(signal_id, detail,
                                         &hook_func, closure,
                                         (GDestroyNotify)&g_closure_unref);
    return hook_id;
}

static VALUE
signal_add_emission_hook(int argc, VALUE* argv, VALUE self)
{
    VALUE tmp, proc;
    guint signal_id, hook_id;
    GQuark detail;
    GClosure* closure;

    if (rb_scan_args(argc, argv, "01&", &tmp, &proc) == 1)
        detail = to_gquark(tmp);
    else
        detail = 0;
    signal_id = to_signal_id(self);

    closure = g_rclosure_new(proc, Qnil, rbgobj_get_signal_func(signal_id));
    hook_id = g_signal_add_emission_hook_closure(signal_id, detail, closure);
    return ULONG2NUM(hook_id);
}

static VALUE
signal_remove_emission_hook(VALUE self, VALUE hook_id)
{
    g_signal_remove_emission_hook(to_signal_id(self), NUM2ULONG(hook_id));
    return Qnil;
}

static void
Init_signal_class()
{
    cSignal = rb_define_class_under(mGLib, "Signal", rb_cData);

    rb_define_method(cSignal, "id", query_signal_id, 0);
    rb_define_method(cSignal, "name", query_signal_name, 0);
    rb_define_method(cSignal, "flags", query_signal_flags, 0);
    rb_define_method(cSignal, "itype", query_itype, 0);
    rb_define_method(cSignal, "return_type", query_return_type, 0);
    rb_define_method(cSignal, "param_types", query_param_types, 0);

    rb_define_method(cSignal, "add_emission_hook", signal_add_emission_hook, -1);
    rb_define_method(cSignal, "remove_emission_hook", signal_remove_emission_hook, 1);

    /* GSignalFlags */
    rb_define_const(cSignal, "RUN_FIRST",   INT2FIX(G_SIGNAL_RUN_FIRST));
    rb_define_const(cSignal, "RUN_LAST",    INT2FIX(G_SIGNAL_RUN_LAST));
    rb_define_const(cSignal, "RUN_CLEANUP", INT2FIX(G_SIGNAL_RUN_CLEANUP));
    rb_define_const(cSignal, "NO_RECURSE",  INT2FIX(G_SIGNAL_NO_RECURSE));
    rb_define_const(cSignal, "DETAILED",    INT2FIX(G_SIGNAL_DETAILED));
    rb_define_const(cSignal, "ACTION",      INT2FIX(G_SIGNAL_ACTION));
    rb_define_const(cSignal, "NO_HOOKS",    INT2FIX(G_SIGNAL_NO_HOOKS));

    rb_define_const(cSignal, "FLAGS_MASK",  INT2FIX(G_SIGNAL_FLAGS_MASK));

    rb_define_method(cSignal, "run_first?", query_is_G_SIGNAL_RUN_FIRST, 0);
    rb_define_method(cSignal, "run_last?", query_is_G_SIGNAL_RUN_LAST, 0);
    rb_define_method(cSignal, "run_cleanup?", query_is_G_SIGNAL_RUN_CLEANUP, 0);
    rb_define_method(cSignal, "no_recurse?", query_is_G_SIGNAL_NO_RECURSE, 0);
    rb_define_method(cSignal, "detailed?", query_is_G_SIGNAL_DETAILED, 0);
    rb_define_method(cSignal, "action?", query_is_G_SIGNAL_ACTION, 0);
    rb_define_method(cSignal, "no_hooks?", query_is_G_SIGNAL_NO_HOOKS, 0);


    /* GConnectFlags */
    rb_define_const(cSignal, "CONNECT_AFTER",   INT2FIX(G_CONNECT_AFTER));
    rb_define_const(cSignal, "CONNECT_SWAPPED", INT2FIX(G_CONNECT_SWAPPED));

    /* GSignalMatchType */
    rb_define_const(cSignal, "MATCH_ID",        INT2FIX(G_SIGNAL_MATCH_ID));
    rb_define_const(cSignal, "MATCH_DETAIL",    INT2FIX(G_SIGNAL_MATCH_DETAIL));
    rb_define_const(cSignal, "MATCH_CLOSURE",   INT2FIX(G_SIGNAL_MATCH_CLOSURE));
    rb_define_const(cSignal, "MATCH_FUNC",      INT2FIX(G_SIGNAL_MATCH_FUNC));
    rb_define_const(cSignal, "MATCH_DATA",      INT2FIX(G_SIGNAL_MATCH_DATA));
    rb_define_const(cSignal, "MATCH_UNBLOCKED", INT2FIX(G_SIGNAL_MATCH_UNBLOCKED));

    rb_define_const(cSignal, "MATCH_MASK", INT2FIX(G_SIGNAL_MATCH_MASK));
    rb_define_const(cSignal, "TYPE_STATIC_SCOPE", INT2FIX(G_SIGNAL_TYPE_STATIC_SCOPE));
}

/**********************************************************************/

#ifdef RBGLIB_ENABLE_EXPERIMENTAL

void
rbgobj_define_action_methods(VALUE klass)
{
    GString* source = g_string_new(NULL);
    guint n_ids;
    guint* ids;
    int i;

    ids = g_signal_list_ids(CLASS2GTYPE(klass), &n_ids);

    for (i = 0; i < n_ids; i++){
        GSignalQuery query;
        g_signal_query(ids[i], &query);

        if (query.signal_flags & G_SIGNAL_ACTION) {
            gchar* method_name = g_strdup(query.signal_name);
            gchar* p;
            GString* args;
            int j;

            for (p = method_name; *p; p++)
                if (*p == '-')
                    *p = '_';

            args = g_string_new(NULL);
            for (j = 0; j < query.n_params; j++)
                g_string_append_printf(args, ",x%d", j);

            g_string_append_printf(
                source,
                "def %s(%s)\n  signal_emit('%s'%s)\nend\n",
                method_name,
                (query.n_params > 0) ? args->str + 1 : "", // hack
                query.signal_name,
                args->str);

            g_free(method_name);
            g_string_free(args, TRUE);
        }
    }

    rb_funcall(klass, rb_intern("module_eval"), 1, rb_str_new2(source->str));
}

#endif // RBGLIB_ENABLE_EXPERIMENTAL

/**********************************************************************/

void
Init_gobject_gsignal()
{
    Init_signal_class();
    Init_signal_misc();
}

