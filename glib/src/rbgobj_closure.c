/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_closure.c -

  $Author: sakai $
  $Date: 2002/08/10 16:07:09 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static ID id_call;

typedef struct _GRClosure GRClosure;

struct _GRClosure
{
    GClosure closure;
    VALUE callback;
    VALUE extra_args;
};

static void
rclosure_marshal(GClosure*      closure,
                 GValue         *return_value,
                 guint           n_param_values,
                 const GValue   *param_values,
                 gpointer        invocation_hint,
                 gpointer        marshal_data)
{
    int i;
    VALUE ret;

    VALUE args = rb_ary_new2(n_param_values);
    
    for (i = 0; i < n_param_values; i++)
        rb_ary_store(args, i, GVAL2RVAL(&param_values[i]));
    if (!NIL_P(((GRClosure*)closure)->extra_args))
        args = rb_ary_concat(args, ((GRClosure*)closure)->extra_args);

    ret = rb_apply(((GRClosure*)closure)->callback, id_call, args);

    if (return_value)
        rbgobj_rvalue_to_gvalue(ret, return_value);
}

static VALUE rclosure_marker_list;

static void
marker_remove(gpointer data, GClosure* closure)
{
    VALUE obj = (VALUE)data;
    rb_hash_aset(rclosure_marker_list, obj, Qnil);
    ((GRClosure*)closure)->callback   = Qnil;
    ((GRClosure*)closure)->extra_args = Qnil;
}

static void
rclosure_mark(GRClosure* closure)
{
    rb_gc_mark(closure->callback);
    rb_gc_mark(closure->extra_args);
}

GClosure*
g_rclosure_new(VALUE callback_proc, VALUE extra_args)
{
    GRClosure* closure;
    VALUE marker;

    closure = (GRClosure*)g_closure_new_simple(sizeof(GRClosure), NULL);

    closure->callback   = callback_proc;
    closure->extra_args = extra_args;

    g_closure_set_marshal((GClosure*)closure, &rclosure_marshal);

    marker = Data_Wrap_Struct(rb_cData, rclosure_mark, NULL, closure);
    rb_hash_aset(rclosure_marker_list, marker, Qtrue);
    g_closure_add_invalidate_notifier((GClosure*)closure, (gpointer)marker,
                                      &marker_remove);

    return (GClosure*)closure;
}

static void
Init_rclosure()
{
    rb_global_variable(&rclosure_marker_list);
    rclosure_marker_list = rb_hash_new();

    id_call = rb_intern("call");
}

/**********************************************************************/

void Init_gobject_gclosure()
{
    Init_rclosure();
}
