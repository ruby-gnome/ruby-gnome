/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-color-picker.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::ColorPicker widget for Ruby-Gnome
 * Copyright (C) 2001 Neil Conway
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

#define _SELF(self) GNOME_COLOR_PICKER(RVAL2GOBJ(self))

/*
 * Class Gnome::ColorPicker
 *
 * Description:
 *   ...
 *
 * Hierarchy:
 *
 *   Object
 *     +----Gtk::Object
 *            +----Gtk::Widget
 *                   +----Gtk::Container
 *                          +----Gtk::Bin
 *                                 +----Gtk::Button
 *                                        +----Gnome::ColorPicker
 */

static VALUE
cpicker_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gnome_color_picker_new());
    return Qnil;
}

static VALUE
cpicker_set_d(self, r, g, b, a)
    VALUE self, r, g, b, a;
{
    gnome_color_picker_set_d(_SELF(self),
                             NUM2DBL(r), NUM2DBL(g), NUM2DBL(b), NUM2DBL(a));
    return self;
}

static VALUE
cpicker_get_d(self)
    VALUE self;
{
    gdouble r, g, b, a;
    VALUE ary;
    gnome_color_picker_get_d(_SELF(self),
			     &r, &g, &b, &a);
    ary = rb_ary_new2(4);
    rb_ary_push(ary, rb_float_new(r));
    rb_ary_push(ary, rb_float_new(g));
    rb_ary_push(ary, rb_float_new(b));
    rb_ary_push(ary, rb_float_new(a));
    return ary;
}

static VALUE
cpicker_set_i8(self, r, g, b, a)
    VALUE self, r, g, b, a;
{
    gnome_color_picker_set_i8(_SELF(self),
			      (guint8)NUM2INT(r), (guint8)NUM2INT(g),
			      (guint8)NUM2INT(b), (guint8)NUM2INT(a));
    return self;
}

static VALUE
cpicker_get_i8(self)
    VALUE self;
{
    guint8 r, g, b, a;
    VALUE ary;
    gnome_color_picker_get_i8(_SELF(self),
                              &r, &g, &b, &a);
    ary = rb_ary_new2(4);
    rb_ary_push(ary, INT2NUM((int)r));
    rb_ary_push(ary, INT2NUM((int)g));
    rb_ary_push(ary, INT2NUM((int)b));
    rb_ary_push(ary, INT2NUM((int)a));
    return ary;
}

static VALUE
cpicker_set_i16(self, r, g, b, a)
    VALUE self, r, g, b, a;
{
    gnome_color_picker_set_i8(_SELF(self),
                              (gushort)NUM2INT(r), (gushort)NUM2INT(g),
                              (gushort)NUM2INT(b), (gushort)NUM2INT(a));
    return self;
}

static VALUE
cpicker_get_i16(self)
    VALUE self;
{
    gushort r, g, b, a;
    VALUE ary;
    gnome_color_picker_get_i16(_SELF(self),
                               &r, &g, &b, &a);
    ary = rb_ary_new2(4);
    rb_ary_push(ary, INT2NUM((int)r));
    rb_ary_push(ary, INT2NUM((int)g));
    rb_ary_push(ary, INT2NUM((int)b));
    rb_ary_push(ary, INT2NUM((int)a));
    return ary;
}

static VALUE
cpicker_set_dither(self, dither)
    VALUE self, dither;
{
    gnome_color_picker_set_dither(_SELF(self),
                                  RTEST(dither));
    return self;
}

static VALUE
cpicker_set_use_alpha(self, use_alpha)
    VALUE self, use_alpha;
{
    gnome_color_picker_set_use_alpha(_SELF(self),
                                     RTEST(use_alpha));
    return self;
}

static VALUE
cpicker_set_title(self, title)
    VALUE self, title;
{
    gnome_color_picker_set_title(_SELF(self),
                                 RVAL2CSTR(title));
    return self;
}

void
Init_gnome_color_picker(mGnome)
    VALUE mGnome;
{
    VALUE gnoColorPicker = G_DEF_CLASS(GNOME_TYPE_COLOR_PICKER, "ColorPicker", mGnome);

    /* Instance methods */
    rb_define_method(gnoColorPicker, "initialize", cpicker_initialize, 0);
    rb_define_method(gnoColorPicker, "set_d", cpicker_set_d, 4);
    rb_define_method(gnoColorPicker, "get_d", cpicker_get_d, 0);
    rb_define_method(gnoColorPicker, "set_i8", cpicker_set_i8, 4);
    rb_define_method(gnoColorPicker, "get_i8", cpicker_get_i8, 0);
    rb_define_method(gnoColorPicker, "set_i16", cpicker_set_i16, 4);
    rb_define_method(gnoColorPicker, "get_i16", cpicker_get_i16, 0);
    rb_define_method(gnoColorPicker, "set_dither", cpicker_set_dither, 1);
    rb_define_method(gnoColorPicker, "set_use_alpha", cpicker_set_use_alpha, 1);
    rb_define_method(gnoColorPicker, "set_title", cpicker_set_title, 1);
}
