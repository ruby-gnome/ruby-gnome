/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE cRectangle

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_initialize(VALUE self, VALUE x1, VALUE y1, VALUE x2, VALUE y2)
{
    PopplerRectangle rectangle;

    rectangle.x1 = NUM2DBL(x1);
    rectangle.y1 = NUM2DBL(y1);
    rectangle.x2 = NUM2DBL(x2);
    rectangle.y2 = NUM2DBL(y2);

    G_INITIALIZE(self, &rectangle);
    return Qnil;
}

DEF_ACCESSOR(rectangle, x1, RVAL2POPPLERRECTANGLE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, y1, RVAL2POPPLERRECTANGLE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, x2, RVAL2POPPLERRECTANGLE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(rectangle, y2, RVAL2POPPLERRECTANGLE, rb_float_new, NUM2DBL)

static VALUE
rg_to_a(VALUE self)
{
    PopplerRectangle *rectangle = RVAL2POPPLERRECTANGLE(self);
    return rb_ary_new3(4,
                       rb_float_new(rectangle->x1),
                       rb_float_new(rectangle->y1),
                       rb_float_new(rectangle->x2),
                       rb_float_new(rectangle->y2));
}

static VALUE
rg_inspect(VALUE self)
{
    VALUE inspected;
    gchar *points;
    PopplerRectangle *rectangle;

    rectangle = RVAL2POPPLERRECTANGLE(self);
    inspected = rb_call_super(0, NULL);
    rb_str_resize(inspected, RSTRING_LEN(inspected) - 1);
    points = g_strdup_printf(": [%g, %g, %g, %g]>",
                             rectangle->x1, rectangle->y1,
                             rectangle->x2, rectangle->y2);
    rb_str_cat2(inspected, points);
    g_free(points);
    return inspected;
}

void
Init_poppler_rectangle(VALUE mPoppler)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_RECTANGLE, "Rectangle", mPoppler);

    RG_DEF_METHOD(initialize, 4);
    rbg_define_method(RG_TARGET_NAMESPACE, "x1", rectangle_get_x1, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "y1", rectangle_get_y1, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "x2", rectangle_get_x2, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "y2", rectangle_get_y2, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_x1", rectangle_set_x1, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_y1", rectangle_set_y1, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_x2", rectangle_set_x2, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_y2", rectangle_set_y2, 1);
    RG_DEF_METHOD(to_a, 0);
    RG_DEF_METHOD(inspect, 0);
}
