/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas.c,v 1.2 2002/09/20 16:22:16 tkubo Exp $ */

/* Gnome::Canvas widget for Ruby/Gnome
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
#include "rbart.h"

VALUE gnoCanvas;

static VALUE
canvas_s_new(klass)
    VALUE klass;
{
    return make_widget(klass, gnome_canvas_new());
}

static VALUE
canvas_s_new_aa(klass)
    VALUE klass;
{
    return make_widget(klass, gnome_canvas_new_aa());
}

static VALUE
canvas_root(self)
    VALUE self;
{
    GnomeCanvasGroup* cg = gnome_canvas_root(GNOME_CANVAS(get_widget(self)));
    return get_value_from_gno_obj(GTK_OBJECT(cg));
}

static VALUE
canvas_set_scroll_region(self, x1, y1, x2, y2)
    VALUE self, x1, y1, x2, y2;
{
    gnome_canvas_set_scroll_region(GNOME_CANVAS(get_widget(self)),
                                   NUM2DBL(x1),
                                   NUM2DBL(y1),
                                   NUM2DBL(x2),
                                   NUM2DBL(y2));
    return Qnil;
}

static VALUE
canvas_get_scroll_region(self)
    VALUE self;
{
    double x1, y1, x2, y2;
    gnome_canvas_get_scroll_region(GNOME_CANVAS(get_widget(self)),
                                   &x1,
                                   &y1,
                                   &x2,
                                   &y2);
    return rb_ary_new3(4, rb_float_new(x1), rb_float_new(y1),
                       rb_float_new(x2), rb_float_new(y2));
}

static VALUE
canvas_set_pixels_per_unit(self, n)
    VALUE self, n;
{
    gnome_canvas_set_pixels_per_unit(GNOME_CANVAS(get_widget(self)),
                                     NUM2DBL(n));
    return Qnil;
}

static VALUE
canvas_scroll_to(self, cx, cy)
    VALUE self, cx, cy;
{
    gnome_canvas_scroll_to(GNOME_CANVAS(get_widget(self)),
                           NUM2INT(cx),
                           NUM2INT(cy));
    return Qnil;
}

static VALUE
canvas_get_scroll_offsets(self)
    VALUE self;
{
    int cx, cy;
    gnome_canvas_get_scroll_offsets(GNOME_CANVAS(get_widget(self)),
                                    &cx,
                                    &cy);
    return rb_ary_new3(2, INT2NUM(cx), INT2NUM(cy));
}

static VALUE
canvas_update_now(self)
    VALUE self;
{
    gnome_canvas_update_now(GNOME_CANVAS(get_widget(self)));
    return Qnil;
}

static VALUE
canvas_get_item_at(self, x, y)
    VALUE self, x, y;
{
    GnomeCanvasItem* item;
    item = gnome_canvas_get_item_at(GNOME_CANVAS(get_widget(self)),
                                    NUM2DBL(x),
                                    NUM2DBL(y));
    return make_gnobject_auto_type(GTK_OBJECT(item));
}

static VALUE
canvas_request_redraw_uta(self, uta)
    VALUE self, uta;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

static VALUE
canvas_request_redraw(self, x1, y1, x2, y2)
    VALUE self, x1, y1, x2, y2;
{
    gnome_canvas_request_redraw(GNOME_CANVAS(get_widget(self)),
                                NUM2INT(x1),
                                NUM2INT(y1),
                                NUM2INT(x2),
                                NUM2INT(y2));
    return Qnil;
}

static VALUE
canvas_w2c_affine(self)
    VALUE self;
{
    double affine[6];
    gnome_canvas_w2c_affine(GNOME_CANVAS(get_widget(self)), affine);
    return make_art_affine(affine);
}

static VALUE
canvas_w2c(self, wx, wy)
    VALUE self, wx, wy;
{
    double cx, cy;
    gnome_canvas_w2c_d(GNOME_CANVAS(get_widget(self)),
                       NUM2DBL(wx),
                       NUM2DBL(wy),
                       &cx,
                       &cy);
    return rb_ary_new3(2, rb_float_new(cx), rb_float_new(cy));
}

static VALUE
canvas_c2w(self, cx, cy)
    VALUE self, cx, cy;
{
    double wx, wy;
    gnome_canvas_c2w(GNOME_CANVAS(get_widget(self)),
                     NUM2INT(cx),
                     NUM2INT(cy),
                     &wx,
                     &wy);
    return rb_ary_new3(2, rb_float_new(wx), rb_float_new(wy));
}

static VALUE
canvas_window_to_world(self, winx, winy)
    VALUE self, winx, winy;
{
    double worldx, worldy;
    gnome_canvas_window_to_world(GNOME_CANVAS(get_widget(self)),
                                 NUM2DBL(winx),
                                 NUM2DBL(winy),
                                 &worldx,
                                 &worldy);
    return rb_ary_new3(2, rb_float_new(worldx), rb_float_new(worldy));
}

static VALUE
canvas_world_to_window(self, worldx, worldy)
    VALUE self, worldx, worldy;
{
    double winx, winy;
    gnome_canvas_world_to_window(GNOME_CANVAS(get_widget(self)),
                                 NUM2DBL(worldx),
                                 NUM2DBL(worldy),
                                 &winx,
                                 &winy);
    return rb_ary_new3(2, rb_float_new(winx), rb_float_new(winy));
}

static VALUE
canvas_get_color(self, spec, color)
    VALUE self, spec, color;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

static VALUE
canvas_set_stipple_origin(self, gc)
    VALUE self, gc;
{
    gnome_canvas_set_stipple_origin(GNOME_CANVAS(get_widget(self)),
                                    get_gdkgc(gc));
    return Qnil;
}

static VALUE
canvas_set_dither(self, dither)
    VALUE self, dither;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

static VALUE
canvas_get_dither(self)
    VALUE self;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

static VALUE
canvas_get_aa(self)
    VALUE self;
{
    return GNOME_CANVAS(get_widget(self))->aa ? Qtrue : Qfalse;
}

void
Init_gnome_canvas()
{
    gnoCanvas = rb_define_class_under(mGnome, "Canvas", gLayout);

    rb_define_singleton_method(gnoCanvas, "new", canvas_s_new, 0);
    rb_define_singleton_method(gnoCanvas, "new_aa", canvas_s_new_aa, 0);
    rb_define_method(gnoCanvas, "root", canvas_root, 0);
    rb_define_method(gnoCanvas, "set_scroll_region", canvas_set_scroll_region, 4);
    rb_define_method(gnoCanvas, "get_scroll_region", canvas_get_scroll_region, 0);
    rb_define_method(gnoCanvas, "set_pixels_per_unit", canvas_set_pixels_per_unit, 1);
    rb_define_method(gnoCanvas, "scroll_to", canvas_scroll_to, 2);
    rb_define_method(gnoCanvas, "get_scroll_offsets", canvas_get_scroll_offsets, 0);
    rb_define_method(gnoCanvas, "update_now", canvas_update_now, 0);
    rb_define_method(gnoCanvas, "get_item_at", canvas_get_item_at, 2);
    rb_define_method(gnoCanvas, "request_redraw_uta", canvas_request_redraw_uta, 1);
    rb_define_method(gnoCanvas, "request_redraw", canvas_request_redraw, 4);
    rb_define_method(gnoCanvas, "w2c_affine", canvas_w2c_affine, 0);
    rb_define_method(gnoCanvas, "w2c", canvas_w2c, 2);
    rb_define_method(gnoCanvas, "c2w", canvas_c2w, 2);
    rb_define_method(gnoCanvas, "window_to_world", canvas_window_to_world, 2);
    rb_define_method(gnoCanvas, "world_to_window", canvas_world_to_window, 2);
    rb_define_method(gnoCanvas, "get_color", canvas_get_color, 2);
    rb_define_method(gnoCanvas, "set_stipple_origin", canvas_set_stipple_origin, 1);
    rb_define_method(gnoCanvas, "set_dither", canvas_set_dither, 1);
    rb_define_method(gnoCanvas, "get_dither", canvas_get_dither, 0);
    /* TODO: wrap structure elements. */
    rb_define_method(gnoCanvas, "aa?", canvas_get_aa, 0);

    rb_define_alias(gnoCanvas, "w2c_d", "w2c");

    /* child init */
    Init_gnome_icon_list();
}
