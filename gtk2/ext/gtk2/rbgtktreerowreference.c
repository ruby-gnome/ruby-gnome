/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTreeRowReference
#define _SELF(s) RVAL2TREEROWREFERENCE(s)

/*****************************************/

static ID id_proxy;
static ID id_model;
static ID id_path;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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
rg_path(VALUE self)
{
    VALUE ret = GTKTREEPATH2RVAL(gtk_tree_row_reference_get_path(_SELF(self)));
    G_CHILD_SET(self, id_path, ret);
    return ret;
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_model(VALUE self)
{
    VALUE ret = GOBJ2RVAL(gtk_tree_row_reference_get_model(_SELF(self)));
    G_CHILD_SET(self, id_model, ret);
    return ret;
}
#endif

static VALUE
rg_valid_p(VALUE self)
{
    return CBOOL2RVAL(gtk_tree_row_reference_valid(_SELF(self)));
}

static VALUE
rg_s_inserted(VALUE self, VALUE proxy, VALUE path)
{
    gtk_tree_row_reference_inserted(RVAL2GOBJ(proxy), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
rg_s_deleted(VALUE self, VALUE proxy, VALUE path)
{
    gtk_tree_row_reference_deleted(RVAL2GOBJ(proxy), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
rg_s_reordered(VALUE self, VALUE rbproxy, VALUE rbpath, VALUE rbiter, VALUE rbnew_order)
{
    GObject *proxy = RVAL2GOBJ(rbproxy);
    GtkTreePath *path = RVAL2GTKTREEPATH(rbpath);
    GtkTreeIter *iter = RVAL2GTKTREEITER(rbiter);
    /* TODO: How do we implement this check?
    gint columns = gtk_tree_model_get_n_columns(proxy);
    */
    long n;
    gint *new_order = RVAL2GINTS(rbnew_order, n);

    /*
    if (n != columns) {
        g_free(new_order);

        rb_raise(rb_eArgError,
                 "new order array must contain same number of elements as the number of columns in the store: %ld != %d",
                 n, columns);
    }
    */

    gtk_tree_row_reference_reordered(proxy, path, iter, new_order);

    g_free(new_order);

    return self;
}

void 
Init_gtk_treerowreference(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TREE_ROW_REFERENCE, "TreeRowReference", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(path, 0);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(model, 0);
#endif
    RG_DEF_METHOD_P(valid, 0);

    RG_DEF_SMETHOD(inserted, 2);
    RG_DEF_SMETHOD(deleted, 2);
    RG_DEF_SMETHOD(reordered, 4);

    id_proxy = rb_intern("proxy");
    id_model = rb_intern("model");
    id_path = rb_intern("path");
}
