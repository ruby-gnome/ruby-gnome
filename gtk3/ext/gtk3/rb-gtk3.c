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

#include "rb-gtk3-private.h"

/*
#if GTK_CHECK_VERSION(3, 10, 0)
#    define RB_GTK_ACTION_IS_DEPRECATED
#    define RB_GTK_ACTION_GROUP_IS_DEPRECATED
#    define RB_GTK_UI_MANAGER_IS_DEPRECATED
#endif
*/

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

static void
rb_gtk3_tree_cell_data_func_callback(GtkTreeViewColumn *column,
                                     GtkCellRenderer *cell,
                                     GtkTreeModel *model,
                                     GtkTreeIter *iter,
                                     gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    ID id_call;
    VALUE rb_model;
    VALUE rb_iter;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

    CONST_ID(id_call, "call");
    rb_funcall(callback_data->rb_callback,
               id_call,
               4,
               GOBJ2RVAL(column),
               GOBJ2RVAL(cell),
               rb_model,
               rb_iter);
}

static gboolean
rb_gtk3_tree_model_foreach_func_callback(GtkTreeModel *model,
                                         GtkTreePath *path,
                                         GtkTreeIter *iter,
                                         gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    ID id_call;
    VALUE rb_model;
    VALUE rb_iter;
    gboolean stop = FALSE;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

    CONST_ID(id_call, "call");
    rb_funcall(callback_data->rb_callback,
               id_call,
               3,
               rb_model,
               BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
               rb_iter);

    return stop;
}

static void
rb_gtk3_tree_selection_foreach_func_callback(GtkTreeModel *model,
                                             GtkTreePath *path,
                                             GtkTreeIter *iter,
                                             gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    ID id_call;
    VALUE rb_model;
    VALUE rb_iter;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

    CONST_ID(id_call, "call");
    rb_funcall(callback_data->rb_callback,
               id_call,
               3,
               rb_model,
               BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
               rb_iter);
}

static void
rb_gtk3_tree_view_mapping_func_callback(GtkTreeView *tree_view,
                                        GtkTreePath *path,
                                        gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_call;

    CONST_ID(id_call, "call");
    rb_funcall(callback_data->rb_callback,
               id_call,
               2,
               GOBJ2RVAL(tree_view),
               BOXED2RVAL(path, GTK_TYPE_TREE_PATH));
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
    } else if (name_equal(info, "TreeCellDataFunc")) {
        return rb_gtk3_tree_cell_data_func_callback;
    } else if (name_equal(info, "TreeModelForeachFunc")) {
        return rb_gtk3_tree_model_foreach_func_callback;
    } else if (name_equal(info, "TreeSelectionForeachFunc")) {
        return rb_gtk3_tree_selection_foreach_func_callback;
    } else if (name_equal(info, "TreeViewMappingFunc")) {
        return rb_gtk3_tree_view_mapping_func_callback;
    } else {
        return NULL;
    }
}

#ifndef RB_GTK_ACTION_IS_DEPRECATED
static void
rb_gtk3_action_mark(gpointer object)
{
    GtkAction *action;
    GSList *proxies, *node;

    action = GTK_ACTION(object);
    proxies = gtk_action_get_proxies(action);
    for (node = proxies; node; node = g_slist_next(node)) {
        GtkWidget *proxy = node->data;
        rbgobj_gc_mark_instance(proxy);
    }
}
#endif

static void
rb_gtk3_builder_mark(gpointer object)
{
    GtkBuilder *builder;
    GSList *objects;

    builder = GTK_BUILDER(object);
    objects = gtk_builder_get_objects(builder);
    g_slist_foreach(objects, (GFunc)rbgobj_gc_mark_instance, NULL);
    g_slist_free(objects);
}

#ifndef RB_GTK_ACTION_GROUP_IS_DEPRECATED
static void
rb_gtk3_action_group_mark(gpointer object)
{
    GtkActionGroup *group;
    GList *actions, *node;

    group = GTK_ACTION_GROUP(object);
    actions = gtk_action_group_list_actions(group);
    for (node = actions; node; node = g_list_next(node)) {
        GtkWidget *action = node->data;
        rbgobj_gc_mark_instance(action);
    }
    g_list_free(actions);
}
#endif

#ifndef RB_GTK_UI_MANAGER_IS_DEPRECATED
static void
rb_gtk3_ui_manager_mark(gpointer object)
{
    GtkUIManager *manager;
    GList *action_groups, *node;

    manager = GTK_UI_MANAGER(object);
    action_groups = gtk_ui_manager_get_action_groups(manager);
    for (node = action_groups; node; node = g_list_next(node)) {
        GtkWidget *action_group = node->data;
        rbgobj_gc_mark_instance(action_group);
    }

    rbgobj_gc_mark_instance(gtk_ui_manager_get_accel_group(manager));
}
#endif

void
Init_gtk3(void)
{
    rb_gi_callback_register_finder(rb_gtk3_callback_finder);

#ifndef RB_GTK_ACTION_IS_DEPRECATED
    rbgobj_register_mark_func(GTK_TYPE_ACTION, rb_gtk3_action_mark);
#endif
    rbgobj_register_mark_func(GTK_TYPE_BUILDER, rb_gtk3_builder_mark);
#ifndef RB_GTK_ACTION_GROUP_IS_DEPRECATED
    rbgobj_register_mark_func(GTK_TYPE_ACTION_GROUP, rb_gtk3_action_group_mark);
#endif
#ifndef RB_GTK_UI_MANAGER_IS_DEPRECATED
    rbgobj_register_mark_func(GTK_TYPE_UI_MANAGER, rb_gtk3_ui_manager_mark);
#endif

    rbgtk3_container_init();

    rbgobj_boxed_not_copy_obj(GTK_TYPE_SELECTION_DATA);
}
