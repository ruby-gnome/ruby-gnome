/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2013  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE mTreeModel
#define _SELF(s) (RVAL2GTKTREEMODEL(s))

static VALUE
rg_flags(VALUE self)
{
    return GTKTREEMODELFLAGS2RVAL(gtk_tree_model_get_flags(_SELF(self)));
}

static VALUE
rg_n_columns(VALUE self)
{
    return INT2NUM(gtk_tree_model_get_n_columns(_SELF(self)));
}

static VALUE
rg_get_column_type(VALUE self, VALUE index)
{
    return GTYPE2CLASS(gtk_tree_model_get_column_type(_SELF(self), 
                                                      NUM2INT(index)));
}

static VALUE
rg_iter_first(VALUE self)
{
    VALUE val = Qnil;
    GtkTreeIter iter;
    GtkTreeModel* model = _SELF(self);
    gboolean ret = (gtk_tree_model_get_iter_first(model, &iter));
    iter.user_data3 = model;

    if (ret) {
        val = GTKTREEITER2RVAL(&iter);
    }

    return val;
}

static VALUE
rg_get_iter(VALUE self, VALUE path)
{
    VALUE val = Qnil;
    GtkTreeIter iter;
    gboolean ret;
    GtkTreeModel* model = _SELF(self);

    if (TYPE(path) == T_STRING){
        ret = gtk_tree_model_get_iter_from_string(model, &iter,
                                                  RVAL2CSTR(path));
    } else {
        ret = gtk_tree_model_get_iter(model, &iter,
                                      RVAL2GTKTREEPATH(path));
    }
    iter.user_data3 = model;

    if (ret) {
        val = GTKTREEITER2RVAL(&iter);
    }

    return val;
}

static VALUE
rg_get_path(VALUE self, VALUE iter)
{
    GtkTreePath *path;
    VALUE ret;
    path = gtk_tree_model_get_path(_SELF(self), RVAL2GTKTREEITER(iter));
    ret = GTKTREEPATH2RVAL(path);
    gtk_tree_path_free(path);
    return ret;
}

static VALUE
rg_get_value(VALUE self, VALUE iter, VALUE column)
{
    GValue value = G_VALUE_INIT;
    VALUE ret = Qnil;
    gtk_tree_model_get_value(_SELF(self), RVAL2GTKTREEITER(iter), NUM2INT(column), &value);
    if (G_VALUE_TYPE(&value) != G_TYPE_INVALID){
        ret = GVAL2RVAL(&value);
        g_value_unset(&value);
    }
    return ret;
}

static gboolean
treemodel_foreach_func(GtkTreeModel *model,
                       GtkTreePath *path,
                       GtkTreeIter *iter,
                       G_GNUC_UNUSED gpointer func)
{
    iter->user_data3 = model;
    rb_yield(rb_ary_new3(3, GOBJ2RVAL(model), GTKTREEPATH2RVAL(path), GTKTREEITER2RVAL(iter)));
    return FALSE;
}

static VALUE
rg_each(VALUE self)
{
    gtk_tree_model_foreach(_SELF(self),
                           (GtkTreeModelForeachFunc)treemodel_foreach_func,
                           (gpointer)NULL);
    return self;
}

static VALUE
rg_row_changed(VALUE self, VALUE path, VALUE iter)
{
    gtk_tree_model_row_changed(_SELF(self), RVAL2GTKTREEPATH(path), RVAL2GTKTREEITER(iter));
    return self;
}

static VALUE
rg_row_inserted(VALUE self, VALUE path, VALUE iter)
{
    gtk_tree_model_row_inserted(_SELF(self), RVAL2GTKTREEPATH(path), RVAL2GTKTREEITER(iter));
    return self;
}

static VALUE
rg_row_has_child_toggled(VALUE self, VALUE path, VALUE iter)
{
    gtk_tree_model_row_has_child_toggled(_SELF(self), RVAL2GTKTREEPATH(path), RVAL2GTKTREEITER(iter));
    return self;
}

static VALUE
rg_row_deleted(VALUE self, VALUE path)
{
    gtk_tree_model_row_deleted(_SELF(self), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
rg_rows_reordered(VALUE self, VALUE rbpath, VALUE rbiter, VALUE rbnew_order)
{
    GtkTreeModel *proxy = _SELF(self);
    GtkTreePath *path = RVAL2GTKTREEPATH(rbpath);
    GtkTreeIter *iter = RVAL2GTKTREEITER(rbiter);
    gint columns = gtk_tree_model_get_n_columns(proxy);
    long n;
    gint *new_order = RVAL2GINTS(rbnew_order, n);

    if (n != columns) {
        g_free(new_order);

        rb_raise(rb_eArgError,
                 "new order array must contain same number of elements as the number of columns in the store: %ld != %d",
                 n, columns);
    }

    gtk_tree_model_rows_reordered(proxy, path, iter, new_order);

    g_free(new_order);

    return self;
}

static VALUE
rg_iter_is_valid_p(G_GNUC_UNUSED VALUE self, G_GNUC_UNUSED VALUE iter)
{
    return Qtrue;
}

static VALUE
signal_func(G_GNUC_UNUSED guint num, const GValue *values)
{
    GtkTreeModel* model = g_value_get_object(&values[0]);
    GtkTreePath* path = g_value_get_boxed(&values[1]);
    GtkTreeIter* iter = g_value_get_boxed(&values[2]);
    iter->user_data3 = model;

    return rb_ary_new3(3, GOBJ2RVAL(model), GTKTREEPATH2RVAL(path), GTKTREEITER2RVAL(iter));
}

static VALUE
signal_rows_reordered_func(G_GNUC_UNUSED guint num, const GValue *values)
{
    GtkTreeModel* model = g_value_get_object(&values[0]);
    GtkTreePath* path = g_value_get_boxed(&values[1]);
    GtkTreeIter* iter = g_value_get_boxed(&values[2]);
    gint* new_orders = (gint*)g_value_get_pointer(&values[3]);
    gint len = gtk_tree_model_iter_n_children(model, iter);
    VALUE orders = Qnil;
    int i;

    iter->user_data3 = model;

    orders = rb_ary_new2(len);
    for (i = 0; i < len; i++, new_orders++) {
        rb_ary_push(orders, INT2NUM(*new_orders));
    }
    return rb_ary_new3(4, GOBJ2RVAL(model), GTKTREEPATH2RVAL(path), GTKTREEITER2RVAL(iter), orders);
}

void
Init_gtk_treemodel(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_TREE_MODEL, "TreeModel", mGtk);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(n_columns, 0);
    RG_DEF_METHOD(get_column_type, 1);
    RG_DEF_METHOD(iter_first, 0);
    RG_DEF_METHOD(get_iter, 1);
    RG_DEF_METHOD(get_path, 1);
    RG_DEF_METHOD(get_value, 2);
    RG_DEF_METHOD(each, 0);
    RG_DEF_METHOD(row_changed, 2);
    RG_DEF_METHOD(row_inserted, 2);
    RG_DEF_METHOD(row_has_child_toggled, 2);
    RG_DEF_METHOD(row_deleted, 1);
    RG_DEF_METHOD(rows_reordered, 3);
    RG_DEF_METHOD_P(iter_is_valid, 1);

    G_DEF_CLASS(GTK_TYPE_TREE_MODEL_FLAGS, "Flags", RG_TARGET_NAMESPACE);

    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "row_changed", (GValToRValSignalFunc)signal_func);
    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "row_inserted", (GValToRValSignalFunc)signal_func);
    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "row_has_child_toggled", (GValToRValSignalFunc)signal_func);
    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "rows_reordered", (GValToRValSignalFunc)signal_rows_reordered_func);
}
