/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-font-picker.c,v 1.6 2003/02/02 12:51:06 tkubo Exp $ */
/* based on libgnomeui/gnome-font-picker.h */

/* Gnome::FontPicker widget for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgnome.h"

#define _SELF(self) GNOME_FONT_PICKER(RVAL2GOBJ(self))

static VALUE
picker_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gnome_font_picker_new());
    return Qnil;
}

static VALUE
picker_set_use_font_in_label(self, use_font, size)
    VALUE self, use_font, size;
{
    GnomeFontPicker *gfp = _SELF(self);
    if (gnome_font_picker_get_mode(gfp) != GNOME_FONT_PICKER_MODE_FONT_INFO)
        rb_raise(rb_eRuntimeError, "mode is not a Gnome::FontPicker::MODE_FONT_INFO.");
    gnome_font_picker_fi_set_use_font_in_label(gfp,
                                               RTEST(use_font),
                                               NUM2INT(size));
    return self;
}

static VALUE
picker_set_widget(self, user_widget)
    VALUE self, user_widget;
{
    GnomeFontPicker *gfp = _SELF(self);
    if (gnome_font_picker_get_mode(gfp) != GNOME_FONT_PICKER_MODE_USER_WIDGET)
        rb_raise(rb_eRuntimeError, "mode is not a Gnome::FontPicker::MODE_USER_WIDGET.");
    gnome_font_picker_uw_set_widget(gfp, GTK_WIDGET(RVAL2GOBJ(user_widget)));
    return self;
}

static VALUE
picker_get_widget(self)
    VALUE self;
{
    GnomeFontPicker *gfp = _SELF(self);
    if (gnome_font_picker_get_mode(gfp) != GNOME_FONT_PICKER_MODE_USER_WIDGET)
        rb_raise(rb_eRuntimeError, "mode is not a Gnome::FontPicker::MODE_USER_WIDGET.");
    return GOBJ2RVAL(gnome_font_picker_uw_get_widget(gfp));
}

void
Init_gnome_font_picker(mGnome)
    VALUE mGnome;
{
    VALUE gnoFontPicker = G_DEF_CLASS(GNOME_TYPE_FONT_PICKER, "FontPicker", mGnome);

    rb_define_method(gnoFontPicker, "initialize", picker_initialize, 0);
    rb_define_method(gnoFontPicker, "set_use_font_in_label", picker_set_use_font_in_label, 2);
    rb_define_method(gnoFontPicker, "set_widget", picker_set_widget, 1);
    rb_define_method(gnoFontPicker, "widget", picker_get_widget, 0);

    rb_define_const(gnoFontPicker, "MODE_PIXMAP", INT2NUM(GNOME_FONT_PICKER_MODE_PIXMAP));
    rb_define_const(gnoFontPicker, "MODE_FONT_INFO", INT2NUM(GNOME_FONT_PICKER_MODE_FONT_INFO));
    rb_define_const(gnoFontPicker, "MODE_USER_WIDGET", INT2NUM(GNOME_FONT_PICKER_MODE_USER_WIDGET));
    rb_define_const(gnoFontPicker, "MODE_UNKNOWN", INT2NUM(GNOME_FONT_PICKER_MODE_UNKNOWN));

    G_DEF_SETTER(gnoFontPicker, "widget");
}
