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

#define RG_TARGET_NAMESPACE cTreeSelection
#define _SELF(s) (GTK_TREE_SELECTION(RVAL2GOBJ(s)))
#define RVAL2TREEMODEL(s) (GTK_TREE_MODEL(RVAL2GOBJ(s)))

static VALUE
rg_set_mode(VALUE self, VALUE type)
{
    gtk_tree_selection_set_mode(_SELF(self), RVAL2GENUM(type, GTK_TYPE_SELECTION_MODE));
    return self;
}

static VALUE
rg_mode(VALUE self)
{
    return GENUM2RVAL(gtk_tree_selection_get_mode(_SELF(self)), GTK_TYPE_SELECTION_MODE);
}

static gboolean
selection_func(GtkTreeSelection *selection, GtkTreeModel *model, GtkTreePath *path, gboolean path_currently_selected, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 4, 
                            GOBJ2RVAL(selection),
                            GOBJ2RVAL(model),
                            GTKTREEPATH2RVAL(path),
                            CBOOL2RVAL(path_currently_selected)));
}

static VALUE
rg_set_select_function(VALUE self)
{
    volatile VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    gtk_tree_selection_set_select_function(_SELF(self),
                                    (GtkTreeSelectionFunc)selection_func,
                                    (gpointer)func, NULL);
    return self;
}

/* We don't need this(?)
gpointer    gtk_tree_selection_get_user_data
(GtkTreeSelection *selection);
*/

static VALUE
rg_tree_view(VALUE self)
{
    return GOBJ2RVAL(gtk_tree_selection_get_tree_view(_SELF(self)));
}

static VALUE
rg_selected(VALUE self)
{
    GtkTreeIter iter;
    GtkTreeModel* model;
    gboolean ret = gtk_tree_selection_get_selected(_SELF(self), &model, &iter);
    iter.user_data3 = model;
    return ret ? GTKTREEITER2RVAL(&iter) : Qnil;
}

static void
foreach_func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data)
{
    iter->user_data3 = model;
    rb_funcall((VALUE)data, id_call, 3, GOBJ2RVAL(model), 
               GTKTREEPATH2RVAL(path), GTKTREEITER2RVAL(iter));
}

static VALUE
rg_selected_each(VALUE self)
{
    volatile VALUE func = rb_block_proc();
    gtk_tree_selection_selected_foreach(_SELF(self), 
                                        (GtkTreeSelectionForeachFunc)foreach_func, 
                                        (gpointer)func);
    return self;
}

static VALUE
rg_select_path(VALUE self, VALUE path)
{
    gtk_tree_selection_select_path(_SELF(self), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
rg_unselect_path(VALUE self, VALUE path)
{
    gtk_tree_selection_unselect_path(_SELF(self), RVAL2GTKTREEPATH(path));
    return self;
}

static VALUE
rg_path_is_selected_p(VALUE self, VALUE path)
{
    return CBOOL2RVAL(gtk_tree_selection_path_is_selected(_SELF(self), RVAL2GTKTREEPATH(path)));
}

static VALUE
rg_select_iter(VALUE self, VALUE iter)
{
    gtk_tree_selection_select_iter(_SELF(self), RVAL2GTKTREEITER(iter));
    return self;
}

static VALUE
rg_unselect_iter(VALUE self, VALUE iter)
{
    gtk_tree_selection_unselect_iter(_SELF(self), RVAL2GTKTREEITER(iter));
    return self;
}

static VALUE
rg_iter_is_selected_p(VALUE self, VALUE iter)
{
    return CBOOL2RVAL(gtk_tree_selection_iter_is_selected(_SELF(self), RVAL2GTKTREEITER(iter)));
}

static VALUE
rg_select_all(VALUE self)
{
    gtk_tree_selection_select_all(_SELF(self));
    return self;
}

static VALUE
rg_unselect_all(VALUE self)
{
    gtk_tree_selection_unselect_all(_SELF(self));
    return self;
}

static VALUE
rg_select_range(VALUE self, VALUE start_path, VALUE end_path)
{
    gtk_tree_selection_select_range(_SELF(self), RVAL2GTKTREEPATH(start_path), 
                                    RVAL2GTKTREEPATH(end_path));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_selected_rows(VALUE self)
{
    GtkTreeModel* model;
    GList* list = gtk_tree_selection_get_selected_rows(_SELF(self), &model);
    VALUE ret = GLIST2ARY2(list, GTK_TYPE_TREE_PATH);
    g_list_foreach(list, (GFunc)gtk_tree_path_free, (gpointer)NULL);
    g_list_free(list);
    return ret;
}

static VALUE
rg_count_selected_rows(VALUE self)
{
    return INT2NUM(gtk_tree_selection_count_selected_rows(_SELF(self)));
}

static VALUE
rg_unselect_range(VALUE self, VALUE start_path, VALUE end_path)
{
    gtk_tree_selection_unselect_range(_SELF(self), RVAL2GTKTREEPATH(start_path), 
                                      RVAL2GTKTREEPATH(end_path));
    return self;
}
#endif

void
Init_gtk_treeselection(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TREE_SELECTION, "TreeSelection", mGtk); 

    RG_DEF_METHOD(set_mode, 1);
    RG_DEF_METHOD(mode, 0);
    RG_DEF_METHOD(set_select_function, 0);
    RG_DEF_METHOD(tree_view, 0);
    RG_DEF_METHOD(selected, 0);
    RG_DEF_METHOD(selected_each, 0);
    RG_DEF_METHOD(select_path, 1);
    RG_DEF_METHOD(unselect_path, 1);
    RG_DEF_METHOD_P(path_is_selected, 1);
    RG_DEF_METHOD(select_iter, 1);
    RG_DEF_METHOD(unselect_iter, 1);
    RG_DEF_METHOD_P(iter_is_selected, 1);
    RG_DEF_METHOD(select_all, 0);
    RG_DEF_METHOD(unselect_all, 0);
    RG_DEF_METHOD(select_range, 2);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(selected_rows, 0);
    RG_DEF_METHOD(count_selected_rows, 0);
    RG_DEF_METHOD(unselect_range, 2);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
