/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreestore.c -

  $Author: mutoh $
  $Date: 2002/10/02 15:39:06 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TREE_STORE(RVAL2GOBJ(s)))
#define RVAL2ITR(i) ((GtkTreeIter*)(RVAL2BOXED(i, GTK_TYPE_TREE_ITER)))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))

static VALUE
tstore_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    gint cnt;
    GtkTreeStore* store;
    GType* buf;

    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");

    buf = ALLOCA_N(GType, argc);
    for (cnt = 0; cnt < argc; cnt++) {
        buf[cnt] = CLASS2GTYPE(argv[cnt]);
    }

    store = gtk_tree_store_newv(argc, buf);
 
    G_INITIALIZE(self, store);

    return Qnil;
}

static VALUE
tstore_set_column_types(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE  self;
{
    gint cnt;
    GtkTreeStore* store;
    GType* buf;
  
    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");
    buf = ALLOCA_N(GType, argc);
    for (cnt = 0; cnt < argc; cnt++) {
        buf[cnt] = CLASS2GTYPE(argv[cnt]);
    }

    gtk_tree_store_set_column_types(_SELF(self), argc, buf);

    return self;
}

static VALUE
tstore_set_value(self, iter, column, value)
    VALUE self, iter, column, value;
{
    GValue gval = {RVAL2GTYPE(value),};
    rbgobj_rvalue_to_gvalue(value, &gval);
    gtk_tree_store_set_value(_SELF(self), RVAL2ITR(iter), NUM2INT(column), &gval);
    return self;
}

/*
  void        gtk_tree_store_set (GtkTreeStore *tree_store,
                                               GtkTreeIter *iter,
					                                                    ...);

  void        gtk_tree_store_set_valist       (GtkTreeStore *tree_store,
  GtkTreeIter *iter,
  va_list var_args);
*/
    
static VALUE
tstore_remove(self, iter)
    VALUE self, iter;
{
    /* XXX This method should return boolean.
       but at least v2.0.6, it returns nothing(void).
    return gtk_tree_store_remove(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
    */
    gtk_tree_store_remove(_SELF(self), RVAL2ITR(iter));

    return Qtrue;
}

static VALUE
tstore_insert(self, parent, position)
    VALUE self, parent, position;
{
    GtkTreeIter iter;
    gtk_tree_store_insert(_SELF(self), &iter, 
                          NIL_P(parent)?NULL:RVAL2ITR(parent), 
                          NUM2INT(position));
    return ITR2RVAL(&iter);
}

static VALUE
tstore_insert_before(self, parent, sibling)
    VALUE self, parent, sibling;
{
    GtkTreeIter iter;
    gtk_tree_store_insert_before(_SELF(self), &iter, 
                                 NIL_P(parent) ? NULL : RVAL2ITR(parent), 
                                 NIL_P(sibling) ? NULL : RVAL2ITR(sibling));
  
    return ITR2RVAL(&iter);
}

static VALUE
tstore_insert_after(self, parent, sibling)
    VALUE self, parent, sibling;
{ 
    GtkTreeIter iter;
    gtk_tree_store_insert_after(_SELF(self), &iter, 
                                NIL_P(parent) ? NULL : RVAL2ITR(parent), 
                                NIL_P(sibling) ? NULL : RVAL2ITR(sibling));
  
    return ITR2RVAL(&iter);
}

static VALUE
tstore_prepend(self, parent)
    VALUE self, parent;
{
    GtkTreeIter iter;
    gtk_tree_store_prepend(_SELF(self), &iter, 
                           NIL_P(parent)?NULL:RVAL2ITR(parent));
  
    return ITR2RVAL(&iter);
}

static VALUE
tstore_append(self, parent)
    VALUE self, parent;
{
    GtkTreeIter iter;
    gtk_tree_store_append(_SELF(self), &iter, 
                          NIL_P(parent)?NULL:RVAL2ITR(parent));
  
    return ITR2RVAL(&iter);
}

static VALUE
tstore_is_ancestor(self, iter, descendant)
    VALUE self, iter, descendant;
{
    return gtk_tree_store_is_ancestor(_SELF(self), RVAL2ITR(iter), 
                                      RVAL2ITR(descendant));
}

static VALUE
tstore_iter_depth(self, iter)
    VALUE self, iter;
{
    return INT2NUM(gtk_tree_store_iter_depth(_SELF(self), RVAL2ITR(iter)));
}

static VALUE
tstore_clear(self)
    VALUE self;
{
    gtk_tree_store_clear(_SELF(self));
}

void
Init_gtk_tree_store()
{
    VALUE ts = G_DEF_CLASS(GTK_TYPE_TREE_STORE, "TreeStore", mGtk);
  
    rb_define_method(ts, "initialize", tstore_initialize, -1);
    rb_define_method(ts, "set_column_types", tstore_set_column_types, -1);
    rb_define_method(ts, "set_value", tstore_set_value, 3);
    rb_define_method(ts, "remove", tstore_remove, 1);
    rb_define_method(ts, "insert", tstore_insert, 2);
    rb_define_method(ts, "insert_before", tstore_insert_before, 2);
    rb_define_method(ts, "insert_after", tstore_insert_after, 2);
    rb_define_method(ts, "prepend", tstore_prepend, 1);
    rb_define_method(ts, "append", tstore_append, 1);
    rb_define_method(ts, "ancestor?", tstore_is_ancestor, 2);
    rb_define_method(ts, "iter_depth", tstore_iter_depth, 1);
    rb_define_method(ts, "clear", tstore_clear, 0);
}


