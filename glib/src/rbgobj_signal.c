/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_signal.c -

  $Author: sakai $
  $Date: 2002/09/01 13:19:21 $
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

    method_id = rb_to_id(rb_str_concat(rb_str_new2("do_"), signal_name));
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

/* TODO: handle 'detail' */
static VALUE
gobj_sig_emit(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE signal;
    VALUE rest;
    guint signal_id;

    rb_scan_args(argc, argv, "1*", &signal, &rest);

    if (rb_respond_to(signal, rb_intern("to_str"))){
        StringValue(signal);
        signal_id = g_signal_lookup(StringValuePtr(signal),
                                    CLASS2GTYPE(CLASS_OF(self)));
    } else {
        signal_id = NUM2UINT(signal);
    }

    return emit_impl(self, signal_id, 0, rest);
}

/* TODO */
static VALUE
gobj_sig_emit_by_name(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rb_notimplement();
    return self;
}

static VALUE
gobj_sig_emit_stop(self, sig_id)
    VALUE self, sig_id;
{
    g_signal_stop_emission(RVAL2GOBJ(self), NUM2INT(sig_id), 0);
    return self;
}

static VALUE
gobj_sig_emit_stop_by_name(self, sig_name)
    VALUE self, sig_name;
{
    GObject* gobj = RVAL2GOBJ(self);
    StringValue(sig_name);
    g_signal_stop_emission(gobj,
                           g_signal_lookup(StringValuePtr(sig_name), G_OBJECT_TYPE(gobj)), 0);
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

/**********************************************************************/

void
Init_gobject_gsignal()
{
    VALUE cGObject = GTYPE2CLASS(G_TYPE_OBJECT);

    id_send = rb_intern("__send__");

    rb_define_singleton_method(cGObject, "signal_new", gobj_s_signal_new, -1);
    rb_define_singleton_method(cGObject, "signal_list", gobj_s_signal_list, 0);
    rb_define_singleton_method(cGObject, "signal_lookup", gobj_s_signal_lookup, 1);

    rb_define_method(cGObject, "signal_connect", gobj_sig_connect, -1);
    rb_define_method(cGObject, "signal_connect_after",
                     gobj_sig_connect_after, -1);

    rb_define_method(cGObject, "signal_emit",
                     gobj_sig_emit, -1);
    rb_define_method(cGObject, "signal_emit_by_name",
                     gobj_sig_emit_by_name, -1);
    rb_define_method(cGObject, "signal_emit_stop",
                     gobj_sig_emit_stop, 1);
    rb_define_method(cGObject, "signal_emit_stop_by_name",
                     gobj_sig_emit_stop_by_name, 1);
    rb_define_method(cGObject, "signal_handler_block",
                     gobj_sig_handler_block, 1);
    rb_define_method(cGObject, "signal_handler_unblock",
                     gobj_sig_handler_unblock, 1);
    rb_define_method(cGObject, "signal_disconnect",
                     gobj_sig_handler_disconnect, 1);
}
