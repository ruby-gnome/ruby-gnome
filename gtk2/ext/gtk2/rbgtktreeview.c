/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#define _SELF(s) (GTK_TREE_VIEW(RVAL2GOBJ(s)))
#define TREEVIEW_COL(c) (GTK_TREE_VIEW_COLUMN(RVAL2GOBJ(c)))
#define RVAL2CELLRENDERER(c) (GTK_CELL_RENDERER(RVAL2GOBJ(c)))

static ID id_model;
static ID id_selection;

static VALUE
treeview_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE model;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &model) == 1) {
        G_CHILD_SET(self, id_model, model);
        widget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(RVAL2GOBJ(model)));
    }
    else {
        widget = gtk_tree_view_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
treeview_get_selection(VALUE self)
{
    VALUE ret = GOBJ2RVAL(gtk_tree_view_get_selection(_SELF(self)));
    G_CHILD_SET(self, id_selection, ret);
    return ret;
}

static VALUE
treeview_columns_autosize(VALUE self)
{
    gtk_tree_view_columns_autosize(_SELF(self));
    return self;
}

static VALUE
treeview_append_column(VALUE self, VALUE column)
{
    G_CHILD_ADD(self, column);
    return INT2NUM(gtk_tree_view_append_column(_SELF(self), 
                                               TREEVIEW_COL(column)));
}

static VALUE
treeview_remove_column(VALUE self, VALUE column)
{
    G_CHILD_REMOVE(self, column);
    return INT2NUM(gtk_tree_view_remove_column(_SELF(self), 
                                               TREEVIEW_COL(column)));
}  

static void
cell_data_func(GtkTreeViewColumn *column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter, gpointer func)
{
    iter->user_data3 = model;
    rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(column),
               GOBJ2RVAL(cell), GOBJ2RVAL(model), 
               GTKTREEITER2RVAL(iter));
}

static VALUE
treeview_insert_column(int argc, VALUE *argv, VALUE self)
{
    VALUE args[4];

    rb_scan_args(argc, argv, "22", /* NORMAL    ATTRIBUTES  DATA_FUNC */
                 &args[0],         /* column    position    position  */
                 &args[1],         /* position  title       title     */
                 &args[2],         /*           renderer    renderer  */
                 &args[3]);        /*           attributes            */

    if (argc == 2) {
        G_CHILD_ADD(self, args[0]);
        return INT2NUM(gtk_tree_view_insert_column(_SELF(self),
                                                   TREEVIEW_COL(args[0]),
                                                   NUM2INT(args[1])));
    } else if (argc == 3) {
        int ret;
        VALUE func = rb_block_proc();

        G_RELATIVE(self, func);
        ret = gtk_tree_view_insert_column_with_data_func(_SELF(self),
                                                         NUM2INT(args[0]),
                                                         RVAL2CSTR(args[1]),
                                                         RVAL2CELLRENDERER(args[2]),
                                                         (GtkTreeCellDataFunc)cell_data_func,
                                                         (gpointer)func,
                                                         NULL);
        return INT2NUM(ret);
    } else if (argc == 4) {
        int i;
        int col;
        int ret;
        const gchar *name;
        VALUE ary;
        GtkCellRenderer* renderer = RVAL2CELLRENDERER(args[2]);
        
        GtkTreeViewColumn* column = gtk_tree_view_column_new();

        Check_Type(args[3], T_HASH);

        /* TODO: Should this really be done before we know that everything
         * below worked without error? */
        G_CHILD_ADD(self, args[2]);
        G_CHILD_ADD(self, args[3]);

        gtk_tree_view_column_set_title(column, RVAL2CSTR(args[1]));
        gtk_tree_view_column_pack_start(column, renderer, TRUE);
        
        ret = gtk_tree_view_insert_column(_SELF(self), column, NUM2INT(args[0]));
        ary = rb_funcall(args[3], rb_intern("to_a"), 0);
        for (i = 0; i < RARRAY_LEN(ary); i++) {
            VALUE val = RARRAY_PTR(RARRAY_PTR(ary)[i])[0];
            if (SYMBOL_P(val)) {
                name = rb_id2name(SYM2ID(val));
            } else {
                name = RVAL2CSTR(val);
            }
            col = NUM2INT(RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
            gtk_tree_view_column_add_attribute(column,
                                               renderer,
                                               name, col);
        }
        return INT2NUM(ret);
    } else {
	rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
    }

    return Qnil;
}

static VALUE
treeview_get_column(VALUE self, VALUE num)
{
    return GOBJ2RVAL(gtk_tree_view_get_column(_SELF(self), NUM2INT(num)));
}

static VALUE
treeview_get_columns(VALUE self)
{
    return GLIST2ARYF(gtk_tree_view_get_columns(_SELF(self)));
}

static VALUE
treeview_move_column_after(VALUE self, VALUE column, VALUE base_column)
{
    gtk_tree_view_move_column_after(_SELF(self), TREEVIEW_COL(column),
                                    NIL_P(base_column) ? NULL : TREEVIEW_COL(base_column));
    return self;
}

static gboolean
column_drop_func(GtkTreeView *treeview, GtkTreeViewColumn *column, GtkTreeViewColumn *prev_column, GtkTreeViewColumn *next_column, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(treeview),
                      GOBJ2RVAL(column), GOBJ2RVAL(prev_column), 
                      GOBJ2RVAL(next_column)));
}

static VALUE
treeview_set_column_drag_function(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_tree_view_set_column_drag_function(_SELF(self), 
                                           (GtkTreeViewColumnDropFunc)column_drop_func,
                                           (gpointer)func, NULL);
    return self;
}

static VALUE
treeview_scroll_to_point(VALUE self, VALUE x, VALUE y)
{
    gtk_tree_view_scroll_to_point(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
treeview_scroll_to_cell(VALUE self, VALUE path, VALUE column, VALUE use_align, VALUE row_align, VALUE col_align)
{
    gtk_tree_view_scroll_to_cell(_SELF(self),
                                 NIL_P(path) ? NULL : RVAL2GTKTREEPATH(path),
                                 NIL_P(column) ? NULL : TREEVIEW_COL(column), 
                                 RVAL2CBOOL(use_align),
                                 NUM2DBL(row_align), NUM2DBL(col_align));
    return self;
}

static VALUE
treeview_set_cursor(VALUE self, VALUE path, VALUE focus_column, VALUE start_editing)
{
    gtk_tree_view_set_cursor(_SELF(self), RVAL2GTKTREEPATH(path),
                             NIL_P(focus_column) ? NULL : TREEVIEW_COL(focus_column), 
                             RVAL2CBOOL(start_editing));
    return self;
}

static VALUE
treeview_get_cursor(VALUE self)
{
    GtkTreePath* path;
    GtkTreeViewColumn* focus_column;

    gtk_tree_view_get_cursor(_SELF(self), &path, &focus_column);
    return rb_ary_new3(2, path ? GTKTREEPATH2RVAL(path) : Qnil,
                       GOBJ2RVAL(focus_column));
}

/* Defined as Signals
void                gtk_tree_view_row_activated         (GtkTreeView *tree_view,
                                                         GtkTreePath *path,
                                                         GtkTreeViewColumn *column);
*/

static VALUE
treeview_expand_all(VALUE self)
{
    gtk_tree_view_expand_all(_SELF(self));
    return self;
}

static VALUE
treeview_collapse_all(VALUE self)
{
    gtk_tree_view_collapse_all(_SELF(self));
    return self;
}

static VALUE
treeview_expand_row(VALUE self, VALUE path, VALUE open_all)
{
    return CBOOL2RVAL(gtk_tree_view_expand_row(_SELF(self), 
                                               RVAL2GTKTREEPATH(path),
                                               RVAL2CBOOL(open_all)));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
treeview_expand_to_path(VALUE self, VALUE path)
{
    gtk_tree_view_expand_to_path(_SELF(self), RVAL2GTKTREEPATH(path));
    return self;
}
#endif

static VALUE
treeview_collapse_row(VALUE self, VALUE path)
{
    return CBOOL2RVAL(gtk_tree_view_collapse_row(_SELF(self), 
                                                 RVAL2GTKTREEPATH(path)));
}

static void
mapping_func(GtkTreeView *treeview, GtkTreePath *path, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(treeview),
               GTKTREEPATH2RVAL(path));
}

static VALUE
treeview_map_expanded_rows(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_tree_view_map_expanded_rows(_SELF(self), 
                                    (GtkTreeViewMappingFunc)mapping_func,
                                    (gpointer)func);
    return self;
}

static VALUE
treeview_row_expanded(VALUE self, VALUE path)
{
    return CBOOL2RVAL(gtk_tree_view_row_expanded(_SELF(self), 
                                                 RVAL2GTKTREEPATH(path)));
}

static VALUE
treeview_get_path_at_pos(VALUE self, VALUE x, VALUE y)
{
    GtkTreePath* path;
    GtkTreeViewColumn* column;
    gint cell_x, cell_y;
    gboolean ret;

    ret = gtk_tree_view_get_path_at_pos(_SELF(self), 
                                        NUM2INT(x), NUM2INT(y),
                                        &path, &column, &cell_x, &cell_y);
    return ret ? rb_ary_new3(4, 
                             path ? GTKTREEPATH2RVAL(path) : Qnil,
                             column ? GOBJ2RVAL(column) : Qnil,
                             INT2NUM(cell_x), INT2NUM(cell_y)) : Qnil;
}

static VALUE
treeview_get_cell_area(VALUE self, VALUE path, VALUE column)
{
    GdkRectangle rect;
    gtk_tree_view_get_cell_area(_SELF(self), 
                                NIL_P(path) ? NULL : RVAL2GTKTREEPATH(path),
                                NIL_P(column) ? NULL : TREEVIEW_COL(column), 
                                &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
treeview_get_background_area(VALUE self, VALUE path, VALUE column)
{
    GdkRectangle rect;
    gtk_tree_view_get_background_area(_SELF(self), 
                                      NIL_P(path) ? NULL : RVAL2GTKTREEPATH(path),
                                      NIL_P(column) ? NULL : TREEVIEW_COL(column), 
                                      &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
treeview_get_visible_rect(VALUE self)
{
    GdkRectangle rect;
    gtk_tree_view_get_visible_rect(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
treeview_get_visible_range(VALUE self)
{
    GtkTreePath* start_path;
    GtkTreePath* end_path;

    gboolean valid_paths = gtk_tree_view_get_visible_range(_SELF(self), &start_path, &end_path);

    return valid_paths ? rb_assoc_new(GTKTREEPATH2RVAL(start_path),
                                      GTKTREEPATH2RVAL(end_path)) : Qnil;
}
#endif

static VALUE
treeview_get_bin_window(VALUE self)
{
    return GOBJ2RVAL(gtk_tree_view_get_bin_window(_SELF(self)));
}

static VALUE
treeview_widget_to_tree_coords(VALUE self, VALUE wx, VALUE wy)
{
    gint tx, ty;
    gtk_tree_view_widget_to_tree_coords(_SELF(self), 
                                        NUM2INT(wx), NUM2INT(wy), 
                                        &tx, &ty);
    return rb_ary_new3(2, INT2NUM(tx), INT2NUM(ty));
}

static VALUE
treeview_tree_to_widget_coords(VALUE self, VALUE tx, VALUE ty)
{
    gint wx, wy;
    gtk_tree_view_tree_to_widget_coords(_SELF(self),
                                        NUM2INT(tx), NUM2INT(ty), 
                                        &wx, &wy);
    return rb_ary_new3(2, INT2NUM(wx), INT2NUM(wy));
}

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
treeview_convert_bin_window_to_tree_coords(VALUE self, VALUE bx, VALUE by)
{
    gint tx, ty;
    gtk_tree_view_convert_bin_window_to_tree_coords(_SELF(self),
                                                    NUM2INT(bx), NUM2INT(by),
                                                    &tx, &ty);
    return rb_ary_new3(2, INT2NUM(tx), INT2NUM(ty));
}
static VALUE
treeview_convert_bin_window_to_widget_coords(VALUE self, VALUE bx, VALUE by)
{
    gint wx, wy;
    gtk_tree_view_convert_bin_window_to_widget_coords(_SELF(self),
                                                      NUM2INT(bx), NUM2INT(by),
                                                      &wx, &wy);
    return rb_ary_new3(2, INT2NUM(wx), INT2NUM(wy));
}

static VALUE
treeview_convert_tree_to_bin_window_coords(VALUE self, VALUE tx, VALUE ty)
{
    gint bx, by;
    gtk_tree_view_convert_tree_to_bin_window_coords(_SELF(self),
                                                    NUM2INT(tx), NUM2INT(ty),
                                                    &bx, &by);
    return rb_ary_new3(2, INT2NUM(bx), INT2NUM(by));
}
static VALUE
treeview_convert_tree_to_widget_coords(VALUE self, VALUE tx, VALUE ty)
{
    gint wx, wy;
    gtk_tree_view_convert_tree_to_widget_coords(_SELF(self),
                                                NUM2INT(tx), NUM2INT(ty),
                                                &wx, &wy);
    return rb_ary_new3(2, INT2NUM(wx), INT2NUM(wy));
}

static VALUE
treeview_convert_widget_to_bin_window_coords(VALUE self, VALUE wx, VALUE wy)
{
    gint bx, by;
    gtk_tree_view_convert_widget_to_bin_window_coords(_SELF(self),
                                                      NUM2INT(wx), NUM2INT(wy),
                                                      &bx, &by);
    return rb_ary_new3(2, INT2NUM(bx), INT2NUM(by));
}
static VALUE
treeview_convert_widget_to_tree_coords(VALUE self, VALUE wx, VALUE wy)
{
    gint tx, ty;
    gtk_tree_view_convert_widget_to_tree_coords(_SELF(self),
                                                NUM2INT(wx), NUM2INT(wy),
                                                &tx, &ty);
    return rb_ary_new3(2, INT2NUM(tx), INT2NUM(ty));
}
#endif

static VALUE
treeview_enable_model_drag_dest(VALUE self, VALUE rbtargets, VALUE rbactions)
{
    GtkTreeView *view = _SELF(self);
    GdkDragAction actions = RVAL2GFLAGS(rbactions, GDK_TYPE_DRAG_ACTION);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES(rbtargets, &n);

    gtk_tree_view_enable_model_drag_dest(view, targets, n, actions);

    g_free(targets);

    return self;
}

static VALUE
treeview_enable_model_drag_source(VALUE self, VALUE rbstart_button_mask, VALUE rbtargets, VALUE rbactions)
{
    GtkTreeView *view = _SELF(self);
    GdkModifierType start_button_mask = RVAL2GFLAGS(rbstart_button_mask, GDK_TYPE_MODIFIER_TYPE);
    GdkDragAction actions = RVAL2GFLAGS(rbactions, GDK_TYPE_DRAG_ACTION);
    long n;
    GtkTargetEntry *targets = RVAL2GTKTARGETENTRIES_ACCEPT_NIL(rbtargets, &n);

    if (targets == NULL)
        return self;

    gtk_tree_view_enable_model_drag_source(view, start_button_mask, targets, n, actions);

    g_free(targets);

    return self;
}

static VALUE
treeview_unset_rows_drag_source(VALUE self)
{
    gtk_tree_view_unset_rows_drag_source(_SELF(self));
    return self;
}

static VALUE
treeview_unset_rows_drag_dest(VALUE self)
{
    gtk_tree_view_unset_rows_drag_dest(_SELF(self));
    return self;
}

static VALUE
treeview_set_drag_dest_row(VALUE self, VALUE path, VALUE pos)
{
    gtk_tree_view_set_drag_dest_row(_SELF(self), 
                                    NIL_P(path)?NULL:RVAL2GTKTREEPATH(path),
                                    RVAL2GENUM(pos, GTK_TYPE_TREE_VIEW_DROP_POSITION));
    return self;
}

static VALUE
treeview_get_drag_dest_row(VALUE self)
{
    GtkTreePath* path = NULL;
    GtkTreeViewDropPosition pos;
    gtk_tree_view_get_drag_dest_row(_SELF(self), &path, &pos);
    return rb_ary_new3(2, path ? GTKTREEPATH2RVAL(path) : Qnil, 
                       GENUM2RVAL(pos, GTK_TYPE_TREE_VIEW_DROP_POSITION));
}

static VALUE
treeview_get_dest_row_at_pos(VALUE self, VALUE drag_x, VALUE drag_y)
{
    GtkTreePath* path;
    GtkTreeViewDropPosition pos;
    gboolean ret;

    ret = gtk_tree_view_get_dest_row_at_pos(_SELF(self), 
                                            NUM2INT(drag_x), NUM2INT(drag_y),
                                            &path, &pos);
    return ret ? rb_ary_new3(2, path ? GTKTREEPATH2RVAL(path) : Qnil, 
                             GENUM2RVAL(pos, GTK_TYPE_TREE_VIEW_DROP_POSITION)) : Qnil;
}

static VALUE
treeview_create_row_drag_icon(VALUE self, VALUE path)
{
    return GOBJ2RVAL(gtk_tree_view_create_row_drag_icon(_SELF(self),
                                                        RVAL2GTKTREEPATH(path)));
}

/*
  We don't need this.
  GtkTreeViewSearchEqualFunc gtk_tree_view_get_search_equal_func
  (GtkTreeView *tree_view);
*/

static gboolean
search_equal_func(GtkTreeModel *model, gint column, const gchar *key, GtkTreeIter *iter, gpointer func)
{
    iter->user_data3 = model;
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 4, 
                            GOBJ2RVAL(model), INT2NUM(column),
                            CSTR2RVAL(key), GTKTREEITER2RVAL(iter)));
}

static VALUE
treeview_set_search_equal_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_tree_view_set_search_equal_func(_SELF(self),
                                        (GtkTreeViewSearchEqualFunc)search_equal_func,
                                        (gpointer)func, NULL);
    return self;
}



/*
 * Optional Signals
 */
static VALUE
treeview_signal_func(G_GNUC_UNUSED guint num, const GValue *values)
{
    GtkTreeView* view = g_value_get_object(&values[0]);
    GtkTreeIter* iter = g_value_get_boxed(&values[1]);
    iter->user_data3 = gtk_tree_view_get_model(view);

    return rb_ary_new3(3, GOBJ2RVAL(view), GTKTREEITER2RVAL(iter), GVAL2RVAL(&values[2]));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
treeview_set_cursor_on_cell(VALUE self, VALUE path, VALUE focus_column, VALUE focus_cell, VALUE start_editing)
{
    gtk_tree_view_set_cursor_on_cell(_SELF(self), RVAL2GTKTREEPATH(path),
                                     NIL_P(focus_column) ? NULL : TREEVIEW_COL(focus_column), 
                                     NIL_P(focus_cell) ? NULL : GTK_CELL_RENDERER(RVAL2GOBJ(focus_cell)), 
                                     RVAL2CBOOL(start_editing));
    return self;
}
#endif

/* Defined as Property.
gboolean    gtk_tree_view_get_fixed_height_mode
                                            (GtkTreeView *tree_view);
void        gtk_tree_view_set_fixed_height_mode
                                            (GtkTreeView *tree_view,
                                             gboolean enable);
gboolean    gtk_tree_view_get_hover_selection
                                            (GtkTreeView *tree_view);
void        gtk_tree_view_set_hover_selection
                                            (GtkTreeView *tree_view,
                                             gboolean hover);
gboolean    gtk_tree_view_get_hover_expand  (GtkTreeView *tree_view);
void        gtk_tree_view_set_hover_expand  (GtkTreeView *tree_view,
                                             gboolean expand);
*/

/* How can I implement this?
GtkTreeViewRowSeparatorFunc gtk_tree_view_get_row_separator_func
                                            (GtkTreeView *tree_view);
*/

#if GTK_CHECK_VERSION(2,6,0)
static gboolean
row_separator_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer func)
{
    VALUE ret;
    iter->user_data3 = model;
    ret = rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(model), 
                     GTKTREEITER2RVAL(iter));
    return CBOOL2RVAL(ret);
}

static VALUE
treeview_set_row_separator_func(VALUE self)
{
    VALUE func = rb_block_proc();

    G_RELATIVE(self, func);
    gtk_tree_view_set_row_separator_func(_SELF(self), 
                                         row_separator_func, 
                                         (gpointer)func, 
                                         (GtkDestroyNotify)NULL);
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
treeview_get_search_entry(VALUE self)
{
    return GOBJ2RVAL(gtk_tree_view_get_search_entry(_SELF(self)));
}

static VALUE
treeview_set_search_entry(VALUE self, VALUE entry)
{
    gtk_tree_view_set_search_entry(_SELF(self), RVAL2GOBJ(entry));
    return self;
}

/* Can't define this.
GtkTreeViewSearchPositionFunc gtk_tree_view_get_search_position_func
                                            (GtkTreeView *tree_view);
*/

struct callback_arg
{
    VALUE callback;
    VALUE tree_view;
    VALUE search_dialog;
};

static VALUE
invoke_callback(VALUE data)
{
    struct callback_arg *arg = (struct callback_arg *)data;
    rb_funcall(arg->callback, id_call, 2, arg->tree_view, arg->search_dialog);
    return Qnil;
}

static void
search_position_func(GtkTreeView *tree_view, GtkWidget *search_dialog, gpointer func)
{
    struct callback_arg arg;

    arg.callback = (VALUE)func;
    arg.tree_view = GOBJ2RVAL(tree_view);
    arg.search_dialog = GOBJ2RVAL(search_dialog);
    G_PROTECT_CALLBACK(invoke_callback, &arg);
}

static void
remove_callback_reference(gpointer data)
{
    G_CHILD_REMOVE(mGtk, (VALUE)data);
}


static VALUE
treeview_set_search_position_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_CHILD_ADD(mGtk, func);
    gtk_tree_view_set_search_position_func(_SELF(self),
                                           (GtkTreeViewSearchPositionFunc)search_position_func,
                                           (gpointer)func,
                                           (GDestroyNotify)remove_callback_reference);
    return self;
}

#endif
/* Defined as properties
gboolean    gtk_tree_view_get_headers_clickable
                                            (GtkTreeView *tree_view);
void        gtk_tree_view_set_headers_clickable
                                            (GtkTreeView *tree_view,
                                             gboolean setting);
gboolean    gtk_tree_view_get_rubber_banding
                                            (GtkTreeView *tree_view);
void        gtk_tree_view_set_rubber_banding
                                            (GtkTreeView *tree_view,
                                             gboolean enable);
gboolean    gtk_tree_view_get_enable_tree_lines
                                            (GtkTreeView *tree_view);
void        gtk_tree_view_set_enable_tree_lines
                                            (GtkTreeView *tree_view,
                                             gboolean enabled);

// Use Gtk::TreeView#enable_grid_lines, #set_enable_grid_lines instead.
GtkTreeViewGridLines gtk_tree_view_get_grid_lines
                                            (GtkTreeView *tree_view);

void        gtk_tree_view_set_grid_lines    (GtkTreeView *tree_view,
                                             GtkTreeViewGridLines grid_lines);
*/

void 
Init_gtk_treeview(void)
{
    VALUE gTv = G_DEF_CLASS(GTK_TYPE_TREE_VIEW, "TreeView", mGtk);

    id_selection = rb_intern("selection");
    id_model = rb_intern("model");

    rb_define_method(gTv, "initialize", treeview_initialize, -1);
    rb_define_method(gTv, "selection", treeview_get_selection, 0);
    rb_define_method(gTv, "columns_autosize", treeview_columns_autosize, 0);
    rb_define_method(gTv, "append_column", treeview_append_column, 1);
    rb_define_method(gTv, "remove_column", treeview_remove_column, 1);
    rb_define_method(gTv, "insert_column", treeview_insert_column, -1);
    rb_define_method(gTv, "get_column", treeview_get_column, 1);
    rb_define_method(gTv, "columns", treeview_get_columns, 0);
    rb_define_method(gTv, "move_column_after", treeview_move_column_after, 2);
    rb_define_method(gTv, "set_column_drag_function", treeview_set_column_drag_function, 0);
    rb_define_method(gTv, "scroll_to_point", treeview_scroll_to_point, 2);
    rb_define_method(gTv, "scroll_to_cell", treeview_scroll_to_cell, 5);
    rb_define_method(gTv, "set_cursor", treeview_set_cursor, 3);
    rb_define_method(gTv, "cursor", treeview_get_cursor, 0);
    rb_define_method(gTv, "expand_all", treeview_expand_all, 0);
    rb_define_method(gTv, "collapse_all", treeview_collapse_all, 0);
    rb_define_method(gTv, "expand_row", treeview_expand_row, 2);
    rb_define_method(gTv, "collapse_row", treeview_collapse_row, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gTv, "expand_to_path", treeview_expand_to_path, 1);
#endif
    rb_define_method(gTv, "map_expanded_rows", treeview_map_expanded_rows, 0);
    rb_define_method(gTv, "row_expanded?", treeview_row_expanded, 1);
    rb_define_method(gTv, "get_path_at_pos", treeview_get_path_at_pos, 2);
    rb_define_alias(gTv, "get_path", "get_path_at_pos");
    rb_define_method(gTv, "get_cell_area", treeview_get_cell_area, 2);
    rb_define_method(gTv, "get_background_area", treeview_get_background_area, 2);
    rb_define_method(gTv, "visible_rect", treeview_get_visible_rect, 0);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gTv, "visible_range", treeview_get_visible_range, 0);
#endif

    rb_define_method(gTv, "bin_window", treeview_get_bin_window, 0);
    rb_define_method(gTv, "widget_to_tree_coords", treeview_widget_to_tree_coords, 2);
    rb_define_method(gTv, "tree_to_widget_coords", treeview_tree_to_widget_coords, 2);
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gTv, "convert_bin_window_to_tree_coords", treeview_convert_bin_window_to_tree_coords, 2);
    rb_define_method(gTv, "convert_bin_window_to_widget_coords", treeview_convert_bin_window_to_widget_coords, 2);
    rb_define_method(gTv, "convert_tree_to_bin_window_coords", treeview_convert_tree_to_bin_window_coords, 2);
    rb_define_method(gTv, "convert_tree_to_widget_coords", treeview_convert_tree_to_widget_coords, 2);
    rb_define_method(gTv, "convert_widget_to_bin_window_coords", treeview_convert_widget_to_bin_window_coords, 2);
    rb_define_method(gTv, "convert_widget_to_tree_coords", treeview_convert_widget_to_tree_coords, 2);
#endif
    rb_define_method(gTv, "enable_model_drag_dest", treeview_enable_model_drag_dest, 2);
    rb_define_method(gTv, "enable_model_drag_source", treeview_enable_model_drag_source, 3);
    rb_define_method(gTv, "unset_rows_drag_source", treeview_unset_rows_drag_source, 0);
    rb_define_method(gTv, "unset_rows_drag_dest", treeview_unset_rows_drag_dest, 0);
    rb_define_method(gTv, "set_drag_dest_row", treeview_set_drag_dest_row, 2);
    rb_define_method(gTv, "drag_dest_row", treeview_get_drag_dest_row, 0);
    rb_define_method(gTv, "get_dest_row_at_pos", treeview_get_dest_row_at_pos, 2);
    rb_define_alias(gTv, "get_dest_row", "get_dest_row_at_pos");
    rb_define_method(gTv, "create_row_drag_icon", treeview_create_row_drag_icon, 1);
    rb_define_method(gTv, "set_search_equal_func", treeview_set_search_equal_func, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gTv, "set_cursor_on_cell", treeview_set_cursor_on_cell, 4);
#endif

#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gTv, "set_row_separator_func", treeview_set_row_separator_func, 0);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gTv, "search_entry", treeview_get_search_entry, 0);
    rb_define_method(gTv, "set_search_entry", treeview_set_search_entry, 1);
    rb_define_method(gTv, "set_search_position_func", treeview_set_search_position_func, 0);
#endif

    /* Constants */
    G_DEF_CLASS(GTK_TYPE_TREE_VIEW_DROP_POSITION, "DropPosition", gTv);
    G_DEF_CONSTANTS(gTv, GTK_TYPE_TREE_VIEW_DROP_POSITION, "GTK_TREE_VIEW_");

#if GTK_CHECK_VERSION(2,10,0)
    /* GtkTreeViewGridLines */
    G_DEF_CLASS(GTK_TYPE_TREE_VIEW_GRID_LINES, "GridLines", gTv);
    G_DEF_CONSTANTS(gTv, GTK_TYPE_TREE_VIEW_GRID_LINES, "GTK_TREE_VIEW_");
#endif

    /* Option Signals */
    G_DEF_SIGNAL_FUNC(gTv, "row-collapsed", (GValToRValSignalFunc)treeview_signal_func);
    G_DEF_SIGNAL_FUNC(gTv, "row-expanded", (GValToRValSignalFunc)treeview_signal_func);
    G_DEF_SIGNAL_FUNC(gTv, "test-collapse-row", (GValToRValSignalFunc)treeview_signal_func);
    G_DEF_SIGNAL_FUNC(gTv, "test-expand-row", (GValToRValSignalFunc)treeview_signal_func);
}
