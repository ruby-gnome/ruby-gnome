/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreeiter.c -

  $Author: mutoh $
  $Date: 2002/11/11 15:32:35 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define TREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))

void
rbgtk_register_treeiter_set_value_func(gtype, func)
    GType gtype;
    rbgtkiter_set_value_func func;
{
    rb_hash_aset(treeiter_set_value_table, INT2NUM(gtype), 
                 Data_Wrap_Struct(rb_cData, NULL, NULL, func));
}

static VALUE
treeiter_first(self)
    VALUE self;
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    VALUE ret = (gtk_tree_model_get_iter_first(model, iter)) ? Qtrue : Qfalse;
    iter->user_data3 = model;
    return ret;
}

static VALUE
treeiter_next(self)
    VALUE self;
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    VALUE ret = (gtk_tree_model_iter_next(model, iter)) ? Qtrue : Qfalse;
    iter->user_data3 = model;
    return ret;
}

static VALUE
treeiter_get_value(self, column)
    VALUE self, column;
{
    GValue value = {0, };
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gtk_tree_model_get_value(model, iter, NUM2INT(column), &value);
    return G_VALUE_TYPE(&value) != G_TYPE_INVALID ? GVAL2RVAL(&value) : Qnil;
}

static VALUE
treeiter_children(self)
    VALUE self;
{
    GtkTreeIter child;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gboolean ret = gtk_tree_model_iter_children(model, &child, iter);
    child.user_data3 = model;
    return ret ? ITR2RVAL(&child) : Qnil;
}

static VALUE
treeiter_get_path(self)
    VALUE self;
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    return TREEPATH2RVAL(gtk_tree_model_get_path(model, iter));
}

static VALUE
treeiter_has_child(self)
    VALUE self;
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    return gtk_tree_model_iter_has_child(model, iter) ? Qtrue : Qfalse;
}

static VALUE
treeiter_n_children(self)
    VALUE self;
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    return INT2NUM(gtk_tree_model_iter_n_children(model, iter));
}

static VALUE
treeiter_nth_child(self, n)
    VALUE self, n;
{
    GtkTreeIter child;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gboolean ret = gtk_tree_model_iter_nth_child(model, &child, iter, NUM2INT(n));
    return ret ? ITR2RVAL(&child) : Qnil;
}

static VALUE
treeiter_parent(self)
    VALUE self;
{
    GtkTreeIter parent;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;
    gboolean ret = gtk_tree_model_iter_parent(model, &parent, iter);
    return ret ? ITR2RVAL(&parent) : Qnil;
}

static VALUE
treeiter_set_value(self, column, value)
    VALUE self, column, value;
{
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* model = (GtkTreeModel*)iter->user_data3;

    VALUE obj = rb_hash_aref(treeiter_set_value_table, INT2NUM(G_TYPE_FROM_INSTANCE(model)));
    rbgtkiter_set_value_func func;
    GValue gval = {0,};

    if (NIL_P(obj))
        rb_raise(rb_eTypeError, "Gtk::TreeModel is invalid.");

    Data_Get_Struct(obj, void, func);
    g_value_init(&gval, RVAL2GTYPE(value));
    rbgobj_rvalue_to_gvalue(value, &gval);
    func(model, iter, NUM2INT(column), &gval);
    return self;
}

void 
Init_gtk_treeiter()
{
    VALUE gTreeIter = G_DEF_CLASS(GTK_TYPE_TREE_ITER, "TreeIter", mGtk);

    rb_define_method(gTreeIter, "first!", treeiter_first, 0);
    rb_define_method(gTreeIter, "next!", treeiter_next, 0);
    rb_define_method(gTreeIter, "get_value", treeiter_get_value, 1);
    rb_define_method(gTreeIter, "first_child", treeiter_children, 0);
    rb_define_method(gTreeIter, "path", treeiter_get_path, 0);
    rb_define_method(gTreeIter, "has_child?", treeiter_has_child, 0);
    rb_define_method(gTreeIter, "n_children", treeiter_n_children, 0);
    rb_define_method(gTreeIter, "nth_child", treeiter_nth_child, 2);
    rb_define_method(gTreeIter, "parent", treeiter_parent, 0);
    rb_define_method(gTreeIter, "set_value", treeiter_set_value, 2);
}
