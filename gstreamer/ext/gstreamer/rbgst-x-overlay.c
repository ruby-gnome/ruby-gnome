/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Sjoerd Simons <sjoerd@luon.net>
 *  Copyright (C) 2005  Laurent Sansonetti <lrz@gnome.org>
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

#ifdef HAVE_GST_OVERLAY

#include "rbgst-private.h"
#include <gst/interfaces/xoverlay.h>

#define RG_TARGET_NAMESPACE mXOverlay
#define SELF(o)           (GST_X_OVERLAY(RVAL2GOBJ(o)))

static GQuark xoverlay_xid_data_quark;

struct xid_callback_data
{
    GObject *bus;
    gulong cb_id;
    gulong xid;
};

static void
bus_sync_func_cb(GstBus *bus, GstMessage *message, gpointer user_data)
{
    struct xid_callback_data *xid_cb_data;
    GstXOverlay *sink = GST_X_OVERLAY(user_data);
    xid_cb_data = (struct xid_callback_data *)
        g_object_get_qdata(G_OBJECT(sink), xoverlay_xid_data_quark);

    if (xid_cb_data == NULL) {
        return;
    }

    if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_ELEMENT) {
        if (message->structure
            && GST_MESSAGE_SRC(message) == GST_OBJECT(sink)
            && strcmp(gst_structure_get_name(message->structure),
                      "prepare-xwindow-id") == 0) {
            gst_x_overlay_set_xwindow_id(sink, xid_cb_data->xid);
        }
    }
}

static void
xid_callback_data_destroy_cb(gpointer data)
{
    struct xid_callback_data *xid_cb_data = (struct xid_callback_data *)data;

    if (g_signal_handler_is_connected(xid_cb_data->bus, xid_cb_data->cb_id)) {
        g_signal_handler_disconnect (xid_cb_data->bus, xid_cb_data->cb_id);
    }
    g_slice_free(struct xid_callback_data, data);
}

static VALUE
rg_set_xwindow_id_with_buswatch(VALUE self, VALUE bus,
                                              VALUE x_window_id)
{
    struct xid_callback_data* xid_cb_data;
    GObject *sink;

    xid_cb_data = g_slice_new(struct xid_callback_data);
    xid_cb_data->xid = NUM2ULL(x_window_id);
    xid_cb_data->bus = RVAL2GOBJ(bus);

    sink = RVAL2GOBJ(self);
    g_object_set_qdata_full(sink, xoverlay_xid_data_quark,
                            xid_cb_data, xid_callback_data_destroy_cb);

    gst_bus_enable_sync_message_emission(GST_BUS(xid_cb_data->bus));
    xid_cb_data->cb_id =
        g_signal_connect_object(xid_cb_data->bus, "sync-message",
                                G_CALLBACK(bus_sync_func_cb), sink, 0);
    return self;
}

static VALUE
rg_set_xwindow_id(VALUE self, VALUE x_window_id)
{
    gst_x_overlay_set_xwindow_id(SELF(self), NUM2ULONG(x_window_id));
    return self;
}

static VALUE
rg_expose(VALUE self)
{
    gst_x_overlay_expose(SELF(self));
    return self;
}

static VALUE
rg_got_xwindow_id(VALUE self, VALUE x_window_id)
{
    gst_x_overlay_got_xwindow_id(SELF(self), NUM2ULL(x_window_id));
    return self;
}

void
Init_gst_x_overlay(VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GST_TYPE_X_OVERLAY, "XOverlay", mGst);

    xoverlay_xid_data_quark =
      g_quark_from_static_string("__rbgst_xoverlay_xid_data_quark__");

    RG_DEF_METHOD(set_xwindow_id, 1);
    RG_DEF_METHOD(expose, 0);

    RG_DEF_METHOD(got_xwindow_id, 1);
    RG_DEF_METHOD(set_xwindow_id_with_buswatch, 2);
}

#endif /* HAVE_GST_OVERLAY */
