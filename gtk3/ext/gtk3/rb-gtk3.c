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

static ID id_call;
static VALUE cGdkAtom;

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

    rb_funcall(callback_data->rb_callback, id_call, 1,
               GOBJ2RVAL(widget));
}

static gint
rb_gtk3_assistant_page_func_callback(gint current_page, gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_next_page;

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
    VALUE rb_object;

    CONST_ID(id___connect_signals__, "__connect_signals__");
    rb_object = GOBJ2RVAL(object);
    G_RELATIVE(rb_object, callback_data->rb_callback);
    rb_funcall(GOBJ2RVAL(builder), id___connect_signals__, 6,
               callback_data->rb_callback,
               rb_object,
               CSTR2RVAL(signal_name),
               CSTR2RVAL(handler_name),
               GOBJ2RVAL(connect_object),
               GCONNECTFLAGS2RVAL(flags));
}

static void
rb_gtk3_cell_layout_data_func_callback(GtkCellLayout *cell_layout,
                                       GtkCellRenderer *cell,
                                       GtkTreeModel *model,
                                       GtkTreeIter *iter,
                                       gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    VALUE rb_model;
    VALUE rb_iter;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

    rb_funcall(callback_data->rb_callback,
               id_call,
               4,
               GOBJ2RVAL(cell_layout),
               GOBJ2RVAL(cell),
               rb_model,
               rb_iter);
}

static void
rb_gtk3_clipboard_received_func_callback(GtkClipboard *clipboard,
                                         GtkSelectionData *selection_data,
                                         gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;

    rb_funcall(callback_data->rb_callback,
               id_call,
               2,
               GOBJ2RVAL(clipboard),
               BOXED2RVAL(selection_data, GTK_TYPE_SELECTION_DATA));
}

static void
rb_gtk3_clipboard_rich_text_received_func_callback(GtkClipboard *clipboard,
                                                   GdkAtom format,
                                                   const guint *text,
                                                   gsize length,
                                                   gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;

    rb_funcall(callback_data->rb_callback,
               id_call,
               3,
               GOBJ2RVAL(clipboard),
               Data_Wrap_Struct(cGdkAtom, NULL, NULL, format),
               CSTR2RVAL(text));
}

static void
rb_gtk3_clipboard_image_received_func_callback(GtkClipboard *clipboard,
                                               GdkPixbuf *pixbuf,
                                               gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;

    rb_funcall(callback_data->rb_callback,
               id_call,
               2,
               GOBJ2RVAL(clipboard),
               GOBJ2RVAL(pixbuf));
}

static void
rb_gtk3_clipboard_targets_received_func_callback(GtkClipboard *clipboard,
                                                 GdkAtom *atoms,
                                                 gint n_atoms,
                                                 gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_atoms;
    gint i;

    if (!atoms) {
        rb_atoms = Qnil;
    } else {
        rb_atoms = rb_ary_new2(n_atoms);
        for (i = 0; i < n_atoms; i++) {
            VALUE rb_atom;
            rb_atom = Data_Wrap_Struct(cGdkAtom, NULL, NULL, atoms[i]);
            rb_ary_push(rb_atoms, rb_atom);
        }
    }

    rb_funcall(callback_data->rb_callback,
               id_call,
               2,
               GOBJ2RVAL(clipboard),
               rb_atoms);
}

static void
rb_gtk3_clipboard_text_received_func_callback(GtkClipboard *clipboard,
                                              const gchar *text,
                                              gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;

    rb_funcall(callback_data->rb_callback,
               id_call,
               2,
               GOBJ2RVAL(clipboard),
               CSTR2RVAL(text));
}

static const gchar *
rb_gtk3_translate_func_callback(const gchar *path,
                                gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_translated;

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
    VALUE rb_model;
    VALUE rb_iter;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

    rb_funcall(callback_data->rb_callback,
               id_call,
               4,
               GOBJ2RVAL(column),
               GOBJ2RVAL(cell),
               rb_model,
               rb_iter);
}

static void
rb_gtk3_tree_model_filter_modify_func_callback(GtkTreeModel *model,
                                               GtkTreeIter *iter,
                                               GValue *value,
                                               gint column,
                                               gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    VALUE rb_model;
    VALUE rb_iter;
    VALUE rb_value;
    VALUE rb_column;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);
    rb_column = INT2NUM(column);

    rb_value = rb_funcall(callback_data->rb_callback,
                          id_call,
                          3,
                          rb_model,
                          rb_iter,
                          rb_column);
    rbgobj_rvalue_to_gvalue(rb_value, value);
}

static gboolean
rb_gtk3_tree_model_filter_visible_func_callback(GtkTreeModel *model,
                                                GtkTreeIter *iter,
                                                gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    VALUE rb_model;
    VALUE rb_iter;
    VALUE rb_visible;
    gboolean visible = FALSE;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

    rb_visible = rb_funcall(callback_data->rb_callback,
                            id_call,
                            2,
                            rb_model,
                            rb_iter);
    visible = RVAL2CBOOL(rb_visible);

    return visible;
}

static gboolean
rb_gtk3_tree_model_foreach_func_callback(GtkTreeModel *model,
                                         GtkTreePath *path,
                                         GtkTreeIter *iter,
                                         gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    VALUE rb_model;
    VALUE rb_iter;
    gboolean stop = FALSE;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

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
    VALUE rb_model;
    VALUE rb_iter;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

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
    } else if (name_equal(info, "CellLayoutDataFunc")) {
        return rb_gtk3_cell_layout_data_func_callback;
    } else if (name_equal(info, "ClipboardReceivedFunc")) {
        return rb_gtk3_clipboard_received_func_callback;
    } else if (name_equal(info, "ClipboardRichTextReceivedFunc")) {
        return rb_gtk3_clipboard_rich_text_received_func_callback;
    } else if (name_equal(info, "ClipboardImageReceivedFunc")) {
        return rb_gtk3_clipboard_image_received_func_callback;
    } else if (name_equal(info, "ClipboardTargetsReceivedFunc")) {
        return rb_gtk3_clipboard_targets_received_func_callback;
    } else if (name_equal(info, "ClipboardTextReceivedFunc")) {
        return rb_gtk3_clipboard_text_received_func_callback;
    } else if (name_equal(info, "TranslateFunc")) {
        return rb_gtk3_translate_func_callback;
    } else if (name_equal(info, "TreeCellDataFunc")) {
        return rb_gtk3_tree_cell_data_func_callback;
    } else if (name_equal(info, "TreeModelFilterModifyFunc")) {
        return rb_gtk3_tree_model_filter_modify_func_callback;
    } else if (name_equal(info, "TreeModelFilterVisibleFunc")) {
        return rb_gtk3_tree_model_filter_visible_func_callback;
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
rbgtk3_class_init_func(gpointer g_class, G_GNUC_UNUSED gpointer class_data)
{
    VALUE rb_class;

    rb_class = GTYPE2CLASS(G_TYPE_FROM_CLASS(g_class));
    rb_funcall(rb_class, rb_intern("init"), 0);
}

void
rbgtk3_initialize(VALUE self)
{
    GObject *object;

    object = RVAL2GOBJ(self);
    g_object_ref_sink(object);

    rb_funcall(self, rb_intern("initialize_post"), 0);
}

void
Init_gtk3(void)
{
    id_call = rb_intern("call");
    cGdkAtom = rb_const_get(rb_const_get(rb_cObject, rb_intern("Gdk")),
                            rb_intern("Atom"));

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

    rbgtk3_cell_layout_init();
    rbgtk3_container_init();
    rbgtk3_tree_model_init();
    rbgtk3_tree_view_init();
    rbgtk3_widget_init();

    rbgobj_boxed_not_copy_obj(GTK_TYPE_SELECTION_DATA);
}
