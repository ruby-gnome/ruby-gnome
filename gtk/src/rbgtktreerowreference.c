/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreerowreference.c -

  $Author: mutoh $
  $Date: 2003/07/14 18:12:53 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) RVAL2TREEROWREFERENCE(s)
#define TREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2TREEPATH(p) ((GtkTreePath*)RVAL2BOXED(p, GTK_TYPE_TREE_PATH))
#define RVAL2ITR(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

/*****************************************/

static VALUE
treerowref_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
  VALUE proxy, model, path;
  GtkTreeRowReference* ref;
    
  if (argc == 3){
    rb_scan_args(argc, argv, "3", &proxy, &model, &path);
    ref = gtk_tree_row_reference_new_proxy(RVAL2GOBJ(proxy),
                                           GTK_TREE_MODEL(RVAL2GOBJ(model)), 
                                           RVAL2TREEPATH(path));
  } else {
    rb_scan_args(argc, argv, "2", &model, &path);
    ref = gtk_tree_row_reference_new(GTK_TREE_MODEL(RVAL2GOBJ(model)), 
                                     RVAL2TREEPATH(path));
  }
  if (ref == NULL)
      rb_raise(rb_eArgError, "Invalid arguments were passed.");
  
  G_INITIALIZE(self, ref);
  return Qnil;
}


static VALUE
treerowref_get_path(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_tree_row_reference_get_path(_SELF(self)), 
                      RBGTK_TYPE_TREE_ROW_REFERENCE);
}

static VALUE
treerowref_valid(self)
    VALUE self;
{
    return gtk_tree_row_reference_valid(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
treerowref_s_inserted(self, proxy, path)
    VALUE self, proxy, path;
{
    gtk_tree_row_reference_inserted(RVAL2GOBJ(proxy), RVAL2TREEPATH(path));
    return self;
}

static VALUE
treerowref_s_deleted(self, proxy, path)
    VALUE self, proxy, path;
{
    gtk_tree_row_reference_deleted(RVAL2GOBJ(proxy), RVAL2TREEPATH(path));
    return self;
}

static VALUE
treerowref_s_reordered(self, proxy, path, iter, new_orders)
    VALUE self, proxy, path, iter, new_orders;
{
    gint i, len;
    gint* orders;

    Check_Type(new_orders, T_ARRAY);

    len = RARRAY(new_orders)->len;
    orders = ALLOCA_N(gint, len);

    for (i = 0; i < len; i++) {
        orders[i] = RARRAY(new_orders)->ptr[i];
    }
  
    gtk_tree_row_reference_reordered(RVAL2GOBJ(proxy), RVAL2TREEPATH(path), 
                                     RVAL2ITR(iter), orders);
    return self;
}

void 
Init_gtk_treerowreference()
{
    if (rbgtk_tree_row_reference_get_type) {
        VALUE gTreeref = G_DEF_CLASS(RBGTK_TYPE_TREE_ROW_REFERENCE, "TreeRowReference", mGtk);
  
        rb_define_method(gTreeref, "initialize", treerowref_initialize, -1);
        rb_define_method(gTreeref, "path", treerowref_get_path, 0);
        rb_define_method(gTreeref, "valid?", treerowref_valid, 0);

        rb_define_singleton_method(gTreeref, "inserted", treerowref_s_inserted, 2);
        rb_define_singleton_method(gTreeref, "deleted", treerowref_s_deleted, 2);
        rb_define_singleton_method(gTreeref, "reordered", treerowref_s_reordered, 4);
    }
}

