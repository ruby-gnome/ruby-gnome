/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreerowreference.c -

  $Author: sakai $
  $Date: 2002/10/05 07:42:46 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) ((GtkTreeRowReference*)RVAL2BOXED(s, GTK_TYPE_TREE_ROW_REFERENCE))
#define TREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define RVAL2TREEPATH(p) ((GtkTreePath*)RVAL2BOXED(p, GTK_TYPE_TREE_PATH))
#define RVAL2ITR(i) ((GtkTreeIter*)(RVAL2BOXED(i, GTK_TYPE_TREE_ITER)))

/*****************************************/
static GtkTreeRowReference*
treerowref_copy(ref)
    const GtkTreeRowReference* ref;
{ 
/* XXXX This code is broken, Is GtkTreeRowReference not available ?
  GtkTreeRowReference* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(GtkTreeRowReference, 1);
  *new_ref = *ref;
  return new_ref;
*/
  return (GtkTreeRowReference*)ref;
}

GType
gtk_treerowreference_get_type(void)
{ 
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkTreeRowReference",
                    (GBoxedCopyFunc)treerowref_copy,
                    (GBoxedFreeFunc)gtk_tree_row_reference_free);
  return our_type;
}

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
  
  G_INITIALIZE(self, ref);
  return Qnil;
}


static VALUE
treerowref_get_path(self)
    VALUE self;
{
    return BOXED2RVAL(gtk_tree_row_reference_get_path(_SELF(self)), 
                      GTK_TYPE_TREE_ROW_REFERENCE);
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
  
    gtk_tree_row_reference_reordered(RVAL2GOBJ(proxy), RVAL2TREEPATH(path), RVAL2ITR(iter), orders);
    return self;
}

void 
Init_gtk_treerowreference()
{
  VALUE gTreeref = G_DEF_CLASS(GTK_TYPE_TREE_ROW_REFERENCE, "TreeRowReference", mGtk);
  
  rb_define_method(gTreeref, "initialize", treerowref_initialize, -1);
  rb_define_method(gTreeref, "path", treerowref_get_path, 0);
  rb_define_method(gTreeref, "valid?", treerowref_valid, 0);

  rb_define_singleton_method(gTreeref, "inserted", treerowref_s_inserted, 2);
  rb_define_singleton_method(gTreeref, "deleted", treerowref_s_deleted, 2);
  rb_define_singleton_method(gTreeref, "reordered", treerowref_s_reordered, 4);
}

