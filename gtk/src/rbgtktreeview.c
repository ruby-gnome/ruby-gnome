/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreeview.c -

  $Author: mutoh $
  $Date: 2004/03/01 15:04:46 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TREE_VIEW(RVAL2GOBJ(s)))
#define TREEVIEW_COL(c) (GTK_TREE_VIEW_COLUMN(RVAL2GOBJ(c)))
#define TREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2TREEPATH(p) ((GtkTreePath*)RVAL2BOXED(p, GTK_TYPE_TREE_PATH))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define RVAL2ITR(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))
#define RVAL2CELLRENDERER(c) (GTK_CELL_RENDERER(RVAL2GOBJ(c)))

static ID id_column;

static VALUE
treeview_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE model;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &model) == 1) {
        widget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(RVAL2GOBJ(model)));
    }
    else {
        widget = gtk_tree_view_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
treeview_get_selection(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_tree_view_get_selection(_SELF(self)));
}

static VALUE
treeview_columns_autosize(self)
    VALUE self;
{
    gtk_tree_view_columns_autosize(_SELF(self));
    return self;
}

static VALUE
treeview_append_column(self, column)
    VALUE self, column;
{
    G_RELATIVE2(self, column, id_column, column);
    return INT2NUM(gtk_tree_view_append_column(_SELF(self), 
                                               TREEVIEW_COL(column)));
}

static VALUE
treeview_remove_column(self, column)
    VALUE self, column;
{
    G_REMOVE_RELATIVE(self, id_column, column);
    return INT2NUM(gtk_tree_view_remove_column(_SELF(self), 
                                               TREEVIEW_COL(column)));
}  

static void
cell_data_func(column, cell, model, iter, func)
    GtkTreeViewColumn* column;
    GtkCellRenderer* cell;
    GtkTreeModel* model;
    GtkTreeIter* iter;
    gpointer func;
{
    iter->user_data3 = model;
    rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(column),
               GOBJ2RVAL(cell), GOBJ2RVAL(model), 
               BOXED2RVAL(iter, GTK_TYPE_TREE_ITER));
}

static VALUE
treeview_insert_column(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    VALUE args[4];

    rb_scan_args(argc, argv, "22", /* NORMAL    ATTRIBUTES  DATA_FUNC */
                 &args[0],         /* column    position    position  */
                 &args[1],         /* position  title       title     */
                 &args[2],         /*           renderer    renderer  */
                 &args[3]);        /*           attributes            */

    if (argc == 2) {
        G_RELATIVE2(self, args[0], id_column, args[0]);
        return INT2NUM(gtk_tree_view_insert_column(_SELF(self),
                                                   TREEVIEW_COL(args[0]),
                                                   NUM2INT(args[1])));
    } else if (argc == 3) {
        int ret;
        VALUE func = G_BLOCK_PROC();

        G_RELATIVE(self, func);
        ret = gtk_tree_view_insert_column_with_data_func(_SELF(self),
                                                         NUM2INT(args[0]),
                                                         RVAL2CSTR(args[1]),
                                                         RVAL2CELLRENDERER(args[2]),
                                                         cell_data_func,
                                                         (gpointer)func,
                                                         NULL);
        return INT2NUM(ret);
    } else if (argc == 4) {
        int i;
        int col;
        int ret;
        gchar* name;
        VALUE ary;
        GtkCellRenderer* renderer = RVAL2CELLRENDERER(args[2]);
        
        GtkTreeViewColumn* column = gtk_tree_view_column_new();

        Check_Type(args[3], T_HASH);
        gtk_tree_view_column_set_title(column, RVAL2CSTR(args[1]));
        gtk_tree_view_column_pack_start(column, renderer, TRUE);
        
        ret = gtk_tree_view_insert_column(_SELF(self), column, NUM2INT(args[0]));
        ary = rb_funcall(args[3], rb_intern("to_a"), 0);
        for (i = 0; i < RARRAY(ary)->len; i++) {
            VALUE val = RARRAY(RARRAY(ary)->ptr[i])->ptr[0];
            if (SYMBOL_P(val)) {
                name = rb_id2name(SYM2ID(val));
            } else {
                name = RVAL2CSTR(val);
            }
            col = NUM2INT(RARRAY(RARRAY(ary)->ptr[i])->ptr[1]);
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
treeview_get_column(self, num)
    VALUE self, num;
{
    return GOBJ2RVAL(gtk_tree_view_get_column(_SELF(self), NUM2INT(num)));
}

static VALUE
treeview_get_columns(self)
    VALUE self;
{
    return GLIST2ARY(gtk_tree_view_get_columns(_SELF(self)));
}

static VALUE
treeview_move_column_after(self, column, base_column)
    VALUE self, column, base_column;
{
    gtk_tree_view_move_column_after(_SELF(self), TREEVIEW_COL(column),
                                    TREEVIEW_COL(base_column));
    return self;
}

static gboolean
column_drop_func(treeview, column, prev_column, next_column, func)
    GtkTreeView* treeview;
    GtkTreeViewColumn* column;
    GtkTreeViewColumn* prev_column;
    GtkTreeViewColumn* next_column;
    gpointer func;
{
    return RTEST(rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(treeview),
                      GOBJ2RVAL(column), GOBJ2RVAL(prev_column), 
                      GOBJ2RVAL(next_column)));
}

static VALUE
treeview_set_column_drag_function(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_tree_view_set_column_drag_function(_SELF(self), 
                                           (GtkTreeViewColumnDropFunc)column_drop_func,
                                           (gpointer)func, NULL);
    return self;
}

static VALUE
treeview_scroll_to_point(self, x, y)
    VALUE self, x, y;
{
    gtk_tree_view_scroll_to_point(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
treeview_scroll_to_cell(self, path, column, use_align, row_align, col_align)
    VALUE self, path, column, use_align, row_align, col_align;
{
    gtk_tree_view_scroll_to_cell(_SELF(self), RVAL2TREEPATH(path),
                                 TREEVIEW_COL(column), RTEST(use_align),
                                 NUM2DBL(row_align), NUM2DBL(col_align));
    return self;
}

static VALUE
treeview_set_cursor(self, path, focus_column, start_editing)
    VALUE self, path, focus_column, start_editing;
{
    gtk_tree_view_set_cursor(_SELF(self), RVAL2TREEPATH(path),
                             TREEVIEW_COL(focus_column), RTEST(start_editing));
    return self;
}

static VALUE
treeview_get_cursor(self)
    VALUE self;
{
    GtkTreePath* path;
    GtkTreeViewColumn* focus_column;

    gtk_tree_view_get_cursor(_SELF(self), &path, &focus_column);
    return rb_ary_new3(2, (path == NULL) ? Qnil : TREEPATH2RVAL(path),
                       (focus_column == NULL) ? Qnil : GOBJ2RVAL(focus_column));
}

static VALUE
treeview_row_activated(self, path, column)
    VALUE self, path, column;
{
    gtk_tree_view_row_activated(_SELF(self), RVAL2TREEPATH(path),
                                TREEVIEW_COL(column));
    return self;
}

static VALUE
treeview_expand_all(self)
    VALUE self;
{
    gtk_tree_view_expand_all(_SELF(self));
    return self;
}

static VALUE
treeview_collapse_all(self)
    VALUE self;
{
    gtk_tree_view_collapse_all(_SELF(self));
    return self;
}

static VALUE
treeview_expand_row(self, path, open_all)
    VALUE self, path, open_all;
{
    return gtk_tree_view_expand_row(_SELF(self), 
                                    RVAL2TREEPATH(path),
                                    RTEST(open_all)) ? Qtrue: Qfalse;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
treeview_expand_to_path(self, path)
    VALUE self, path;
{
    gtk_tree_view_expand_to_path(_SELF(self), RVAL2TREEPATH(path));
    return self;
}
#endif

static VALUE
treeview_collapse_row(self, path)
    VALUE self, path;
{
    return gtk_tree_view_collapse_row(_SELF(self), 
                                      RVAL2TREEPATH(path)) ? Qtrue : Qfalse;
}

static void
mapping_func(treeview, path, func)
    GtkTreeView* treeview;
    GtkTreePath* path;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(treeview),
               TREEPATH2RVAL(path));
}

static VALUE
treeview_map_expanded_rows(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
    G_RELATIVE(self, func);
    gtk_tree_view_map_expanded_rows(_SELF(self), 
                                    (GtkTreeViewMappingFunc)mapping_func,
                                    (gpointer)func);
    return self;
}

static VALUE
treeview_row_expanded(self, path)
    VALUE self, path;
{
    return gtk_tree_view_row_expanded(_SELF(self), 
                                      RVAL2TREEPATH(path)) ? Qtrue : Qfalse;
}

static VALUE
treeview_get_path_at_pos(self, x, y)
    VALUE self, x, y;
{
    GtkTreePath* path;
    GtkTreeViewColumn* column;
    gint cell_x, cell_y;
    gboolean ret;

    ret = gtk_tree_view_get_path_at_pos(_SELF(self), 
                                        NUM2INT(x), NUM2INT(y),
                                        &path, &column, &cell_x, &cell_y);
    return ret ? rb_ary_new3(4, 
                             (path == NULL) ? Qnil : TREEPATH2RVAL(path),
                             (column == NULL) ? Qnil : GOBJ2RVAL(column),
                             INT2NUM(cell_x), INT2NUM(cell_y)) : Qnil;
}

static VALUE
treeview_get_cell_area(self, path, column)
    VALUE self, path, column;
{
    GdkRectangle rect;
    gtk_tree_view_get_cell_area(_SELF(self), RVAL2TREEPATH(path),
                                TREEVIEW_COL(column), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
treeview_get_background_area(self, path, column)
    VALUE self, path, column;
{
    GdkRectangle rect;
    gtk_tree_view_get_background_area(_SELF(self), 
                                      RVAL2TREEPATH(path),
                                      TREEVIEW_COL(column), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
treeview_get_visible_rect(self)
    VALUE self;
{
    GdkRectangle rect;
    gtk_tree_view_get_visible_rect(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
treeview_get_bin_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_tree_view_get_bin_window(_SELF(self)));
}

static VALUE
treeview_widget_to_tree_coords(self, wx, wy)
    VALUE self, wx, wy;
{
    gint tx, ty;
    gtk_tree_view_widget_to_tree_coords(_SELF(self), 
                                        NUM2INT(wx), NUM2INT(wy), 
                                        &tx, &ty);
    return rb_ary_new3(2, INT2NUM(tx), INT2NUM(ty));
}

static VALUE
treeview_tree_to_widget_coords(self, tx, ty)
    VALUE self, tx, ty;
{
    gint wx, wy;
    gtk_tree_view_tree_to_widget_coords(_SELF(self),
                                        NUM2INT(tx), NUM2INT(ty), 
                                        &wx, &wy);
    return rb_ary_new3(2, INT2NUM(wx), INT2NUM(wy));
}

static VALUE
treeview_enable_model_drag_dest(self, targets, actions)
    VALUE self, targets, actions;
{
    const GtkTargetEntry* entries = rbgtk_get_target_entry(targets);
    int num = RARRAY(targets)->len;

    gtk_tree_view_enable_model_drag_dest(_SELF(self),  entries, 
                                         num, RVAL2GFLAGS(actions, GDK_TYPE_DRAG_ACTION));
    return self;
}

static VALUE
treeview_enable_model_drag_source(self, start_button_mask, targets, actions)
        VALUE self, start_button_mask, targets, actions;
{
    GtkTargetEntry* entries = rbgtk_get_target_entry(targets);
    if (entries){
        gint num = RARRAY(targets)->len;
        
        gtk_tree_view_enable_model_drag_source(_SELF(self), 
                                               RVAL2GFLAGS(start_button_mask, GDK_TYPE_MODIFIER_TYPE), 
                                               entries, num,
                                               RVAL2GFLAGS(actions, GDK_TYPE_DRAG_ACTION));
    }
    return self;
}

static VALUE
treeview_unset_rows_drag_source(self)
    VALUE self;
{
    gtk_tree_view_unset_rows_drag_source(_SELF(self));
    return self;
}

static VALUE
treeview_unset_rows_drag_dest(self)
    VALUE self;
{
    gtk_tree_view_unset_rows_drag_dest(_SELF(self));
    return self;
}

static VALUE
treeview_set_drag_dest_row(self, path, pos)
    VALUE self, path, pos;
{
    gtk_tree_view_set_drag_dest_row(_SELF(self), RVAL2TREEPATH(path),
                                    RVAL2GENUM(pos, GTK_TYPE_TREE_VIEW_DROP_POSITION));
    return self;
}

static VALUE
treeview_get_drag_dest_row(self)
    VALUE self;
{
    GtkTreePath* path;
    GtkTreeViewDropPosition pos;
    gtk_tree_view_get_drag_dest_row(_SELF(self), &path, &pos);
    return rb_ary_new3(2, TREEPATH2RVAL(path), 
                       GENUM2RVAL(pos, GTK_TYPE_TREE_VIEW_DROP_POSITION));
}

static VALUE
treeview_get_dest_row_at_pos(self, drag_x, drag_y)
    VALUE self;
{
    GtkTreePath* path;
    GtkTreeViewDropPosition pos;
    gboolean ret;

    ret = gtk_tree_view_get_dest_row_at_pos(_SELF(self), 
                                            NUM2INT(drag_x), NUM2INT(drag_y),
                                            &path, &pos);
    return ret ? rb_ary_new3(2, TREEPATH2RVAL(path), 
                             GENUM2RVAL(pos, GTK_TYPE_TREE_VIEW_DROP_POSITION)) : Qnil;
}

static VALUE
treeview_create_row_drag_icon(self, path)
    VALUE self, path;
{
    return GOBJ2RVAL(gtk_tree_view_create_row_drag_icon(_SELF(self),
                                                        RVAL2TREEPATH(path)));
}

/*
  We don't need this.
  GtkTreeViewSearchEqualFunc gtk_tree_view_get_search_equal_func
  (GtkTreeView *tree_view);
*/

static gboolean
search_equal_func(model, column, key, iter, func)
    GtkTreeModel* model;
    gint column;
    const gchar* key;
    GtkTreeIter* iter;
    gpointer func;
{
    iter->user_data3 = model;
    return RTEST(rb_funcall((VALUE)func, id_call, 4, 
                            GOBJ2RVAL(model), INT2NUM(column),
                            CSTR2RVAL(key), ITR2RVAL(iter)));
}

static VALUE
treeview_set_search_equal_func(self)
    VALUE self;
{
    VALUE func = G_BLOCK_PROC();
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
treeview_signal_func(num, values)
    guint num;
    const GValue* values;
{
    GtkTreeView* view = g_value_get_object(&values[0]);
    GtkTreeIter* iter = g_value_get_boxed(&values[1]);
    iter->user_data3 = gtk_tree_view_get_model(view);

    return rb_ary_new3(3, GOBJ2RVAL(view), ITR2RVAL(iter), GVAL2RVAL(&values[2]));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
treeview_set_cursor_on_cell(self, path, focus_column, focus_cell, start_editing)
    VALUE self, path, focus_column, focus_cell, start_editing;
{
    gtk_tree_view_set_cursor_on_cell(_SELF(self), RVAL2TREEPATH(path),
                             TREEVIEW_COL(focus_column), GTK_CELL_RENDERER(RVAL2GOBJ(focus_cell)), 
                             RTEST(start_editing));
    return self;
}
#endif

void 
Init_gtk_treeview()
{
    VALUE gTv = G_DEF_CLASS(GTK_TYPE_TREE_VIEW, "TreeView", mGtk);

    id_column = rb_intern("__columns__");
    rb_ivar_set(gTv, id_column, Qnil);

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
    rb_define_method(gTv, "row_activated", treeview_row_activated, 2);
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
    rb_define_method(gTv, "get_cell_area", treeview_get_cell_area, 2);
    rb_define_method(gTv, "get_background_area", treeview_get_background_area, 2);
    rb_define_method(gTv, "visible_rect", treeview_get_visible_rect, 0);
    rb_define_method(gTv, "bin_window", treeview_get_bin_window, 0);
    rb_define_method(gTv, "widget_to_tree_coords", treeview_widget_to_tree_coords, 2);
    rb_define_method(gTv, "tree_to_widget_coords", treeview_tree_to_widget_coords, 2);
    rb_define_method(gTv, "enable_model_drag_dest", treeview_enable_model_drag_dest, 2);
    rb_define_method(gTv, "enable_model_drag_source", treeview_enable_model_drag_source, 3);
    rb_define_method(gTv, "unset_rows_drag_source", treeview_unset_rows_drag_source, 0);
    rb_define_method(gTv, "unset_rows_drag_dest", treeview_unset_rows_drag_dest, 0);
    rb_define_method(gTv, "set_drag_dest_row", treeview_set_drag_dest_row, 2);
    rb_define_method(gTv, "drag_dest_row", treeview_get_drag_dest_row, 0);
    rb_define_method(gTv, "get_dest_row_at_pos", treeview_get_dest_row_at_pos, 2);
    rb_define_method(gTv, "create_row_drag_icon", treeview_create_row_drag_icon, 1);
    rb_define_method(gTv, "set_search_equal_func", treeview_set_search_equal_func, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gTv, "set_cursor_on_cell", treeview_set_cursor_on_cell, 4);
#endif
    
    /* Constants */
    G_DEF_CLASS(GTK_TYPE_TREE_VIEW_DROP_POSITION, "DropPosition", gTv);
    G_DEF_CONSTANTS(gTv, GTK_TYPE_TREE_VIEW_DROP_POSITION, "GTK_TREE_VIEW_");

    /* Option Signals */
    G_DEF_SIGNAL_FUNC(gTv, "row-collapsed", treeview_signal_func);
    G_DEF_SIGNAL_FUNC(gTv, "row-expanded", treeview_signal_func);
    G_DEF_SIGNAL_FUNC(gTv, "test-collapse-row", treeview_signal_func);
    G_DEF_SIGNAL_FUNC(gTv, "test-expand-row", treeview_signal_func);
}
