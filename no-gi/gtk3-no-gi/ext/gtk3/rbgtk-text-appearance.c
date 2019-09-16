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

#include "rbgtk3private.h"

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
#define _SELF(t) (RVAL2GTKTEXTAPPEARANCE(t))

static VALUE
rg_initialize(VALUE self)
{
    GtkTextAppearance* app = ALLOC(GtkTextAppearance);
    memset(app, 0, sizeof(GtkTextAppearance));
    G_INITIALIZE(self, app);
    return Qnil;
}

static VALUE
rg_bg_color(VALUE self)
{
    VALUE val = GDKCOLOR2RVAL(&_SELF(self)->bg_color);
    G_CHILD_SET(self, rb_intern("bg_color"), val);
    return val;
}

static VALUE
rg_set_bg_color(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("bg_color"), val);
    _SELF(self)->bg_color = *RVAL2GDKCOLOR(val);
    return self;
}

static VALUE
rg_fg_color(VALUE self)
{
    VALUE val = GDKCOLOR2RVAL(&_SELF(self)->fg_color);
    G_CHILD_SET(self, rb_intern("fg_color"), val);
    return val;
}

static VALUE
rg_set_fg_color(VALUE self, VALUE val)
{
    G_CHILD_SET(self, rb_intern("fg_color"), val);
    _SELF(self)->fg_color = *RVAL2GDKCOLOR(val);
    return self;
}

/* deprecated
static VALUE
rg_bg_stipple(VALUE self)
{
    VALUE val;
    if (_SELF(self)->bg_stipple == NULL) return Qnil;
    val = GOBJ2RVAL(_SELF(self)->bg_stipple);
    G_CHILD_SET(self, rb_intern("bg_stipple"), val);
    return val;
}

static VALUE
rg_set_bg_stipple(VALUE self, VALUE val)
{
    _SELF(self)->bg_stipple = RVAL2GOBJ(val);
    G_CHILD_SET(self, rb_intern("bg_stipple"), val);
    return self;
}

static VALUE
rg_fg_stipple(VALUE self)
{
    VALUE val;
    if (_SELF(self)->fg_stipple == NULL) return Qnil;
    val = GOBJ2RVAL(_SELF(self)->fg_stipple);
    G_CHILD_SET(self, rb_intern("fg_stipple"), val);
    return val;
}

static VALUE
rg_set_fg_stipple(VALUE self, VALUE val)
{
    _SELF(self)->fg_stipple = RVAL2GOBJ(val);
    G_CHILD_SET(self, rb_intern("fg_stipple"), val);
    return self;
}
*/

static VALUE
rg_rise(VALUE self)
{
    return INT2NUM(_SELF(self)->rise);
}

static VALUE
rg_set_rise(VALUE self, VALUE val)
{
    _SELF(self)->rise = NUM2INT(val);
    return self;
}

static VALUE
rg_underline(VALUE self)
{
    return PANGOUNDERLINE2RVAL(_SELF(self)->underline);
}

static VALUE
rg_set_underline(VALUE self, VALUE val)
{
    _SELF(self)->underline = RVAL2PANGOUNDERLINE(val);
    return self;
}

static VALUE
rg_strikethrough_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->strikethrough);
}

static VALUE
rg_set_strikethrough(VALUE self, VALUE val)
{
    _SELF(self)->strikethrough = RVAL2CBOOL(val);
    return self;
}

static VALUE
rg_draw_bg_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->draw_bg);
}

static VALUE
rg_set_draw_bg(VALUE self, VALUE val)
{
    _SELF(self)->draw_bg = RVAL2CBOOL(val);
    return self;
}

static VALUE
rg_inside_selection_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->inside_selection);
}

static VALUE
rg_set_inside_selection(VALUE self, VALUE val)
{
    _SELF(self)->inside_selection = RVAL2CBOOL(val);
    return self;
}

static VALUE
rg_text_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->is_text);
}

static VALUE
rg_set_text(VALUE self, VALUE val)
{
    _SELF(self)->is_text = RVAL2CBOOL(val);
    return self;
}

void
Init_gtk_text_appearance(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TEXT_APPEARANCE, "TextAppearance", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(bg_color, 0);
    RG_DEF_METHOD(set_bg_color, 1);
    RG_DEF_METHOD(fg_color, 0);
    RG_DEF_METHOD(set_fg_color, 1);
/* deprecated
    RG_DEF_METHOD(bg_stipple, 0);
    RG_DEF_METHOD(set_bg_stipple, 1);
    RG_DEF_METHOD(fg_stipple, 0);
    RG_DEF_METHOD(set_fg_stipple, 1);
*/
    RG_DEF_METHOD(rise, 0);
    RG_DEF_METHOD(set_rise, 1);
    RG_DEF_METHOD(underline, 0);
    RG_DEF_METHOD(set_underline, 1);

    RG_DEF_METHOD_P(strikethrough, 0);
    RG_DEF_METHOD(set_strikethrough, 1);
    RG_DEF_METHOD_P(draw_bg, 0);
    RG_DEF_METHOD(set_draw_bg, 1);
    RG_DEF_METHOD_P(inside_selection, 0);
    RG_DEF_METHOD(set_inside_selection, 1);
    RG_DEF_METHOD_P(text, 0);
    RG_DEF_METHOD(set_text, 1);
}
