/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#ifdef HAVE_GST_PBUTILS
#include "rbgst-private.h"
#include <gst/pbutils/pbutils-enumtypes.h>
#include <gst/pbutils/install-plugins.h>

#define RG_TARGET_NAMESPACE cInstallPluginsContext

static VALUE RG_TARGET_NAMESPACE;

static VALUE
context2robj(gpointer context, G_GNUC_UNUSED gpointer user_data)
{
    return Data_Wrap_Struct(RG_TARGET_NAMESPACE, NULL,
                            gst_install_plugins_context_free,
                            (GstInstallPluginsContext *)context);
}

static gpointer
robj2context(VALUE object, G_GNUC_UNUSED gpointer user_data)
{
    gpointer instance;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(object, RG_TARGET_NAMESPACE))) {
      rb_raise(rb_eTypeError, "not a Gst::InstallPluginsContext");
    }
    Data_Get_Struct(object, GstInstallPluginsContext, instance);
    return instance;
}

static VALUE
rg_initialize(VALUE self)
{
    GstInstallPluginsContext *context;

    context = gst_install_plugins_context_new();
    G_INITIALIZE(self, context);
    return Qnil;
}

static VALUE
rg_set_xid(VALUE self, VALUE xid)
{
    GstInstallPluginsContext *context;

    context = (GstInstallPluginsContext *)RVAL2GOBJ(self);
    gst_install_plugins_context_set_xid(context, NUM2INT(xid));/*FIXME: segfault on ruby exit*/
    return self;
}

void
Init_gst_install_plugins_context(VALUE mGst)
{
    RGConvertTable context_table;

    memset(&context_table, 0, sizeof(RGConvertTable));
    context_table.type = GST_TYPE_INSTALL_PLUGINS_CONTEXT;
    context_table.instance2robj = context2robj;
    context_table.robj2instance = robj2context;
    RG_DEF_CONVERSION(&context_table);

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_INSTALL_PLUGINS_CONTEXT,
                                               "InstallPluginsContext", mGst);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_xid, 1);
}
#endif /* HAVE_GST_PBUTILS */
