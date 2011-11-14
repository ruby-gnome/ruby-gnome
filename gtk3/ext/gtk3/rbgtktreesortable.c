/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE mTreeSortable
#define _SELF(s)    (GTK_TREE_SORTABLE(RVAL2GOBJ(s)))

static VALUE
rg_sort_column_changed(VALUE self)
{
    gtk_tree_sortable_sort_column_changed(_SELF(self));
    return self;
}

static VALUE
rg_sort_column_id(VALUE self)
{
    gint sort_column_id;
    GtkSortType order;

    if (gtk_tree_sortable_get_sort_column_id(_SELF(self), &sort_column_id,
                                             &order)) {
        VALUE ary;

        ary = rb_ary_new2(2);
        rb_ary_push(ary, INT2NUM(sort_column_id));
        rb_ary_push(ary, GENUM2RVAL(order, GTK_TYPE_SORT_TYPE));
        return ary;
    } else {
        return Qnil;   /* XXX: or something else? exception? */
    }
}

static VALUE
rg_set_sort_column_id(int argc, VALUE *argv, VALUE self)
{
    gint sort_column_id;
    GtkSortType order;

    if (argc == 1 || argc == 2) {
        sort_column_id = NUM2INT(argv[0]);
        order = (argc == 2) ? RVAL2GENUM(argv[1], GTK_TYPE_SORT_TYPE) : GTK_SORT_ASCENDING;
    } else {
        rb_raise(rb_eArgError, "need 1 or 2 arguments.");
    }

    gtk_tree_sortable_set_sort_column_id(_SELF(self), sort_column_id,
                                         order);
    return self;
}

static gint
sort_func(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer func)
{
    a->user_data3 = model;
    b->user_data3 = model;
    return NUM2INT(rb_funcall((VALUE)func, id_call, 2, GTKTREEITER2RVAL(a),
                              GTKTREEITER2RVAL(b)));
}

static VALUE
rg_set_sort_func(VALUE self, VALUE sort_column_id)
{
    volatile VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_tree_sortable_set_sort_func(_SELF(self), NUM2INT(sort_column_id),
                                    (GtkTreeIterCompareFunc)sort_func,
                                    (gpointer)func, NULL);
    return self;
}

static VALUE
rg_set_default_sort_func(VALUE self)
{
    volatile VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_tree_sortable_set_default_sort_func(_SELF(self),
                                            (GtkTreeIterCompareFunc)sort_func,
                                            (gpointer)func, NULL);
    return self;
}

static VALUE
rg_has_default_sort_func_p(VALUE self)
{
    return RVAL2CBOOL(gtk_tree_sortable_has_default_sort_func(_SELF(self)));
}

void
Init_gtk_treesortable(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_TREE_SORTABLE, "TreeSortable", mGtk);

    RG_DEF_METHOD(sort_column_changed, 0);
    RG_DEF_METHOD(sort_column_id, 0);
    RG_DEF_METHOD(set_sort_column_id, -1);
    RG_DEF_METHOD(set_sort_func, 1);
    RG_DEF_METHOD(set_default_sort_func, 0);
    RG_DEF_METHOD_P(has_default_sort_func, 0);

    rb_define_const(RG_TARGET_NAMESPACE, "DEFAULT_SORT_COLUMN_ID", GTK_TREE_SORTABLE_DEFAULT_SORT_COLUMN_ID);
}

/* vim: set sts=4 sw=4 ts=8: */
