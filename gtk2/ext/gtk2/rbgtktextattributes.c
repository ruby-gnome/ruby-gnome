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

#define _SELF(s) ((GtkTextAttributes*)RVAL2BOXED(s, GTK_TYPE_TEXT_ATTRIBUTES))

/***********************************************/
#define ATTR_INT(name)\
static VALUE \
txt_attr_int_ ## name (VALUE self)\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
txt_attr_int_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = NUM2INT(val); \
    return self;\
}

#define ATTR_BOOL(name)\
static VALUE \
txt_attr_bool_ ## name (VALUE self)\
{\
    return CBOOL2RVAL(_SELF(self)->name);\
}\
static VALUE \
txt_attr_bool_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2CBOOL(val);\
    return self;\
}

#define ATTR_FLAGS(name, gtype)\
static VALUE \
txt_attr_flags_ ## name (VALUE self)\
{\
    return GFLAGS2RVAL(_SELF(self)->name, gtype);\
}\
static VALUE \
txt_attr_flags_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2GFLAGS(val, gtype);\
    return self;\
}

#define ATTR_ENUM(name, gtype)\
static VALUE \
txt_attr_enums_ ## name (VALUE self)\
{\
    return GENUM2RVAL(_SELF(self)->name, gtype);\
}\
static VALUE \
txt_attr_enums_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2GENUM(val, gtype);\
    return self;\
}

#define ATTR_BOXED(name, gtype)\
static VALUE \
txt_attr_boxed_ ## name (VALUE self)\
{\
    VALUE val; \
    if (_SELF(self)->name == NULL) return Qnil;\
    val = BOXED2RVAL(_SELF(self)->name, gtype);\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    return val;\
}\
static VALUE \
txt_attr_boxed_set_ ## name (VALUE self, VALUE val)\
{\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    _SELF(self)->name = RVAL2BOXED(val, gtype);\
    return self;\
}

#define DEFINE_ACCESSOR(gt, type, name)                                  \
    rb_define_method(gt, G_STRINGIFY(name), txt_attr_ ## type ## _## name, 0);\
    rb_define_method(gt, G_STRINGIFY(set_ ## name), txt_attr_ ## type ## _set_## name, 1);

/***********************************************/
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

ATTR_ENUM(justification, GTK_TYPE_JUSTIFICATION);
ATTR_ENUM(direction, GTK_TYPE_DIRECTION_TYPE);
ATTR_BOXED(font, PANGO_TYPE_FONT_DESCRIPTION);

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

ATTR_INT(left_margin);
ATTR_INT(indent);
ATTR_INT(right_margin);
ATTR_INT(pixels_above_lines);
ATTR_INT(pixels_below_lines);
ATTR_INT(pixels_inside_wrap);

ATTR_BOXED(tabs, PANGO_TYPE_TAB_ARRAY);

ATTR_ENUM(wrap_mode, GTK_TYPE_WRAP_MODE);
ATTR_BOXED(language, PANGO_TYPE_LANGUAGE);

ATTR_BOOL(invisible);
ATTR_BOOL(bg_full_height);
ATTR_BOOL(editable);
ATTR_BOOL(realized);

/***********************************************/

static VALUE
txt_attr_initialize(VALUE self)
{
    GtkTextAttributes *attr;

    attr = gtk_text_attributes_new();
    G_INITIALIZE(self, attr);
    return Qnil;
}

static VALUE
txt_attr_copy_values(VALUE self, VALUE dest)
{
    gtk_text_attributes_copy_values(_SELF(self), _SELF(dest));
    return self;
}

void
Init_gtk_text_attributes(void)
{
    VALUE gTextAttributes = G_DEF_CLASS(GTK_TYPE_TEXT_ATTRIBUTES, "TextAttributes", mGtk);
  
    rb_define_method(gTextAttributes, "initialize", txt_attr_initialize, 0);
    rb_define_method(gTextAttributes, "copy_values", txt_attr_copy_values, 1);

    DEFINE_ACCESSOR(gTextAttributes, boxed, appearance);
    DEFINE_ACCESSOR(gTextAttributes, enums, justification);
    DEFINE_ACCESSOR(gTextAttributes, enums, direction);
    DEFINE_ACCESSOR(gTextAttributes, boxed, font);
    DEFINE_ACCESSOR(gTextAttributes, double, font_scale);
    DEFINE_ACCESSOR(gTextAttributes, int, left_margin);
    DEFINE_ACCESSOR(gTextAttributes, int, indent);
    DEFINE_ACCESSOR(gTextAttributes, int, right_margin);
    DEFINE_ACCESSOR(gTextAttributes, int, pixels_above_lines);
    DEFINE_ACCESSOR(gTextAttributes, int, pixels_below_lines);
    DEFINE_ACCESSOR(gTextAttributes, int, pixels_inside_wrap);
    DEFINE_ACCESSOR(gTextAttributes, boxed, tabs);
    DEFINE_ACCESSOR(gTextAttributes, enums, wrap_mode);
    DEFINE_ACCESSOR(gTextAttributes, boxed, language);

    rb_define_method(gTextAttributes, "invisible?", txt_attr_bool_invisible, 0);
    rb_define_method(gTextAttributes, "set_invisible", txt_attr_bool_set_invisible, 1);
    rb_define_method(gTextAttributes, "bg_full_height?", txt_attr_bool_bg_full_height, 0);
    rb_define_method(gTextAttributes, "set_bg_full_height", txt_attr_bool_set_bg_full_height, 1);
    rb_define_method(gTextAttributes, "editable?", txt_attr_bool_editable, 0);
    rb_define_method(gTextAttributes, "set_editable", txt_attr_bool_set_editable, 1);
    rb_define_method(gTextAttributes, "realized?", txt_attr_bool_realized, 0);
    rb_define_method(gTextAttributes, "set_realized", txt_attr_bool_set_realized, 1);

    G_DEF_SETTERS(gTextAttributes);
}
