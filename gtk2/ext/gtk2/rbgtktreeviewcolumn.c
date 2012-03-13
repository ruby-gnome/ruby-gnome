/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTreeViewColumn
#define _SELF(s) (GTK_TREE_VIEW_COLUMN(RVAL2GOBJ(s)))
#define RVAL2CELLRENDERER(c) (GTK_CELL_RENDERER(RVAL2GOBJ(c)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    int i;
    int col;
    GtkTreeViewColumn *tvc;
    GtkCellRenderer *renderer;
    const gchar *name;
    VALUE ary, val;

    tvc = gtk_tree_view_column_new();
    if (argc > 0){
        gtk_tree_view_column_set_title(tvc, RVAL2CSTR(argv[0]));
        if (argc > 1) {
            gtk_tree_view_column_pack_start(tvc, RVAL2CELLRENDERER(argv[1]), TRUE);
            G_CHILD_ADD(self, argv[1]);
        }
    }

    RBGTK_INITIALIZE(self, tvc);

    if (argc == 3){
        ary = rb_funcall(argv[2], rb_intern("to_a"), 0);
        renderer = RVAL2CELLRENDERER(argv[1]);
        for (i = 0; i < RARRAY_LEN(ary); i++) {
            val = RARRAY_PTR(RARRAY_PTR(ary)[i])[0];
            if (SYMBOL_P(val)) {
                name = rb_id2name(SYM2ID(val));
            } else {
                name = RVAL2CSTR(val);
            }
            col = NUM2INT(RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
            gtk_tree_view_column_add_attribute(_SELF(self), renderer, name, col);
        }       
    }

    return Qnil;
}

static VALUE
rg_pack_start(VALUE self, VALUE cell, VALUE expand)
{
    G_CHILD_ADD(self, cell);
    gtk_tree_view_column_pack_start(_SELF(self), RVAL2CELLRENDERER(cell), RVAL2CBOOL(expand));
    return self;
}

static VALUE
rg_pack_end(VALUE self, VALUE cell, VALUE expand)
{
    G_CHILD_ADD(self, cell);
    gtk_tree_view_column_pack_end(_SELF(self), RVAL2CELLRENDERER(cell), RVAL2CBOOL(expand));
    return self;
}

static VALUE
rg_clear(VALUE self)
{
    G_CHILD_REMOVE_ALL(self);
    gtk_tree_view_column_clear(_SELF(self));
    return self;
}

static VALUE
rg_cell_renderers(VALUE self)
{
    return GLIST2ARYF(gtk_tree_view_column_get_cell_renderers(_SELF(self)));
}

static VALUE
rg_add_attribute(VALUE self, VALUE cell, VALUE attribute, VALUE column)
{
    const gchar *name;
    if (SYMBOL_P(attribute)) {
        name = rb_id2name(SYM2ID(attribute));
    } else {
        name = RVAL2CSTR(attribute);
    }
    gtk_tree_view_column_add_attribute(_SELF(self), RVAL2CELLRENDERER(cell), 
                                       name, NUM2INT(column));
    return self;
}

static VALUE
rg_set_attributes(VALUE self, VALUE renderer, VALUE attributes)
{
    GtkTreeViewColumn *tvc;
    GtkCellRenderer *grenderer;
    const gchar *name;
    int i, col;
    VALUE ary, val;

    Check_Type(attributes, T_HASH);

    tvc = _SELF(self);
    grenderer = RVAL2CELLRENDERER(renderer);
    gtk_tree_view_column_clear_attributes(tvc, grenderer);

    ary = rb_funcall(attributes, rb_intern("to_a"), 0);
    for (i = 0; i < RARRAY_LEN(ary); i++) {
        val = RARRAY_PTR(RARRAY_PTR(ary)[i])[0];
        if (SYMBOL_P(val)) {
            name = rb_id2name(SYM2ID(val));
        } else {
            name = RVAL2CSTR(val);
        }
        col = NUM2INT(RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
        gtk_tree_view_column_add_attribute(tvc, grenderer, name, col);
    }       
    return self;
}

static void
cell_data_func(GtkTreeViewColumn *tree_column, GtkCellRenderer *cell, GtkTreeModel *model, GtkTreeIter *iter, gpointer func)
{
    iter->user_data3 = model;
    rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(tree_column),
               GOBJ2RVAL(cell), GOBJ2RVAL(model), 
               GTKTREEITER2RVAL(iter));
}

static VALUE
rg_set_cell_data_func(VALUE self, VALUE renderer)
{
    volatile VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    G_RELATIVE(renderer, func);
    gtk_tree_view_column_set_cell_data_func(_SELF(self), RVAL2CELLRENDERER(renderer),
                                            (GtkTreeCellDataFunc)cell_data_func, (gpointer)func, NULL);
    return self;
}

static VALUE
rg_clear_attributes(VALUE self, VALUE cell)
{
    gtk_tree_view_column_clear_attributes(_SELF(self), RVAL2CELLRENDERER(cell));
    return self;
}

static VALUE
rg_clicked(VALUE self)
{
    gtk_tree_view_column_clicked(_SELF(self));
    return self;
}

static VALUE
rg_cell_set_cell_data(VALUE self, VALUE model, VALUE iter, VALUE is_expander, VALUE is_expanded)
{
    gtk_tree_view_column_cell_set_cell_data(_SELF(self), 
                                            GTK_TREE_MODEL(RVAL2GOBJ(model)),
                                            RVAL2GTKTREEITER(iter), 
                                            RVAL2CBOOL(is_expander), 
                                            RVAL2CBOOL(is_expanded));
    return self;
}

static VALUE
rg_cell_size(VALUE self)
{
    GdkRectangle cell_area;
    gint x_offset, y_offset, width, height;
    VALUE cell;

    /* Is this collect implement for cell_area ? */
    cell_area.x = -1;
    cell_area.y = -1;
    cell_area.width = -1;
    cell_area.height = -1;

    gtk_tree_view_column_cell_get_size(_SELF(self), &cell_area,
                                       &x_offset, &y_offset, 
                                       &width, &height);
    if (cell_area.x == -1 || cell_area.y == -1 || 
        cell_area.width == -1 || cell_area.height == -1){
        cell = Qnil;
    } else {
        cell = BOXED2RVAL(&cell_area, GDK_TYPE_RECTANGLE);
    }
    return rb_ary_new3(5, cell,
                       x_offset ? INT2NUM(x_offset) : Qnil,
                       y_offset ? INT2NUM(y_offset) : Qnil,
                       width ? INT2NUM(width) : Qnil,
                       height ? INT2NUM(height) : Qnil);
}

static VALUE
rg_cell_is_visible_p(VALUE self)
{
    return CBOOL2RVAL(gtk_tree_view_column_cell_is_visible(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,2,0)

static VALUE
rg_focus_cell(VALUE self, VALUE renderer)
{
    gtk_tree_view_column_focus_cell(_SELF(self), RVAL2CELLRENDERER(renderer));

    return self;
}

#endif

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_queue_resize(VALUE self)
{
    gtk_tree_view_column_queue_resize(_SELF(self));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
rg_tree_view(VALUE self)
{
    return GOBJ2RVAL(gtk_tree_view_column_get_tree_view(_SELF(self)));
}
#endif

#if !GTK_CHECK_VERSION(2,18,0)
static VALUE
rg_sort_column_id(VALUE self)
{
    return INT2NUM(gtk_tree_view_column_get_sort_column_id(_SELF(self)));
}

static VALUE
rg_set_sort_column_id(VALUE self, VALUE id)
{
    gtk_tree_view_column_set_sort_column_id(_SELF(self), NUM2INT(id));
    return self;
}
#endif

void
Init_gtk_treeviewcolumn(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TREE_VIEW_COLUMN, "TreeViewColumn", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(pack_start, 2);
    RG_DEF_METHOD(pack_end, 2);
    RG_DEF_METHOD(clear, 0);
    RG_DEF_METHOD(cell_renderers, 0);
    RG_DEF_METHOD(add_attribute, 3);
    RG_DEF_METHOD(set_attributes, 2);
    RG_DEF_METHOD(set_cell_data_func, 1);
    RG_DEF_METHOD(clear_attributes, 1);
    RG_DEF_METHOD(clicked, 0);
    RG_DEF_METHOD(cell_set_cell_data, 4);
    RG_DEF_METHOD(cell_size, 0);
    RG_DEF_METHOD_P(cell_is_visible, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(focus_cell, 1);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(queue_resize, 0);
#endif
#if GTK_CHECK_VERSION(2,12,0)
    RG_DEF_METHOD(tree_view, 0);
#endif
#if !GTK_CHECK_VERSION(2,18,0)
    RG_DEF_METHOD(sort_column_id, 0);
    RG_DEF_METHOD(set_sort_column_id, 1);
    RG_DEF_ALIAS("sort_column_id=", "set_sort_column_id");
#endif
    /* GtkTreeViewColumnSizing */
    G_DEF_CLASS(GTK_TYPE_TREE_VIEW_COLUMN_SIZING, "Sizing", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_TREE_VIEW_COLUMN_SIZING, "GTK_TREE_VIEW_COLUMN_");
}
