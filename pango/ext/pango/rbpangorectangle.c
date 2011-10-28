/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#include "rbpangoprivate.h"

#define RG_TARGET_NAMESPACE cRectangle
#define _SELF(s) ((PangoRectangle*)RVAL2BOXED(s, PANGO_TYPE_RECTANGLE))

/**********************************/
static PangoRectangle*
pango_rectangle_copy(const PangoRectangle *ref)
{
  PangoRectangle* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(PangoRectangle, 1);
  *new_ref = *ref;
  return new_ref;
}

GType
pango_rectangle_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoRectangle",
                    (GBoxedCopyFunc)pango_rectangle_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
pango_rectangle_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    PangoRectangle new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);

    G_INITIALIZE(self, &new);
    return Qnil;
}


static VALUE
pango_rectangle_x(VALUE self)
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
pango_rectangle_y(VALUE self)
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
pango_rectangle_w(VALUE self)
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
pango_rectangle_h(VALUE self)
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
pango_rectangle_set_x(VALUE self, VALUE x)
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
pango_rectangle_set_y(VALUE self, VALUE y)
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}
static VALUE
pango_rectangle_set_w(VALUE self, VALUE width)
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
pango_rectangle_set_h(VALUE self, VALUE height)
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}
static VALUE
pango_rectangle_to_a(VALUE self)
{
  PangoRectangle* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}

static VALUE
pango_rectangle_ascent(VALUE self)
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_ASCENT(*r));
}

static VALUE
pango_rectangle_descent(VALUE self)
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_DESCENT(*r));
}

static VALUE
pango_rectangle_lbearing(VALUE self)
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_LBEARING(*r));
}

static VALUE
pango_rectangle_rbearing(VALUE self)
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_RBEARING(*r));
}


void
Init_pango_rectangle(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_RECTANGLE, "Rectangle", mPango);
    rb_define_method(RG_TARGET_NAMESPACE, "initialize", pango_rectangle_initialize, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "x", pango_rectangle_x, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "y", pango_rectangle_y, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "width", pango_rectangle_w, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "height", pango_rectangle_h, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_x", pango_rectangle_set_x, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_y", pango_rectangle_set_y, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_width", pango_rectangle_set_w, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_height", pango_rectangle_set_h, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "to_a", pango_rectangle_to_a, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "ascent", pango_rectangle_ascent, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "descent", pango_rectangle_descent, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "lbearing", pango_rectangle_lbearing, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "rbearing", pango_rectangle_rbearing, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
