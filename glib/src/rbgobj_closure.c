/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_closure.c -

  $Author: sakai $
  $Date: 2007/07/07 08:15:26 $

  Copyright (C) 2002-2006  Ruby-GNOME2 Project
  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include "global.h"

static ID id_call;
static gboolean rclosure_initialized = FALSE;

typedef struct _GRClosure GRClosure;
struct _GRClosure
{
    GClosure closure;
    VALUE callback;
    VALUE extra_args;
    VALUE rb_holder;
    gint count;
    GList *objects;
    GValToRValSignalFunc g2r_func;
};

static VALUE
rclosure_default_g2r_func(num, values)
    guint num;
    const GValue* values;
{
    int i;
    VALUE args = rb_ary_new2(num);
    for (i = 0; i < num; i++)
        rb_ary_store(args, i, GVAL2RVAL(&values[i]));
    return args;
}

struct marshal_arg
{
    GClosure*       closure;
    GValue*         return_value;
    guint           n_param_values;
    const GValue*   param_values;
    gpointer        invocation_hint;
    gpointer        marshal_data;
};

static int
rclosure_alive_p(GRClosure *rclosure)
{
    return (rclosure->count > 0 && !NIL_P(rclosure->rb_holder));
}

static VALUE
rclosure_marshal_do(VALUE arg_)
{
    struct marshal_arg *arg;
    GRClosure*      rclosure;
    GValue*         return_value;
    guint           n_param_values;
    const GValue*   param_values;
    /* gpointer        invocation_hint;*/
    /* gpointer        marshal_data; */

    VALUE ret = Qnil;
    VALUE args;
    GValToRValSignalFunc func;

    arg = (struct marshal_arg*)arg_;
    rclosure        = (GRClosure *)(arg->closure);
    return_value    = arg->return_value;   
    n_param_values  = arg->n_param_values; 
    param_values    = arg->param_values;
    /* invocation_hint = arg->invocation_hint; */
    /* marshal_data    = arg->marshal_data; */

    if (rclosure->g2r_func){
        func = (GValToRValSignalFunc)rclosure->g2r_func;
    } else { 
        func = (GValToRValSignalFunc)rclosure_default_g2r_func;
    }
    args = (*func)(n_param_values, param_values);

    if (rclosure_alive_p(rclosure)) {
        VALUE callback, extra_args;
        callback = rclosure->callback;
        extra_args = rclosure->extra_args;

        if (!NIL_P(extra_args)) {
            args = rb_ary_concat(args, extra_args);
        }

        ret = rb_apply(callback, id_call, args);
    } else {
        rb_warn("GRClosure invoking callback: already destroyed");
    }

    if (return_value && G_VALUE_TYPE(return_value))
        rbgobj_rvalue_to_gvalue(ret, return_value);

    return Qnil;
}

static void
rclosure_marshal(GClosure*       closure,
                 GValue*         return_value,
                 guint           n_param_values,
                 const GValue*   param_values,
                 gpointer        invocation_hint,
                 gpointer        marshal_data)
{
    struct marshal_arg arg;

    if (!rclosure_initialized) {
        g_closure_invalidate(closure);
        return;
    }

    arg.closure         = closure;
    arg.return_value    = return_value;
    arg.n_param_values  = n_param_values;
    arg.param_values    = param_values;
    arg.invocation_hint = invocation_hint;
    arg.marshal_data    = marshal_data;

    G_PROTECT_CALLBACK(rclosure_marshal_do, &arg);
}

static void rclosure_weak_notify(gpointer data, GObject* where_the_object_was);

static void
rclosure_unref(GRClosure *rclosure)
{
    rclosure->count--;

    if (!rclosure_alive_p(rclosure)) {
        GList *next;
        for (next = rclosure->objects; next; next = next->next) {
            GObject *object = G_OBJECT(next->data);
            VALUE obj = rbgobj_ruby_object_from_instance2(object, FALSE);
            if (!NIL_P(rclosure->rb_holder) && !NIL_P(obj))
                G_CHILD_REMOVE(obj, rclosure->rb_holder);
            g_object_weak_unref(object, rclosure_weak_notify, rclosure);
        }
        g_list_free(rclosure->objects);
        rclosure->objects = NULL;
        if (!NIL_P(rclosure->rb_holder)) {
            RDATA(rclosure->rb_holder)->dmark = NULL;
            RDATA(rclosure->rb_holder)->dfree = NULL;
            rclosure->rb_holder = Qnil;
        }
    }
}

static void
rclosure_invalidate(gpointer data, GClosure* closure)
{
    GRClosure *rclosure = (GRClosure*)closure;

    if (rclosure->count > 0) {
        rclosure->count = 1;
        rclosure_unref(rclosure);
    }
}

static void
gr_closure_holder_mark(GRClosure *rclosure)
{
    rb_gc_mark(rclosure->callback);
    rb_gc_mark(rclosure->extra_args);
}

static void
gr_closure_holder_free(GRClosure *rclosure)
{
    rclosure_invalidate(NULL, (GClosure*)rclosure);
}

GClosure*
g_rclosure_new(VALUE callback_proc, VALUE extra_args, GValToRValSignalFunc g2r_func)
{
    GRClosure* closure;

    closure = (GRClosure*)g_closure_new_simple(sizeof(GRClosure), NULL);

    closure->count      = 1;
    closure->g2r_func   = g2r_func;
    closure->objects    = NULL;
    closure->callback   = callback_proc;
    closure->extra_args = extra_args;
    closure->rb_holder  = Data_Wrap_Struct(rb_cData,
                                           gr_closure_holder_mark,
                                           gr_closure_holder_free,
                                           closure);

    g_closure_set_marshal((GClosure*)closure, &rclosure_marshal);
    g_closure_add_invalidate_notifier((GClosure*)closure, NULL,
                                      &rclosure_invalidate);

    return (GClosure*)closure;
}

static void
rclosure_weak_notify(gpointer data, GObject* where_the_object_was)
{
    GRClosure *rclosure = data;
    if (rclosure_alive_p(rclosure)) {
        rclosure->objects =
            g_list_remove(rclosure->objects, where_the_object_was);
        rclosure_unref(rclosure);
    }
}

void
g_rclosure_attach(GClosure *closure, VALUE object)
{
    static VALUE mGLibObject = (VALUE)NULL;
    GRClosure *rclosure = (GRClosure *)closure;

    G_CHILD_ADD(object, rclosure->rb_holder);

    if (!mGLibObject) {
        mGLibObject = rb_const_get(mGLib, rb_intern("Object"));
    }
    if (rb_obj_is_kind_of(object, mGLibObject)) {
        GObject *gobject;
        gobject = RVAL2GOBJ(object);
        rclosure->count++;
        g_object_weak_ref(gobject, rclosure_weak_notify, rclosure);
        rclosure->objects = g_list_prepend(rclosure->objects, gobject);
    }
}

static void
rclosure_end_proc(VALUE _)
{
    rclosure_initialized = FALSE;
}

static void
Init_rclosure()
{
    id_call = rb_intern("call");
    rclosure_initialized = TRUE;
    rb_set_end_proc(rclosure_end_proc, Qnil);
}

/**********************************************************************/

static VALUE
closure_initialize(self)
    VALUE self;
{
    GClosure* closure = g_rclosure_new(G_BLOCK_PROC(), Qnil, NULL);
    G_INITIALIZE(self, closure);
    g_closure_sink(closure);
    return self;
}

static VALUE
closure_in_marshal(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return closure->in_marshal ? Qtrue : Qfalse;
}

static VALUE
closure_is_invalid(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return closure->is_invalid ? Qtrue : Qfalse;
}

static VALUE
closure_invalidate(self)
    VALUE self;
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    g_closure_invalidate(closure);
    return self;
}

static void
Init_closure()
{
    VALUE cClosure = G_DEF_CLASS(G_TYPE_CLOSURE, "Closure", mGLib);
    rb_define_method(cClosure, "initialize", closure_initialize, 0);
    rb_define_method(cClosure, "in_marshal?", closure_in_marshal, 0);
    rb_define_method(cClosure, "invalid?", closure_is_invalid, 0);
    rb_define_method(cClosure, "invalidate", closure_invalidate, 0);
}

/**********************************************************************/

void Init_gobject_gclosure()
{
    Init_rclosure();
    Init_closure();
}

