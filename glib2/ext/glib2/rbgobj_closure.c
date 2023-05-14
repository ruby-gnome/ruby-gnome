/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2022  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE cClosure

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
    GList *objects;
    GValToRValSignalFunc g2r_func;
    RGClosureCallFunc call_func;
    gchar tag[TAG_SIZE];
};

static VALUE
rclosure_default_g2r_func(guint num, const GValue *values)
{
    guint i;
    VALUE args = rb_ary_new_capa(num);
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
    return !NIL_P(rclosure->rb_holder);
}

static VALUE
rclosure_marshal_do(VALUE arg_)
{
    VALUE ret = Qnil;
    struct marshal_arg *arg;
    GRClosure  *rclosure;
    GValue *return_value;

    arg = (struct marshal_arg *)arg_;
    rclosure = (GRClosure *)(arg->closure);
    return_value = arg->return_value;

    if (rclosure_alive_p(rclosure)) {
        guint n_param_values;
        const GValue *param_values;
        /* gpointer invocation_hint;*/
        /* gpointer marshal_data; */
        GValToRValSignalFunc g2r_func;
        VALUE callback;
        VALUE extra_args;
        VALUE args;

        n_param_values  = arg->n_param_values;
        param_values = arg->param_values;
        /* invocation_hint = arg->invocation_hint; */
        /* marshal_data = arg->marshal_data; */

        callback = rclosure->callback;
        extra_args = rclosure->extra_args;

        if (rclosure->call_func) {
            RGClosureCallData data;
            data.return_value = return_value;
            data.n_param_values = n_param_values;
            data.param_values = param_values;
            data.callback = callback;
            data.extra_args = extra_args;
            rclosure->call_func(&data);
            return Qnil;
        }

        if (rclosure->g2r_func) {
            g2r_func = (GValToRValSignalFunc)rclosure->g2r_func;
        } else {
            g2r_func = (GValToRValSignalFunc)rclosure_default_g2r_func;
        }
        args = (*g2r_func)(n_param_values, param_values);

        if (!NIL_P(extra_args)) {
            args = rb_ary_concat(args, extra_args);
        }

        ret = rb_apply(callback, id_call, args);
    } else {
        g_warning("GRClosure invoking callback: already destroyed: %s",
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
rclosure_invalidate(G_GNUC_UNUSED gpointer data, GClosure *closure)
{
    GRClosure *rclosure = (GRClosure *)closure;

    GList *next;
    for (next = rclosure->objects; next; next = next->next) {
        GObject *object = G_OBJECT(next->data);
        g_object_weak_unref(object, rclosure_weak_notify, rclosure);
        g_closure_unref(closure);
        VALUE obj = rbgobj_ruby_object_from_instance2(object, FALSE);
        if (!NIL_P(rclosure->rb_holder) && !NIL_P(obj)) {
            rbgobj_remove_relative(obj, id_closures, rclosure->rb_holder);
        }
    }
    g_list_free(rclosure->objects);
    rclosure->objects = NULL;

    if (!NIL_P(rclosure->rb_holder)) {
        RTYPEDDATA_DATA(rclosure->rb_holder) = NULL;
        rclosure->rb_holder = Qnil;
    }
}

static void
gr_closure_holder_mark(void *data)
{
    GRClosure *rclosure = data;

    if (!rclosure)
        return;

    rb_gc_mark(rclosure->callback);
    rb_gc_mark(rclosure->extra_args);
}

static void
gr_closure_holder_free(void *data)
{
    GRClosure *rclosure = data;

    if (!rclosure)
        return;

    GClosure *closure = (GClosure *)rclosure;
    gboolean last_reference = (closure->ref_count == 1);
    g_closure_unref(closure);
    if (!last_reference) {
        g_closure_invalidate(closure);
    }
}

static const rb_data_type_t rbg_closure_holder_type = {
    "GLib::ClosureHolder",
    {
        gr_closure_holder_mark,
        gr_closure_holder_free,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY | RUBY_TYPED_FROZEN_SHAREABLE,
};

static GClosure *
g_rclosure_new_raw(VALUE callback_proc,
                   VALUE extra_args,
                   GValToRValSignalFunc g2r_func,
                   RGClosureCallFunc call_func)
{
    GClosure *closure = g_closure_new_simple(sizeof(GRClosure), NULL);

    GRClosure *rclosure = (GRClosure *)closure;

    rclosure->g2r_func   = g2r_func;
    rclosure->call_func  = call_func;
    rclosure->objects    = NULL;
    rclosure->callback   = callback_proc;
    rclosure->extra_args = extra_args;
    rclosure->rb_holder  = TypedData_Wrap_Struct(rb_cObject,
                                                 &rbg_closure_holder_type,
                                                 rclosure);
    rclosure->tag[0]     = '\0';

    g_closure_ref(closure);
    g_closure_sink(closure);
    g_closure_set_marshal(closure, &rclosure_marshal);
    g_closure_add_invalidate_notifier(closure, NULL, rclosure_invalidate);

    return closure;
}

GClosure *
g_rclosure_new(VALUE callback_proc,
               VALUE extra_args,
               GValToRValSignalFunc g2r_func)
{
    return g_rclosure_new_raw(callback_proc, extra_args, g2r_func, NULL);
}

GClosure *
g_rclosure_new_call(VALUE callback_proc,
                    VALUE extra_args,
                    RGClosureCallFunc call_func)
{
    return g_rclosure_new_raw(callback_proc,
                              extra_args,
                              NULL,
                              call_func);
}

static void
rclosure_weak_notify(gpointer data, GObject *where_the_object_was)
{
    GRClosure *rclosure = data;
    rclosure->objects = g_list_remove(rclosure->objects, where_the_object_was);
    g_closure_unref((GClosure *)rclosure);
}

void
g_rclosure_attach(GClosure *closure, VALUE object)
{
    GRClosure *rclosure = (GRClosure *)closure;
    rbgobj_add_relative_removable(object,
                                  Qnil,
                                  id_closures,
                                  rclosure->rb_holder);
}

void
g_rclosure_attach_gobject(GClosure *closure, VALUE object)
{
    GRClosure *rclosure = (GRClosure *)closure;
    g_rclosure_attach(closure, object);

    GObject *gobject = RVAL2GOBJ(object);
    g_closure_ref(closure);
    g_object_weak_ref(gobject, rclosure_weak_notify, rclosure);
    rclosure->objects = g_list_prepend(rclosure->objects, gobject);
}

void
g_rclosure_detach(GClosure *closure, VALUE object)
{
    GRClosure *rclosure = (GRClosure *)closure;
    rbgobj_remove_relative(object, id_closures, rclosure->rb_holder);
}

void
g_rclosure_detach_gobject(GClosure *closure, VALUE object)
{
    GRClosure *rclosure = (GRClosure *)closure;
    g_rclosure_detach(closure, object);

    GObject *gobject = RVAL2GOBJ(object);
    g_object_weak_unref(gobject, rclosure_weak_notify, rclosure);
    rclosure_weak_notify(rclosure, gobject);
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
init_rclosure(void)
{
    id_call = rb_intern("call");
    id_closures = rb_intern("closures");
    rclosure_initialized = TRUE;
    rb_set_end_proc(rclosure_end_proc, Qnil);
}

/**********************************************************************/

static VALUE
rg_initialize(VALUE self)
{
    GClosure *closure = g_rclosure_new(rb_block_proc(), Qnil, NULL);
    G_INITIALIZE(self, closure);
    return self;
}

static VALUE
rg_in_marshal_p(VALUE self)
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return CBOOL2RVAL(closure->in_marshal);
}

static VALUE
rg_invalid_p(VALUE self)
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    return CBOOL2RVAL(closure->is_invalid);
}

static VALUE
rg_invalidate(VALUE self)
{
    GClosure* closure = RVAL2BOXED(self, G_TYPE_CLOSURE);
    g_closure_invalidate(closure);
    return self;
}

/**********************************************************************/

void
Init_gobject_gclosure(void)
{
    VALUE RG_TARGET_NAMESPACE;

    init_rclosure();

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_CLOSURE, "Closure", rbg_mGLib());

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD_P(in_marshal, 0);
    RG_DEF_METHOD_P(invalid, 0);
    RG_DEF_METHOD(invalidate, 0);
}
