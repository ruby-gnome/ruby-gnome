/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

static ID id_call;

#define RG_TARGET_NAMESPACE cSource
#define _SELF(s) ((GSource*)RVAL2BOXED(s, G_TYPE_SOURCE))

/*
GSource*    g_source_new                    (GSourceFuncs *source_funcs,
                                             guint struct_size);
*/

static VALUE
rg_attach(int argc, VALUE *argv, VALUE self)
{
    VALUE context;

    rb_scan_args(argc, argv, "01", &context);
    return UINT2NUM(g_source_attach(_SELF(self),
                                    RVAL2BOXED(context, G_TYPE_MAIN_CONTEXT)));
}

static VALUE
rg_destroy(VALUE self)
{
    g_source_destroy(_SELF(self));
    return self;
}

static VALUE
rg_destroyed_p(VALUE self)
{
    return CBOOL2RVAL(g_source_is_destroyed(_SELF(self)));
}

static VALUE
rg_set_priority(VALUE self, VALUE priority)
{
    g_source_set_priority(_SELF(self), NUM2INT(priority));
    return self;
}

static VALUE
rg_priority(VALUE self)
{
    return INT2NUM(g_source_get_priority(_SELF(self)));
}

static VALUE
rg_set_can_recurse(VALUE self, VALUE can_recurse)
{
    g_source_set_can_recurse(_SELF(self), RVAL2CBOOL(can_recurse));
    return self;
}

static VALUE
rg_can_recurse_p(VALUE self)
{
    return CBOOL2RVAL(g_source_get_can_recurse(_SELF(self)));
}

static VALUE
rg_id(VALUE self)
{
    return UINT2NUM(g_source_get_id(_SELF(self)));
}

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(g_source_get_name(_SELF(self)));
}

static VALUE
rg_set_name(VALUE self, VALUE name)
{
    g_source_set_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
rg_context(VALUE self)
{
    GMainContext* context = g_source_get_context(_SELF(self));
    return BOXED2RVAL(context, G_TYPE_MAIN_CONTEXT);
}

static gboolean
source_func(gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 0));
}

static VALUE
rg_set_callback(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    g_source_set_callback(_SELF(self),
                          (GSourceFunc)source_func,
                          (gpointer)func,
                          (GDestroyNotify)NULL);
    return self;
}

/*
void        g_source_set_callback_indirect  (GSource *source,
                                             gpointer callback_data,
                                             GSourceCallbackFuncs *callback_funcs);
*/

static VALUE
rg_ready_time(VALUE self)
{
    gint64 ready_time;
    ready_time = g_source_get_ready_time(_SELF(self));
    return LL2NUM(ready_time);
}

static VALUE
rg_set_ready_time(VALUE self, VALUE ready_time)
{
    g_source_set_ready_time(_SELF(self), NUM2LL(ready_time));
    return self;
}

static VALUE
rg_add_poll(VALUE self, VALUE fd)
{
    g_source_add_poll(_SELF(self), RVAL2GPOLLFD(fd));
    return self;
}

static VALUE
rg_remove_poll(VALUE self, VALUE fd)
{
    g_source_remove_poll(_SELF(self), RVAL2GPOLLFD(fd));
    return self;
}

static VALUE
rg_time(VALUE self)
{
    gint64 time;
    time = g_source_get_time(_SELF(self));
    return LL2NUM(time);
}

/* How can I implement them ?
gboolean    g_source_remove_by_funcs_user_data
                                            (GSourceFuncs *funcs,
                                             gpointer user_data);
gboolean    g_source_remove_by_user_data    (gpointer user_data);
*/

void
Init_glib_source(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_SOURCE, "Source", mGLib);

    id_call = rb_intern("call");

    rb_define_const(RG_TARGET_NAMESPACE,
                    "REMOVE", CBOOL2RVAL(G_SOURCE_REMOVE));
    rb_define_const(RG_TARGET_NAMESPACE,
                    "CONTINUE", CBOOL2RVAL(G_SOURCE_CONTINUE));

    RG_DEF_METHOD(attach, -1);
    RG_DEF_METHOD(destroy, 0);
    RG_DEF_METHOD_P(destroyed, 0);
    RG_DEF_METHOD(set_priority, 1);
    RG_DEF_METHOD(priority, 0);
    RG_DEF_METHOD(set_can_recurse, 1);
    RG_DEF_METHOD_P(can_recurse, 0);
    RG_DEF_METHOD(id, 0);
    RG_DEF_METHOD(name, 0);
    RG_REPLACE_SET_PROPERTY(name, 1);
    RG_DEF_METHOD(context, 0);
    RG_DEF_METHOD(set_callback, 0);
    RG_DEF_METHOD(ready_time, 0);
    RG_REPLACE_SET_PROPERTY(ready_time, 1);
    RG_DEF_METHOD(add_poll, 1);
    RG_DEF_METHOD(remove_poll, 1);
    RG_DEF_METHOD(time, 0);

    /* GLib::Source.remove is moved to rbglib_maincontext.c */
}
