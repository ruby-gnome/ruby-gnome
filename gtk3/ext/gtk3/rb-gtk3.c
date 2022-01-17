/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015-2018  Ruby-GNOME2 Project Team
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
#include <locale.h>

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

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
               id_call, 1,
               GOBJ2RVAL(widget));
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

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
               id_call,
               2,
               GOBJ2RVAL(clipboard),
               BOXED2RVAL(selection_data, GTK_TYPE_SELECTION_DATA));
}

static void
rb_gtk3_clipboard_image_received_func_callback(GtkClipboard *clipboard,
                                               GdkPixbuf *pixbuf,
                                               gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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
            rb_atom = rb_gi_struct_new_raw(cGdkAtom, atoms[i], FALSE);
            rb_ary_push(rb_atoms, rb_atom);
        }
    }

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
               id_call,
               2,
               GOBJ2RVAL(clipboard),
               CSTR2RVAL(text));
}

static void
rb_gtk3_clipboard_uri_received_func_callback(GtkClipboard *clipboard,
                                             gchar **uris,
                                             gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
               id_call,
               2,
               GOBJ2RVAL(clipboard),
               STRV2RVAL((const gchar **)uris));
}

static gboolean
rb_gtk3_entry_completion_match_func_callback(GtkEntryCompletion *completion,
                                             const gchar *key,
                                             GtkTreeIter *iter,
                                             gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    VALUE rb_model;
    VALUE rb_iter;
    GtkTreeModel *model;
    VALUE rb_match;
    gboolean match = FALSE;

    model = gtk_entry_completion_get_model(completion);
    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);
    rb_iter = BOXED2RVAL(iter, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter, id_set_model, 1, rb_model);

    rb_match = rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
                       id_call,
                       3,
                       GOBJ2RVAL(completion),
                       CSTR2RVAL(key),
                       rb_iter);

    match = RVAL2CBOOL(rb_match);
    return match;
}

static void
rb_gtk3_menu_position_func_callback(GtkMenu *menu,
                                    gint *x,
                                    gint *y,
                                    gboolean *push_in,
                                    gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_result_raw;
    VALUE rb_result;

    rb_result_raw = rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
                               id_call,
                               3,
                               GOBJ2RVAL(menu),
                               INT2NUM(*x),
                               INT2NUM(*y));
    rb_result = rbg_check_array_type(rb_result_raw);
    if (NIL_P(rb_result) ||
        !(RARRAY_LEN(rb_result) == 2 ||
          RARRAY_LEN(rb_result) == 3)) {
        /* TODO: Raising an error will abort the program. :< */
        rb_raise(rb_eArgError,
                 "block should return [x, y, push_in]: %s",
                 RBG_INSPECT(rb_result_raw));
    }

    *x = NUM2INT(RARRAY_PTR(rb_result)[0]);
    *y = NUM2INT(RARRAY_PTR(rb_result)[1]);
    if (RARRAY_LEN(rb_result) == 3) {
        *push_in = RVAL2CBOOL(RARRAY_PTR(rb_result)[2]);
    }
}

static const gchar *
rb_gtk3_translate_func_callback(const gchar *path,
                                gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    VALUE rb_translated;

    rb_translated = rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
               id_call,
               4,
               GOBJ2RVAL(column),
               GOBJ2RVAL(cell),
               rb_model,
               rb_iter);
}

static gint
rb_gtk3_tree_iter_compare_func_callback(GtkTreeModel *model,
                                        GtkTreeIter *iter1,
                                        GtkTreeIter *iter2,
                                        gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;
    ID id_set_model;
    VALUE rb_model;
    VALUE rb_iter1;
    VALUE rb_iter2;
    VALUE rb_result;

    CONST_ID(id_set_model, "model=");
    rb_model = GOBJ2RVAL(model);

    rb_iter1 = BOXED2RVAL(iter1, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter1, id_set_model, 1, rb_model);

    rb_iter2 = BOXED2RVAL(iter2, GTK_TYPE_TREE_ITER);
    rb_funcall(rb_iter2, id_set_model, 1, rb_model);

    rb_result = rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
                           id_call,
                           3,
                           rb_model,
                           rb_iter1,
                           rb_iter2);
    return NUM2INT(rb_result);
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

    rb_value = rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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

    rb_visible = rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
               id_call,
               3,
               rb_model,
               BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
               rb_iter);
}

static gboolean
rb_gtk3_tree_selection_func_callback(GtkTreeSelection *selection,
                                     GtkTreeModel *model,
                                     GtkTreePath *path,
                                     gboolean path_currently_selected,
                                     gpointer data)
{
    RBGICallbackData *callback_data = data;
    VALUE rb_selection_is_changeable;

    rb_selection_is_changeable =
        rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
                   id_call,
                   4,
                   GOBJ2RVAL(selection),
                   GOBJ2RVAL(model),
                   BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
                   CBOOL2RVAL(path_currently_selected));
    return RVAL2CBOOL(rb_selection_is_changeable);
}

static void
rb_gtk3_tree_view_mapping_func_callback(GtkTreeView *tree_view,
                                        GtkTreePath *path,
                                        gpointer user_data)
{
    RBGICallbackData *callback_data = user_data;

    rb_funcall(rb_gi_callback_data_get_rb_callback(callback_data),
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
    } else if (name_equal(info, "CellLayoutDataFunc")) {
        return rb_gtk3_cell_layout_data_func_callback;
    } else if (name_equal(info, "ClipboardReceivedFunc")) {
        return rb_gtk3_clipboard_received_func_callback;
    } else if (name_equal(info, "ClipboardImageReceivedFunc")) {
        return rb_gtk3_clipboard_image_received_func_callback;
    } else if (name_equal(info, "ClipboardTargetsReceivedFunc")) {
        return rb_gtk3_clipboard_targets_received_func_callback;
    } else if (name_equal(info, "ClipboardTextReceivedFunc")) {
        return rb_gtk3_clipboard_text_received_func_callback;
    } else if (name_equal(info, "ClipboardURIReceivedFunc")) {
        return rb_gtk3_clipboard_uri_received_func_callback;
    } else if (name_equal(info, "EntryCompletionMatchFunc")) {
        return rb_gtk3_entry_completion_match_func_callback;
    } else if (name_equal(info, "MenuPositionFunc")) {
        return rb_gtk3_menu_position_func_callback;
    } else if (name_equal(info, "TranslateFunc")) {
        return rb_gtk3_translate_func_callback;
    } else if (name_equal(info, "TreeCellDataFunc")) {
        return rb_gtk3_tree_cell_data_func_callback;
    } else if (name_equal(info, "TreeIterCompareFunc")) {
        return rb_gtk3_tree_iter_compare_func_callback;
    } else if (name_equal(info, "TreeModelFilterModifyFunc")) {
        return rb_gtk3_tree_model_filter_modify_func_callback;
    } else if (name_equal(info, "TreeModelFilterVisibleFunc")) {
        return rb_gtk3_tree_model_filter_visible_func_callback;
    } else if (name_equal(info, "TreeModelForeachFunc")) {
        return rb_gtk3_tree_model_foreach_func_callback;
    } else if (name_equal(info, "TreeSelectionForeachFunc")) {
        return rb_gtk3_tree_selection_foreach_func_callback;
    } else if (name_equal(info, "TreeSelectionFunc")) {
        return rb_gtk3_tree_selection_func_callback;
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

static void
rb_gtk3_text_tag_table_mark_body(GtkTextTag *tag, gpointer data)
{
    rbgobj_gc_mark_instance(tag);
}

static void
rb_gtk3_text_tag_table_mark(gpointer object)
{
    GtkTextTagTable *table;

    table = GTK_TEXT_TAG_TABLE(object);
    gtk_text_tag_table_foreach(table, rb_gtk3_text_tag_table_mark_body, NULL);
}

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
    rbgobj_register_mark_func(GTK_TYPE_TEXT_TAG_TABLE,
                              rb_gtk3_text_tag_table_mark);

    rbgtk3_cell_layout_init();
    rbgtk3_container_init();
    rbgtk3_spin_button_init();
    rbgtk3_tree_model_init();
    rbgtk3_tree_view_init();
    rbgtk3_widget_init();
    rbgtk3_window_init();

    rbgobj_boxed_not_copy_obj(GTK_TYPE_SELECTION_DATA);
    setlocale(LC_NUMERIC, "C");
}
