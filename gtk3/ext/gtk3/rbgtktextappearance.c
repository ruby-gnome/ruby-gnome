/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

/*****************************************/
static GtkTextAppearance*
app_copy(const GtkTextAppearance* app)
{
  GtkTextAppearance* new_app;
  g_return_val_if_fail (app != NULL, NULL);
  new_app = g_new(GtkTextAppearance, 1);
  *new_app = *app;
  return new_app;
}

GType
gtk_text_appearance_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkTextAppearance",
                    (GBoxedCopyFunc)app_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}
/*****************************************/

#define RG_TARGET_NAMESPACE cTextAppearance
#define _SELF(t) ((GtkTextAppearance*)RVAL2BOXED(t, GTK_TYPE_TEXT_APPEARANCE))

/***********************************************/
#define ATTR_INT(name)\
static VALUE \
txt_app_int_ ## name (VALUE self)\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
txt_app_int_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = NUM2INT(val); \
    return self;\
}

#define ATTR_BOOL(name)\
static VALUE \
txt_app_bool_ ## name (VALUE self)\
{\
    return CBOOL2RVAL(_SELF(self)->name);\
}\
static VALUE \
txt_app_bool_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2CBOOL(val);\
    return self;\
}

#define ATTR_ENUM(name, gtype)\
static VALUE \
txt_app_enums_ ## name (VALUE self)\
{\
    return GENUM2RVAL(_SELF(self)->name, gtype);\
}\
static VALUE \
txt_app_enums_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2GENUM(val, gtype);\
    return self;\
}

#define ATTR_GOBJ(name)\
static VALUE \
txt_app_gobj_ ## name (VALUE self)\
{\
    VALUE val; \
    if (_SELF(self)->name == NULL) return Qnil;\
    val = GOBJ2RVAL(_SELF(self)->name);  \
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    return val; \
}\
static VALUE \
txt_app_gobj_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = RVAL2GOBJ(val);\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    return self;\
}

#define ATTR_COLOR(name)\
static VALUE \
txt_app_color_ ## name (VALUE self)\
{\
    VALUE val = BOXED2RVAL(&_SELF(self)->name, GDK_TYPE_COLOR);\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    return val;\
}\
static VALUE \
txt_app_color_set_ ## name (VALUE self, VALUE val)\
{\
    G_CHILD_SET(self, rb_intern(G_STRINGIFY(name)), val);\
    _SELF(self)->name = *RVAL2GDKCOLOR(val);    \
    return self;\
}

#define DEFINE_ACCESSOR(gt, type, name)         \
    rb_define_method(gt, G_STRINGIFY(name), txt_app_ ## type ## _## name, 0);\
    rb_define_method(gt, G_STRINGIFY(set_ ## name), txt_app_ ## type ## _set_## name, 1);
/***********************************************/
ATTR_COLOR(bg_color);
ATTR_COLOR(fg_color);
ATTR_GOBJ(bg_stipple);
ATTR_GOBJ(fg_stipple);

ATTR_INT(rise);
ATTR_ENUM(underline, PANGO_TYPE_UNDERLINE);
ATTR_BOOL(strikethrough);
ATTR_BOOL(draw_bg);
ATTR_BOOL(inside_selection);
ATTR_BOOL(is_text);

static VALUE
rg_initialize(VALUE self)
{
    GtkTextAppearance* app = ALLOC(GtkTextAppearance);
    memset(app, 0, sizeof(GtkTextAppearance));
    G_INITIALIZE(self, app);
    return Qnil;
}

void
Init_gtk_text_appearance(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_APPEARANCE, "TextAppearance", mGtk);

    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, color, bg_color);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, color, fg_color);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, gobj, bg_stipple);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, gobj, fg_stipple);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, int, rise);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, enums, underline);

    RG_DEF_METHOD(initialize, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "strikethrough?", txt_app_bool_strikethrough, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_strikethrough", txt_app_bool_set_strikethrough, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "draw_bg?", txt_app_bool_draw_bg, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_draw_bg", txt_app_bool_set_draw_bg, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "inside_selection?", txt_app_bool_inside_selection, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_inside_selection", txt_app_bool_set_inside_selection, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "text?", txt_app_bool_is_text, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_text", txt_app_bool_set_is_text, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
