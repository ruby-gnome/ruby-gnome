/* $Id: rbgnome-canvas-item.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::CanvasItem widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

VALUE gnoCanvasItem;

static VALUE
citem_move(self, dx, dy)
    VALUE self, dx, dy;
{
    gnome_canvas_item_move(GNOME_CANVAS_ITEM(get_gobject(self)),
                           NUM2DBL(dx),
                           NUM2DBL(dy));
    return Qnil;
}

static VALUE
citem_affine_relative(self, ary)
    VALUE self, ary;
{
    int i;
    double* affine;
    Check_Type(ary, T_ARRAY);
    affine = ALLOCA_N(double, 6);
    for (i = 0; i < 6; ++i) {
        affine[i] = NUM2DBL(RARRAY(ary)->ptr[i]);
    }
    gnome_canvas_item_affine_relative(GNOME_CANVAS_ITEM(get_gobject(self)),
                                       affine);
    return Qnil;
}

static VALUE
citem_affine_absolute(self, ary)
    VALUE self, ary;
{
    int i;
    double* affine;
    Check_Type(ary, T_ARRAY);
    affine = ALLOCA_N(double, 6);
    for (i = 0; i < 6; ++i) {
        affine[i] = NUM2DBL(RARRAY(ary)->ptr[i]);
    }
    gnome_canvas_item_affine_absolute(GNOME_CANVAS_ITEM(get_gobject(self)),
                                      affine);
    return Qnil;
}

static VALUE
citem_scale(self, x, y, scale_x, scale_y)
    VALUE self, x, y, scale_x, scale_y;
{
    gnome_canvas_item_scale(GNOME_CANVAS_ITEM(get_gobject(self)),
                            NUM2DBL(x),
                            NUM2DBL(y),
                            NUM2DBL(scale_x),
                            NUM2DBL(scale_y));
    return Qnil;
}

static VALUE
citem_rotate(self, x, y, angle)
    VALUE self, x, y, angle;
{
    gnome_canvas_item_rotate(GNOME_CANVAS_ITEM(get_gobject(self)),
                             NUM2DBL(x),
                             NUM2DBL(y),
                             NUM2DBL(angle));
    return Qnil;
}

static VALUE
citem_raise(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_raise(GNOME_CANVAS_ITEM(get_gobject(self)),
                            NUM2INT(positions));
    return Qnil;
}

static VALUE
citem_lower(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_lower(GNOME_CANVAS_ITEM(get_gobject(self)),
                            NUM2INT(positions));
    return Qnil;
}

static VALUE
citem_raise_to_top(self)
    VALUE self;
{
    gnome_canvas_item_raise_to_top(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_lower_to_bottom(self)
    VALUE self;
{
    gnome_canvas_item_lower_to_bottom(
            GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_show(self)
    VALUE self;
{
    gnome_canvas_item_show(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_hide(self)
    VALUE self;
{
    gnome_canvas_item_hide(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_grab(self, event_mask, cursor, etime)
    VALUE self, event_mask, cursor, etime;
{
    return NUM2INT(
            gnome_canvas_item_grab(GNOME_CANVAS_ITEM(get_gobject(self)),
                                   NUM2INT(event_mask),
                                   NIL_P(cursor) ? get_gdkcursor(cursor) : NULL,
                                   NIL_P(etime)  ? NUM2INT(etime) : 0));
}

static VALUE
citem_grab_focus(self)
    VALUE self;
{
    gnome_canvas_item_grab_focus(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

static VALUE
citem_get_bounds(self)
    VALUE self;
{
    double x1, y1, x2, y2;
    gnome_canvas_item_get_bounds(GNOME_CANVAS_ITEM(get_gobject(self)),
                                 &x1,
                                 &y1,
                                 &x2,
                                 &y2);
    return rb_ary_new3(4, rb_float_new(x1), rb_float_new(y1),
                          rb_float_new(x2), rb_float_new(y2));
}

static VALUE
citem_request_update(self)
    VALUE self;
{
    gnome_canvas_item_request_update(GNOME_CANVAS_ITEM(get_gobject(self)));
    return Qnil;
}

void
Init_gnome_canvas_item()
{
    gnoCanvasItem = rb_define_class_under(mGnome, "CanvasItem", gObject);
    rb_define_method(gnoCanvasItem, "move",
                     citem_move, 2);
    rb_define_method(gnoCanvasItem, "affine_relative",
                     citem_affine_relative, 1);
    rb_define_method(gnoCanvasItem, "affine_absolute",
                     citem_affine_absolute, 1);
    rb_define_method(gnoCanvasItem, "scale",
                     citem_scale, 4);
    rb_define_method(gnoCanvasItem, "rotate",
                     citem_rotate, 3);
    rb_define_method(gnoCanvasItem, "raise",
                     citem_raise, 1);
    rb_define_method(gnoCanvasItem, "lower",
                     citem_lower, 1);
    rb_define_method(gnoCanvasItem, "raise_to_top",
                     citem_raise_to_top, 0);
    rb_define_method(gnoCanvasItem, "lower_to_bottom",
                     citem_lower_to_bottom, 0);
    rb_define_method(gnoCanvasItem, "show",
                     citem_show, 0);
    rb_define_method(gnoCanvasItem, "hide",
                     citem_hide, 0);
    rb_define_method(gnoCanvasItem, "grab",
                     citem_grab, 3);
    rb_define_method(gnoCanvasItem, "grab_focus",
                     citem_grab_focus, 0);
    rb_define_method(gnoCanvasItem, "get_bounds",
                     citem_get_bounds, 0);
    rb_define_method(gnoCanvasItem, "request_update",
                     citem_request_update, 0);

    /* child init */
    Init_gnome_canvas_group();
}
