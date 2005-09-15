/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkiconview.c -

  $Author: ggc $
  $Date: 2005/09/15 18:23:00 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

#define _SELF(s) (GTK_ICON_VIEW(RVAL2GOBJ(s)))

static VALUE
iview_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE model;
    rb_scan_args(argc, argv, "01", &model);
    if (NIL_P(model)){
        RBGTK_INITIALIZE(self, gtk_icon_view_new());
    } else {
        RBGTK_INITIALIZE(self, 
                         gtk_icon_view_new_with_model(GTK_TREE_MODEL(RVAL2GOBJ(model))));
    }
    return Qnil;
}

/* Defined as Properties
void        gtk_icon_view_set_model         (GtkIconView *icon_view,
                                             GtkTreeModel *model);
GtkTreeModel* gtk_icon_view_get_model       (GtkIconView *icon_view);
void        gtk_icon_view_set_text_column   (GtkIconView *icon_view,
                                             gint column);
gint        gtk_icon_view_get_text_column   (GtkIconView *icon_view);
void        gtk_icon_view_set_markup_column (GtkIconView *icon_view,
                                             gint column);
gint        gtk_icon_view_get_markup_column (GtkIconView *icon_view);
void        gtk_icon_view_set_pixbuf_column (GtkIconView *icon_view,
                                             gint column);
gint        gtk_icon_view_get_pixbuf_column (GtkIconView *icon_view);
*/

static VALUE
iview_get_path_at_pos(self, x, y)
    VALUE self;
{
    GtkTreePath* path = gtk_icon_view_get_path_at_pos(_SELF(self),
                                                      NUM2INT(x), NUM2INT(y));
    return path ? BOXED2RVAL(path, GTK_TYPE_TREE_PATH) : Qnil;
}

static void
iview_foreach_func(iview, path, func)
    GtkIconView* iview;
    GtkTreePath* path;
    gpointer* func;
{
    rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(iview),
               BOXED2RVAL(path, GTK_TYPE_TREE_PATH)); 
}

static VALUE
iview_selected_foreach(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_icon_view_selected_foreach(_SELF(self), 
                                   (GtkIconViewForeachFunc)iview_foreach_func, 
                                   (gpointer)func);
    return self;
}

/* Defined as Properties 
void        gtk_icon_view_set_selection_mode
                                            (GtkIconView *icon_view,
                                             GtkSelectionMode mode);
GtkSelectionMode gtk_icon_view_get_selection_mode
                                            (GtkIconView *icon_view);
void        gtk_icon_view_set_orientation   (GtkIconView *icon_view,
                                             GtkOrientation orientation);
GtkOrientation gtk_icon_view_get_orientation
                                            (GtkIconView *icon_view);
void        gtk_icon_view_set_columns       (GtkIconView *icon_view,
                                             gint columns);
gint        gtk_icon_view_get_columns       (GtkIconView *icon_view);
void        gtk_icon_view_set_item_width    (GtkIconView *icon_view,
                                             gint item_width);
gint        gtk_icon_view_get_item_width    (GtkIconView *icon_view);
void        gtk_icon_view_set_spacing       (GtkIconView *icon_view,
                                             gint spacing);
gint        gtk_icon_view_get_spacing       (GtkIconView *icon_view);
void        gtk_icon_view_set_row_spacing   (GtkIconView *icon_view,
                                             gint row_spacing);
gint        gtk_icon_view_get_row_spacing   (GtkIconView *icon_view);
void        gtk_icon_view_set_column_spacing
                                            (GtkIconView *icon_view,
                                             gint column_spacing);
gint        gtk_icon_view_get_column_spacing
                                            (GtkIconView *icon_view);
void        gtk_icon_view_set_margin        (GtkIconView *icon_view,
                                             gint margin);
gint        gtk_icon_view_get_margin        (GtkIconView *icon_view);
void        gtk_icon_view_set_reorderable   (GtkIconView *icon_view,
                                             gboolean reorderable);
gboolean    gtk_icon_view_get_reorderable   (GtkIconView *icon_view);
*/

static VALUE
iview_select_path(self, path)
    VALUE self, path;
{
    gtk_icon_view_select_path(_SELF(self), RVAL2BOXED(path, GTK_TYPE_TREE_PATH));
    return self;
}

static VALUE
iview_unselect_path(self, path)
       VALUE self, path;
{
    gtk_icon_view_unselect_path(_SELF(self), RVAL2BOXED(path, GTK_TYPE_TREE_PATH));
    return self;
}
 
static VALUE
iview_path_is_selected(self, path)
       VALUE self, path;
{
    return CBOOL2RVAL(gtk_icon_view_path_is_selected(_SELF(self), 
                                                     RVAL2BOXED(path, GTK_TYPE_TREE_PATH)));
    return self;
}

static VALUE
iview_get_selected_items(self)
    VALUE self;
{
    GList* list = gtk_icon_view_get_selected_items(_SELF(self));
    VALUE ret = GLIST2ARY2(list, GTK_TYPE_TREE_PATH);
    g_list_foreach(list, (GFunc)gtk_tree_path_free, NULL);
    g_list_free(list);
    return ret;
}

static VALUE
iview_select_all(self)
    VALUE self;
{
    gtk_icon_view_select_all(_SELF(self));
    return self;
}

static VALUE
iview_unselect_all(self)
    VALUE self;
{
    gtk_icon_view_unselect_all(_SELF(self));
    return self;
}
 
static VALUE
iview_item_activated(self, path)
       VALUE self, path;
{
    gtk_icon_view_item_activated(_SELF(self), 
                                 RVAL2BOXED(path, GTK_TYPE_TREE_PATH));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
iview_create_drag_icon(self, path)
       VALUE self, path;
{
    return GOBJ2RVAL(gtk_icon_view_create_drag_icon(_SELF(self), RVAL2BOXED(path, GTK_TYPE_TREE_PATH)));
}

static VALUE
iview_enable_model_drag_dest(self, targets, actions)
    VALUE self, targets, actions;
{
    gtk_icon_view_enable_model_drag_dest(_SELF(self),
                                         rbgtk_get_target_entry(targets),
                                         RARRAY(targets)->len,
                                         RVAL2GFLAGS(actions, GDK_TYPE_DRAG_ACTION));
    return self;
}

static VALUE
iview_enable_model_drag_source(self, flags, targets, actions)
    VALUE self, flags, targets, actions;
{
    gtk_icon_view_enable_model_drag_source(_SELF(self),
                                           RVAL2GFLAGS(flags, GDK_TYPE_MODIFIER_TYPE),
                                           rbgtk_get_target_entry(targets),
                                           RARRAY(targets)->len,
                                           RVAL2GFLAGS(actions, GDK_TYPE_DRAG_ACTION));
    return self;
}

static VALUE
iview_cursor(self)
    VALUE self;
{
    GtkTreePath* path;
    GtkCellRenderer* cell;
    gboolean cursor_set = gtk_icon_view_get_cursor(_SELF(self), &path, &cell);
    return rb_ary_new3(3, RTEST(cursor_set), BOXED2RVAL(path, GTK_TYPE_TREE_PATH), GOBJ2RVAL(cell));
}

static VALUE
iview_get_dest_item_at_pos(self, drag_x, drag_y)
    VALUE self, drag_x, drag_y;
{
    GtkTreePath* path;
    GtkIconViewDropPosition pos;
    gboolean item_at_pos = gtk_icon_view_get_dest_item_at_pos(_SELF(self), NUM2INT(drag_x), NUM2INT(drag_y), &path, &pos);
    return rb_ary_new3(3,
                       RTEST(item_at_pos),
                       BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
                       GENUM2RVAL(pos, GTK_TYPE_ICON_VIEW_DROP_POSITION));
}

static VALUE
iview_drag_dest_item(self)
    VALUE self;
{
    GtkTreePath* path;
    GtkIconViewDropPosition pos;
    gtk_icon_view_get_drag_dest_item(_SELF(self), &path, &pos);
    return rb_ary_new3(2,
                       BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
                       GENUM2RVAL(pos, GTK_TYPE_ICON_VIEW_DROP_POSITION));
}

static VALUE
iview_get_item_at_pos(self, x, y)
    VALUE self, x, y;
{
    GtkTreePath* path;
    GtkCellRenderer* cell;
    gboolean item_at_pos = gtk_icon_view_get_item_at_pos(_SELF(self), NUM2INT(x), NUM2INT(y), &path, &cell);
    return rb_ary_new3(3,
                       RTEST(item_at_pos),
                       BOXED2RVAL(path, GTK_TYPE_TREE_PATH),
                       GOBJ2RVAL(cell));
}

static VALUE
iview_visible_range(self)
    VALUE self;
{
    GtkTreePath* start_path;
    GtkTreePath* end_path;

    gboolean valid_paths = gtk_icon_view_get_visible_range(_SELF(self), &start_path, &end_path);

    return rb_ary_new3(3,
                       RTEST(valid_paths),
                       BOXED2RVAL(start_path, GTK_TYPE_TREE_PATH),
                       BOXED2RVAL(end_path, GTK_TYPE_TREE_PATH));
}
#endif

void
Init_gtk_iconview()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE iview = G_DEF_CLASS(GTK_TYPE_ICON_VIEW, "IconView", mGtk);
    rb_define_method(iview, "initialize", iview_initialize, -1);
    rb_define_method(iview, "get_path_at_pos", iview_get_path_at_pos, 2);
    rb_define_method(iview, "selected_each", iview_selected_foreach, 0);
    rb_define_method(iview, "select_path", iview_select_path, 1);
    rb_define_method(iview, "unselect_path", iview_unselect_path, 1);
    rb_define_method(iview, "path_is_selected?", iview_path_is_selected, 1);
    rb_define_method(iview, "selected_items", iview_get_selected_items, 0);
    rb_define_method(iview, "select_all", iview_select_all, 0);
    rb_define_method(iview, "unselect_all", iview_unselect_all, 0);
    rb_define_method(iview, "item_activated", iview_item_activated, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(iview, "create_drag_icon", iview_create_drag_icon, 1);
    rb_define_method(iview, "enable_model_drag_dest", iview_enable_model_drag_dest, 2);
    rb_define_method(iview, "enable_model_drag_source", iview_enable_model_drag_source, 3);
    rb_define_method(iview, "cursor", iview_cursor, 0);
    rb_define_method(iview, "get_dest_item_at_pos", iview_get_dest_item_at_pos, 2);
    rb_define_method(iview, "drag_dest_item", iview_drag_dest_item, 0);
    rb_define_method(iview, "get_item_at_pos", iview_get_item_at_pos, 2);
    rb_define_method(iview, "visible_range", iview_visible_range, 0);

    /* GtkIconViewDropPosition */
    G_DEF_CLASS(GTK_TYPE_ICON_VIEW_DROP_POSITION, "Type", iview);
    G_DEF_CONSTANTS(iview, GTK_TYPE_ICON_VIEW_DROP_POSITION, "GTK_ICON_VIEW_");
#endif
}
