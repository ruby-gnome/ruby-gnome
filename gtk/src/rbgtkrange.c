/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkrange.c -

  $Author: mutoh $
  $Date: 2002/10/25 17:51:25 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_RANGE(RVAL2GOBJ(self)))

static VALUE
range_get_value(self)
    VALUE self;
{
    return rb_float_new(gtk_range_get_value(_SELF(self)));
}

static VALUE
range_set_increments(self, step, page)
    VALUE self, step, page;
{
    gtk_range_set_increments(_SELF(self), NUM2DBL(step), NUM2DBL(page));
    return self;
}

static VALUE
range_set_range(self, min, max)
    VALUE self, min, max;
{
    gtk_range_set_range(_SELF(self), NUM2DBL(min), NUM2DBL(max));
    return self;
}

static VALUE
range_set_value(self, value)
    VALUE self, value;
{
    gtk_range_set_value(_SELF(self), NUM2DBL(value));
    return self;
}

void 
Init_gtk_range()
{
  VALUE gRange = G_DEF_CLASS(GTK_TYPE_RANGE, "Range", mGtk);

  rb_define_method(gRange, "value", range_get_value, 0);
  rb_define_method(gRange, "set_increments", range_set_increments, 2);
  rb_define_method(gRange, "set_range", range_set_range, 2);
  rb_define_method(gRange, "set_value", range_set_value, 1);

  G_DEF_SETTERS(gRange);
}
