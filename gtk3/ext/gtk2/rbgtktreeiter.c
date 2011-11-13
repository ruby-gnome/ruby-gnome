/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
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

#include "global.h"

#define RG_TARGET_NAMESPACE cTreeIter
#define _SELF(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

void
rbgtk_register_treeiter_set_value_func(GType gtype, rbgtkiter_set_value_func func)
{
    rb_hash_aset(treeiter_set_value_table, INT2NUM(gtype), 
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));
}

static VALUE
rg_first_bang(VALUE self)
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    VALUE ret = CBOOL2RVAL(gtk_tree_model_get_iter_first(model, iter));
    iter->user_data3 = model;
    return ret;
}

static VALUE
rg_next_bang(VALUE self)
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gboolean ret = gtk_tree_model_iter_next(model, iter);
    if (! ret){
        gtk_tree_model_get_iter_first(model, iter);
    }
    iter->user_data3 = model;

    return CBOOL2RVAL(ret);
}

static VALUE
rg_get_value(VALUE self, VALUE column)
{
    GValue value = G_VALUE_INIT;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    VALUE ret = Qnil;

    gtk_tree_model_get_value(model, iter, NUM2INT(column), &value);
    if (G_VALUE_TYPE(&value) != G_TYPE_INVALID){
        ret = GVAL2RVAL(&value);
        g_value_unset(&value);
    } 
    return ret;
}

static VALUE
rg_first_child(VALUE self)
{
    GtkTreeIter child;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gboolean ret = gtk_tree_model_iter_children(model, &child, iter);
    child.user_data3 = model;
    return ret ? GTKTREEITER2RVAL(&child) : Qnil;
}

static VALUE
rg_path(VALUE self)
{
    VALUE retval = Qnil;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    GtkTreePath* path = gtk_tree_model_get_path(model, iter);
    retval= GTKTREEPATH2RVAL(path);
    gtk_tree_path_free(path);
    return retval;
}

static VALUE
rg_has_child_p(VALUE self)
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    return CBOOL2RVAL(gtk_tree_model_iter_has_child(model, iter));
}

static VALUE
rg_n_children(VALUE self)
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    return INT2NUM(gtk_tree_model_iter_n_children(model, iter));
}

static VALUE
rg_nth_child(VALUE self, VALUE n)
{
    GtkTreeIter child;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gboolean ret = gtk_tree_model_iter_nth_child(model, &child, iter, NUM2INT(n));
    child.user_data3 = model;
    return ret ? GTKTREEITER2RVAL(&child) : Qnil;
}

static VALUE
rg_parent(VALUE self)
{
    GtkTreeIter parent;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gboolean ret = gtk_tree_model_iter_parent(model, &parent, iter);
    parent.user_data3 = model;
    return ret ? GTKTREEITER2RVAL(&parent) : Qnil;
}

static VALUE
rg_set_value(VALUE self, VALUE column, VALUE value)
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;

    VALUE obj = rb_hash_aref(treeiter_set_value_table, INT2NUM(G_TYPE_FROM_INSTANCE(model)));
    rbgtkiter_set_value_func func;
    GType gtype = gtk_tree_model_get_column_type(model, NUM2INT(column));
    GValue gval = G_VALUE_INIT;

    if (NIL_P(obj))
        rb_raise(rb_eTypeError, "Gtk::TreeModel is invalid.");

    Data_Get_Struct(obj, void, func);

    g_value_init(&gval, gtype);
    rbgobj_rvalue_to_gvalue(value, &gval);
    func(model, iter, NUM2INT(column), &gval);
    g_value_unset(&gval);

    return self;
}

static VALUE
rg_operator_treeiter_set_value_eql(VALUE self, VALUE column, VALUE value)
{
    rg_set_value(self, column, value);
    return value;
}

static VALUE
rg_operator_treeiter_eql(VALUE self, VALUE other)
{
    gint i, num1, num2;
    GtkTreeIter* iter1 = _SELF(self);
    GtkTreeIter* iter2;
    GtkTreeModel* model1;
    GtkTreeModel* model2;
    GtkTreePath* path1;
    GtkTreePath* path2;

    if (!rb_obj_is_kind_of(other, GTYPE2CLASS(GTK_TYPE_TREE_ITER)))
        return Qfalse;

    iter2 = _SELF(other);

    model1 = (GtkTreeModel*)iter1->user_data3;
    model2 = (GtkTreeModel*)iter2->user_data3;
    if (model1 != model2) return Qfalse;

    num1 = gtk_tree_model_get_n_columns(model1);
    num2 = gtk_tree_model_get_n_columns(model2);
    if (num1 != num2) return Qfalse;

    path1 = gtk_tree_model_get_path(model1, iter1);
    path2 = gtk_tree_model_get_path(model2, iter2);
    if (gtk_tree_path_compare(path1, path2) != 0) {
        gtk_tree_path_free(path1);
        gtk_tree_path_free(path2);
        return Qfalse;
    } else {
        gtk_tree_path_free(path1);
        gtk_tree_path_free(path2);
    }

    for (i = 0; i < num1; i++){
        GValue gval1 = G_VALUE_INIT;
        GValue gval2 = G_VALUE_INIT;
        VALUE ret1, ret2;
        gtk_tree_model_get_value(model1, iter1, i, &gval1);
        gtk_tree_model_get_value(model2, iter2, i, &gval2);

        ret1 = GVAL2RVAL(&gval1);
        ret2 = GVAL2RVAL(&gval2);
        g_value_unset(&gval1);
        g_value_unset(&gval2);
        if (rb_equal(ret1, ret2) == Qfalse) 
            return Qfalse;
    }
    return Qtrue;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_to_str(VALUE self)
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;

    gchar* s = gtk_tree_model_get_string_from_iter (model, iter);
    VALUE result = CSTR2RVAL(s);
    g_free(s);
    return result;
}
#endif

void 
Init_gtk_treeiter(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TREE_ITER, "TreeIter", mGtk);

    RG_DEF_METHOD_BANG(first, 0);
    RG_DEF_METHOD_BANG(next, 0);
    RG_DEF_METHOD(get_value, 1);
    RG_DEF_ALIAS("[]", "get_value");
    RG_DEF_METHOD(first_child, 0);
    RG_DEF_METHOD(path, 0);
    RG_DEF_METHOD_P(has_child, 0);
    RG_DEF_METHOD(n_children, 0);
    RG_DEF_METHOD(nth_child, 1);
    RG_DEF_METHOD(parent, 0);
    RG_DEF_METHOD(set_value, 2);
    RG_DEF_METHOD_OPERATOR("[]=", treeiter_set_value_eql, 2);
    RG_DEF_METHOD_OPERATOR("==", treeiter_eql, 1);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(to_str, 0);
    RG_DEF_ALIAS("to_s", "to_str");
#endif
}
