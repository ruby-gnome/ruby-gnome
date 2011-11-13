/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003-2005 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTreeModelSort
#define _SELF(s) (GTK_TREE_MODEL_SORT(RVAL2GOBJ(s)))

static ID id_model;

static VALUE
rg_initialize(VALUE self, VALUE model)
{
    G_CHILD_SET(self, id_model, model);
    G_INITIALIZE(self, 
                 gtk_tree_model_sort_new_with_model(GTK_TREE_MODEL(RVAL2GOBJ(model))));
    return Qnil;
}

static VALUE
rg_convert_child_path_to_path(VALUE self, VALUE child_path)
{
    return GTKTREEPATH2RVAL(gtk_tree_model_sort_convert_child_path_to_path(
                             _SELF(self),
                             RVAL2GTKTREEPATH(child_path)));
}

static VALUE
rg_convert_child_iter_to_iter(VALUE self, VALUE child_iter)
{
    GtkTreeIter sort_iter;
    GtkTreeModelSort* modelsort = _SELF(self);
    gtk_tree_model_sort_convert_child_iter_to_iter(modelsort, &sort_iter,
                                                   RVAL2GTKTREEITER(child_iter));
    sort_iter.user_data3 = gtk_tree_model_sort_get_model(modelsort);
    return GTKTREEITER2RVAL(&sort_iter);
}

static VALUE
rg_convert_path_to_child_path(VALUE self, VALUE sorted_path)
{
    return GTKTREEPATH2RVAL(gtk_tree_model_sort_convert_path_to_child_path(
                             _SELF(self),
                             RVAL2GTKTREEPATH(sorted_path)));
}

static VALUE
rg_convert_iter_to_child_iter(VALUE self, VALUE sorted_iter)
{
    GtkTreeIter child_iter;
    GtkTreeModelSort* modelsort = _SELF(self);
    gtk_tree_model_sort_convert_iter_to_child_iter(modelsort, &child_iter,
                                                   RVAL2GTKTREEITER(sorted_iter));
    child_iter.user_data3 = gtk_tree_model_sort_get_model(modelsort);
    return GTKTREEITER2RVAL(&child_iter);
} 

static VALUE
rg_reset_default_sort_func(VALUE self)
{
    gtk_tree_model_sort_reset_default_sort_func(_SELF(self));
    return self;
}

static VALUE
rg_clear_cache(VALUE self)
{
    gtk_tree_model_sort_clear_cache(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_iter_is_valid_p(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_tree_model_sort_iter_is_valid(_SELF(self), RVAL2GTKTREEITER(iter)));
}
#endif

void 
Init_gtk_tmodelsort(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TREE_MODEL_SORT, "TreeModelSort", mGtk);

    id_model = rb_intern("model");

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(convert_child_path_to_path, 1);
    RG_DEF_METHOD(convert_child_iter_to_iter, 1);
    RG_DEF_METHOD(convert_path_to_child_path, 1);
    RG_DEF_METHOD(convert_iter_to_child_iter, 1);
    RG_DEF_METHOD(reset_default_sort_func, 0);
    RG_DEF_METHOD(clear_cache, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD_P(iter_is_valid, 1);
#endif

}
