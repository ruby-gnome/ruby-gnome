/* $Id: rbgnome-font-picker.c,v 1.1 2002/05/19 12:29:58 mutoh Exp $ */

/* Gnome::FontPicker widget for Ruby/Gnome
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

VALUE gnoFontPicker;

static VALUE
picker_initialize(self)
    VALUE self;
{
    GtkWidget* gfp = gnome_font_picker_new();
    set_widget(self, gfp);
    return Qnil;
}

static VALUE
picker_set_title(self, title)
    VALUE self, title;
{
    gnome_font_picker_set_title(GNOME_FONT_PICKER(get_widget(self)),
                                STR2CSTR(title));
    return Qnil;
}

static VALUE
picker_get_mode(self)
    VALUE self;
{
    return INT2NUM(gnome_font_picker_get_mode(
                GNOME_FONT_PICKER(get_widget(self))));
}

static VALUE
picker_set_mode(self, mode)
    VALUE self, mode;
{
    gnome_font_picker_set_mode(GNOME_FONT_PICKER(get_widget(self)),
                               NUM2INT(mode));
    return Qnil;
}

static VALUE
picker_set_use_font_in_label(self, use_font, size)
    VALUE self, use_font, size;
{
    gnome_font_picker_fi_set_use_font_in_label(
                GNOME_FONT_PICKER(get_widget(self)),
                RTEST(use_font),
                NUM2INT(size));
    return Qnil;
}

static VALUE
picker_set_show_size(self, show_size)
    VALUE self, show_size;
{
    gnome_font_picker_set_mode(GNOME_FONT_PICKER(get_widget(self)),
                               NUM2INT(show_size));
    return Qnil;
}

static VALUE
picker_set_widget(self, user_widget)
    VALUE self, user_widget;
{
    gnome_font_picker_uw_set_widget(GNOME_FONT_PICKER(get_widget(self)),
                                    get_widget(user_widget));
    return Qnil;
}

static VALUE
picker_get_font_name(self)
    VALUE self;
{
    return rb_str_new2(gnome_font_picker_get_font_name(
                GNOME_FONT_PICKER(get_widget(self))));
}

static VALUE
picker_get_font(self)
    VALUE self;
{
    return make_gdkfont(gnome_font_picker_get_font(
                GNOME_FONT_PICKER(get_widget(self))));
}

static VALUE
picker_set_font_name(self, font_name)
    VALUE self, font_name;
{
    gnome_font_picker_set_font_name(GNOME_FONT_PICKER(get_widget(self)),
                                    STR2CSTR(font_name));
    return Qnil;
}

static VALUE
picker_get_preview_text(self)
    VALUE self;
{
    return rb_str_new2(gnome_font_picker_get_preview_text(
                GNOME_FONT_PICKER(get_widget(self))));
}

static VALUE
picker_set_preview_text(self, preview_text)
    VALUE self, preview_text;
{
    gnome_font_picker_set_preview_text(GNOME_FONT_PICKER(get_widget(self)),
                                       STR2CSTR(preview_text));
    return Qnil;
}

static VALUE
picker_get_use_font_in_label(self)
    VALUE self;
{
    gboolean result = GNOME_FONT_PICKER(get_widget(self))->use_font_in_label;
    return result?Qtrue:Qfalse;
}

static VALUE
picker_get_use_font_in_label_size(self)
    VALUE self;
{
    gboolean result =
        GNOME_FONT_PICKER(get_widget(self))->use_font_in_label_size;
    return result?Qtrue:Qfalse;
}

static VALUE
picker_get_show_size(self)
    VALUE self;
{
    gboolean result = GNOME_FONT_PICKER(get_widget(self))->show_size;
    return result?Qtrue:Qfalse;
}

void
Init_gnome_font_picker()
{
    gnoFontPicker = rb_define_class_under(mGnome, "FontPicker", gButton);

    rb_define_method(gnoFontPicker, "initialize", picker_initialize, 0);
    rb_define_method(gnoFontPicker, "set_title", picker_set_title, 1);
    rb_define_method(gnoFontPicker, "get_mode", picker_get_mode, 0);
    rb_define_method(gnoFontPicker, "set_mode", picker_set_mode, 1);
    rb_define_method(gnoFontPicker, "fi_set_use_font_in_label",
            picker_set_use_font_in_label, 2);
    rb_define_method(gnoFontPicker, "fi_set_show_size",
            picker_set_show_size, 1);
    rb_define_method(gnoFontPicker, "uw_set_widget",
            picker_set_widget, 1);
    rb_define_method(gnoFontPicker, "get_font_name",
            picker_get_font_name, 0);
    rb_define_method(gnoFontPicker, "get_font",
            picker_get_font, 0);
    rb_define_method(gnoFontPicker, "set_font_name",
            picker_set_font_name, 1);
    rb_define_method(gnoFontPicker, "get_preview_text",
            picker_get_preview_text, 0);
    rb_define_method(gnoFontPicker, "set_preview_text",
            picker_set_preview_text, 1);
    rb_define_method(gnoFontPicker, "get_use_font_in_label",
            picker_get_use_font_in_label, 0);
    rb_define_method(gnoFontPicker, "get_use_font_in_label_size",
            picker_get_use_font_in_label_size, 0);
    rb_define_method(gnoFontPicker, "get_show_size",
            picker_get_show_size, 0);

    rb_define_const(gnoFontPicker, "MODE_PIXMAP",
            INT2NUM(GNOME_FONT_PICKER_MODE_PIXMAP));
    rb_define_const(gnoFontPicker, "MODE_FONT_INFO",
            INT2NUM(GNOME_FONT_PICKER_MODE_FONT_INFO));
    rb_define_const(gnoFontPicker, "MODE_USER_WIDGET",
            INT2NUM(GNOME_FONT_PICKER_MODE_USER_WIDGET));
    rb_define_const(gnoFontPicker, "MODE_UNKNOWN",
            INT2NUM(GNOME_FONT_PICKER_MODE_UNKNOWN));

    /* Signals */
    rb_define_const(gnoFontPicker, "SIGNAL_FONT_SET",
            rb_str_new2("font-set"));
}
