/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreepath.c -

  $Author: mutoh $
  $Date: 2002/10/02 15:39:06 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) ((GtkTreePath*)RVAL2BOXED(s, GTK_TYPE_TREE_PATH))

static VALUE
treepath_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE path;
    GtkTreePath* widget;
    
    if (rb_scan_args(argc, argv, "01", &path) == 1) {
        widget = gtk_tree_path_new_from_string(RVAL2CSTR(path));
    }
    else {
        widget = gtk_tree_path_new();
    }
    
    G_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
treepath_to_string(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_tree_path_to_string(_SELF(self)));
}

/* Do we need this ? 
GtkTreePath* gtk_tree_path_new_first (void);

Deprecated
#define     gtk_tree_path_new_root ()
*/

static VALUE
treepath_append_index(self, index)
    VALUE self, index;
{
    gtk_tree_path_append_index(_SELF(self), NUM2INT(index));
    return self;
}

static VALUE
treepath_prepend_index(self, index)
    VALUE self, index;
{
    gtk_tree_path_prepend_index(_SELF(self), NUM2INT(index));
    return self;
}

static VALUE
treepath_get_depth(self)
    VALUE self;
{
    return INT2NUM(gtk_tree_path_get_depth(_SELF(self)));
}

static VALUE
treepath_get_indices(self)
    VALUE self;
{
/* How can I get lenght of indices ?
    gint i, len;
    gint len = ????;
    gint* indices = gtk_tree_path_get_indices(_SELF(self));
*/
    rb_notimplement();
    return Qnil;
}

static VALUE
treepath_compare(self, other)
    VALUE self, other;
{
    return NUM2INT(gtk_tree_path_compare(_SELF(self), _SELF(other)));
}

static VALUE
treepath_next(self)
    VALUE self;
{
    gtk_tree_path_next(_SELF(self));
    return Qtrue;
}

static VALUE
treepath_prev(self)
    VALUE self;
{
    return gtk_tree_path_prev(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
treepath_up(self)
    VALUE self;
{
    return gtk_tree_path_up(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
treepath_down(self)
    VALUE self;
{
    gtk_tree_path_down(_SELF(self));
    return Qtrue;
}

static VALUE
treepath_is_ancestor(self, descendant)
    VALUE self, descendant;
{
    return gtk_tree_path_is_ancestor(_SELF(self), _SELF(descendant)) 
        ? Qtrue : Qfalse;
}

static VALUE
treepath_is_descendant(self, ancestor)
    VALUE self, ancestor;
{
    return gtk_tree_path_is_descendant(_SELF(self), _SELF(ancestor)) 
        ? Qtrue : Qfalse;
}

void 
Init_gtk_treepath()
{
    VALUE gTreepath = G_DEF_CLASS(GTK_TYPE_TREE_PATH, "TreePath", mGtk);

    rb_define_method(gTreepath, "initialize", treepath_initialize, -1);
    rb_define_method(gTreepath, "to_str", treepath_to_string, 0);
    rb_define_method(gTreepath, "append_index", treepath_append_index, 1);
    rb_define_method(gTreepath, "prepend_index", treepath_prepend_index, 1);
    rb_define_method(gTreepath, "depth", treepath_get_depth, 0);
    rb_define_method(gTreepath, "indices", treepath_get_indices, 0);
    rb_define_method(gTreepath, "<=>", treepath_compare, 1);
    rb_define_method(gTreepath, "next", treepath_next, 0);
    rb_define_method(gTreepath, "prev", treepath_prev, 0);
    rb_define_method(gTreepath, "up", treepath_up, 0);
    rb_define_method(gTreepath, "down", treepath_down, 0);
    rb_define_method(gTreepath, "ancestor?", treepath_is_ancestor, 1);
    rb_define_method(gTreepath, "descendant?", treepath_is_descendant, 1);
}
