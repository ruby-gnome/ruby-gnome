/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 OGASAWARA, Takeshi
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

#include "global.h"

#define RG_TARGET_NAMESPACE cTextAttributes
#define _SELF(s) ((GtkTextAttributes*)RVAL2BOXED(s, GTK_TYPE_TEXT_ATTRIBUTES))

static VALUE
rg_initialize(VALUE self)
{
    GtkTextAttributes *attr;

    attr = gtk_text_attributes_new();
    G_INITIALIZE(self, attr);
    return Qnil;
}

static VALUE
txt_attr_boxed_appearance(VALUE self)
{
    GtkTextAppearance app = _SELF(self)->appearance;
    return BOXED2RVAL(&app, GTK_TYPE_TEXT_APPEARANCE);
}

static VALUE
txt_attr_boxed_set_appearance(VALUE self, VALUE val)
{
    GtkTextAppearance* app = (GtkTextAppearance*)RVAL2BOXED(val, GTK_TYPE_TEXT_APPEARANCE);
    _SELF(self)->appearance = *app;
    return self;
}

static VALUE
txt_attr_enums_justification(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->justification, GTK_TYPE_JUSTIFICATION);
}

static VALUE
txt_attr_enums_set_justification(VALUE self, VALUE val)
{
    _SELF(self)->justification = RVAL2GENUM(val, GTK_TYPE_JUSTIFICATION);
    return self;
}

static VALUE
txt_attr_enums_direction(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->direction, GTK_TYPE_DIRECTION_TYPE);
}

static VALUE
txt_attr_enums_set_direction(VALUE self, VALUE val)
{
    _SELF(self)->direction = RVAL2GENUM(val, GTK_TYPE_DIRECTION_TYPE);
    return self;
}

static VALUE
txt_attr_boxed_font(VALUE self)
{
    VALUE val;
    if (_SELF(self)->font == NULL) return Qnil;
    val = BOXED2RVAL(_SELF(self)->font, PANGO_TYPE_FONT_DESCRIPTION);
    G_CHILD_SET(self, rb_intern("font"), val);
    return val;
}

static VALUE
txt_attr_boxed_set_font(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("font"), val);
    _SELF(self)->font = RVAL2BOXED(val, PANGO_TYPE_FONT_DESCRIPTION);
    return self;
}

static VALUE
txt_attr_double_font_scale(VALUE self)
{
    return rb_float_new(_SELF(self)->font_scale);
}

static VALUE
txt_attr_double_set_font_scale(VALUE self, VALUE val)
{
    _SELF(self)->font_scale = NUM2DBL(val);
    return self;
}

static VALUE
txt_attr_int_left_margin(VALUE self)
{
    return INT2NUM(_SELF(self)->left_margin);
}

static VALUE
txt_attr_int_set_left_margin(VALUE self, VALUE val)
{
    _SELF(self)->left_margin = NUM2INT(val);
    return self;
}

static VALUE
txt_attr_int_indent(VALUE self)
{
    return INT2NUM(_SELF(self)->indent);
}

static VALUE
txt_attr_int_set_indent(VALUE self, VALUE val)
{
    _SELF(self)->indent = NUM2INT(val);
    return self;
}

static VALUE
txt_attr_int_right_margin(VALUE self)
{
    return INT2NUM(_SELF(self)->right_margin);
}

static VALUE
txt_attr_int_set_right_margin(VALUE self, VALUE val)
{
    _SELF(self)->right_margin = NUM2INT(val);
    return self;
}

static VALUE
txt_attr_int_pixels_above_lines(VALUE self)
{
    return INT2NUM(_SELF(self)->pixels_above_lines);
}

static VALUE
txt_attr_int_set_pixels_above_lines(VALUE self, VALUE val)
{
    _SELF(self)->pixels_above_lines = NUM2INT(val);
    return self;
}

static VALUE
txt_attr_int_pixels_below_lines(VALUE self)
{
    return INT2NUM(_SELF(self)->pixels_below_lines);
}

static VALUE
txt_attr_int_set_pixels_below_lines(VALUE self, VALUE val)
{
    _SELF(self)->pixels_below_lines = NUM2INT(val);
    return self;
}

static VALUE
txt_attr_int_pixels_inside_wrap(VALUE self)
{
    return INT2NUM(_SELF(self)->pixels_inside_wrap);
}

static VALUE
txt_attr_int_set_pixels_inside_wrap(VALUE self, VALUE val)
{
    _SELF(self)->pixels_inside_wrap = NUM2INT(val);
    return self;
}

static VALUE
txt_attr_boxed_tabs(VALUE self)
{
    VALUE val;
    if (_SELF(self)->tabs == NULL) return Qnil;
    val = BOXED2RVAL(_SELF(self)->tabs, PANGO_TYPE_TAB_ARRAY);
    G_CHILD_SET(self, rb_intern("tabs"), val);
    return val;
}

static VALUE
txt_attr_boxed_set_tabs(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("tabs"), val);
    _SELF(self)->tabs = RVAL2BOXED(val, PANGO_TYPE_TAB_ARRAY);
    return self;
}

static VALUE
txt_attr_enums_wrap_mode(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->wrap_mode, GTK_TYPE_WRAP_MODE);
}

static VALUE
txt_attr_enums_set_wrap_mode(VALUE self, VALUE val)
{
    _SELF(self)->wrap_mode = RVAL2GENUM(val, GTK_TYPE_WRAP_MODE);
    return self;
}

static VALUE
txt_attr_boxed_language(VALUE self)
{
    VALUE val;
    if (_SELF(self)->language == NULL) return Qnil;
    val = BOXED2RVAL(_SELF(self)->language, PANGO_TYPE_LANGUAGE);
    G_CHILD_SET(self, rb_intern("language"), val);
    return val;
}

static VALUE
txt_attr_boxed_set_language(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("language"), val);
    _SELF(self)->language = RVAL2BOXED(val, PANGO_TYPE_LANGUAGE);
    return self;
}

static VALUE
txt_attr_bool_invisible(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->invisible);
}

static VALUE
txt_attr_bool_set_invisible(VALUE self, VALUE val)
{
    _SELF(self)->invisible = RVAL2CBOOL(val);
    return self;
}

static VALUE
txt_attr_bool_bg_full_height(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->bg_full_height);
}

static VALUE
txt_attr_bool_set_bg_full_height(VALUE self, VALUE val)
{
    _SELF(self)->bg_full_height = RVAL2CBOOL(val);
    return self;
}

static VALUE
txt_attr_bool_editable(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->editable);
}

static VALUE
txt_attr_bool_set_editable(VALUE self, VALUE val)
{
    _SELF(self)->editable = RVAL2CBOOL(val);
    return self;
}

static VALUE
rg_copy_values(VALUE self, VALUE dest)
{
    gtk_text_attributes_copy_values(_SELF(self), _SELF(dest));
    return self;
}

void
Init_gtk_text_attributes(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_ATTRIBUTES, "TextAttributes", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(copy_values, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "appearance", txt_attr_boxed_appearance, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_appearance", txt_attr_boxed_set_appearance, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "justification", txt_attr_enums_justification, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_justification", txt_attr_enums_set_justification, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "direction", txt_attr_enums_direction, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_direction", txt_attr_enums_set_direction, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "font", txt_attr_boxed_font, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_font", txt_attr_boxed_set_font, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "font_scale", txt_attr_double_font_scale, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_font_scale", txt_attr_double_set_font_scale, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "left_margin", txt_attr_int_left_margin, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_left_margin", txt_attr_int_set_left_margin, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "indent", txt_attr_int_indent, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_indent", txt_attr_int_set_indent, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "right_margin", txt_attr_int_right_margin, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_right_margin", txt_attr_int_set_right_margin, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "pixels_above_lines", txt_attr_int_pixels_above_lines, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_pixels_above_lines", txt_attr_int_set_pixels_above_lines, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "pixels_below_lines", txt_attr_int_pixels_below_lines, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_pixels_below_lines", txt_attr_int_set_pixels_below_lines, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "pixels_inside_wrap", txt_attr_int_pixels_inside_wrap, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_pixels_inside_wrap", txt_attr_int_set_pixels_inside_wrap, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "tabs", txt_attr_boxed_tabs, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_tabs", txt_attr_boxed_set_tabs, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "wrap_mode", txt_attr_enums_wrap_mode, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_wrap_mode", txt_attr_enums_set_wrap_mode, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "language", txt_attr_boxed_language, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_language", txt_attr_boxed_set_language, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "invisible?", txt_attr_bool_invisible, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_invisible", txt_attr_bool_set_invisible, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "bg_full_height?", txt_attr_bool_bg_full_height, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_bg_full_height", txt_attr_bool_set_bg_full_height, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "editable?", txt_attr_bool_editable, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_editable", txt_attr_bool_set_editable, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
