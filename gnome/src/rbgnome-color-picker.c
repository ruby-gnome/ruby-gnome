/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-color-picker.c,v 1.5 2002/12/26 15:13:43 mutoh Exp $ */
/* based on libgnomeui/gnome-color-picker.h */

/* Gnome::ColorPicker widget for Ruby-Gnome
 * Copyright (C) 2001 Neil Conway
 *               2002 KUBO Takehiro <kubo@jiubao.org>
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
    gdouble dr = NUM2DBL(r);
    gdouble dg = NUM2DBL(g);
    gdouble db = NUM2DBL(b);
    gdouble da = NUM2DBL(a);
    if (dr < 0 || 1.0 < dr)
        rb_raise(rb_eArgError, "out of range (%f for [0.0, 1.0])", dr);
    if (dg < 0 || 1.0 < dg)
        rb_raise(rb_eArgError, "out of range (%f for [0.0, 1.0])", dg);
    if (db < 0 || 1.0 < db)
        rb_raise(rb_eArgError, "out of range (%f for [0.0, 1.0])", db);
    if (da < 0 || 1.0 < da)
        rb_raise(rb_eArgError, "out of range (%f for [0.0, 1.0])", da);
    gnome_color_picker_set_d(_SELF(self), dr, dg, db, da);
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
    gint ir = NUM2INT(r);
    gint ig = NUM2INT(g);
    gint ib = NUM2INT(b);
    gint ia = NUM2INT(a);
    if (ir < 0 || 255 < ir)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ir);
    if (ig < 0 || 255 < ig)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ig);
    if (ib < 0 || 255 < ib)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ib);
    if (ia < 0 || 255 < ia)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ia);
    gnome_color_picker_set_i8(_SELF(self),
                              (guint8)ir, (guint8)ig,
                              (guint8)ib, (guint8)ia);
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
    gint ir = NUM2INT(r);
    gint ig = NUM2INT(g);
    gint ib = NUM2INT(b);
    gint ia = NUM2INT(a);
    if (ir < 0 || 65535 < ir)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ir);
    if (ig < 0 || 65535 < ig)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ig);
    if (ib < 0 || 65535 < ib)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ib);
    if (ia < 0 || 65535 < ia)
        rb_raise(rb_eArgError, "out of range (%d for [0, 255])", ia);
    gnome_color_picker_set_i16(_SELF(self),
                               (gushort)ir, (gushort)ig,
                               (gushort)ib, (gushort)ia);
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

void
Init_gnome_color_picker(mGnome)
    VALUE mGnome;
{
    VALUE gnoColorPicker = G_DEF_CLASS(GNOME_TYPE_COLOR_PICKER, "ColorPicker", mGnome);

    /* Instance methods */
    rb_define_method(gnoColorPicker, "initialize", cpicker_initialize, 0);
    rb_define_method(gnoColorPicker, "set_d", cpicker_set_d, 4);
    rb_define_method(gnoColorPicker, "d", cpicker_get_d, 0);
    rb_define_method(gnoColorPicker, "set_i8", cpicker_set_i8, 4);
    rb_define_method(gnoColorPicker, "i8", cpicker_get_i8, 0);
    rb_define_method(gnoColorPicker, "set_i16", cpicker_set_i16, 4);
    rb_define_method(gnoColorPicker, "i16", cpicker_get_i16, 0);
}
