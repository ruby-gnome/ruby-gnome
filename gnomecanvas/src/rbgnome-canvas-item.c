/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas-item.c,v 1.3 2002/09/21 05:35:01 tkubo Exp $ */

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

#include "rbgnomecanvas.h"
#include "rbart.h"

#define _SELF(self) GNOME_CANVAS_ITEM(RVAL2GOBJ(self))

static ID id_set_property;

void
rbgnomecanvas_do_item_set(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    int i;
    if (argc % 1) {
        rb_raise(rb_eArgError, "mismatch the number of name - value pairs.");
    }
    for (i = 0; i < argc / 2; i++) {
        rb_funcall(self, id_set_property, 2, argv[i * 2], argv[i * 2 + 1]);
    }
}

static VALUE
citem_set(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    rbgnomecanvas_do_item_set(argc, argv, self);
    gnome_canvas_item_set(GNOME_CANVAS_ITEM(RVAL2GOBJ(self)), NULL);
    return Qnil;
}

static VALUE
citem_move(self, dx, dy)
    VALUE self, dx, dy;
{
    gnome_canvas_item_move(_SELF(self),
                           NUM2DBL(dx),
                           NUM2DBL(dy));
    return Qnil;
}

static VALUE
citem_affine_relative(self, affine)
    VALUE self, affine;
{
    gnome_canvas_item_affine_relative(_SELF(self),
                                      get_art_affine(affine));
    return Qnil;
}

static VALUE
citem_affine_absolute(self, affine)
    VALUE self, affine;
{
    gnome_canvas_item_affine_absolute(_SELF(self),
                                      get_art_affine(affine));
    return Qnil;
}

static VALUE
citem_raise(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_raise(_SELF(self),
                            NUM2INT(positions));
    return Qnil;
}

static VALUE
citem_lower(self, positions)
    VALUE self, positions;
{
    gnome_canvas_item_lower(_SELF(self),
                            NUM2INT(positions));
    return Qnil;
}

static VALUE
citem_raise_to_top(self)
    VALUE self;
{
    gnome_canvas_item_raise_to_top(_SELF(self));
    return Qnil;
}

static VALUE
citem_lower_to_bottom(self)
    VALUE self;
{
    gnome_canvas_item_lower_to_bottom(_SELF(self));
    return Qnil;
}

static VALUE
citem_show(self)
    VALUE self;
{
    gnome_canvas_item_show(_SELF(self));
    return Qnil;
}

static VALUE
citem_hide(self)
    VALUE self;
{
    gnome_canvas_item_hide(_SELF(self));
    return Qnil;
}

static VALUE
citem_grab(self, event_mask, cursor, etime)
    VALUE self, event_mask, cursor, etime;
{
    return NUM2INT(
        gnome_canvas_item_grab(_SELF(self),
                               NUM2INT(event_mask),
                               RVAL2BOXED(cursor),
                               NIL_P(etime) ? 0 : NUM2INT(etime)));
}

static VALUE
citem_ungrab(self, etime)
    VALUE self, etime;
{
    gnome_canvas_item_ungrab(_SELF(self),
                             NIL_P(etime) ? 0 : NUM2INT(etime));
    return Qnil;
}

static VALUE
citem_w2i(self, x, y)
    VALUE self, x, y;
{
    double dx = NUM2DBL(x), dy = NUM2DBL(y);
    gnome_canvas_item_w2i(_SELF(self), &dx, &dy);
    return rb_ary_new3(2, rb_float_new(dx), rb_float_new(dy));
}

static VALUE
citem_i2w(self, x, y)
    VALUE self, x, y;
{
    double dx = NUM2DBL(x), dy = NUM2DBL(y);
    gnome_canvas_item_i2w(_SELF(self), &dx, &dy);
    return rb_ary_new3(2, rb_float_new(dx), rb_float_new(dy));
}

static VALUE
citem_i2c_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_item_i2c_affine(_SELF(self), affine);
    return make_art_affine(affine);
}

static VALUE
citem_i2w_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_item_i2w_affine(_SELF(self), affine);
    return make_art_affine(affine);
}

static VALUE
citem_reparent(self, new_group)
    VALUE self, new_group;
{
    if (!g_type_is_a(RVAL2GTYPE(new_group), GNOME_TYPE_CANVAS_GROUP)) {
        rb_raise(rb_eTypeError, "not a GnomeCanvasGroup");
    }
    gnome_canvas_item_reparent(_SELF(self), GNOME_CANVAS_GROUP(RVAL2GOBJ(new_group)));
    return Qnil;
}

static VALUE
citem_grab_focus(self)
    VALUE self;
{
    gnome_canvas_item_grab_focus(_SELF(self));
    return Qnil;
}

static VALUE
citem_get_bounds(self)
    VALUE self;
{
    double x1, y1, x2, y2;
    gnome_canvas_item_get_bounds(_SELF(self),
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
    gnome_canvas_item_request_update(_SELF(self));
    return Qnil;
}

static VALUE
citem_parent(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_OBJECT(_SELF(self)->parent));
}

static VALUE
citem_canvas(self)
    VALUE self;
{
    return GOBJ2RVAL(GTK_OBJECT(_SELF(self)->canvas));
}

void
Init_gnome_canvas_item(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvasItem = G_DEF_CLASS(GNOME_TYPE_CANVAS_ITEM, "CanvasItem", mGnome);
    id_set_property = rb_intern("set_property");

    rb_undef_method(gnoCanvasItem, "new");
    rb_define_method(gnoCanvasItem, "set", citem_set, -1);
    rb_define_method(gnoCanvasItem, "move", citem_move, 2);
    rb_define_method(gnoCanvasItem, "affine_relative", citem_affine_relative, 1);
    rb_define_method(gnoCanvasItem, "affine_absolute", citem_affine_absolute, 1);
    rb_define_method(gnoCanvasItem, "raise", citem_raise, 1);
    rb_define_method(gnoCanvasItem, "lower", citem_lower, 1);
    rb_define_method(gnoCanvasItem, "raise_to_top", citem_raise_to_top, 0);
    rb_define_method(gnoCanvasItem, "lower_to_bottom", citem_lower_to_bottom, 0);
    rb_define_method(gnoCanvasItem, "show", citem_show, 0);
    rb_define_method(gnoCanvasItem, "hide", citem_hide, 0);
    rb_define_method(gnoCanvasItem, "grab", citem_grab, 3);
    rb_define_method(gnoCanvasItem, "ungrab", citem_ungrab, 1);
    rb_define_method(gnoCanvasItem, "w2i", citem_w2i, 2);
    rb_define_method(gnoCanvasItem, "i2w", citem_i2w, 2);
    rb_define_method(gnoCanvasItem, "i2w_affine", citem_i2w_affine, 0);
    rb_define_method(gnoCanvasItem, "i2c_affine", citem_i2c_affine, 0);
    rb_define_method(gnoCanvasItem, "reparent", citem_reparent, 1);
    rb_define_method(gnoCanvasItem, "grab_focus", citem_grab_focus, 0);
    rb_define_method(gnoCanvasItem, "get_bounds", citem_get_bounds, 0);
    rb_define_method(gnoCanvasItem, "request_update", citem_request_update, 0);
    rb_define_method(gnoCanvasItem, "parent", citem_parent, 0);
    rb_define_method(gnoCanvasItem, "canvas", citem_canvas, 0);

    G_DEF_CLASS(GNOME_TYPE_CANVAS_BPATH, "CanvasBpath", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_LINE, "CanvasLine", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_PIXBUF, "CanvasPixbuf", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_POLYGON, "CanvasPolygon", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_RECT, "CanvasRect", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_ELLIPSE, "CanvasEllipse", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_RICH_TEXT, "CanvasRichText", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_TEXT, "CanvasText", mGnome);
    G_DEF_CLASS(GNOME_TYPE_CANVAS_WIDGET, "CanvasWidget", mGnome);
}
