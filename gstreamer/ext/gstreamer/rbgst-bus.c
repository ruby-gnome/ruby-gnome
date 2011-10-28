/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
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

#include "rbgst.h"

/* Class: Gst::Bus
 * Asynchronous message bus subsystem.
 */

#define RG_TARGET_NAMESPACE cBus
#define SELF(obj) (RVAL2GST_BUS(obj))

static ID id_call;

typedef struct _callback_info_t
{
    VALUE bus;
    VALUE callback;
    guint id;
} callback_info_t;

static gboolean
invoke_watch_func(GstBus *bus, GstMessage *message, gpointer data)
{
    callback_info_t *info = data;

    return RVAL2CBOOL(rb_funcall(info->callback, id_call, 2,
                                 RGST_BUS2RVAL(bus),
                                 GST_MSG2RVAL(message)));
}

static void
destroy_watch_func(gpointer data)
{
    callback_info_t *info = data;

    G_CHILD_REMOVE(info->bus, info->callback);
    g_free(info);
}

static VALUE
rg_add_watch(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_priority, block, rb_id;
    gint priority;
    callback_info_t *info;

    rb_scan_args(argc, argv, "01&", &rb_priority, &block);

    priority = NIL_P(rb_priority) ? G_PRIORITY_DEFAULT : INT2NUM(rb_priority);
    info = g_new(callback_info_t, 1);
    info->bus = self;
    info->callback = block;
    info->id = gst_bus_add_watch_full(SELF(self),
                                      priority,
                                      invoke_watch_func,
                                      (gpointer)info,
                                      destroy_watch_func);
    rb_id = UINT2NUM(info->id);
    G_CHILD_ADD(self, block);
    return rb_id;
}

void
Init_gst_bus (void)
{
    VALUE RG_TARGET_NAMESPACE;

    id_call = rb_intern("call");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_BUS, "Bus", mGst);

    RG_DEF_METHOD(add_watch, -1);
}
