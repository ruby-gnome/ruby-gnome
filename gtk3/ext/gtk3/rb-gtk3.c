/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#include "rb-gtk3.h"

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
    if (strcmp(namespace, "Gtk") == 0 && strcmp(name, target_name) == 0) {
        equal_name_p = TRUE;
    }
    g_base_info_unref(interface_info);

    return equal_name_p;
}

static void
rb_gtk3_callback_callback(GtkWidget *widget, gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_call;

    CONST_ID(id_call, "call");
    rb_funcall(callback_data->rb_callback, id_call, 1,
               GOBJ2RVAL(widget));
}

static gint
rb_gtk3_assistant_page_func_callback(gint current_page, gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_call;
    VALUE rb_next_page;

    CONST_ID(id_call, "call");
    rb_next_page = rb_funcall(callback_data->rb_callback, id_call, 1,
                              INT2NUM(current_page));
    return NUM2INT(rb_next_page);
}

static void
rb_gtk3_builder_connect_func_callback(GtkBuilder *builder,
                                      GObject *object,
                                      const gchar *signal_name,
                                      const gchar *handler_name,
                                      GObject *connect_object,
                                      GConnectFlags flags,
                                      gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id___connect_signals__;

    CONST_ID(id___connect_signals__, "__connect_signals__");
    rb_funcall(GOBJ2RVAL(builder), id___connect_signals__, 6,
               callback_data->rb_callback,
               GOBJ2RVAL(object),
               CSTR2RVAL(signal_name),
               CSTR2RVAL(handler_name),
               GOBJ2RVAL(connect_object),
               GCONNECTFLAGS2RVAL(flags));
}

static const gchar *
rb_gtk3_translate_func_callback(const gchar *path,
                                gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_call;
    VALUE rb_translated;

    CONST_ID(id_call, "call");
    rb_translated = rb_funcall(callback_data->rb_callback,
                               id_call,
                               1,
                               CSTR2RVAL(path));
    return RVAL2CSTR(rb_translated);
}

static gboolean
rb_gtk3_tree_model_foreach_func_callback(GtkTreeModel *model,
                                         GtkTreePath *path,
                                         GtkTreeIter *iter,
                                         gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_call;
    VALUE rb_gtk_module;
    VALUE rb_tree_iter_class;
    VALUE rb_iter;
    VALUE rb_stop;

    CONST_ID(id_call, "call");
    rb_gtk_module = rb_const_get(rb_cObject, rb_intern("Gtk"));
    rb_tree_iter_class = rb_const_get(rb_gtk_module, rb_intern("TrteeIter"));
    rb_iter = Data_Wrap_Struct(rb_tree_iter_class, NULL, NULL, iter);
    rb_stop = rb_funcall(callback_data->rb_callback,
                         id_call,
                         3,
                         BOXED2RVAL(model, GTK_TYPE_TREE_MODEL),
                         BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
                         rb_iter);

    return RVAL2CBOOL(rb_stop);
}

static gpointer
rb_gtk3_callback_finder(GIArgInfo *info)
{
    if (name_equal(info, "Callback")) {
        return rb_gtk3_callback_callback;
    } else if (name_equal(info, "AssistantPageFunc")) {
        return rb_gtk3_assistant_page_func_callback;
    } else if (name_equal(info, "BuilderConnectFunc")) {
        return rb_gtk3_builder_connect_func_callback;
    } else if (name_equal(info, "TranslateFunc")) {
        return rb_gtk3_translate_func_callback;
    } else if (name_equal(info, "TreeModelForeachFunc")) {
        return rb_gtk3_tree_model_foreach_func_callback;
    } else {
        return NULL;
    }
}

static void
rb_gtk3_container_mark_callback(GtkWidget *widget,
                                G_GNUC_UNUSED gpointer data)
{
    rbgobj_gc_mark_instance(widget);
}

static void
rb_gtk3_container_mark(gpointer object)
{
    gtk_container_forall(GTK_CONTAINER(object),
                         rb_gtk3_container_mark_callback,
                         NULL);
}

void
Init_gtk3 (void)
{
    rb_gi_callback_register_finder(rb_gtk3_callback_finder);

    rbgobj_register_mark_func(GTK_TYPE_CONTAINER, rb_gtk3_container_mark);
}
