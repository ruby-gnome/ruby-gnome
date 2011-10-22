/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006  Ruby-GNOME2 Project
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

static ID id_call, id_closures;
static gboolean rclosure_initialized = FALSE;

#define TAG_SIZE 64
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
    gchar tag[TAG_SIZE];
};

static VALUE
rclosure_default_g2r_func(guint num, const GValue *values)
{
    guint i;
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
        rb_warn("GRClosure invoking callback: already destroyed: %s",
                rclosure->tag[0] ? rclosure->tag : "(anonymous)");
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
            g_object_weak_unref(object, rclosure_weak_notify, rclosure);
        }
        g_list_free(rclosure->objects);
        rclosure->objects = NULL;
        if (!NIL_P(rclosure->rb_holder)) {
            DATA_PTR(rclosure->rb_holder) = NULL;
            rclosure->rb_holder = Qnil;
        }
    }
}

static void
rclosure_invalidate(G_GNUC_UNUSED gpointer data, GClosure *closure)
{
    GRClosure *rclosure = (GRClosure*)closure;

    if (rclosure->count > 0) {
        GList *next;

        rclosure->count = 1;
        for (next = rclosure->objects; next; next = next->next) {
            GObject *object = G_OBJECT(next->data);
            VALUE obj = rbgobj_ruby_object_from_instance2(object, FALSE);
            if (!NIL_P(rclosure->rb_holder) && !NIL_P(obj))
                G_REMOVE_RELATIVE(obj, id_closures, rclosure->rb_holder);
        }

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
    if (!rclosure)
        return;

    if (rclosure->count > 0) {
        rclosure->count = 1;

        /* No need to remove us from the relatives hash of our objects, as
         * those aren't alive anymore anyway */
        rclosure_unref(rclosure);
    }
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
    closure->tag[0]     = '\0';

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

    G_RELATIVE2(object, Qnil, id_closures, rclosure->rb_holder);

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

void
g_rclosure_set_tag(GClosure *closure, const gchar *tag)
{
    GRClosure *rclosure = (GRClosure *)closure;

    if (tag) {
        strncpy(rclosure->tag, tag, TAG_SIZE);
        rclosure->tag[TAG_SIZE - 1] = '\0';
    } else {
        rclosure->tag[0] = '\0';
    }
}

static void
rclosure_end_proc(G_GNUC_UNUSED VALUE _)
{
    rclosure_initialized = FALSE;
}

static void
Init_rclosure(void)
{
    id_call = rb_intern("call");
    id_closures = rb_intern("closures");
    rclosure_initialized = TRUE;
    rb_set_end_proc(rclosure_end_proc, Qnil);
}

/**********************************************************************/

static VALUE
closure_initialize(VALUE self)
{
    GClosure* closure = g_rclosure_new(rb_block_proc(), Qnil, NULL);
    G_INITIALIZE(self, closure);
    g_closure_sink(closure);
    return self;
}

static VALUE
closure_in_marshal(VALUE self)
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return CBOOL2RVAL(closure->in_marshal);
}

static VALUE
closure_is_invalid(VALUE self)
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return CBOOL2RVAL(closure->is_invalid);
}

static VALUE
closure_invalidate(VALUE self)
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    g_closure_invalidate(closure);
    return self;
}

static void
Init_closure(void)
{
    VALUE cClosure = G_DEF_CLASS(G_TYPE_CLOSURE, "Closure", mGLib);
    rb_define_method(cClosure, "initialize", closure_initialize, 0);
    rb_define_method(cClosure, "in_marshal?", closure_in_marshal, 0);
    rb_define_method(cClosure, "invalid?", closure_is_invalid, 0);
    rb_define_method(cClosure, "invalidate", closure_invalidate, 0);
}

/**********************************************************************/

void
Init_gobject_gclosure(void)
{
    Init_rclosure();
    Init_closure();
}

