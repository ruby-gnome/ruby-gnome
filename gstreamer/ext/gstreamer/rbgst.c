/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2013  Ruby-GNOME2 Project Team
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

#include <gst/gst.h>

#include <rb-gobject-introspection.h>

extern void Init_gstreamer (void);

static gboolean
rg_gst_bus_func_p(GIArgInfo *info)
{
    GITypeInfo type_info;
    GIBaseInfo *interface_info;
    const gchar *namespace;
    const gchar *name;
    gboolean gst_bus_func_p = FALSE;

    g_arg_info_load_type(info, &type_info);
    interface_info = g_type_info_get_interface(&type_info);
    namespace = g_base_info_get_namespace(interface_info);
    name = g_base_info_get_name(interface_info);
    if (strcmp(namespace, "Gst") == 0 && strcmp(name, "BusFunc") == 0) {
        gst_bus_func_p = TRUE;
    }
    g_base_info_unref(interface_info);

    return gst_bus_func_p;
}

static gboolean
rg_gst_bus_func_callback(GstBus *bus, GstMessage *message, gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_keep;
    ID id_call;

    CONST_ID(id_call, "call");
    rb_keep = rb_funcall(callback_data->rb_callback, id_call, 2,
                         GOBJ2RVAL(bus),
                         BOXED2RVAL(message, GST_MINI_OBJECT_TYPE(message)));
    if (callback_data->metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
        rb_gi_callback_data_free(callback_data);
    }
    return RVAL2CBOOL(rb_keep);
}

static gpointer
rg_gst_bus_func_callback_finder(GIArgInfo *info)
{
    if (!rg_gst_bus_func_p(info)) {
        return NULL;
    }
    return rg_gst_bus_func_callback;
}

static gboolean
rg_gst_tag_foreach_func_p(GIArgInfo *info)
{
    GITypeInfo type_info;
    GIBaseInfo *interface_info;
    const gchar *namespace;
    const gchar *name;
    gboolean gst_tag_foreach_func_p = FALSE;

    g_arg_info_load_type(info, &type_info);
    interface_info = g_type_info_get_interface(&type_info);
    namespace = g_base_info_get_namespace(interface_info);
    name = g_base_info_get_name(interface_info);
    if (strcmp(namespace, "Gst") == 0 && strcmp(name, "TagForeachFunc") == 0) {
        gst_tag_foreach_func_p = TRUE;
    }
    g_base_info_unref(interface_info);

    return gst_tag_foreach_func_p;
}

static void
rg_gst_tag_foreach_func_callback(const GstTagList *list, const gchar *tag,
                                 gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_call;

    CONST_ID(id_call, "call");
    rb_funcall(callback_data->rb_callback, id_call, 2,
               BOXED2RVAL(list, GST_MINI_OBJECT_TYPE(list)),
               CSTR2RVAL(tag));
    if (callback_data->metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
        rb_gi_callback_data_free(callback_data);
    }
}

static gpointer
rg_gst_tag_foreach_func_callback_finder(GIArgInfo *info)
{
    if (!rg_gst_tag_foreach_func_p(info)) {
        return NULL;
    }
    return rg_gst_tag_foreach_func_callback;
}

void
Init_gstreamer (void)
{
    rb_gi_callback_register_finder(rg_gst_bus_func_callback_finder);
    rb_gi_callback_register_finder(rg_gst_tag_foreach_func_callback_finder);
}
