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

#include "rbgoocanvasprivate.h"

GooCanvasBounds *
ruby_to_goo_canvas_bounds(VALUE rb_bounds, GooCanvasBounds *dest_bounds)
{
    if (rb_bounds == Qnil)
        return NULL;

    if (TYPE(rb_bounds) != T_ARRAY) {
        VALUE rb_array;
        rb_array = rb_funcall(rb_bounds, rb_intern("to_a"), 0);
        return ruby_to_goo_canvas_bounds(rb_array, dest_bounds);
    }
    
    if (RARRAY_LEN(rb_bounds) != 4)
        rb_raise(rb_eRuntimeError, "Bounds must be arrays of length 4");
    
    dest_bounds->x1 = NUM2DBL(RARRAY_PTR(rb_bounds)[0]);
    dest_bounds->y1 = NUM2DBL(RARRAY_PTR(rb_bounds)[1]);
    dest_bounds->x2 = NUM2DBL(RARRAY_PTR(rb_bounds)[2]);
    dest_bounds->y2 = NUM2DBL(RARRAY_PTR(rb_bounds)[3]);

    return dest_bounds;
}
