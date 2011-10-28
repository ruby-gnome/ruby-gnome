/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cRectangle
#define _SELF(r) ((GdkRectangle*)RVAL2BOXED(r, GDK_TYPE_RECTANGLE))

static VALUE
gdkrect_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    GdkRectangle new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);

    G_INITIALIZE(self, &new);
    return Qnil;
}

static VALUE
gdkrect_intersect(VALUE self, VALUE other)
{
    GdkRectangle dest;
    gboolean ret = gdk_rectangle_intersect(_SELF(self), _SELF(other), &dest);
    return ret ? BOXED2RVAL(&dest, GDK_TYPE_RECTANGLE) : Qnil;
}

static VALUE
gdkrect_union(VALUE self, VALUE other)
{
    GdkRectangle dest;
    gdk_rectangle_union(_SELF(self), _SELF(other), &dest);
    return BOXED2RVAL(&dest, GDK_TYPE_RECTANGLE);
}

/* Struct accessors */
static VALUE
gdkrect_x(VALUE self)
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
gdkrect_y(VALUE self)
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
gdkrect_w(VALUE self)
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
gdkrect_h(VALUE self)
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
gdkrect_set_x(VALUE self, VALUE x)
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
gdkrect_set_y(VALUE self, VALUE y)
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}

static VALUE
gdkrect_set_w(VALUE self, VALUE width)
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
gdkrect_set_h(VALUE self, VALUE height)
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}

static VALUE
gdkrect_to_a(VALUE self)
{
  GdkRectangle* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}

void
Init_gtk_gdk_rectangle(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_RECTANGLE, "Rectangle", mGdk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", gdkrect_initialize, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "intersect", gdkrect_intersect, 1);
    rb_define_alias(RG_TARGET_NAMESPACE, "&", "intersect");
    rb_define_method(RG_TARGET_NAMESPACE, "union", gdkrect_union, 1);
    rb_define_alias(RG_TARGET_NAMESPACE, "|", "union");
    rb_define_method(RG_TARGET_NAMESPACE, "x", gdkrect_x, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "y", gdkrect_y, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "width", gdkrect_w, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "height", gdkrect_h, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_x", gdkrect_set_x, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_y", gdkrect_set_y, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_width", gdkrect_set_w, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_height", gdkrect_set_h, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "to_a", gdkrect_to_a, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
