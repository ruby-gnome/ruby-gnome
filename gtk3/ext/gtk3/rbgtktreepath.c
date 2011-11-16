/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2004 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTreePath
#define _SELF(s) ((GtkTreePath*)RVAL2BOXED(s, GTK_TYPE_TREE_PATH))

static ID id_equal;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
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
rg_to_str(VALUE self)
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
rg_append_index(VALUE self, VALUE index)
{
    gtk_tree_path_append_index(_SELF(self), NUM2INT(index));
    return self;
}

static VALUE
rg_prepend_index(VALUE self, VALUE index)
{
    gtk_tree_path_prepend_index(_SELF(self), NUM2INT(index));
    return self;
}

static VALUE
rg_depth(VALUE self)
{
    return INT2NUM(gtk_tree_path_get_depth(_SELF(self)));
}

static VALUE
rg_indices(VALUE self)
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
rg_operator_treepath_compare(VALUE self, VALUE other)
{
    return INT2NUM(gtk_tree_path_compare(_SELF(self), _SELF(other)));
}

static VALUE
rg_operator_treepath_equal(VALUE self, VALUE other)
{
    if (!RVAL2CBOOL(rb_funcall(CLASS_OF(self), id_equal, 1, CLASS_OF(other)))) {
        return Qfalse;
    }

    return CBOOL2RVAL(gtk_tree_path_compare(_SELF(self), _SELF(other)) == 0);
}

static VALUE
rg_next_bang(VALUE self)
{
    gtk_tree_path_next(_SELF(self));
    return self;
}

static VALUE
rg_prev_bang(VALUE self)
{
    return CBOOL2RVAL(gtk_tree_path_prev(_SELF(self)));
}

static VALUE
rg_up_bang(VALUE self)
{
    return CBOOL2RVAL(gtk_tree_path_up(_SELF(self)));
}

static VALUE
rg_down_bang(VALUE self)
{
    gtk_tree_path_down(_SELF(self));
    return Qtrue;
}

static VALUE
rg_ancestor_p(VALUE self, VALUE descendant)
{
    return CBOOL2RVAL(gtk_tree_path_is_ancestor(_SELF(self), _SELF(descendant)));
}

static VALUE
rg_descendant_p(VALUE self, VALUE ancestor)
{
    return CBOOL2RVAL(gtk_tree_path_is_descendant(_SELF(self), _SELF(ancestor)));
}

void 
Init_gtk_treepath(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TREE_PATH, "TreePath", mGtk);

    id_equal = rb_intern("==");

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(to_str, 0);
    RG_DEF_ALIAS("to_s", "to_str");
    RG_DEF_METHOD(append_index, 1);
    RG_DEF_METHOD(prepend_index, 1);
    RG_DEF_METHOD(depth, 0);
    RG_DEF_METHOD(indices, 0);
    RG_DEF_METHOD_OPERATOR("<=>", treepath_compare, 1);
    RG_DEF_METHOD_OPERATOR("==", treepath_equal, 1);
    RG_DEF_METHOD_BANG(next, 0);
    RG_DEF_METHOD_BANG(prev, 0);
    RG_DEF_METHOD_BANG(up, 0);
    RG_DEF_METHOD_BANG(down, 0);
    RG_DEF_METHOD_P(ancestor, 1);
    RG_DEF_METHOD_P(descendant, 1);
}
