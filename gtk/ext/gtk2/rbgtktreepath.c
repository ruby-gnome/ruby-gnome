/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktreepath.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:10 $

  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
  Copyright (C) 2003,2004 Masao Mutoh
************************************************/

#include "global.h"

#define _SELF(s) ((GtkTreePath*)RVAL2BOXED(s, GTK_TYPE_TREE_PATH))

static ID id_equal;

static VALUE
treepath_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE path;
    GtkTreePath* widget;
    
    if (argc == 1) {
        path = argv[0];
        if (TYPE(path) == T_STRING){
            widget = gtk_tree_path_new_from_string(RVAL2CSTR(path));
        } else {
            widget = gtk_tree_path_new();
            gtk_tree_path_append_index(widget, NUM2INT(path));
        }
        if (widget == NULL)
            rb_raise(rb_eArgError, "Invalid path %s was passed.", RVAL2CSTR(path));
    } else {
        int i;
        widget = gtk_tree_path_new();
        for (i = 0; i < argc; i++)
            gtk_tree_path_append_index(widget, NUM2INT(argv[i]));
    }
    
    G_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
treepath_to_string(self)
    VALUE self;
{
    gchar* s = gtk_tree_path_to_string(_SELF(self));
    VALUE result = CSTR2RVAL(s);
    g_free(s);
    return result;
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
    VALUE ary;
    gint i, length;
    gint* indices;

    length = gtk_tree_path_get_depth(_SELF(self));
    indices = gtk_tree_path_get_indices(_SELF(self));

    if (indices != NULL) {
        ary = rb_ary_new2(length);
        for (i = 0; i < length; i++) {
            rb_ary_push(ary, INT2NUM(indices[i]));
        }
    
        return ary;
    } else {
        return Qnil;
    }
}

static VALUE
treepath_compare(self, other)
    VALUE self, other;
{
    return INT2NUM(gtk_tree_path_compare(_SELF(self), _SELF(other)));
}

static VALUE
treepath_equal(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_funcall(CLASS_OF(self), id_equal, 1, CLASS_OF(other)))) {
        return Qfalse;
    }

    return CBOOL2RVAL(gtk_tree_path_compare(_SELF(self), _SELF(other)) == 0);
}

static VALUE
treepath_next(self)
    VALUE self;
{
    gtk_tree_path_next(_SELF(self));
    return self;
}

static VALUE
treepath_prev(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_tree_path_prev(_SELF(self)));
}

static VALUE
treepath_up(self)
    VALUE self;
{
    return CBOOL2RVAL(gtk_tree_path_up(_SELF(self)));
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
    return CBOOL2RVAL(gtk_tree_path_is_ancestor(_SELF(self), _SELF(descendant)));
}

static VALUE
treepath_is_descendant(self, ancestor)
    VALUE self, ancestor;
{
    return CBOOL2RVAL(gtk_tree_path_is_descendant(_SELF(self), _SELF(ancestor)));
}

void 
Init_gtk_treepath()
{
    VALUE gTreepath = G_DEF_CLASS(GTK_TYPE_TREE_PATH, "TreePath", mGtk);

    id_equal = rb_intern("==");

    rb_define_method(gTreepath, "initialize", treepath_initialize, -1);
    rb_define_method(gTreepath, "to_str", treepath_to_string, 0);
    rb_define_alias(gTreepath, "to_s", "to_str");
    rb_define_method(gTreepath, "append_index", treepath_append_index, 1);
    rb_define_method(gTreepath, "prepend_index", treepath_prepend_index, 1);
    rb_define_method(gTreepath, "depth", treepath_get_depth, 0);
    rb_define_method(gTreepath, "indices", treepath_get_indices, 0);
    rb_define_method(gTreepath, "<=>", treepath_compare, 1);
    rb_define_method(gTreepath, "==", treepath_equal, 1);
    rb_define_method(gTreepath, "next!", treepath_next, 0);
    rb_define_method(gTreepath, "prev!", treepath_prev, 0);
    rb_define_method(gTreepath, "up!", treepath_up, 0);
    rb_define_method(gTreepath, "down!", treepath_down, 0);
    rb_define_method(gTreepath, "ancestor?", treepath_is_ancestor, 1);
    rb_define_method(gTreepath, "descendant?", treepath_is_descendant, 1);
}
