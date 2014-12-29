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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cTextAttributes
#define _SELF(s) (RVAL2GTKTEXTATTRIBUTES(s))

static VALUE
rg_initialize(VALUE self)
{
    GtkTextAttributes *attr;

    attr = gtk_text_attributes_new();
    G_INITIALIZE(self, attr);
    return Qnil;
}

static VALUE
rg_appearance(VALUE self)
{
    GtkTextAppearance app = _SELF(self)->appearance;
    return GTKTEXTAPPEARANCE2RVAL(&app);
}

static VALUE
rg_set_appearance(VALUE self, VALUE val)
{
    GtkTextAppearance* app = RVAL2GTKTEXTAPPEARANCE(val);
    _SELF(self)->appearance = *app;
    return self;
}

static VALUE
rg_justification(VALUE self)
{
    return GTKJUSTIFICATION2RVAL(_SELF(self)->justification);
}

static VALUE
rg_set_justification(VALUE self, VALUE val)
{
    _SELF(self)->justification = RVAL2GTKJUSTIFICATION(val);
    return self;
}

static VALUE
rg_direction(VALUE self)
{
    return GTKDIRECTIONTYPE2RVAL(_SELF(self)->direction);
}

static VALUE
rg_set_direction(VALUE self, VALUE val)
{
    _SELF(self)->direction = RVAL2GTKDIRECTIONTYPE(val);
    return self;
}

static VALUE
rg_font(VALUE self)
{
    VALUE val;
    if (_SELF(self)->font == NULL) return Qnil;
    val = PANGOFONTDESCRIPTION2RVAL(_SELF(self)->font);
    G_CHILD_SET(self, rb_intern("font"), val);
    return val;
}

static VALUE
rg_set_font(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("font"), val);
    _SELF(self)->font = RVAL2PANGOFONTDESCRIPTION(val);
    return self;
}

static VALUE
rg_font_scale(VALUE self)
{
    return rb_float_new(_SELF(self)->font_scale);
}

static VALUE
rg_set_font_scale(VALUE self, VALUE val)
{
    _SELF(self)->font_scale = NUM2DBL(val);
    return self;
}

static VALUE
rg_left_margin(VALUE self)
{
    return INT2NUM(_SELF(self)->left_margin);
}

static VALUE
rg_set_left_margin(VALUE self, VALUE val)
{
    _SELF(self)->left_margin = NUM2INT(val);
    return self;
}

static VALUE
rg_indent(VALUE self)
{
    return INT2NUM(_SELF(self)->indent);
}

static VALUE
rg_set_indent(VALUE self, VALUE val)
{
    _SELF(self)->indent = NUM2INT(val);
    return self;
}

static VALUE
rg_right_margin(VALUE self)
{
    return INT2NUM(_SELF(self)->right_margin);
}

static VALUE
rg_set_right_margin(VALUE self, VALUE val)
{
    _SELF(self)->right_margin = NUM2INT(val);
    return self;
}

static VALUE
rg_pixels_above_lines(VALUE self)
{
    return INT2NUM(_SELF(self)->pixels_above_lines);
}

static VALUE
rg_set_pixels_above_lines(VALUE self, VALUE val)
{
    _SELF(self)->pixels_above_lines = NUM2INT(val);
    return self;
}

static VALUE
rg_pixels_below_lines(VALUE self)
{
    return INT2NUM(_SELF(self)->pixels_below_lines);
}

static VALUE
rg_set_pixels_below_lines(VALUE self, VALUE val)
{
    _SELF(self)->pixels_below_lines = NUM2INT(val);
    return self;
}

static VALUE
rg_pixels_inside_wrap(VALUE self)
{
    return INT2NUM(_SELF(self)->pixels_inside_wrap);
}

static VALUE
rg_set_pixels_inside_wrap(VALUE self, VALUE val)
{
    _SELF(self)->pixels_inside_wrap = NUM2INT(val);
    return self;
}

static VALUE
rg_tabs(VALUE self)
{
    VALUE val;
    if (_SELF(self)->tabs == NULL) return Qnil;
    val = PANGOTABARRAY2RVAL(_SELF(self)->tabs);
    G_CHILD_SET(self, rb_intern("tabs"), val);
    return val;
}

static VALUE
rg_set_tabs(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("tabs"), val);
    _SELF(self)->tabs = RVAL2PANGOTABARRAY(val);
    return self;
}

static VALUE
rg_wrap_mode(VALUE self)
{
    return GTKWRAPMODE2RVAL(_SELF(self)->wrap_mode);
}

static VALUE
rg_set_wrap_mode(VALUE self, VALUE val)
{
    _SELF(self)->wrap_mode = RVAL2GTKWRAPMODE(val);
    return self;
}

static VALUE
rg_language(VALUE self)
{
    VALUE val;
    if (_SELF(self)->language == NULL) return Qnil;
    val = PANGOLANGUAGE2RVAL(_SELF(self)->language);
    G_CHILD_SET(self, rb_intern("language"), val);
    return val;
}

static VALUE
rg_set_language(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("language"), val);
    _SELF(self)->language = RVAL2PANGOLANGUAGE(val);
    return self;
}

static VALUE
rg_invisible_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->invisible);
}

static VALUE
rg_set_invisible(VALUE self, VALUE val)
{
    _SELF(self)->invisible = RVAL2CBOOL(val);
    return self;
}

static VALUE
rg_bg_full_height_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->bg_full_height);
}

static VALUE
rg_set_bg_full_height(VALUE self, VALUE val)
{
    _SELF(self)->bg_full_height = RVAL2CBOOL(val);
    return self;
}

static VALUE
rg_editable_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->editable);
}

static VALUE
rg_set_editable(VALUE self, VALUE val)
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

    RG_DEF_METHOD(appearance, 0);
    RG_DEF_METHOD(set_appearance, 1);
    RG_DEF_METHOD(justification, 0);
    RG_DEF_METHOD(set_justification, 1);
    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD(set_direction, 1);
    RG_DEF_METHOD(font, 0);
    RG_DEF_METHOD(set_font, 1);
    RG_DEF_METHOD(font_scale, 0);
    RG_DEF_METHOD(set_font_scale, 1);
    RG_DEF_METHOD(left_margin, 0);
    RG_DEF_METHOD(set_left_margin, 1);
    RG_DEF_METHOD(indent, 0);
    RG_DEF_METHOD(set_indent, 1);
    RG_DEF_METHOD(right_margin, 0);
    RG_DEF_METHOD(set_right_margin, 1);
    RG_DEF_METHOD(pixels_above_lines, 0);
    RG_DEF_METHOD(set_pixels_above_lines, 1);
    RG_DEF_METHOD(pixels_below_lines, 0);
    RG_DEF_METHOD(set_pixels_below_lines, 1);
    RG_DEF_METHOD(pixels_inside_wrap, 0);
    RG_DEF_METHOD(set_pixels_inside_wrap, 1);
    RG_DEF_METHOD(tabs, 0);
    RG_DEF_METHOD(set_tabs, 1);
    RG_DEF_METHOD(wrap_mode, 0);
    RG_DEF_METHOD(set_wrap_mode, 1);
    RG_DEF_METHOD(language, 0);
    RG_DEF_METHOD(set_language, 1);

    RG_DEF_METHOD_P(invisible, 0);
    RG_DEF_METHOD(set_invisible, 1);
    RG_DEF_METHOD_P(bg_full_height, 0);
    RG_DEF_METHOD(set_bg_full_height, 1);
    RG_DEF_METHOD_P(editable, 0);
    RG_DEF_METHOD(set_editable, 1);
}
