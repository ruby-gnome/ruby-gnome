/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkallocation.c -

  $Author: mutoh $
  $Date: 2004/03/05 16:24:30 $

  Copyright (C) 2002-2004  Masao Mutoh
************************************************/

#include "global.h"

static GtkAllocation *
gtk_allocation_copy(const GtkAllocation *alloc)
{
  GtkAllocation *result = g_new (GtkAllocation, 1);
  *result = *alloc;

  return result;
}

GType
gtk_allocation_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkAllocation",
                    (GBoxedCopyFunc)gtk_allocation_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}

#define _SELF(r) ((GtkAllocation*)RVAL2BOXED(r, GTK_TYPE_ALLOCATION))

static VALUE
alloc_initialize(self, x, y, width, height)
    VALUE self, x, y, width, height;
{
    GtkAllocation new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);

    G_INITIALIZE(self, &new);
    return Qnil;
}

static VALUE
alloc_intersect(self, other)
    VALUE self, other;
{
    GtkAllocation dest;
    gboolean ret = gdk_rectangle_intersect(_SELF(self), _SELF(other), &dest);
    return ret ? BOXED2RVAL(&dest, GTK_TYPE_ALLOCATION) : Qnil;
}

static VALUE
alloc_union(self, other)
    VALUE self, other;
{
    GtkAllocation dest;
    gdk_rectangle_union(_SELF(self), _SELF(other), &dest);
    return BOXED2RVAL(&dest, GTK_TYPE_ALLOCATION);
}

/* Struct accessors */
static VALUE
alloc_x(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
alloc_y(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
alloc_w(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
alloc_h(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
alloc_set_x(self, x)
    VALUE self, x;
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
alloc_set_y(self, y)
    VALUE self, y;
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}

static VALUE
alloc_set_w(self, width)
    VALUE self, width;
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
alloc_set_h(self, height)
    VALUE self, height;
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}

static VALUE
alloc_to_a(self)
    VALUE self;
{
  GtkAllocation* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}

static VALUE
alloc_to_rect(self)
    VALUE self;
{
    return BOXED2RVAL(_SELF(self), GDK_TYPE_RECTANGLE);
}

void 
Init_gtk_allocation()
{
    VALUE galloc = G_DEF_CLASS(GTK_TYPE_ALLOCATION, "Allocation", mGtk);

    rb_define_method(galloc, "initialize", alloc_initialize, 4);
    rb_define_method(galloc, "intersect", alloc_intersect, 1);
    rb_define_alias(galloc, "&", "intersect");
    rb_define_method(galloc, "union", alloc_union, 1);
    rb_define_alias(galloc, "|", "union");
    rb_define_method(galloc, "x", alloc_x, 0);
    rb_define_method(galloc, "y", alloc_y, 0);
    rb_define_method(galloc, "width", alloc_w, 0);
    rb_define_method(galloc, "height", alloc_h, 0);
    rb_define_method(galloc, "set_x", alloc_set_x, 1);
    rb_define_method(galloc, "set_y", alloc_set_y, 1);
    rb_define_method(galloc, "set_width", alloc_set_w, 1);
    rb_define_method(galloc, "set_height", alloc_set_h, 1);
    rb_define_method(galloc, "to_a", alloc_to_a, 0);
    rb_define_method(galloc, "to_rect", alloc_to_rect, 0);

    G_DEF_SETTERS(galloc);
}
