/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

#define RG_TARGET_NAMESPACE cIconView
#define _SELF(s) (GTK_ICON_VIEW(RVAL2GOBJ(s)))

static ID id_model;
static ID id_select_path;

static VALUE
iview_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE model;
    rb_scan_args(argc, argv, "01", &model);
    if (NIL_P(model)){
        RBGTK_INITIALIZE(self, gtk_icon_view_new());
    } else {
        G_CHILD_SET(self, id_model, model);
        RBGTK_INITIALIZE(self, 
                         gtk_icon_view_new_with_model(GTK_TREE_MODEL(RVAL2GOBJ(model))));
    }
    return Qnil;
}

static VALUE
iview_get_path_at_pos(VALUE self, VALUE x, VALUE y)
{
    return GTKTREEPATH2RVAL(gtk_icon_view_get_path_at_pos(_SELF(self), NUM2INT(x), NUM2INT(y)));
}

static void
iview_foreach_func(GtkIconView *iview, GtkTreePath *path, gpointer *func)
{
    rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(iview), GTKTREEPATH2RVAL(path));
}

static VALUE
iview_selected_foreach(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_icon_view_selected_foreach(_SELF(self), 
                                   (GtkIconViewForeachFunc)iview_foreach_func, 
                                   (gpointer)func);
    return self;
}

static VALUE
iview_select_path(VALUE self, VALUE path)
{
    G_CHILD_SET(self, id_select_path, path);
    gtk_icon_view_select_path(_SELF(self), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
iview_unselect_path(VALUE self, VALUE path)
{
    G_CHILD_UNSET(self, id_select_path);
    gtk_icon_view_unselect_path(_SELF(self), RVAL2GTKTREEPATH(path));
    return self;
}
 
static VALUE
iview_path_is_selected(VALUE self, VALUE path)
{
    return CBOOL2RVAL(gtk_icon_view_path_is_selected(_SELF(self), RVAL2GTKTREEPATH(path)));
}

static VALUE
iview_get_selected_items(VALUE self)
{
    GList* list = gtk_icon_view_get_selected_items(_SELF(self));
    VALUE ret = GLIST2ARY2(list, GTK_TYPE_TREE_PATH);
    g_list_foreach(list, (GFunc)gtk_tree_path_free, NULL);
    g_list_free(list);
    return ret;
}
 
static VALUE
iview_item_activated(VALUE self, VALUE path)
{
    gtk_icon_view_item_activated(_SELF(self), RVAL2GTKTREEPATH(path));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
iview_create_drag_icon(VALUE self, VALUE path)
{
    return GOBJ2RVAL(gtk_icon_view_create_drag_icon(_SELF(self), RVAL2GTKTREEPATH(path)));
}

static VALUE
iview_enable_model_drag_dest(VALUE self, VALUE rbtargets, VALUE rbactions)
{
    GtkIconView *icon_view = _SELF(self);
    GdkDragAction actions = RVAL2GFLAGS(rbactions, GDK_TYPE_DRAG_ACTION);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);

    gtk_icon_view_enable_model_drag_dest(icon_view, targets, n, actions);

    g_free(targets);

    return self;
}

static VALUE
iview_enable_model_drag_source(VALUE self, VALUE rbstart_button_mask, VALUE rbtargets, VALUE rbactions)
{
    GtkIconView *icon_view = _SELF(self);
    GdkModifierType start_button_mask = RVAL2GFLAGS(rbstart_button_mask, GDK_TYPE_MODIFIER_TYPE);
    GdkDragAction actions = RVAL2GFLAGS(rbactions, GDK_TYPE_DRAG_ACTION);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);

    gtk_icon_view_enable_model_drag_source(icon_view, start_button_mask, targets, n, actions);

    g_free(targets);

    return self;
}

static VALUE
iview_cursor(VALUE self)
{
    GtkTreePath* path;
    GtkCellRenderer* cell;
    gboolean cursor_set = gtk_icon_view_get_cursor(_SELF(self), &path, &cell);
    return cursor_set ? rb_assoc_new(GTKTREEPATH2RVAL(path), GOBJ2RVAL(cell)) : Qnil;
}

static VALUE
iview_get_dest_item_at_pos(VALUE self, VALUE drag_x, VALUE drag_y)
{
    GtkTreePath* path;
    GtkIconViewDropPosition pos;
    gboolean item_at_pos = gtk_icon_view_get_dest_item_at_pos(_SELF(self), NUM2INT(drag_x), NUM2INT(drag_y), &path, &pos);
    return item_at_pos ? rb_assoc_new(GTKTREEPATH2RVAL(path),
                                      GENUM2RVAL(pos, GTK_TYPE_ICON_VIEW_DROP_POSITION)) : Qnil;
}

static VALUE
iview_drag_dest_item(VALUE self)
{
    GtkTreePath* path;
    GtkIconViewDropPosition pos;
    gtk_icon_view_get_drag_dest_item(_SELF(self), &path, &pos);
    return rb_assoc_new(GTKTREEPATH2RVAL(path),
                        GENUM2RVAL(pos, GTK_TYPE_ICON_VIEW_DROP_POSITION));
}

static VALUE
iview_get_item_at_pos(VALUE self, VALUE x, VALUE y)
{
    GtkTreePath* path;
    GtkCellRenderer* cell;
    gboolean item_at_pos = gtk_icon_view_get_item_at_pos(_SELF(self), NUM2INT(x), NUM2INT(y), &path, &cell);
    return item_at_pos ? rb_assoc_new(GTKTREEPATH2RVAL(path), GOBJ2RVAL(cell)) : Qnil;
}

static VALUE
iview_visible_range(VALUE self)
{
    GtkTreePath* start_path;
    GtkTreePath* end_path;

    gboolean valid_paths = gtk_icon_view_get_visible_range(_SELF(self), &start_path, &end_path);

    return valid_paths ? rb_assoc_new(GTKTREEPATH2RVAL(start_path),
                                      GTKTREEPATH2RVAL(end_path)) : Qnil;
}

static VALUE
iview_scroll_to_path(VALUE self, VALUE path, VALUE use_align, VALUE row_align, VALUE col_align)
{
    gtk_icon_view_scroll_to_path(_SELF(self),
                                 RVAL2GTKTREEPATH(path),
                                 RVAL2CBOOL(use_align),
                                 NUM2DBL(row_align),
                                 NUM2DBL(col_align));
    return self;
}

static VALUE
iview_set_cursor(VALUE self, VALUE path, VALUE cell, VALUE start_editing)
{
    gtk_icon_view_set_cursor(_SELF(self), RVAL2GTKTREEPATH(path),
                             NIL_P(cell) ? NULL : RVAL2GOBJ(cell), RVAL2CBOOL(start_editing));
    return self;
}

static VALUE
iview_set_drag_dest_item(VALUE self, VALUE path, VALUE pos)
{
    gtk_icon_view_set_drag_dest_item(_SELF(self),
                                     NIL_P(path) ? NULL : RVAL2GTKTREEPATH(path),
                                     RVAL2GENUM(pos, GTK_TYPE_ICON_VIEW_DROP_POSITION));
    return self;
}

static VALUE
iview_unset_model_drag_dest(VALUE self)
{
    gtk_icon_view_unset_model_drag_dest(_SELF(self));
    return self;
}

static VALUE
iview_unset_model_drag_source(VALUE self)
{
    gtk_icon_view_unset_model_drag_source(_SELF(self));
    return self;
}
#endif

void
Init_gtk_iconview(void)
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ICON_VIEW, "IconView", mGtk);

    id_model = rb_intern("model");
    id_select_path = rb_intern("select_path");

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", iview_initialize, -1);
    rb_define_method(RG_TARGET_NAMESPACE, "get_path_at_pos", iview_get_path_at_pos, 2);
    rb_define_alias(RG_TARGET_NAMESPACE, "get_path", "get_path_at_pos");
    rb_define_method(RG_TARGET_NAMESPACE, "selected_each", iview_selected_foreach, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "select_path", iview_select_path, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "unselect_path", iview_unselect_path, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "path_is_selected?", iview_path_is_selected, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "selected_items", iview_get_selected_items, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "item_activated", iview_item_activated, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(RG_TARGET_NAMESPACE, "create_drag_icon", iview_create_drag_icon, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "enable_model_drag_dest", iview_enable_model_drag_dest, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "enable_model_drag_source", iview_enable_model_drag_source, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "cursor", iview_cursor, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "get_dest_item", iview_get_dest_item_at_pos, 2); 
    rb_define_method(RG_TARGET_NAMESPACE, "drag_dest_item", iview_drag_dest_item, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "get_item", iview_get_item_at_pos, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "visible_range", iview_visible_range, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "scroll_to_path", iview_scroll_to_path, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_cursor", iview_set_cursor, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "set_drag_dest_item", iview_set_drag_dest_item, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "unset_model_drag_dest", iview_unset_model_drag_dest, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "unset_model_drag_source", iview_unset_model_drag_source, 0);

    /* GtkIconViewDropPosition */
    G_DEF_CLASS(GTK_TYPE_ICON_VIEW_DROP_POSITION, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_ICON_VIEW_DROP_POSITION, "GTK_ICON_VIEW_");
#endif
}
