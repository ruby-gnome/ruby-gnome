/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkallocation.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 2002  Masao Mutoh

  This file is derived from rbgtk.c.
  rbgtk.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
gallocation_initialize(self, x, y, w, h)
    VALUE self, x, y, w, h;
{
    GtkAllocation a;

    a.x = NUM2INT(x);
    a.y = NUM2INT(y);
    a.width = NUM2INT(w);
    a.height = NUM2INT(h);
    RBGTK_INITIALIZE(self, make_gallocation(&a));
	return Qnil;
}

static VALUE
gallocation_x(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->x);
}

static VALUE
gallocation_y(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->y);
}

static VALUE
gallocation_w(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->width);
}

static VALUE
gallocation_h(self)
    VALUE self;
{
    return INT2NUM(get_gallocation(self)->height);
}

static VALUE
gallocation_to_a(self)
    VALUE self;
{
    GtkAllocation *a;

    a = get_gallocation(self);
    return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
					   INT2FIX(a->width), INT2FIX(a->height));
}

static VALUE
gallocation_to_s(self)
    VALUE self;
{
    char str[2 +2*3 +5*4  +1]; /* member is guint16. max string size is 5 */
    GtkAllocation *a;

    a = get_gallocation(self);
    sprintf(str, "(%5d, %5d, %5d, %5d)", a->x, a->y, a->width, a->height);
    return rb_str_new2(str);
}

void 
Init_gtk_allocation()
{
	gAllocation = rb_define_class_under(mGtk, "Allocation", rb_cData);

    rb_define_singleton_method(gAllocation, "initialize", gallocation_initialize, 4);
    rb_define_method(gAllocation, "x", gallocation_x, 0);
    rb_define_method(gAllocation, "y", gallocation_y, 0);
    rb_define_method(gAllocation, "width", gallocation_w, 0);
    rb_define_method(gAllocation, "height", gallocation_h, 0);
    rb_define_method(gAllocation, "to_a", gallocation_to_a, 0);
    rb_define_method(gAllocation, "to_s", gallocation_to_s, 0);
}
