/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreeselection.c -

  $Author: ggc $ 
  $Date: 2007/07/13 14:27:10 $

  Copyright (C) 2002-2006 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) (GTK_TREE_SELECTION(RVAL2GOBJ(s)))
#define RVAL2TREEMODEL(s) (GTK_TREE_MODEL(RVAL2GOBJ(s)))
#define RVAL2TREEPATH(p) ((GtkTreePath*)RVAL2BOXED(p, GTK_TYPE_TREE_PATH))
#define TREEPATH2RVAL(t) (BOXED2RVAL(t, GTK_TYPE_TREE_PATH))
#define ITR2RVAL(i) (BOXED2RVAL(i, GTK_TYPE_TREE_ITER))
#define RVAL2ITR(i) ((GtkTreeIter*)RVAL2BOXED(i, GTK_TYPE_TREE_ITER))

static VALUE
treeselection_set_mode(self, type)
    VALUE self, type;
{
    gtk_tree_selection_set_mode(_SELF(self), RVAL2GENUM(type, GTK_TYPE_SELECTION_MODE));
    return self;
}

static VALUE
treeselection_get_mode(self)
    VALUE self;
{
    return GENUM2RVAL(gtk_tree_selection_get_mode(_SELF(self)), GTK_TYPE_SELECTION_MODE);
}

static gboolean
selection_func(selection, model, path, path_currently_selected, func)
    GtkTreeSelection* selection;
    GtkTreeModel* model;
    GtkTreePath* path;
    gboolean path_currently_selected;
    gpointer func;
{
    return RTEST(rb_funcall((VALUE)func, id_call, 4, 
                            GOBJ2RVAL(selection),
                            GOBJ2RVAL(model),
                            TREEPATH2RVAL(path),
                            CBOOL2RVAL(path_currently_selected)));
}

static VALUE
treeselection_set_select_function(self)
    VALUE self;
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
treeselection_get_tree_view(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_tree_selection_get_tree_view(_SELF(self)));
}

static VALUE
treeselection_get_selected(self)
    VALUE self;
{
    GtkTreeIter iter;
    GtkTreeModel* model;
    gboolean ret = gtk_tree_selection_get_selected(_SELF(self), &model, &iter);
    iter.user_data3 = model;
    return ret ? ITR2RVAL(&iter) : Qnil;
}

static void
foreach_func(model, path, iter, data)
    GtkTreeModel* model;
    GtkTreePath* path;
    GtkTreeIter* iter;
    gpointer data;
{
    iter->user_data3 = model;
    rb_funcall((VALUE)data, id_call, 3, GOBJ2RVAL(model), 
               TREEPATH2RVAL(path), ITR2RVAL(iter));
}

static VALUE
treeselection_selected_foreach(self)
    VALUE self;
{
    volatile VALUE func = rb_block_proc();
    gtk_tree_selection_selected_foreach(_SELF(self), 
                                        (GtkTreeSelectionForeachFunc)foreach_func, 
                                        (gpointer)func);
    return self;
}

static VALUE
treeselection_select_path(self, path)
    VALUE self, path;
{
    gtk_tree_selection_select_path(_SELF(self), RVAL2TREEPATH(path));
    return self;
}

static VALUE
treeselection_unselect_path(self, path)
    VALUE self, path;
{
    gtk_tree_selection_unselect_path(_SELF(self), RVAL2TREEPATH(path));
    return self;
}

static VALUE
treeselection_path_is_selected(self, path)
    VALUE self, path;
{
    return CBOOL2RVAL(gtk_tree_selection_path_is_selected(_SELF(self), RVAL2TREEPATH(path)));
}

static VALUE
treeselection_select_iter(self, iter)
    VALUE self, iter;
{
    gtk_tree_selection_select_iter(_SELF(self), RVAL2ITR(iter));
    return self;
}

static VALUE
treeselection_unselect_iter(self, iter)
    VALUE self, iter;
{
    gtk_tree_selection_unselect_iter(_SELF(self), RVAL2ITR(iter));
    return self;
}

static VALUE
treeselection_iter_is_selected(self, iter)
    VALUE self, iter;
{
    return CBOOL2RVAL(gtk_tree_selection_iter_is_selected(_SELF(self), RVAL2ITR(iter)));
}

static VALUE
treeselection_select_all(self)
    VALUE self;
{
    gtk_tree_selection_select_all(_SELF(self));
    return self;
}

static VALUE
treeselection_unselect_all(self)
    VALUE self;
{
    gtk_tree_selection_unselect_all(_SELF(self));
    return self;
}

static VALUE
treeselection_select_range(self, start_path, end_path)
    VALUE self, start_path, end_path;
{
    gtk_tree_selection_select_range(_SELF(self), RVAL2TREEPATH(start_path), 
                                    RVAL2TREEPATH(end_path));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
treeselection_get_selected_rows(self)
    VALUE self;
{
    GtkTreeModel* model;
    GList* list = gtk_tree_selection_get_selected_rows(_SELF(self), &model);
    VALUE ret = GLIST2ARY2(list, GTK_TYPE_TREE_PATH);
    g_list_foreach(list, (GFunc)gtk_tree_path_free, (gpointer)NULL);
    g_list_free(list);
    return ret;
}

static VALUE
treeselection_count_selected_rows(self)
    VALUE self;
{
    return INT2NUM(gtk_tree_selection_count_selected_rows(_SELF(self)));
}

static VALUE
treeselection_unselect_range(self, start_path, end_path)
    VALUE self, start_path, end_path;
{
    gtk_tree_selection_unselect_range(_SELF(self), RVAL2TREEPATH(start_path), 
                                      RVAL2TREEPATH(end_path));
    return self;
}
#endif

void
Init_gtk_treeselection()
{
    VALUE gTs = G_DEF_CLASS(GTK_TYPE_TREE_SELECTION, "TreeSelection", mGtk); 
    
    rb_define_method(gTs, "set_mode", treeselection_set_mode, 1);
    rb_define_method(gTs, "mode", treeselection_get_mode, 0);
    rb_define_method(gTs, "set_select_function", treeselection_set_select_function, 0);
    rb_define_method(gTs, "tree_view", treeselection_get_tree_view, 0);
    rb_define_method(gTs, "selected", treeselection_get_selected, 0);
    rb_define_method(gTs, "selected_each", treeselection_selected_foreach, 0);
    rb_define_method(gTs, "select_path", treeselection_select_path, 1);
    rb_define_method(gTs, "unselect_path", treeselection_unselect_path, 1);
    rb_define_method(gTs, "path_is_selected?", treeselection_path_is_selected, 1);
    rb_define_method(gTs, "select_iter", treeselection_select_iter, 1);
    rb_define_method(gTs, "unselect_iter", treeselection_unselect_iter, 1);
    rb_define_method(gTs, "iter_is_selected?", treeselection_iter_is_selected, 1);
    rb_define_method(gTs, "select_all", treeselection_select_all, 0);
    rb_define_method(gTs, "unselect_all", treeselection_unselect_all, 0);
    rb_define_method(gTs, "select_range", treeselection_select_range, 2);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gTs, "selected_rows", treeselection_get_selected_rows, 0);
    rb_define_method(gTs, "count_selected_rows", treeselection_count_selected_rows, 0);
    rb_define_method(gTs, "unselect_range", treeselection_unselect_range, 2);
#endif

    G_DEF_SETTERS(gTs);
}


