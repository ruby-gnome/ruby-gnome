/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreemodel.c -

  $Author: mutoh $
  $Date: 2003/06/22 17:37:51 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TREE_MODEL(RVAL2GOBJ(s)))
#define TREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2TREEPATH(p) ((GtkTreePath*)RVAL2BOXED(p, GTK_TYPE_TREE_PATH))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define RVAL2ITR(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

static VALUE
treemodel_get_flags(self)
    VALUE self;
{
    return INT2NUM(gtk_tree_model_get_flags(_SELF(self)));
}

static VALUE
treemodel_get_n_columns(self)
    VALUE self;
{
    return INT2NUM(gtk_tree_model_get_n_columns(_SELF(self)));
}

static VALUE
treemodel_get_column_type(self, index)
    VALUE self;
{
    return GTYPE2CLASS(gtk_tree_model_get_column_type(_SELF(self), 
                                                      NUM2INT(index)));
}

static VALUE
treemodel_get_iter(self, path)
    VALUE self, path;
{
    GtkTreeIter iter;
    gboolean ret;
    GtkTreeModel* model = _SELF(self);

    if (TYPE(path) == T_STRING){
        ret = gtk_tree_model_get_iter_from_string(model, &iter, 
                                                  RVAL2CSTR(path));
    } else {
        ret = gtk_tree_model_get_iter(model, &iter, 
                                      RVAL2TREEPATH(path));
    } 
    iter.user_data3 = model;

    return ret ? ITR2RVAL(&iter) : Qnil;
}

static VALUE
treemodel_get_value(self, iter, column)
    VALUE self, iter, column;
{
    GValue value = {0, };
    gtk_tree_model_get_value(_SELF(self), RVAL2ITR(iter), NUM2INT(column), &value);
    return G_VALUE_TYPE(&value) != G_TYPE_INVALID ? GVAL2RVAL(&value) : Qnil;
}

/* These methods may be neededless.
void        gtk_tree_model_ref_node (GtkTreeModel *tree_model,
                                             GtkTreeIter *iter);
void        gtk_tree_model_unref_node (GtkTreeModel *tree_model,
                                             GtkTreeIter *iter);
*/

/* XXXX
void        gtk_tree_model_get_valist (GtkTreeModel *tree_model,
                                             GtkTreeIter *iter,
                                             va_list var_args);
*/

static gboolean
treemodel_foreach_func(model, path, iter, func)
    GtkTreeModel* model;
    GtkTreePath* path;
    GtkTreeIter* iter;
    gpointer func;
{
    iter->user_data3 = model;
    rb_funcall((VALUE)func, id_call, 3, GOBJ2RVAL(model), 
                            TREEPATH2RVAL(path), ITR2RVAL(iter));
	 return FALSE;
}

static VALUE
treemodel_foreach(self)
    VALUE self;
{
    volatile VALUE func = rb_f_lambda();
    gtk_tree_model_foreach(_SELF(self), 
                           (GtkTreeModelForeachFunc)treemodel_foreach_func, 
                           (gpointer)func);
    return self;
}

static VALUE
treemodel_row_changed(self, path, iter)
    VALUE self, path, iter;
{
    gtk_tree_model_row_changed(_SELF(self), RVAL2TREEPATH(path), RVAL2ITR(iter));
    return self;
}

static VALUE
treemodel_row_inserted(self, path, iter)
    VALUE self, path, iter;
{
    gtk_tree_model_row_inserted(_SELF(self), RVAL2TREEPATH(path), RVAL2ITR(iter));
    return self;
}

static VALUE
treemodel_row_has_child_toggled(self, path, iter)
    VALUE self, path, iter;
{
    gtk_tree_model_row_has_child_toggled(_SELF(self), RVAL2TREEPATH(path), RVAL2ITR(iter));
    return self;
}

static VALUE
treemodel_row_deleted(self, path)
    VALUE self, path;
{
    gtk_tree_model_row_deleted(_SELF(self), RVAL2TREEPATH(path));
    return self;
}

static VALUE
treemodel_rows_reordered(self, path, iter, new_orders)
    VALUE self, path, iter, new_orders;
{
    gint i, len;
    gint* orders;

    Check_Type(new_orders, T_ARRAY);

    len = RARRAY(new_orders)->len;
    orders = ALLOCA_N(gint, len);

    for (i = 0; i < len; i++) {
        orders[i] = RARRAY(new_orders)->ptr[i];
    }
  
    gtk_tree_model_rows_reordered(_SELF(self), RVAL2TREEPATH(path), RVAL2ITR(iter), orders);
    return self;
}


void 
Init_gtk_treemodel()
{
    VALUE mTreeModel = G_DEF_INTERFACE(GTK_TYPE_TREE_MODEL, "TreeModel", mGtk);

    rb_define_method(mTreeModel, "flags", treemodel_get_flags, 0);
    rb_define_method(mTreeModel, "n_columns", treemodel_get_n_columns, 0);
    rb_define_method(mTreeModel, "get_column_type", treemodel_get_column_type, 1);
    rb_define_method(mTreeModel, "get_iter", treemodel_get_iter, 1);
    rb_define_method(mTreeModel, "get_value", treemodel_get_value, 2);
    rb_define_method(mTreeModel, "each", treemodel_foreach, 0);
    rb_define_method(mTreeModel, "row_changed", treemodel_row_changed, 2);
    rb_define_method(mTreeModel, "row_inserted", treemodel_row_inserted, 2);
    rb_define_method(mTreeModel, "row_has_child_toggled", treemodel_row_has_child_toggled, 2);
    rb_define_method(mTreeModel, "row_deleted", treemodel_row_deleted, 1);
    rb_define_method(mTreeModel, "rows_reordered", treemodel_rows_reordered, 3);

    /* GtkTreeModelFlags */
    rb_define_const(mTreeModel, "ITERS_PERSIST", INT2NUM(GTK_TREE_MODEL_ITERS_PERSIST));
    rb_define_const(mTreeModel, "LIST_ONLY", INT2NUM(GTK_TREE_MODEL_LIST_ONLY));

    G_DEF_SETTERS(mTreeModel);
}

