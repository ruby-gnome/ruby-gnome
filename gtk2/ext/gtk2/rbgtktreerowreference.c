/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreerowreference.c -

  $Author: mutoh $
  $Date: 2006/10/21 16:58:00 $

  Copyright (C) 2002-2006 Masao Mutoh
************************************************/

#include "global.h"

/*****************************************/

#ifndef GTK_TYPE_TREE_ROW_REFERENCE
static GtkTreeRowReference*
treerowref_copy(const GtkTreeRowReference *ref)
{
    return (GtkTreeRowReference*)ref;
}

GType
rbgtk_tree_row_reference_get_type()
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkTreeRowReference",
                    (GBoxedCopyFunc)treerowref_copy,
                    (GBoxedFreeFunc)gtk_tree_row_reference_free);
  return our_type;
}

GtkTreeRowReference *
rbgtk_get_tree_row_reference(VALUE obj)
{
    return (GtkTreeRowReference*)RVAL2BOXED(obj, GTK_TYPE_TREE_ROW_REFERENCE);
}
#endif

/*****************************************/

#define _SELF(s) RVAL2TREEROWREFERENCE(s)

/*****************************************/

static ID id_proxy;
static ID id_model;
static ID id_path;

static VALUE
treerowref_initialize(int argc, VALUE *argv, VALUE self)
{
  VALUE proxy, model, path;
  GtkTreeRowReference* ref;
    
  if (argc == 3){
    rb_scan_args(argc, argv, "3", &proxy, &model, &path);
    G_CHILD_SET(self, id_proxy, proxy);
    ref = gtk_tree_row_reference_new_proxy(RVAL2GOBJ(proxy),
                                           GTK_TREE_MODEL(RVAL2GOBJ(model)), 
                                           RVAL2GTKTREEPATH(path));
  } else {
    rb_scan_args(argc, argv, "2", &model, &path);
    ref = gtk_tree_row_reference_new(GTK_TREE_MODEL(RVAL2GOBJ(model)), 
                                     RVAL2GTKTREEPATH(path));
  }
  if (ref == NULL)
      rb_raise(rb_eArgError, "Invalid arguments were passed.");

  G_CHILD_SET(self, id_model, model);
  G_CHILD_SET(self, id_path, path);
  
  G_INITIALIZE(self, ref);
  return Qnil;
}


static VALUE
treerowref_get_path(VALUE self)
{
    VALUE ret = GTKTREEPATH2RVAL(gtk_tree_row_reference_get_path(_SELF(self)));
    G_CHILD_SET(self, id_path, ret);
    return ret;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
treerowref_get_model(VALUE self)
{
    VALUE ret = GOBJ2RVAL(gtk_tree_row_reference_get_model(_SELF(self)));
    G_CHILD_SET(self, id_model, ret);
    return ret;
}
#endif

static VALUE
treerowref_valid(VALUE self)
{
    return CBOOL2RVAL(gtk_tree_row_reference_valid(_SELF(self)));
}

static VALUE
treerowref_s_inserted(VALUE self, VALUE proxy, VALUE path)
{
    gtk_tree_row_reference_inserted(RVAL2GOBJ(proxy), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
treerowref_s_deleted(VALUE self, VALUE proxy, VALUE path)
{
    gtk_tree_row_reference_deleted(RVAL2GOBJ(proxy), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
treerowref_s_reordered(VALUE self, VALUE proxy, VALUE path, VALUE iter, VALUE new_orders)
{
    gint i, len;
    gint* orders;

    Check_Type(new_orders, T_ARRAY);

    len = RARRAY_LEN(new_orders);
    orders = ALLOCA_N(gint, len);

    for (i = 0; i < len; i++) {
        orders[i] = RARRAY_PTR(new_orders)[i];
    }
  
    gtk_tree_row_reference_reordered(RVAL2GOBJ(proxy), RVAL2GTKTREEPATH(path), 
                                     RVAL2GTKTREEITER(iter), orders);
    return self;
}

void 
Init_gtk_treerowreference()
{
    id_proxy = rb_intern("proxy");
    id_model = rb_intern("model");
    id_path = rb_intern("path");

    VALUE gTreeref = G_DEF_CLASS(GTK_TYPE_TREE_ROW_REFERENCE, "TreeRowReference", mGtk);
  
    rb_define_method(gTreeref, "initialize", treerowref_initialize, -1);
    rb_define_method(gTreeref, "path", treerowref_get_path, 0);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gTreeref, "model", treerowref_get_model, 0);
#endif
    rb_define_method(gTreeref, "valid?", treerowref_valid, 0);
    
    rb_define_singleton_method(gTreeref, "inserted", treerowref_s_inserted, 2);
    rb_define_singleton_method(gTreeref, "deleted", treerowref_s_deleted, 2);
    rb_define_singleton_method(gTreeref, "reordered", treerowref_s_reordered, 4);
}

