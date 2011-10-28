/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Vincent Isambart <vincent.isambart@gmail.com>
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

#include "rbgoocanvas.h"

#define RG_TARGET_NAMESPACE cCanvasPoints

static VALUE
rb_goo_canvas_points_new(VALUE self, VALUE num_points)
{
    G_INITIALIZE(self, goo_canvas_points_new(NUM2INT(num_points)));
    return Qnil;
}

static VALUE
rb_goo_canvas_points_get(VALUE self, VALUE point)
{
    int i;
    GooCanvasPoints *points;
    
    i = NUM2INT(point);
    points = RVAL2GCPOINTS(self);
    if ((i < 0) || (i >= points->num_points))
        rb_raise(rb_eArgError, "invalid point number %d", i);
    return rb_ary_new3(2,
                       rb_float_new(points->coords[i * 2]),
                       rb_float_new(points->coords[i * 2 + 1]));
}

static VALUE
rb_goo_canvas_points_set(VALUE self, VALUE point, VALUE new_coords)
{
    int i;
    GooCanvasPoints *points;
    
    i = NUM2INT(point);
    points = RVAL2GCPOINTS(self);
    if ((i < 0) || (i >= points->num_points))
        rb_raise(rb_eArgError, "invalid point number %d", i);
    if (TYPE(new_coords) != T_ARRAY)
        rb_raise(rb_eArgError, "rb_goo_canvas_points_set should be given an array as new value");
    if (RARRAY_LEN(new_coords) != 2)
        rb_raise(rb_eArgError, "rb_goo_canvas_points_set should be given an array of length 2 as new value");
    points->coords[i*2] = NUM2DBL(RARRAY_PTR(new_coords)[0]);
    points->coords[i*2+1] = NUM2DBL(RARRAY_PTR(new_coords)[1]);
    return self;
}

static VALUE
rb_goo_canvas_points_get_num_points(VALUE self)
{
    GooCanvasPoints *points;
    
    points = RVAL2GCPOINTS(self);
    return INT2NUM(points->num_points);
}

void
Init_goocanvaspoints(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GOO_TYPE_CANVAS_POINTS, "CanvasPoints", mGoo);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", rb_goo_canvas_points_new, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "[]", rb_goo_canvas_points_get, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "[]=", rb_goo_canvas_points_set, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "num_points",
                     rb_goo_canvas_points_get_num_points, 0);
}
