/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreeiter.c -

  $Author: mutoh $
  $Date: 2002/11/02 11:18:52 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))
#define ITR2RVAL(i) (BOXED2RVAL2(i, GTK_TYPE_TREE_ITER))

#define TREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2TREEPATH(p) ((GtkTreePath*)RVAL2BOXED(p, GTK_TYPE_TREE_PATH))
#define RVAL2MODEL(m) (GTK_TREE_MODEL(RVAL2GOBJ(m)))

static VALUE
treeiter_first(self, model)
    VALUE self, model;
{
    return gtk_tree_model_get_iter_first(RVAL2MODEL(model), _SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
treeiter_next(self, model)
    VALUE self, model;
{
    GtkTreeIter* iter = _SELF(self);
    return (gtk_tree_model_iter_next(RVAL2MODEL(model), iter)) ? Qtrue : Qfalse;
}

static VALUE
treeiter_get_value(self, model, column)
    VALUE self, model, column;
{
    GValue value = {0, };
    gtk_tree_model_get_value(RVAL2MODEL(model), _SELF(self), NUM2INT(column), &value);
    return G_VALUE_TYPE(&value) != G_TYPE_INVALID ? GVAL2RVAL(&value) : Qnil;
}

static VALUE
treeiter_get_current_item(self, model)
    VALUE self, model;
{
    gint i;
    GtkTreeIter* iter = _SELF(self);
    GtkTreeModel* gmodel = RVAL2MODEL(model);
    gint num = gtk_tree_model_get_n_columns(gmodel);
    VALUE ary = rb_ary_new();
    for (i = 0; i < num; i++){
        GValue value = {0,};
        gtk_tree_model_get_value(gmodel, iter, i, &value);
        rb_ary_push(ary, (G_VALUE_TYPE(&value) != G_TYPE_INVALID) ? GVAL2RVAL(&value) : Qnil);
    }
    return ary;
}

static VALUE
treeiter_children(self, model)
    VALUE self, model;
{
    GtkTreeIter child;
    gboolean ret = (gtk_tree_model_iter_children(RVAL2MODEL(model), &child, _SELF(self)));
    return ret ? ITR2RVAL(&child) : Qnil;
}

static VALUE
treeiter_get_path(self, model)
    VALUE self, model;
{
    return TREEPATH2RVAL(gtk_tree_model_get_path(RVAL2MODEL(model), _SELF(self)));
}

static VALUE
treeiter_has_child(self, model)
    VALUE self, model;
{
    return gtk_tree_model_iter_has_child(RVAL2MODEL(model), _SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
treeiter_n_children(self, model)
    VALUE self, model;
{
    return INT2NUM(gtk_tree_model_iter_n_children(RVAL2MODEL(model), _SELF(self)));
}

static VALUE
treeiter_nth_child(self, model, n)
    VALUE self, model, n;
{
    GtkTreeIter child;
    gboolean ret = gtk_tree_model_iter_nth_child(RVAL2MODEL(model), &child, 
                                                 _SELF(self), NUM2INT(n));
    return ret ? ITR2RVAL(&child) : Qnil;
}

static VALUE
treeiter_parent(self, model)
    VALUE self, model;
{
    GtkTreeIter parent;
    gboolean ret = gtk_tree_model_iter_parent(RVAL2MODEL(model), &parent, _SELF(self));
    return ret ? ITR2RVAL(&parent) : Qnil;
}

void 
Init_gtk_treeiter()
{
    VALUE gTreeIter = G_DEF_CLASS(GTK_TYPE_TREE_ITER, "TreeIter", mGtk);

    rb_define_method(gTreeIter, "first!", treeiter_first, 1);
    rb_define_method(gTreeIter, "next!", treeiter_next, 1);
    rb_define_method(gTreeIter, "get_value", treeiter_get_value, 2);
    rb_define_method(gTreeIter, "current_item", treeiter_get_current_item, 1);
    rb_define_method(gTreeIter, "children", treeiter_children, 1);
    rb_define_method(gTreeIter, "path", treeiter_get_path, 1);
    rb_define_method(gTreeIter, "has_child?", treeiter_has_child, 1);
    rb_define_method(gTreeIter, "n_children", treeiter_n_children, 1);
    rb_define_method(gTreeIter, "nth_child", treeiter_nth_child, 3);
    rb_define_method(gTreeIter, "parent", treeiter_parent, 1);
}
