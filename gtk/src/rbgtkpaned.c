/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkpaned.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_PANED(RVAL2GOBJ(self)))
#define RVAL2WIDGET(w) (GTK_WIDGET(RVAL2GOBJ(w)))


static VALUE
paned_add1(self, child)
    VALUE self, child;
{
    gtk_paned_add1(_SELF(self), RVAL2WIDGET(child));
    return self;
}

static VALUE
paned_add2(self, child)
    VALUE self, child;
{
    gtk_paned_add2(_SELF(self), RVAL2WIDGET(child));
    return self;
}

static VALUE
paned_pack1(self, child, resize, shrink)
    VALUE self, child, resize, shrink;
{
    gtk_paned_pack1(_SELF(self), RVAL2WIDGET(child),
                    RTEST(resize), RTEST(shrink));
    return self;
}

static VALUE
paned_pack2(self, child, resize, shrink)
    VALUE self, child, resize, shrink;
{
    gtk_paned_pack2(_SELF(self), RVAL2WIDGET(child),
                    RTEST(resize), RTEST(shrink));
    return self;
}

static VALUE
paned_child1(self)
    VALUE self;
{
    GtkWidget *child = _SELF(self)->child1;
    return (child == NULL) ? Qnil : GOBJ2RVAL(child);
}

static VALUE
paned_child2(self)
    VALUE self;
{
    GtkWidget *child = _SELF(self)->child2;
    return (child == NULL) ? Qnil : GOBJ2RVAL(child);
}

static VALUE
paned_child1_resize(self)
    VALUE self;
{
    return _SELF(self)->child1_resize ? Qtrue : Qfalse;
}

static VALUE
paned_child1_shrink(self)
    VALUE self;
{
    return _SELF(self)->child1_shrink ? Qtrue : Qfalse;
}

static VALUE
paned_child2_resize(self)
    VALUE self;
{
    return _SELF(self)->child2_resize ? Qtrue : Qfalse;
}

static VALUE
paned_child2_shrink(self)
    VALUE self;
{
    return _SELF(self)->child2_shrink ? Qtrue : Qfalse;
}

void 
Init_gtk_paned()
{
    VALUE gPaned = G_DEF_CLASS(GTK_TYPE_PANED, "Paned", mGtk);

    rb_define_method(gPaned, "add1", paned_add1, 1);
    rb_define_method(gPaned, "add2", paned_add2, 1);
    rb_define_method(gPaned, "pack1", paned_pack1, 3);
    rb_define_method(gPaned, "pack2", paned_pack2, 3);
    rb_define_method(gPaned, "child1", paned_child1, 0);
    rb_define_method(gPaned, "child2", paned_child2, 0);
    rb_define_method(gPaned, "child1_resize?", paned_child1_resize, 0);
    rb_define_method(gPaned, "child1_shrink?", paned_child1_shrink, 0);
    rb_define_method(gPaned, "child2_resize?", paned_child2_resize, 0);
    rb_define_method(gPaned, "child2_shrink?", paned_child2_shrink, 0);
}
