/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgprivate.h"

#define RG_TARGET_NAMESPACE cSignal

static VALUE RG_TARGET_NAMESPACE;
VALUE rbgobj_signal_wrap(guint sig_id);

#define default_handler_method_prefix "signal_do_"

/**********************************************************************/

static VALUE signal_func_table;

void
rbgobj_set_signal_func(VALUE klass, const gchar *sig_name, GValToRValSignalFunc func)
{
    VALUE obj = Data_Wrap_Struct(rb_cData, NULL, NULL, func);
    guint signal_id = g_signal_lookup(sig_name, CLASS2GTYPE(klass));
    rb_hash_aset(signal_func_table, UINT2NUM(signal_id), obj);
}

GValToRValSignalFunc
rbgobj_get_signal_func(guint signal_id)
{
    GValToRValSignalFunc func = NULL;
    VALUE func_obj = rb_hash_aref(signal_func_table, UINT2NUM(signal_id));
    if (!NIL_P(func_obj))
        Data_Get_Struct(func_obj, void, func);
    return func;
}

/**********************************************************************/

static VALUE eNoSignalError;

// FIXME: use rb_protect
static gboolean
accumulator_func(G_GNUC_UNUSED GSignalInvocationHint *ihint,
                 GValue *return_accu,
                 const GValue *handler_return,
                 gpointer data)
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
        continue_emission = RVAL2CBOOL(rb_ary_entry(tmp, 0));
        val = rb_ary_entry(tmp, 1);
    } else {
        val = tmp;        
    }
    rbgobj_rvalue_to_gvalue(val, return_accu);

    return continue_emission;
}

struct rval2gtypes_args {
    VALUE ary;
    long n;
    GType *result;
};

static VALUE
rbg_rval2gtypes_body(VALUE value)
{
    long i;
    struct rval2gtypes_args *args = (struct rval2gtypes_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = rbgobj_gtype_get(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gtypes_rescue(VALUE value)
{
    g_free(((struct rval2gtypes_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GType *
rbg_rval2gtypes(volatile VALUE *value, long *n)
{
    struct rval2gtypes_args args;

    args.ary = *value = rb_ary_dup(rb_ary_to_ary(*value));
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GType, args.n + 1);

    rb_rescue(rbg_rval2gtypes_body, (VALUE)&args,
              rbg_rval2gtypes_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

static GType *
rbg_rval2gtypes_accept_nil(volatile VALUE *value, long *n)
{
    if (!NIL_P(*value))
        return rbg_rval2gtypes(value, n);

    if (n != NULL)
        *n = 0;

    return NULL;
}

#define RVAL2GTYPES(value, n) rbg_rval2gtypes(&(value), &(n))
#define RVAL2GTYPES_ACCEPT_NIL(value, n) rbg_rval2gtypes_accept_nil(&(value), &(n))

static VALUE
gobj_s_signal_new(int argc, VALUE* argv, VALUE self)
{
    const RGObjClassInfo *cinfo = rbgobj_lookup_class(self);
    VALUE rbsignal_name, rbsignal_flags, accumulator, rbreturn_type, params;
    const gchar *signal_name;
    GSignalFlags signal_flags;
    GClosure *class_closure;
    GType return_type;
    GType *param_types;
    long n_params;
    guint signal;

    rb_scan_args(argc, argv, "4*",
                 &rbsignal_name, &rbsignal_flags, &accumulator, &rbreturn_type, &params);

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "not a registered class: %s",
                 rb_class2name(self));

    if (SYMBOL_P(rbsignal_name))
        rbsignal_name = rb_str_new2(rb_id2name(SYM2ID(rbsignal_name)));
    signal_name = RVAL2CSTR(rbsignal_name);

    signal_flags = NUM2INT(rbsignal_flags);

    {
        VALUE factory;
        VALUE proc;
        ID method_id;

        method_id = rb_to_id(rb_str_concat(rb_str_new2(default_handler_method_prefix), rbsignal_name));

        factory = rb_eval_string(
          "lambda{|klass, id|\n"
          "  lambda{|instance,*args|\n"
          "    klass.instance_method(id).bind(instance).call(*args)\n"
          "  }\n"
          "}\n");
        proc = rb_funcall(factory, rb_intern("call"), 2, self, ID2SYM(method_id));

        class_closure = g_rclosure_new(proc, Qnil, NULL);
        /* TODO: Should this be done even if something below it fails? */
        g_rclosure_attach(class_closure, self);
    }

    return_type = rbgobj_gtype_get(rbreturn_type);
    param_types = RVAL2GTYPES_ACCEPT_NIL(params, n_params);

    signal = g_signal_newv(signal_name,
                           cinfo->gtype,
                           signal_flags,
                           class_closure,
                           NIL_P(accumulator) ? NULL : accumulator_func,
                           NIL_P(accumulator) ? NULL : (gpointer)accumulator,
                           NULL, /* c_marshaller */
                           return_type,
                           n_params,
                           param_types);

    g_free(param_types);

    if (!signal)
        rb_raise(rb_eRuntimeError, "g_signal_newv failed");

    if (!NIL_P(accumulator))
        G_RELATIVE(self, accumulator); /* FIXME */

    return rbgobj_signal_wrap(signal);
}

static void
_signal_list(VALUE result, GType gtype)
{
    guint n_ids, i;
    guint* ids = g_signal_list_ids(gtype, &n_ids);
    for (i = 0; i < n_ids; i++)
        rb_ary_push(result, rb_str_new2(g_signal_name(ids[i])));
    g_free(ids);
}

static VALUE
gobj_s_signals(int argc, VALUE* argv, VALUE self)
{
    GType gtype;
    VALUE inherited_too, result;

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;
    gtype = CLASS2GTYPE(self);
    result = rb_ary_new();

    if (RVAL2CBOOL(inherited_too)){
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
    const char* sig_name;
    guint sig_id;

    if (SYMBOL_P(name))
        sig_name = rb_id2name(SYM2ID(name));
    else
        sig_name = StringValuePtr(name);

    sig_id = g_signal_lookup(sig_name, CLASS2GTYPE(self));
    if (!sig_id)
        rb_raise(eNoSignalError, "no such signal: %s", sig_name);

    return rbgobj_signal_wrap(sig_id);
}

static VALUE
gobj_sig_has_handler_pending(int argc, VALUE *argv, VALUE self)
{
    VALUE sig, may_be_blocked;
    const char* sig_name;
    guint signal_id;
    GQuark detail;

    rb_scan_args(argc, argv, "11", &sig, &may_be_blocked);

    if (SYMBOL_P(sig))
        sig_name = rb_id2name(SYM2ID(sig));
    else
        sig_name = StringValuePtr(sig);

    if (!g_signal_parse_name(sig_name, CLASS2GTYPE(CLASS_OF(self)), &signal_id, &detail, TRUE))
        rb_raise(eNoSignalError, "no such signal: %s", sig_name);

    return CBOOL2RVAL(g_signal_has_handler_pending(RVAL2GOBJ(self),
                                                   signal_id, detail,
                                                   RVAL2CBOOL(may_be_blocked)));
}

static VALUE
gobj_sig_connect_impl(gboolean after, int argc, VALUE *argv, VALUE self)
{
    VALUE sig, rest;
    gulong handler_id;
    GClosure* rclosure;
    const char* sig_name;
    guint signal_id;
    GQuark detail;
    VALUE func;
    GObject *g_object;
    gchar *tag;

    rb_scan_args(argc, argv, "1*", &sig, &rest);

    if (NIL_P(rest)) rest = rb_ary_new();

    if (SYMBOL_P(sig))
        sig_name = rb_id2name(SYM2ID(sig));
    else
        sig_name = StringValuePtr(sig);

    if (!g_signal_parse_name(sig_name, CLASS2GTYPE(CLASS_OF(self)), &signal_id, &detail, TRUE))
        rb_raise(eNoSignalError, "no such signal: %s", sig_name);

    func = rb_block_proc();
    rclosure = g_rclosure_new(func, rest, 
                              rbgobj_get_signal_func(signal_id));
    g_rclosure_attach((GClosure *)rclosure, self);
    g_object = RVAL2GOBJ(self);
    tag = g_strdup_printf("%s::%s",
                          G_OBJECT_CLASS_NAME(G_OBJECT_GET_CLASS(g_object)),
                          sig_name);
    g_rclosure_set_tag((GClosure *)rclosure, tag);
    g_free(tag);
    handler_id = g_signal_connect_closure_by_id(g_object, signal_id, detail,
                                                rclosure, after);

    return ULONG2NUM(handler_id);
}

static VALUE
gobj_sig_connect(int argc, VALUE *argv, VALUE self)
{
    return gobj_sig_connect_impl(FALSE, argc, argv, self);
}

static VALUE
gobj_sig_connect_after(int argc, VALUE *argv, VALUE self)
{
    return gobj_sig_connect_impl(TRUE, argc, argv, self);
}

#if 0
static VALUE
gobj_sig_get_invocation_hint(VALUE self)
{
    GSignalInvocationHint* hint;
    hint = g_signal_get_invocation_hint(RVAL2GOBJ(self));
    return rb_ary_new3(3,
                       rbgobj_signal_wrap(hint->signal_id),
                       hint->detail ? rb_str_new2(g_quark_to_string(hint->detail)) : Qnil,
                       INT2NUM(hint->run_type));
}
#endif

struct emit_arg {
    VALUE self;
    VALUE args;

    GSignalQuery query;
    GQuark detail;
    GValueArray* instance_and_params;
};

static VALUE
emit_body(struct emit_arg* arg)
{
    GValue param = G_VALUE_INIT;

    g_value_init(&param, G_TYPE_FROM_INSTANCE(RVAL2GOBJ(arg->self)));
    rbgobj_rvalue_to_gvalue(arg->self, &param);
    g_value_array_append(arg->instance_and_params, &param);
    g_value_unset(&param);

    {
        guint i;
        for (i = 0; i < arg->query.n_params; i++){
            GType gtype = arg->query.param_types[i] & ~G_SIGNAL_TYPE_STATIC_SCOPE;

            g_value_init(&param,  gtype);

            rbgobj_rvalue_to_gvalue(rb_ary_entry(arg->args, i), &param);
            g_value_array_append(arg->instance_and_params, &param);
            g_value_unset(&param);
        }
    }

    {
        gboolean use_ret = (arg->query.return_type != G_TYPE_NONE);
        GValue return_value = G_VALUE_INIT;

        if (use_ret)
            g_value_init(&return_value,
                         arg->query.return_type & ~G_SIGNAL_TYPE_STATIC_SCOPE);

        g_signal_emitv(arg->instance_and_params->values,
                       arg->query.signal_id, arg->detail,
                       (use_ret) ? &return_value : NULL);

        if (use_ret) {
            VALUE ret = GVAL2RVAL(&return_value);
            g_value_unset(&return_value);
            return ret;
        } else {
            return Qnil;
        }
    }
}

static VALUE
emit_ensure(struct emit_arg* arg)
{
    g_value_array_free(arg->instance_and_params);
    return Qnil;
}

static VALUE
gobj_sig_emit(int argc, VALUE *argv, VALUE self)
{
    VALUE sig;
    const char* sig_name;
    guint signal_id;
    struct emit_arg arg;

    rb_scan_args(argc, argv, "1*", &sig, &arg.args);

    if (SYMBOL_P(sig))
        sig_name = rb_id2name(SYM2ID(sig));
    else
        sig_name = StringValuePtr(sig);

    if (!g_signal_parse_name(sig_name,
                             CLASS2GTYPE(CLASS_OF(self)),
                             &signal_id, &arg.detail, FALSE))
        rb_raise(eNoSignalError, "invalid signal \"%s\"", sig_name);

    g_signal_query(signal_id, &arg.query);

    if (arg.query.n_params != (guint)RARRAY_LEN(arg.args))
        rb_raise(rb_eArgError, "wrong number of arguments(%ld for %d)",
                 RARRAY_LEN(arg.args) + 1,
                 arg.query.n_params + 1);

    arg.self = self;
    arg.instance_and_params = g_value_array_new(1 + arg.query.n_params);

    return rb_ensure(emit_body, (VALUE)&arg, emit_ensure, (VALUE)&arg);
}

static VALUE
gobj_sig_emit_stop(VALUE self, VALUE sig)
{
    gpointer instance = RVAL2GOBJ(self);
    const char* sig_name;
    guint signal_id;
    GQuark detail;

    if (SYMBOL_P(sig))
        sig_name = rb_id2name(SYM2ID(sig));
    else
        sig_name = StringValuePtr(sig);

    if (!g_signal_parse_name(sig_name,
                             CLASS2GTYPE(CLASS_OF(self)),
                             &signal_id, &detail, FALSE))        
        rb_raise(eNoSignalError, "invalid signal \"%s\"", sig_name);

    g_signal_stop_emission(instance, signal_id, detail);
    return self;
}

static VALUE gobj_sig_handler_unblock(VALUE self, VALUE id);

static VALUE
_sig_handler_block_ensure(VALUE arg)
{
    VALUE self = RARRAY_PTR(arg)[0];
    VALUE id   = RARRAY_PTR(arg)[1];
    gobj_sig_handler_unblock(self, id);
    return Qnil;
}

static VALUE
gobj_sig_handler_block(VALUE self, VALUE id)
{
    g_signal_handler_block(RVAL2GOBJ(self), NUM2ULONG(id));
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, _sig_handler_block_ensure,
                  rb_ary_new3(2, self, id));
    return self;
}

static VALUE
gobj_sig_handler_unblock(VALUE self, VALUE id)
{
    g_signal_handler_unblock(RVAL2GOBJ(self), NUM2ULONG(id));
    return self;
}

static VALUE
gobj_sig_handler_disconnect(VALUE self, VALUE id)
{
    g_signal_handler_disconnect(RVAL2GOBJ(self), NUM2ULONG(id));
    return self;
}

static VALUE
gobj_sig_handler_is_connected(VALUE self, VALUE id)
{
    return CBOOL2RVAL(g_signal_handler_is_connected(RVAL2GOBJ(self), NUM2ULONG(id)));
}

#if 0
gulong   g_signal_handler_find             (gpointer        instance,
                           GSignalMatchType      mask,
                           guint         signal_id,
                           GQuark        detail,
                           GClosure         *closure,
                           gpointer          func,
                           gpointer          data);
guint    g_signal_handlers_block_matched      (gpointer         instance,
                           GSignalMatchType      mask,
                           guint         signal_id,
                           GQuark        detail,
                           GClosure         *closure,
                           gpointer          func,
                           gpointer          data);
guint    g_signal_handlers_unblock_matched    (gpointer         instance,
                           GSignalMatchType      mask,
                           guint         signal_id,
                           GQuark        detail,
                           GClosure         *closure,
                           gpointer          func,
                           gpointer          data);
guint    g_signal_handlers_disconnect_matched (gpointer         instance,
                           GSignalMatchType      mask,
                           guint         signal_id,
                           GQuark        detail,
                           GClosure         *closure,
                           gpointer          func,
                           gpointer          data);
#endif

static VALUE
chain_from_overridden_body(struct emit_arg* arg)
{
    g_value_init(arg->instance_and_params->values,
                 G_TYPE_FROM_INSTANCE(RVAL2GOBJ(arg->self)));
    rbgobj_rvalue_to_gvalue(arg->self, arg->instance_and_params->values);

    {
        GValue* params = arg->instance_and_params->values + 1;
        guint i;
        for (i = 0; i < arg->query.n_params; i++) {
            GType gtype = arg->query.param_types[i] & ~G_SIGNAL_TYPE_STATIC_SCOPE;
            g_value_init(params + i, gtype);
            rbgobj_rvalue_to_gvalue(rb_ary_entry(arg->args, i), params + i);
        }
    }

    {
        gboolean use_ret = (arg->query.return_type != G_TYPE_NONE);
        GValue return_value = G_VALUE_INIT;

        if (use_ret)
            g_value_init(&return_value,
                         arg->query.return_type & ~G_SIGNAL_TYPE_STATIC_SCOPE);

        g_signal_chain_from_overridden(arg->instance_and_params->values,
                                       (use_ret) ? &return_value : NULL);

        if (use_ret) {
            VALUE ret = GVAL2RVAL(&return_value);
            g_value_unset(&return_value);
            return ret;
        } else {
            return Qnil;
        }
    }
}

static VALUE
gobj_sig_chain_from_overridden(int argc, VALUE *argv, VALUE self)
{
    struct emit_arg arg;

    {
        GSignalInvocationHint* hint;
        hint = g_signal_get_invocation_hint(RVAL2GOBJ(self));
        if (!hint)
            rb_raise(rb_eRuntimeError, "can't get signal invocation hint");
        g_signal_query(hint->signal_id, &arg.query);
    }

    if (arg.query.n_params != (guint)argc)
        rb_raise(rb_eArgError, "wrong number of arguments(%d for %d)",
                 argc, arg.query.n_params);

    arg.self = self;
    arg.args = rb_ary_new4(argc, argv);
    arg.instance_and_params = g_value_array_new(1 + argc);

    return rb_ensure(chain_from_overridden_body, (VALUE)&arg,
                     emit_ensure, (VALUE)&arg);
}

static VALUE
gobj_s_method_added(VALUE klass, VALUE id)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(klass);
    const char* name = rb_id2name(SYM2ID(id));
    const int prefix_len = strlen(default_handler_method_prefix);
    guint signal_id;

    if (cinfo->klass != klass) return Qnil;
    if (strncmp(default_handler_method_prefix, name, prefix_len)) return Qnil;

    signal_id = g_signal_lookup(name + prefix_len, cinfo->gtype);    
    if (!signal_id) return Qnil;

    {
        GSignalQuery query;
        g_signal_query(signal_id, &query);
        if (query.itype == cinfo->gtype)
            return Qnil;
    }

    {
        VALUE f = rb_eval_string(
          "lambda{|klass, id|\n"
          "  lambda{|instance,*args|\n"
          "    klass.instance_method(id).bind(instance).call(*args)\n"
          "  }\n"
          "}\n");
        VALUE proc = rb_funcall(f, rb_intern("call"), 2, klass, id);
        GClosure* rclosure = g_rclosure_new(proc, Qnil,
                                            rbgobj_get_signal_func(signal_id));
        g_rclosure_attach((GClosure *)rclosure, klass);
        g_signal_override_class_closure(signal_id, cinfo->gtype, rclosure);
    }

    {
        VALUE mod = rb_define_module_under(klass, RubyGObjectHookModule);
        rb_include_module(klass, mod);
        rbg_define_method(mod, name, gobj_sig_chain_from_overridden, -1);
    }

    return Qnil;
}

/**********************************************************************/

VALUE
rbgobj_signal_wrap(guint sig_id)
{
    VALUE result;
    GSignalQuery* query;

    result = Data_Make_Struct(RG_TARGET_NAMESPACE, GSignalQuery, NULL, free, query);
    g_signal_query(sig_id, query);
    return result;
}

static VALUE
rg_id(VALUE self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return UINT2NUM(query->signal_id);
}

static VALUE
rg_name(VALUE self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return rb_str_new2(query->signal_name);
}

static VALUE
rg_itype(VALUE self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return rbgobj_gtype_new(query->itype);
}

static VALUE
rg_owner(VALUE self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return GTYPE2CLASS(query->itype);
}

static VALUE
rg_return_type(VALUE self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return rbgobj_gtype_new(query->return_type);
}

static VALUE
rg_flags(VALUE self)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    return UINT2NUM(query->signal_flags);
}

static VALUE
rg_param_types(VALUE self)
{
    GSignalQuery* query;
    VALUE result;
    guint i;
    Data_Get_Struct(self, GSignalQuery, query);

    result = rb_ary_new2(query->n_params);
    for (i = 0; i < query->n_params; i++)
        rb_ary_store(result, i, rbgobj_gtype_new(query->param_types[i]));

    return result;
}

static VALUE
rg_inspect(VALUE self)
{
    GSignalQuery* query;
    gchar* s;
    VALUE result, v;

    Data_Get_Struct(self, GSignalQuery, query);

    v = rb_inspect(GTYPE2CLASS(query->itype));

    s = g_strdup_printf("#<%s: %s#%s>",
                        rb_class2name(CLASS_OF(self)),
                        StringValuePtr(v),
                        query->signal_name);
    result = rb_str_new2(s);
    g_free(s);

    return result;
}

#define query_is_flag(flag) \
    static VALUE \
    query_is_##flag(VALUE self) \
    { \
        GSignalQuery* query; \
        Data_Get_Struct(self, GSignalQuery, query); \
        return CBOOL2RVAL(query->signal_flags & flag); \
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
    GValue ret_val =G_VALUE_INIT;
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
rg_add_emission_hook(int argc, VALUE* argv, VALUE self)
{
    GSignalQuery* query;
    VALUE proc;
    guint hook_id;
    GQuark detail = 0;
    GClosure* closure;

    Data_Get_Struct(self, GSignalQuery, query);

    if (query->signal_flags & G_SIGNAL_DETAILED) {
        VALUE detail_obj;
        if (rb_scan_args(argc, argv, "01&", &detail_obj, &proc) == 1) {
            if (SYMBOL_P(detail_obj))
                detail = g_quark_from_string(rb_id2name(SYM2ID(detail_obj)));
            else
                detail = g_quark_from_string(StringValuePtr(detail_obj));
        }
    } else {
        rb_scan_args(argc, argv, "00&", &proc);
    }

    closure = g_rclosure_new(proc, Qnil,
                             rbgobj_get_signal_func(query->signal_id));
    g_rclosure_attach(closure, self);
    hook_id = g_signal_add_emission_hook_closure(query->signal_id, detail, closure);
    return ULONG2NUM(hook_id);
}

static VALUE
rg_remove_emission_hook(VALUE self, VALUE hook_id)
{
    GSignalQuery* query;
    Data_Get_Struct(self, GSignalQuery, query);
    g_signal_remove_emission_hook(query->signal_id, NUM2ULONG(hook_id));
    return Qnil;
}

/**********************************************************************/

void
rbgobj_define_action_methods(VALUE klass)
{
    GType gtype = CLASS2GTYPE(klass);
    GString* source;
    guint n_ids;
    guint* ids;
    guint i;

    if (gtype == G_TYPE_INTERFACE)
        return;

    ids = g_signal_list_ids(gtype, &n_ids);
    if (n_ids == 0)
        return;

    source = g_string_new(NULL);
    for (i = 0; i < n_ids; i++){
        GSignalQuery query;
        g_signal_query(ids[i], &query);

        if (query.signal_flags & G_SIGNAL_ACTION) {
            gchar* method_name = g_strdup(query.signal_name);
            gchar* p;
            GString* args;
            guint j;

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
                (query.n_params > 0) ? args->str + 1 : "", // Skip initial ','
                query.signal_name,
                args->str);

            g_free(method_name);
            g_string_free(args, TRUE);
        }
    }

    if (source->len > 0)
        rb_funcall(klass, rb_intern("module_eval"), 1, rb_str_new2(source->str));
    g_string_free(source, TRUE);
}

/**********************************************************************/

void
Init_gobject_gsignal(void)
{
    VALUE cSignalFlags, cSignalMatchType;

    RG_TARGET_NAMESPACE = rb_define_class_under(mGLib, "Signal", rb_cData);

    RG_DEF_METHOD(id, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(itype, 0);
    RG_DEF_METHOD(owner, 0);
    RG_DEF_METHOD(return_type, 0);
    RG_DEF_METHOD(param_types, 0);
    RG_DEF_METHOD(inspect, 0);

    RG_DEF_METHOD(add_emission_hook, -1);
    RG_DEF_METHOD(remove_emission_hook, 1);

    /* GSignalFlags */
    cSignalFlags = G_DEF_CLASS(G_TYPE_SIGNAL_FLAGS, "SignalFlags", mGLib);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_SIGNAL_FLAGS, "G_SIGNAL_");
    rb_define_const(cSignalFlags, "MASK", INT2NUM(G_SIGNAL_FLAGS_MASK));
    rb_define_const(RG_TARGET_NAMESPACE, "FLAGS_MASK", INT2NUM(G_SIGNAL_FLAGS_MASK));

    rbg_define_method(RG_TARGET_NAMESPACE, "run_first?", query_is_G_SIGNAL_RUN_FIRST, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "run_last?", query_is_G_SIGNAL_RUN_LAST, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "run_cleanup?", query_is_G_SIGNAL_RUN_CLEANUP, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "no_recurse?", query_is_G_SIGNAL_NO_RECURSE, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "detailed?", query_is_G_SIGNAL_DETAILED, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "action?", query_is_G_SIGNAL_ACTION, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "no_hooks?", query_is_G_SIGNAL_NO_HOOKS, 0);

    /* GConnectFlags */
    G_DEF_CLASS(G_TYPE_CONNECT_FLAGS, "ConnectFlags", mGLib);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_CONNECT_FLAGS, "G_");

    /* GSignalMatchType */
    cSignalMatchType = G_DEF_CLASS(G_TYPE_SIGNAL_MATCH_TYPE,
                                   "SignalMatchType", mGLib);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_SIGNAL_MATCH_TYPE, "G_SIGNAL_");
    rb_define_const(cSignalMatchType, "MASK", INT2NUM(G_SIGNAL_MATCH_MASK));
    rb_define_const(RG_TARGET_NAMESPACE, "MATCH_MASK", INT2NUM(G_SIGNAL_MATCH_MASK));

    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_STATIC_SCOPE", INT2FIX(G_SIGNAL_TYPE_STATIC_SCOPE));

    eNoSignalError = rb_define_class_under(mGLib, "NoSignalError", rb_eNameError);

    signal_func_table = rb_hash_new();
    rb_global_variable(&signal_func_table);

    rbg_define_method(mMetaInterface, "signal_new", gobj_s_signal_new, -1);
    rbg_define_method(mMetaInterface, "signals", gobj_s_signals, -1);
    rbg_define_method(mMetaInterface, "signal", gobj_s_signal, 1);

    rbg_define_method(cInstantiatable, "signal_has_handler_pending?",
                     gobj_sig_has_handler_pending, -1);
    rbg_define_method(cInstantiatable, "signal_connect", gobj_sig_connect, -1);
    rbg_define_method(cInstantiatable, "signal_connect_after",
                     gobj_sig_connect_after, -1);

#if 0
    rbg_define_method(cInstantiatable, "signal_invocation_hint",
                     gobj_sig_get_invocation_hint, 0);
#endif

    rbg_define_method(cInstantiatable, "signal_emit",
                     gobj_sig_emit, -1);
    rbg_define_method(cInstantiatable, "signal_emit_stop",
                     gobj_sig_emit_stop, 1);
    rbg_define_method(cInstantiatable, "signal_handler_block",
                     gobj_sig_handler_block, 1);
    rbg_define_method(cInstantiatable, "signal_handler_unblock",
                     gobj_sig_handler_unblock, 1);
    rbg_define_method(cInstantiatable, "signal_handler_disconnect",
                     gobj_sig_handler_disconnect, 1);

    rbg_define_method(cInstantiatable, "signal_handler_is_connected?",
                     gobj_sig_handler_is_connected, 1);

    rbg_define_singleton_method(cInstantiatable, "method_added",
                               gobj_s_method_added, 1);
}
