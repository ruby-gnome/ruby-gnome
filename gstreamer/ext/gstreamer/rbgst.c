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

#include "rbgst.h"

static gboolean
name_equal(GIArgInfo *info, const gchar *target_name)
{
    GITypeInfo type_info;
    GIBaseInfo *interface_info;
    const gchar *namespace;
    const gchar *name;
    gboolean equal_name_p = FALSE;

    g_arg_info_load_type(info, &type_info);
    interface_info = g_type_info_get_interface(&type_info);
    namespace = g_base_info_get_namespace(interface_info);
    name = g_base_info_get_name(interface_info);
    if (strcmp(namespace, "Gst") == 0 && strcmp(name, target_name) == 0) {
        equal_name_p = TRUE;
    }
    g_base_info_unref(interface_info);

    return equal_name_p;
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
    if (!name_equal(info, "BusFunc")) {
        return NULL;
    }
    return rg_gst_bus_func_callback;
}

static gboolean
rg_gst_bus_sync_handler_callback(GstBus *bus, GstMessage *message,
                                 gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_bus_sync_reply;
    ID id_call;

    CONST_ID(id_call, "call");
    rb_bus_sync_reply =
        rb_funcall(callback_data->rb_callback, id_call, 2,
                   GOBJ2RVAL(bus),
                   BOXED2RVAL(message, GST_MINI_OBJECT_TYPE(message)));
    if (callback_data->metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
        rb_gi_callback_data_free(callback_data);
    }
    return RVAL2GENUM(rb_bus_sync_reply, GST_TYPE_BUS_SYNC_REPLY);
}

static gpointer
rg_gst_bus_sync_handler_callback_finder(GIArgInfo *info)
{
    if (!name_equal(info, "BusSyncHandler")) {
        return NULL;
    }
    return rg_gst_bus_sync_handler_callback;
}

static void
rg_gst_tag_foreach_func_callback(const GstTagList *list, const gchar *tag,
                                 gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_call;

    CONST_ID(id_call, "call");
    rb_funcall(callback_data->rb_callback, id_call, 2,
               /*
                * XXX: Use gst_tag_list_copy() instead if we don't trust
                * users. Users should not use destructive methods such as
                * #insert. If many users use these methods, we shuold use
                * gst_tag_list_copy().
                */
               BOXED2RVAL((GstTagList *)list, GST_MINI_OBJECT_TYPE(list)),
               CSTR2RVAL(tag));
    if (callback_data->metadata->scope_type == GI_SCOPE_TYPE_ASYNC) {
        rb_gi_callback_data_free(callback_data);
    }
}

static gpointer
rg_gst_tag_foreach_func_callback_finder(GIArgInfo *info)
{
    if (!name_equal(info, "TagForeachFunc")) {
        return NULL;
    }
    return rg_gst_tag_foreach_func_callback;
}

static void
rg_gst_value_list_r2g(VALUE from, GValue *to)
{
    int i, n;

    if (NIL_P(from)) {
        return;
    }

    from = rbg_to_array(from);
    n = RARRAY_LEN(from);
    for (i = 0; i < n; i++) {
        VALUE rb_element;
        const RGObjClassInfo *class_info;
        GValue element = G_VALUE_INIT;

        rb_element = rb_ary_entry(from, i);
        class_info = RVAL2CINFO(rb_element);
        g_value_init(&element, class_info->gtype);
        rbgobj_rvalue_to_gvalue(rb_element, &element);
        gst_value_list_append_value(to, &element);
    }
}

static VALUE
rg_gst_value_list_g2r(const GValue *from)
{
    guint i, n;
    VALUE rb_values;

    n = gst_value_list_get_size(from);
    rb_values = rb_ary_new();
    for (i = 0; i < n; i++) {
        const GValue *value;
        VALUE rb_value;

        value = gst_value_list_get_value(from, i);
        rb_value = rbgobj_gvalue_to_rvalue(value);
        rb_ary_push(rb_values, rb_value);
    }

    return rb_values;
}

void
Init_gstreamer (void)
{
    rb_gi_callback_register_finder(rg_gst_bus_func_callback_finder);
    rb_gi_callback_register_finder(rg_gst_bus_sync_handler_callback_finder);
    rb_gi_callback_register_finder(rg_gst_tag_foreach_func_callback_finder);

    rbgobj_register_r2g_func(GST_TYPE_LIST, rg_gst_value_list_r2g);
    rbgobj_register_g2r_func(GST_TYPE_LIST, rg_gst_value_list_g2r);

    rb_gst_init_element_factory();
}
