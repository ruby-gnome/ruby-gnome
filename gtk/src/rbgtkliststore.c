/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkliststore.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_LIST_STORE(RVAL2GOBJ(s)))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define RVAL2ITR(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

static VALUE
lstore_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    gint cnt;
    GtkListStore* store;
    GType* buf;

    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");

    buf = ALLOCA_N(GType, argc);
    for (cnt = 0; cnt < argc; cnt++) {
        buf[cnt] = CLASS2GTYPE(argv[cnt]);
    }

    store = gtk_list_store_newv(argc, buf);
 
    G_INITIALIZE(self, store);

    return Qnil;
}

static VALUE
lstore_set_column_types(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    gint cnt;
    GType* buf;
  
    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");
    buf = ALLOCA_N(GType, argc);
    for (cnt = 0; cnt < argc; cnt++) {
        buf[cnt] = CLASS2GTYPE(argv[cnt]);
    }

    gtk_list_store_set_column_types(_SELF(self), argc, buf);

    return self;
}

static VALUE
lstore_set_value(self, iter, column, value)
    VALUE self, iter, column, value;
{
    GValue gval = {0,};
    g_value_init(&gval, RVAL2GTYPE(value));

    rbgobj_rvalue_to_gvalue(value, &gval);

    gtk_list_store_set_value(_SELF(self), RVAL2ITR(iter), NUM2INT(column), &gval);
    return self;
}

/*
static VALUE
lstore_set(self, iter, tv, column1, column2)
    VALUE self, iter, tv, column1, column2;
{
}
  void        gtk_tree_store_set (GtkTreeStore *tree_store,
                                               GtkTreeIter *iter,
					                                                    ...);

  void        gtk_tree_store_set_valist       (GtkTreeStore *tree_store,
  GtkTreeIter *iter,
  va_list var_args);
*/
    
static VALUE
lstore_remove(self, iter)
    VALUE self, iter;
{
    /* XXX This method should return boolean.
       but at least v2.0.6, it returns nothing(void).
    return gtk_tree_store_remove(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
    */
    gtk_list_store_remove(_SELF(self), RVAL2ITR(iter));

    return Qtrue;
}

static VALUE
lstore_insert(self, position)
    VALUE self, position;
{
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_insert(model, &iter, NUM2INT(position));
    iter.user_data3 = model;
    return ITR2RVAL(&iter);
}

static VALUE
lstore_insert_before(self, sibling)
    VALUE self, sibling;
{
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_insert_before(model, &iter, NIL_P(sibling) ? NULL : RVAL2ITR(sibling));
    iter.user_data3 = model;
    return ITR2RVAL(&iter);
}

static VALUE
lstore_insert_after(self, sibling)
    VALUE self, sibling;
{ 
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_insert_after(model, &iter, NIL_P(sibling) ? NULL : RVAL2ITR(sibling));
    iter.user_data3 = model;
    return ITR2RVAL(&iter);
}

static VALUE
lstore_prepend(self)
    VALUE self;
{
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_prepend(model, &iter);
    iter.user_data3 = model;
    return ITR2RVAL(&iter);
}

static VALUE
lstore_append(self)
    VALUE self;
{
    GtkTreeIter iter;
    GtkListStore* model = _SELF(self);
    gtk_list_store_append(model, &iter);
    iter.user_data3 = model;
    return ITR2RVAL(&iter);
}

static VALUE
lstore_clear(self)
    VALUE self;
{
    gtk_list_store_clear(_SELF(self));
    return self;
}

void
Init_gtk_list_store()
{
    VALUE ls = G_DEF_CLASS(GTK_TYPE_LIST_STORE, "ListStore", mGtk);
  
    rbgtk_register_treeiter_set_value_func(GTK_TYPE_LIST_STORE, 
                                           (rbgtkiter_set_value_func)&gtk_list_store_set_value);
    rb_define_method(ls, "initialize", lstore_initialize, -1);
    rb_define_method(ls, "set_column_types", lstore_set_column_types, -1);
    rb_define_method(ls, "set_value", lstore_set_value, 3);
    rb_define_method(ls, "remove", lstore_remove, 1);
    rb_define_method(ls, "insert", lstore_insert, 1);
    rb_define_method(ls, "insert_before", lstore_insert_before, 1);
    rb_define_method(ls, "insert_after", lstore_insert_after, 1);
    rb_define_method(ls, "prepend", lstore_prepend, 0);
    rb_define_method(ls, "append", lstore_append, 0);
    rb_define_method(ls, "clear", lstore_clear, 0);
}


