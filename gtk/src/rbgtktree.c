/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktree.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#ifdef GTK_ENABLE_BROKEN

static VALUE
tree_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_tree_new());
    return Qnil;
}

static VALUE
tree_append(self, child)
    VALUE self, child;
{
    gtk_tree_append(GTK_TREE(RVAL2GOBJ(self)), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
tree_prepend(self, child)
    VALUE self, child;
{
    gtk_tree_prepend(GTK_TREE(RVAL2GOBJ(self)), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
tree_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_tree_insert(GTK_TREE(RVAL2GOBJ(self)), GTK_WIDGET(RVAL2GOBJ(child)),
		    NUM2INT(pos));
    return self;
}

static VALUE
tree_set_sel_mode(self, mode)
    VALUE self, mode;
{
    gtk_tree_set_selection_mode(GTK_TREE(RVAL2GOBJ(self)),
								(GtkSelectionMode)NUM2INT(mode));
    return self;
}

static VALUE
tree_set_view_lines(self, lines)
    VALUE self, lines;
{
    gtk_tree_set_view_lines(GTK_TREE(RVAL2GOBJ(self)),
			    RTEST(lines));
    return self;
}

static VALUE
tree_set_view_mode(self, mode)
    VALUE self, mode;
{
    gtk_tree_set_selection_mode(GTK_TREE(RVAL2GOBJ(self)),
				NUM2INT(mode));
    return self;
}

static VALUE
tree_child_position(self, child)
    VALUE self, child;
{
    return INT2NUM(gtk_tree_child_position(GTK_TREE(RVAL2GOBJ(self)),
					   GTK_WIDGET(RVAL2GOBJ(child))));
}

static VALUE
tree_clear_items(self, start, end)
    VALUE self, start, end;
{
    gtk_tree_clear_items(GTK_TREE(RVAL2GOBJ(self)),
			 NUM2INT(start), NUM2INT(end));
    return self;
}

static VALUE
tree_is_root_tree_p(self)
    VALUE self;
{
    return GTK_IS_ROOT_TREE(GTK_TREE(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

static VALUE
tree_remove_item(self, child)
    VALUE self, child;
{
    gtk_tree_remove_item(GTK_TREE(RVAL2GOBJ(self)), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

void 
Init_gtk_tree() /* GTK_ENABLE_BROKEN */
{
    VALUE gTree = G_DEF_CLASS(GTK_TYPE_TREE, "Tree", mGtk);

    rb_define_const(gTree, "VIEW_LINE", INT2FIX(GTK_TREE_VIEW_LINE));
    rb_define_const(gTree, "VIEW_ITEM", INT2FIX(GTK_TREE_VIEW_ITEM));;

    rb_define_method(gTree, "initialize", tree_initialize, 0);
    rb_define_method(gTree, "append", tree_append, 1);
    rb_define_method(gTree, "prepend", tree_prepend, 1);
    rb_define_method(gTree, "insert", tree_insert, 2);
    rb_define_method(gTree, "set_selection_mode", tree_set_sel_mode, 1);
    rb_define_method(gTree, "set_view_lines", tree_set_view_lines, 1);
    rb_define_method(gTree, "set_view_mode", tree_set_view_mode, 1);
    rb_define_method(gTree, "child_position", tree_child_position, 1);
    rb_define_method(gTree, "clear_items", tree_clear_items, 2);
    rb_define_method(gTree, "is_root_tree?", tree_is_root_tree_p, 0);
    rb_define_method(gTree, "remove_item", tree_remove_item, 1);
    /* rb_define_method(gTree, "selection", tree_selection, 0); */
}
#endif
