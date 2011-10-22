/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Masao Mutoh
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
                                                                                
#if GTK_CHECK_VERSION(2,4,0)
#define _SELF(s) (GTK_TREE_MODEL_FILTER(RVAL2GOBJ(s)))

static ID id_child_model;
static ID id_root;

static VALUE
treemodelfilter_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE child_model, root;
    GtkTreeModel* widget;

    rb_scan_args(argc, argv, "11", &child_model, &root);

    G_CHILD_SET(self, id_child_model, child_model);
    if (NIL_P(root)){
        widget = gtk_tree_model_filter_new(GTK_TREE_MODEL(RVAL2GOBJ(child_model)), 
                                           (GtkTreePath*)NULL);
    } else {
        G_CHILD_SET(self, id_root, root);
        widget = gtk_tree_model_filter_new(GTK_TREE_MODEL(RVAL2GOBJ(child_model)), 
                                           (GtkTreePath*)RVAL2GTKTREEPATH(root));
    }

    G_INITIALIZE(self, widget);
    return Qnil;
}

static gboolean
visible_func(GtkTreeModel *model, GtkTreeIter *iter, gpointer func)
{
    VALUE ret;

    iter->user_data3 = model;
    ret = rb_funcall((VALUE)func, id_call, 2, GOBJ2RVAL(model), GTKTREEITER2RVAL(iter));
    return RVAL2CBOOL(ret);
}

static VALUE
treemodelfilter_set_visible_func(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_tree_model_filter_set_visible_func(_SELF(self), 
                                           (GtkTreeModelFilterVisibleFunc)visible_func, 
                                           (gpointer)func, NULL);
    return self;
}

static void
modify_func(GtkTreeModel *model, GtkTreeIter *iter, GValue *value, gint column, gpointer func)
{
    VALUE ret;
    iter->user_data3 = model;
    ret = rb_funcall((VALUE)func, id_call, 3, GOBJ2RVAL(model), GTKTREEITER2RVAL(iter),
                     INT2NUM(column));
    rbgobj_rvalue_to_gvalue(ret, value);
}

/*
 * Should return value
 *
 * e.g. 
 * filter.set_modify_func(String) do |model, iter, column|
 *   "foo"
 * end
 */
static VALUE
treemodelfilter_set_modify_func(int argc, VALUE *argv, VALUE self)
{
    VALUE func = rb_block_proc();
    gint i;
    GType* types;

    if (argc == 0) rb_raise(rb_eArgError, "need more than 1 class type.");

    types = ALLOCA_N(GType, argc);  

    G_RELATIVE(self, func);

    for (i = 0; i < argc; i++){
        types[i] = CLASS2GTYPE(argv[i]);
    }
    gtk_tree_model_filter_set_modify_func(_SELF(self),
                                          argc, types, 
                                          (GtkTreeModelFilterModifyFunc)modify_func,
                                          (gpointer)func, NULL);
    return self;
}

static VALUE
treemodelfilter_set_visible_column(VALUE self, VALUE column)
{
    gtk_tree_model_filter_set_visible_column(_SELF(self), NUM2INT(column));
    return self;
}

static VALUE
treemodelfilter_get_model(VALUE self)
{
    return GOBJ2RVAL(gtk_tree_model_filter_get_model(_SELF(self)));
}

static VALUE
treemodelfilter_convert_child_iter_to_iter(VALUE self, VALUE child_iter)
{
    GtkTreeIter filter_iter;
    GtkTreeModelFilter* modelfilter = _SELF(self);
    gtk_tree_model_filter_convert_child_iter_to_iter(modelfilter, &filter_iter,
                                                     RVAL2GTKTREEITER(child_iter));
    filter_iter.user_data3 = gtk_tree_model_filter_get_model(modelfilter);
    return GTKTREEITER2RVAL(&filter_iter);
}

static VALUE
treemodelfilter_convert_iter_to_child_iter(VALUE self, VALUE filtered_iter)
{
    GtkTreeIter child_iter;
    GtkTreeModelFilter* modelfilter = _SELF(self);
    gtk_tree_model_filter_convert_iter_to_child_iter(modelfilter, &child_iter,
                                                     RVAL2GTKTREEITER(filtered_iter));
    child_iter.user_data3 = gtk_tree_model_filter_get_model(modelfilter);
    return GTKTREEITER2RVAL(&child_iter);
} 

static VALUE
treemodelfilter_convert_child_path_to_path(VALUE self, VALUE child_path)
{
    return GTKTREEPATH2RVAL(gtk_tree_model_filter_convert_child_path_to_path(
                             _SELF(self),
                             RVAL2GTKTREEPATH(child_path)));
}
static VALUE
treemodelfilter_convert_path_to_child_path(VALUE self, VALUE filter_path)
{
    return GTKTREEPATH2RVAL(gtk_tree_model_filter_convert_path_to_child_path(
                             _SELF(self),
                             RVAL2GTKTREEPATH(filter_path)));
}

static VALUE
treemodelfilter_refilter(VALUE self)
{
    gtk_tree_model_filter_refilter(_SELF(self));
    return self;
}

static VALUE
treemodelfilter_clear_cache(VALUE self)
{
    gtk_tree_model_filter_clear_cache(_SELF(self));
    return self;
}
#endif

void 
Init_gtk_treemodelfilter(void)
{
#if GTK_CHECK_VERSION(2,4,0)
    VALUE tmf = G_DEF_CLASS(GTK_TYPE_TREE_MODEL_FILTER, "TreeModelFilter", mGtk);

    id_child_model = rb_intern("child_model");
    id_root = rb_intern("root");

    rb_define_method(tmf, "initialize", treemodelfilter_initialize, -1);
    rb_define_method(tmf, "set_visible_func", treemodelfilter_set_visible_func, 0);
    rb_define_method(tmf, "set_modify_func", treemodelfilter_set_modify_func, -1);
    rb_define_method(tmf, "set_visible_column", treemodelfilter_set_visible_column, 1);
    rb_define_method(tmf, "model", treemodelfilter_get_model, 0);
    rb_define_method(tmf, "convert_child_iter_to_iter", treemodelfilter_convert_child_iter_to_iter, 1);
    rb_define_method(tmf, "convert_iter_to_child_iter", treemodelfilter_convert_iter_to_child_iter, 1);
    rb_define_method(tmf, "convert_child_path_to_path", treemodelfilter_convert_child_path_to_path, 1);
    rb_define_method(tmf, "convert_path_to_child_path", treemodelfilter_convert_path_to_child_path, 1);
    rb_define_method(tmf, "refilter", treemodelfilter_refilter, 0);
    rb_define_method(tmf, "clear_cache", treemodelfilter_clear_cache, 0);
    G_DEF_SETTERS(tmf);
#endif
}
