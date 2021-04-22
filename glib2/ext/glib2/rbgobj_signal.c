/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2021  Ruby-GNOME Project Team
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

static const rb_data_type_t rg_glib_signal_type = {
    "GLib::Signal",
    {
        NULL,
        NULL,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

VALUE
rbgobj_signal_new(guint id)
{
    VALUE rb_query;
    GSignalQuery *query;

    rb_query = TypedData_Make_Struct(RG_TARGET_NAMESPACE,
                                     GSignalQuery,
                                     &rg_glib_signal_type,
                                     query);
    g_signal_query(id, query);
    return rb_query;
}

static GSignalQuery *
rbgobj_signal_get_raw(VALUE rb_signal)
{
    GSignalQuery *query;
    TypedData_Get_Struct(rb_signal, GSignalQuery, &rg_glib_signal_type, query);
    return query;
}

static GHashTable *rbg_signal_func_table;
static GMutex rbg_signal_func_table_mutex;

void
rbgobj_set_signal_func(VALUE klass,
                       const gchar *signal_name,
                       GValToRValSignalFunc func)
{
    guint signal_id = g_signal_lookup(signal_name, CLASS2GTYPE(klass));
    g_mutex_lock(&rbg_signal_func_table_mutex);
    g_hash_table_insert(rbg_signal_func_table,
                        GUINT_TO_POINTER(signal_id),
                        func);
    g_mutex_unlock(&rbg_signal_func_table_mutex);
}

GValToRValSignalFunc
rbgobj_get_signal_func(guint signal_id)
{
    g_mutex_lock(&rbg_signal_func_table_mutex);
    GValToRValSignalFunc func =
        g_hash_table_lookup(rbg_signal_func_table,
                            GUINT_TO_POINTER(signal_id));
    g_mutex_unlock(&rbg_signal_func_table_mutex);
    return func;
}

static GHashTable *rbg_signal_call_func_table;
static GMutex rbg_signal_call_func_table_mutex;

void
rbgobj_set_signal_call_func(VALUE klass,
                            const gchar *signal_name,
                            RGClosureCallFunc func)
{
    guint signal_id = g_signal_lookup(signal_name, CLASS2GTYPE(klass));
    g_mutex_lock(&rbg_signal_call_func_table_mutex);
    g_hash_table_insert(rbg_signal_call_func_table,
                        GUINT_TO_POINTER(signal_id),
                        func);
    g_mutex_unlock(&rbg_signal_call_func_table_mutex);
}

RGClosureCallFunc
rbgobj_get_signal_call_func(guint signal_id)
{
    g_mutex_lock(&rbg_signal_call_func_table_mutex);
    RGClosureCallFunc func =
        g_hash_table_lookup(rbg_signal_call_func_table,
                            GUINT_TO_POINTER(signal_id));
    g_mutex_unlock(&rbg_signal_call_func_table_mutex);
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
        args->result[i] = rbgobj_gtype_from_ruby(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbg_rval2gtypes_rescue(VALUE value, VALUE error)
{
    g_free(((struct rval2gtypes_args *)value)->result);

    rb_exc_raise(error);
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
gobj_s_define_signal(int argc, VALUE* argv, VALUE self)
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
    VALUE rb_signal;

    rb_scan_args(argc, argv, "4*",
                 &rbsignal_name, &rbsignal_flags, &accumulator, &rbreturn_type, &params);

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "not a registered class: %s",
                 rb_class2name(self));

    if (SYMBOL_P(rbsignal_name))
        rbsignal_name = rb_str_new2(rb_id2name(SYM2ID(rbsignal_name)));
    signal_name = RVAL2CSTR(rbsignal_name);

    signal_flags = RVAL2GFLAGS(rbsignal_flags, G_TYPE_SIGNAL_FLAGS);

    {
        VALUE rb_method_name;
        VALUE proc;
        ID method_id;

        rb_method_name =
            rb_str_concat(rb_str_new_cstr(default_handler_method_prefix),
                          rbsignal_name);
        method_id = rb_to_id(rb_method_name);

        proc = rb_funcall(mMetaInterface, rb_intern("signal_callback"), 2,
                          self, ID2SYM(method_id));

        class_closure = g_rclosure_new(proc, Qnil, NULL);
        g_rclosure_set_tag(class_closure, RVAL2CSTR(rb_method_name));
    }

    return_type = rbgobj_gtype_from_ruby(rbreturn_type);
    param_types = RVAL2GTYPES_ACCEPT_NIL(params, n_params);

    signal = g_signal_newv(signal_name,
                           cinfo->gtype,
                           signal_flags,
                           class_closure,
                           NIL_P(accumulator) ? NULL : accumulator_func,
                           NIL_P(accumulator) ? NULL : (gpointer)accumulator,
                           NULL, /* c_marshaller */
                           return_type,
                           (guint)n_params,
                           param_types);

    g_free(param_types);

    if (!signal)
        rb_raise(rb_eRuntimeError, "g_signal_newv failed");

    rb_signal = rbgobj_signal_new(signal);
    if (!NIL_P(accumulator))
        rbgobj_add_relative(rb_signal, accumulator);
    g_rclosure_attach(class_closure, rb_signal);
    rbgobj_add_relative(self, rb_signal);

    return rb_signal;
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

    return rbgobj_signal_new(sig_id);
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

    {
        ID id_create_signal_handler;
        const char *normalized_signal_name;

        CONST_ID(id_create_signal_handler, "create_signal_handler");
        normalized_signal_name = g_signal_name(signal_id);
        func = rb_funcall(self,
                          id_create_signal_handler,
                          2,
                          rb_str_new_cstr(normalized_signal_name),
                          rb_block_proc());
    }
    {
        RGClosureCallFunc call_func;
        call_func = rbgobj_get_signal_call_func(signal_id);
        if (call_func) {
            rclosure = g_rclosure_new_call(func,
                                           rest,
                                           call_func);
        } else {
            rclosure = g_rclosure_new(func,
                                      rest,
                                      rbgobj_get_signal_func(signal_id));
        }
    }
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
                       rbgobj_signal_new(hint->signal_id),
                       hint->detail ? rb_str_new2(g_quark_to_string(hint->detail)) : Qnil,
                       INT2NUM(hint->run_type));
}
#endif

struct emit_arg {
    VALUE self;
    VALUE args;

    GSignalQuery query;
    GQuark detail;
    GArray *instance_and_params;
};

static VALUE
emit_body(VALUE rb_arg)
{
    struct emit_arg *arg = (struct emit_arg *)rb_arg;

    gsize value_index = 0;
    GValue *gself = &g_array_index(arg->instance_and_params,
                                   GValue,
                                   value_index);
    g_value_init(gself, G_TYPE_FROM_INSTANCE(RVAL2GOBJ(arg->self)));
    rbgobj_rvalue_to_gvalue(arg->self, gself);
    value_index++;

    {
        guint i;
        for (i = 0; i < arg->query.n_params; i++){
            GType gtype = arg->query.param_types[i] & ~G_SIGNAL_TYPE_STATIC_SCOPE;

            GValue *gparam = &g_array_index(arg->instance_and_params,
                                            GValue,
                                            value_index);
            g_value_init(gparam,  gtype);
            rbgobj_rvalue_to_gvalue(rb_ary_entry(arg->args, i), gparam);
            value_index++;
        }
    }

    {
        gboolean use_ret = (arg->query.return_type != G_TYPE_NONE);
        GValue return_value = G_VALUE_INIT;

        if (use_ret)
            g_value_init(&return_value,
                         arg->query.return_type & ~G_SIGNAL_TYPE_STATIC_SCOPE);

        g_signal_emitv((GValue *)(arg->instance_and_params->data),
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
emit_ensure(VALUE rb_arg)
{
    struct emit_arg *arg = (struct emit_arg *)rb_arg;
    g_array_unref(arg->instance_and_params);
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
    arg.instance_and_params = g_array_sized_new(FALSE,
                                                TRUE,
                                                sizeof(GValue),
                                                1 + arg.query.n_params);
    g_array_set_clear_func(arg.instance_and_params,
                           (GDestroyNotify)g_value_unset);
    g_array_set_size(arg.instance_and_params, 1 + arg.query.n_params);

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
chain_from_overridden_body(VALUE rb_arg)
{
    struct emit_arg *arg = (struct emit_arg *)rb_arg;
    gsize value_index = 0;
    GValue *gself = &g_array_index(arg->instance_and_params,
                                   GValue,
                                   value_index);
    g_value_init(gself, G_TYPE_FROM_INSTANCE(RVAL2GOBJ(arg->self)));
    rbgobj_rvalue_to_gvalue(arg->self, gself);
    value_index++;

    {
        guint i;
        for (i = 0; i < arg->query.n_params; i++) {
            GType gtype = arg->query.param_types[i] & ~G_SIGNAL_TYPE_STATIC_SCOPE;
            GValue *gparam = &g_array_index(arg->instance_and_params,
                                            GValue,
                                            value_index);
            g_value_init(gparam, gtype);
            rbgobj_rvalue_to_gvalue(rb_ary_entry(arg->args, i), gparam);
            value_index++;
        }
    }

    {
        gboolean use_ret = (arg->query.return_type != G_TYPE_NONE);
        GValue return_value = G_VALUE_INIT;

        if (use_ret)
            g_value_init(&return_value,
                         arg->query.return_type & ~G_SIGNAL_TYPE_STATIC_SCOPE);

        g_signal_chain_from_overridden((GValue *)(arg->instance_and_params->data),
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
    arg.instance_and_params = g_array_sized_new(FALSE,
                                                TRUE,
                                                sizeof(GValue),
                                                1 + argc);
    g_array_set_clear_func(arg.instance_and_params,
                           (GDestroyNotify)g_value_unset);
    g_array_set_size(arg.instance_and_params, 1 + argc);

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
        VALUE proc = rb_funcall(mMetaInterface, rb_intern("signal_callback"), 2,
                                klass, id);
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
    return rbgobj_signal_new(sig_id);
}

static VALUE
rg_id(VALUE self)
{
    return UINT2NUM(rbgobj_signal_get_raw(self)->signal_id);
}

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(rbgobj_signal_get_raw(self)->signal_name);
}

static VALUE
rg_itype(VALUE self)
{
    return rbgobj_gtype_new(rbgobj_signal_get_raw(self)->itype);
}

static VALUE
rg_owner(VALUE self)
{
    return GTYPE2CLASS(rbgobj_signal_get_raw(self)->itype);
}

static VALUE
rg_return_type(VALUE self)
{
    return rbgobj_gtype_new(rbgobj_signal_get_raw(self)->return_type);
}

static VALUE
rg_flags(VALUE self)
{
    return UINT2NUM(rbgobj_signal_get_raw(self)->signal_flags);
}

static VALUE
rg_param_types(VALUE self)
{
    GSignalQuery *query;
    VALUE result;
    guint i;

    query = rbgobj_signal_get_raw(self);
    result = rb_ary_new_capa(query->n_params);
    for (i = 0; i < query->n_params; i++)
        rb_ary_store(result, i, rbgobj_gtype_new(query->param_types[i]));

    return result;
}

static VALUE
rg_inspect(VALUE self)
{
    GSignalQuery *query;
    query = rbgobj_signal_get_raw(self);
    return rb_sprintf("#<%" PRIsVALUE " %+" PRIsVALUE "#%s>",
                      CLASS_OF(self),
                      GTYPE2CLASS(query->itype),
                      query->signal_name);
}

#define query_is_flag(flag) \
    static VALUE \
    query_is_##flag(VALUE self) \
    { \
        return CBOOL2RVAL(rbgobj_signal_get_raw(self)->signal_flags & flag); \
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
    GClosure *closure = data;
    GValue ret_val = G_VALUE_INIT;
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
    gulong hook_id;
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
    GSignalQuery *query;
    VALUE proc;
    gulong hook_id;
    GQuark detail = 0;
    GClosure* closure;

    query = rbgobj_signal_get_raw(self);

    if (query->signal_flags & G_SIGNAL_DETAILED) {
        VALUE detail_obj;
        if (rb_scan_args(argc, argv, "01&", &detail_obj, &proc) == 1) {
            if (SYMBOL_P(detail_obj))
                detail = g_quark_from_string(rb_id2name(SYM2ID(detail_obj)));
            else
                detail = g_quark_from_string(RVAL2CSTR(detail_obj));
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
    g_signal_remove_emission_hook(rbgobj_signal_get_raw(self)->signal_id,
                                  NUM2ULONG(hook_id));
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
        rb_funcall(klass, rb_intern("module_eval"), 3,
                   rb_str_new2(source->str),
                   rb_str_new2(__FILE__),
                   INT2NUM(__LINE__));
    g_string_free(source, TRUE);
}

/**********************************************************************/

void
Init_gobject_gsignal(void)
{
    VALUE cSignalFlags, cSignalMatchType;

    RG_TARGET_NAMESPACE = rb_define_class_under(mGLib, "Signal", rb_cObject);

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

    rbg_signal_func_table = g_hash_table_new(g_direct_hash, g_direct_equal);
    g_mutex_init(&rbg_signal_func_table_mutex);

    rbg_signal_call_func_table = g_hash_table_new(g_direct_hash, g_direct_equal);
    g_mutex_init(&rbg_signal_call_func_table_mutex);

    rbg_define_method(mMetaInterface, "define_signal", gobj_s_define_signal, -1);
    rb_define_alias(mMetaInterface, "signal_new", "define_signal");
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
