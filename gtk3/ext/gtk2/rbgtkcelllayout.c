/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,4,0)

#define RG_TARGET_NAMESPACE mCellLayout
#define _SELF(s) (GTK_CELL_LAYOUT(RVAL2GOBJ(s)))
#define RVAL2RENDERER(s) (GTK_CELL_RENDERER(RVAL2GOBJ(s)))

static VALUE
rg_pack_start(VALUE self, VALUE cell, VALUE expand)
{
    gtk_cell_layout_pack_start(_SELF(self), RVAL2RENDERER(cell),
                               RVAL2CBOOL(expand));
    return self;
}

static VALUE
rg_pack_end(VALUE self, VALUE cell, VALUE expand)
{
    gtk_cell_layout_pack_end(_SELF(self), RVAL2RENDERER(cell),
                             RVAL2CBOOL(expand));
    return self;
}

static VALUE
rg_reorder(VALUE self, VALUE cell, VALUE position)
{
    gtk_cell_layout_reorder(_SELF(self), RVAL2RENDERER(cell),
                            NUM2INT(position));
    return self;
}

static VALUE
rg_clear(VALUE self)
{
    gtk_cell_layout_clear(_SELF(self));
    return self;
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

    gtk_cell_layout_add_attribute(_SELF(self), RVAL2RENDERER(cell),
                                  name, NUM2INT(column));
    return self;
}

static void
layout_data_func(GtkCellLayout *layout, GtkCellRenderer *cell, GtkTreeModel *tree_model, GtkTreeIter *iter, gpointer func)
{
    iter->user_data3 = tree_model;
    rb_funcall((VALUE)func, id_call, 4, GOBJ2RVAL(layout), GOBJ2RVAL(cell),
               GOBJ2RVAL(tree_model), GTKTREEITER2RVAL(iter));
}

static VALUE
rg_set_cell_data_func(VALUE self, VALUE cell)
{
    if (rb_block_given_p()) {
        VALUE func = rb_block_proc();
        G_RELATIVE(self, func);
        gtk_cell_layout_set_cell_data_func(_SELF(self), RVAL2RENDERER(cell),
                                           (GtkCellLayoutDataFunc)layout_data_func, 
                                           (gpointer)func, NULL);
    } else {
        gtk_cell_layout_set_cell_data_func(_SELF(self), RVAL2RENDERER(cell),
                                           NULL, (gpointer)NULL, NULL);
    }
    return self;
}

static VALUE
rg_clear_attributes(VALUE self, VALUE cell)
{
    gtk_cell_layout_clear_attributes(_SELF(self), RVAL2RENDERER(cell));
    return self;
}

static VALUE
rg_set_attributes(VALUE self, VALUE cell, VALUE attrs)
{
    gint i;
    VALUE ary;
    Check_Type(attrs, T_HASH);

    rg_clear_attributes(self, cell);

    ary = rb_funcall(attrs, rb_intern("to_a"), 0);
    for (i = 0; i < RARRAY_LEN(ary); i++){
        rg_add_attribute(self, cell, 
                             RARRAY_PTR(RARRAY_PTR(ary)[i])[0],
                             RARRAY_PTR(RARRAY_PTR(ary)[i])[1]);
    }
    return self;
}
#endif

void
Init_gtk_celllayout(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_CELL_LAYOUT, "CellLayout", mGtk);

    RG_DEF_METHOD(pack_start, 2);
    RG_DEF_METHOD(pack_end, 2);
    RG_DEF_METHOD(reorder, 2);
    RG_DEF_METHOD(clear, 0);
    RG_DEF_METHOD(add_attribute, 3);
    RG_DEF_METHOD(set_cell_data_func, 1);
    RG_DEF_METHOD(clear_attributes, 1);
    RG_DEF_METHOD(set_attributes, 2);
#endif
}
