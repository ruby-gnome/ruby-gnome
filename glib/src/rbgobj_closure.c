/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_closure.c -

  $Author: sakai $
  $Date: 2002/06/10 18:51:43 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

VALUE rbgobj_cClosure;

#if 0
static
VALUE closure_invoke(argc, argv, self)
    int argc;
    VALUE* argvl
    VALUE self;
{
    int i;
    GValue* params = ALLOCA_N(GValue, argc);

    for (i = 0; i < argc; i++)
        rbgobj_rvalue_to_gvalue(argv[i], &params[i]);
    
    g_closure_invoke();
}
#endif

/**********************************************************************/

typedef struct _GRClosure GRClosure;

struct _GRClosure
{
    GClosure closure;
    VALUE callback;
};

void rclosure_marshal(GClosure*      closure,
                      GValue         *return_value,
                      guint           n_param_values,
                      const GValue   *param_values,
                      gpointer        invocation_hint,
                      gpointer        marshal_data)
{
    int i;
    VALUE* params = ALLOCA_N(VALUE, n_param_values);
    VALUE ret;
    
    for (i = 0; i < n_param_values; i++)
        params[i] = rbgobj_gvalue_to_rvalue(&param_values[i]);

    ret = rb_apply(((GRClosure*)closure)->callback,
                   rb_intern("call"),
                   rb_ary_new4(n_param_values, params));

    if (return_value)
        rbgobj_rvalue_to_gvalue(ret, return_value);
}

static VALUE rclosure_list;

static
void rclosure_remove(GClosure* closure, gpointer data)
{
    VALUE obj = (VALUE)data;
    rb_hash_aset(rclosure_list, obj, Qnil);
}

static
void rclosure_mark(GRClosure* closure)
{
    rb_gc_mark(closure->callback);
}

GClosure* g_rclosure_new(VALUE callback_proc)
{
    GRClosure* closure;
    VALUE obj;

    closure = (GRClosure*)g_closure_new_simple(sizeof(GRClosure), NULL);

    closure->callback = callback_proc;
    g_closure_set_marshal((GClosure*)closure, &rclosure_marshal);

    obj = Data_Wrap_Struct(rbgobj_cClosure, &rclosure_mark, NULL, closure);
    rb_hash_aset(rclosure_list, obj, callback_proc);
    g_closure_add_finalize_notifier((GClosure*)closure, &rclosure_remove,
                                    (gpointer)obj);

    return (GClosure*)closure;
}

static
void Init_rclosure()
{
    rb_global_variable(&rclosure_list);
    rclosure_list = rb_hash_new();
}

/**********************************************************************/

void Init_gobject_gclosure()
{
    rbgobj_cClosure = rb_define_class_under(mGLib, "Closure", rb_cData);
    /* rb_define_method(rbgobj_cClosure, "call", &closure_invoke, -1); */

    Init_rclosure();
}
