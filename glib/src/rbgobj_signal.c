/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_signal.c -

  $Author: sakai $
  $Date: 2002/09/24 03:00:02 $
  created at: Sat Jul 27 16:56:01 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

/**********************************************************************/

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

/**********************************************************************/

#ifdef RBGLIB_ENABLE_EXPERIMENTAL

static VALUE
gobj_s_signal_new(int argc, VALUE* argv, VALUE self)
{
    VALUE signal_name, signal_flags, accumulator, return_type, params;
    ID method_id;
    GClosure* class_closure;
    GType* param_types;
    int i;
    guint sig;

    rb_scan_args(argc, argv, "3*&", &signal_name, &signal_flags,
                 &return_type, &params, &accumulator);

    StringValue(signal_name);

    method_id = rb_to_id(rb_str_concat(rb_str_new2("real_"), signal_name));
    class_closure = dispatch_closure_new(method_id);

    param_types = ALLOCA_N(GType, RARRAY(params)->len);
    for (i = 0; i < RARRAY(params)->len; i++)
        param_types[i] = rbgobj_gtype_get(RARRAY(params)->ptr[i]);

    sig = g_signal_newv(StringValuePtr(signal_name),
                        CLASS2GTYPE(self),
                        NUM2INT(signal_flags),
                        class_closure,
                        NULL,
                        NULL,
                        NULL,
                        rbgobj_gtype_get(return_type),
                        RARRAY(params)->len,
                        param_types);

    return UINT2NUM(sig);
}

#endif /* RBGLIB_ENABLE_EXPERIMENTAL */

static VALUE
gobj_s_signal_list(VALUE self)
{
    guint n_ids;
    guint* ids = g_signal_list_ids(CLASS2GTYPE(self), &n_ids);
    VALUE result;
    guint i;

    result = rb_ary_new2(n_ids);
    for (i = 0; i < n_ids; i++)
        rb_ary_store(result, i, UINT2NUM(ids[i]));

    return result;
}

static VALUE
gobj_s_signal_lookup(VALUE self, VALUE name)
{
    StringValue(name);
    return UINT2NUM(g_signal_lookup(StringValuePtr(name), CLASS2GTYPE(self)));
}

/**********************************************************************/

static VALUE
gobj_sig_connect(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE sig, rest;
    int i;
    GClosure* rclosure;

    rb_scan_args(argc, argv, "1*", &sig, &rest);
    StringValue(sig);

    rclosure = g_rclosure_new(rb_f_lambda(), rest);
    i = g_signal_connect_closure(RVAL2GOBJ(self),
                                 StringValuePtr(sig), rclosure, FALSE);

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

    rb_scan_args(argc, argv, "1*", &sig, &rest);
    StringValue(sig);

    rclosure = g_rclosure_new(rb_f_lambda(), rest);
    i = g_signal_connect_closure(RVAL2GOBJ(self),
                                 StringValuePtr(sig), rclosure, TRUE);

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

static guint
to_signal_id(signal, gtype)
     VALUE signal;
     GType gtype;
{
    if (rb_respond_to(signal, rb_intern("to_str"))){
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

static VALUE
gobj_sig_emit(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE signal, detail, params;

    rb_scan_args(argc, argv, "11*", &signal, &detail, &params);

    return emit_impl(self,
                     to_signal_id(signal, CLASS2GTYPE(CLASS_OF(self))),
                     to_gquark(detail),
                     params);
}

static VALUE
gobj_sig_emit_by_name(argc, argv, self)
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
gobj_sig_emit_stop(argc, argv, self)
     int argc;
     VALUE* argv;
     VALUE self;
{
    VALUE sig_id, detail;
    rb_scan_args(argc, argv, "11", &sig_id, &detail);
    g_signal_stop_emission(RVAL2GOBJ(self),
                           to_signal_id(sig_id, CLASS2GTYPE(CLASS_OF(self))),
                           to_gquark(detail));
    return self;
}

static VALUE
gobj_sig_emit_stop_by_name(self, detailed_signal)
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

static VALUE
gobj_sig_handler_block(self, id)
    VALUE self, id;
{
    g_signal_handler_block(RVAL2GOBJ(self), NUM2INT(id));
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

/**********************************************************************/

void
Init_gobject_gsignal()
{
    id_send = rb_intern("__send__");

#ifdef RBGLIB_ENABLE_EXPERIMENTAL
    rb_define_singleton_method(cInstantiatable, "signal_new", gobj_s_signal_new, -1);
#endif
    rb_define_singleton_method(cInstantiatable, "signal_list", gobj_s_signal_list, 0);
    rb_define_singleton_method(cInstantiatable, "signal_lookup", gobj_s_signal_lookup, 1);

    rb_define_method(cInstantiatable, "signal_connect", gobj_sig_connect, -1);
    rb_define_method(cInstantiatable, "signal_connect_after",
                     gobj_sig_connect_after, -1);

    rb_define_method(cInstantiatable, "signal_emit",
                     gobj_sig_emit, -1);
    rb_define_method(cInstantiatable, "signal_emit_by_name",
                     gobj_sig_emit_by_name, -1);
    rb_define_method(cInstantiatable, "signal_emit_stop",
                     gobj_sig_emit_stop, -1);
    rb_define_method(cInstantiatable, "signal_emit_stop_by_name",
                     gobj_sig_emit_stop_by_name, 1);
    rb_define_method(cInstantiatable, "signal_handler_block",
                     gobj_sig_handler_block, 1);
    rb_define_method(cInstantiatable, "signal_handler_unblock",
                     gobj_sig_handler_unblock, 1);
    rb_define_method(cInstantiatable, "signal_handler_disconnect",
                     gobj_sig_handler_disconnect, 1);

    rb_define_method(cInstantiatable, "signal_handler_is_connected?",
                     gobj_sig_handler_is_connected, 1);

    /* --- run, match and connect types --- */
    {
        VALUE mSignal = rb_define_module_under(mGLib, "Signal");

        /* GSignalFlags */
        rb_define_const(mSignal, "RUN_FIRST",   INT2FIX(G_SIGNAL_RUN_FIRST));
        rb_define_const(mSignal, "RUN_LAST",    INT2FIX(G_SIGNAL_RUN_LAST));
        rb_define_const(mSignal, "RUN_CLEANUP", INT2FIX(G_SIGNAL_RUN_CLEANUP));
        rb_define_const(mSignal, "NO_RECURSE",  INT2FIX(G_SIGNAL_NO_RECURSE));
        rb_define_const(mSignal, "DETAILED",    INT2FIX(G_SIGNAL_DETAILED));
        rb_define_const(mSignal, "ACTION",      INT2FIX(G_SIGNAL_ACTION));
        rb_define_const(mSignal, "NO_HOOKS",    INT2FIX(G_SIGNAL_NO_HOOKS));

        rb_define_const(mSignal, "FLAGS_MASK",  INT2FIX(G_SIGNAL_FLAGS_MASK));

        /* GConnectFlags */
        rb_define_const(mSignal, "CONNECT_AFTER",   INT2FIX(G_CONNECT_AFTER));
        rb_define_const(mSignal, "CONNECT_SWAPPED", INT2FIX(G_CONNECT_SWAPPED));

        /* GSignalMatchType */
        rb_define_const(mSignal, "MATCH_ID",        INT2FIX(G_SIGNAL_MATCH_ID));
        rb_define_const(mSignal, "MATCH_DETAIL",    INT2FIX(G_SIGNAL_MATCH_DETAIL));
        rb_define_const(mSignal, "MATCH_CLOSURE",   INT2FIX(G_SIGNAL_MATCH_CLOSURE));
        rb_define_const(mSignal, "MATCH_FUNC",      INT2FIX(G_SIGNAL_MATCH_FUNC));
        rb_define_const(mSignal, "MATCH_DATA",      INT2FIX(G_SIGNAL_MATCH_DATA));
        rb_define_const(mSignal, "MATCH_UNBLOCKED", INT2FIX(G_SIGNAL_MATCH_UNBLOCKED));

        rb_define_const(mSignal, "MATCH_MASK", INT2FIX(G_SIGNAL_MATCH_MASK));
        rb_define_const(mSignal, "TYPE_STATIC_SCOPE", INT2FIX(G_SIGNAL_TYPE_STATIC_SCOPE));
    }
}
