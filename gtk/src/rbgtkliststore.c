/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkliststore.c -

  $Author: mutoh $
  $Date: 2003/11/14 07:14:57 $

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
    GType gtype = gtk_tree_model_get_column_type(GTK_TREE_MODEL(RVAL2GOBJ(self)), NUM2INT(column));
    GValue gval = {0,};
    g_value_init(&gval, gtype);

    rbgobj_rvalue_to_gvalue(value, &gval);

    gtk_list_store_set_value(_SELF(self), RVAL2ITR(iter), NUM2INT(column), &gval);

    g_value_unset(&gval);
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
lstore_remove(self, iter)
    VALUE self, iter;
{
#if GTK_MINOR_VERSION >= 2
    return gtk_list_store_remove(_SELF(self), RVAL2ITR(iter)) ? Qtrue : Qfalse;
#else
    gtk_list_store_remove(_SELF(self), RVAL2ITR(iter));
    return Qtrue;
#endif
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

#if GTK_MINOR_VERSION >= 2
static VALUE
lstore_iter_is_valid(self, iter)
    VALUE self, iter;
{
    return CBOOL2RVAL(gtk_list_store_iter_is_valid(_SELF(self), RVAL2ITR(iter)));
}
static VALUE
lstore_reorder(self, new_order)
    VALUE self, new_order;
{
    gint i;
    gint len = RARRAY(new_order)->len;
    gint* gnew_order = g_new(gint, len);

    for (i = 0; i < len; i++){
        gnew_order[i] = RARRAY(new_order)->ptr[i];
    }

    gtk_list_store_reorder(_SELF(self), gnew_order);
    g_free(gnew_order);
    return self;
}
static VALUE
lstore_swap(self, iter1, iter2)
    VALUE self, iter1, iter2;
{
    gtk_list_store_swap(_SELF(self), RVAL2ITR(iter1), RVAL2ITR(iter2));
    return self;
}
static VALUE
lstore_move_before(self, iter, position)
    VALUE self, iter, position;
{
    gtk_list_store_move_before(_SELF(self), RVAL2ITR(iter), 
                               NIL_P(position) ? NULL : RVAL2ITR(position));
    return self;
}
static VALUE
lstore_move_after(self, iter, position)
    VALUE self, iter, position;
{
    gtk_list_store_move_after(_SELF(self), RVAL2ITR(iter), 
                               NIL_P(position) ? NULL : RVAL2ITR(position));
    return self;
}
#endif

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
#if GTK_MINOR_VERSION >= 2
    rb_define_method(ls, "iter_is_valid?", lstore_iter_is_valid, 1);
    rb_define_method(ls, "reorder", lstore_reorder, 1);
    rb_define_method(ls, "swap", lstore_swap, 2);
    rb_define_method(ls, "move_before", lstore_move_before, 2);
    rb_define_method(ls, "move_after", lstore_move_after, 2);
#endif

}


