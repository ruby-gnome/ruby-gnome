/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#define _SELF(rc) (GTK_RC_STYLE(RVAL2GOBJ(rc)))

static VALUE
rcstyle_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_rc_style_new());
    return Qnil;
}

static VALUE
rcstyle_name(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->name);
}

static VALUE
rcstyle_set_name(VALUE self, VALUE name)
{
    _SELF(self)->name = g_strdup(RVAL2CSTR(name));
    return self;
}

static VALUE
rcstyle_bg_pixmap_name(VALUE self, VALUE idx)
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return CSTR2RVAL(_SELF(self)->bg_pixmap_name[i]);
}

static VALUE
rcstyle_set_bg_pixmap_name(VALUE self, VALUE idx, VALUE bg_pixmap_name)
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    _SELF(self)->bg_pixmap_name[i] = g_strdup(RVAL2CSTR(bg_pixmap_name));
    return self;
}
    
static VALUE
rcstyle_font_desc(VALUE self)
{
    PangoFontDescription* desc = _SELF(self)->font_desc;
    return BOXED2RVAL(desc, PANGO_TYPE_FONT_DESCRIPTION);
}

static VALUE
rcstyle_set_font_desc(VALUE self, VALUE font_desc)
{
    _SELF(self)->font_desc = (PangoFontDescription*)RVAL2BOXED(font_desc, PANGO_TYPE_FONT_DESCRIPTION);
    return self;
}

static VALUE
rcstyle_color_flags(VALUE self, VALUE rb_state_type)
{
    GtkStateType state_type;

    state_type = RVAL2GTKSTATETYPE(rb_state_type);
    return GTKRCFLAGS2RVAL(_SELF(self)->color_flags[state_type]);
}

static VALUE
rcstyle_set_color_flags(VALUE self, VALUE rb_state_type, VALUE flags)
{
    GtkStateType state_type;

    state_type = RVAL2GTKSTATETYPE(rb_state_type);
    _SELF(self)->color_flags[state_type] = RVAL2GTKRCFLAGS(flags);
    return self;
}

#define DEFINE_STYLE_COLOR(type) \
static VALUE \
rcstyle_set_ ## type(VALUE self, VALUE idx, VALUE r, VALUE g, VALUE b) \
{ \
  GdkColor *color; \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  color =  &(_SELF(self) -> type [i]); \
  color->red   = NUM2INT(r); \
  color->green = NUM2INT(g); \
  color->blue  = NUM2INT(b); \
  return self;\
} \
static VALUE \
rcstyle_ ## type(VALUE self, VALUE idx) \
{ \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  return BOXED2RVAL(&_SELF(self)-> type [i], GDK_TYPE_COLOR); \
} \

DEFINE_STYLE_COLOR(fg);
DEFINE_STYLE_COLOR(bg);
DEFINE_STYLE_COLOR(text);
DEFINE_STYLE_COLOR(base);

static VALUE
rcstyle_xthickness(VALUE self)
{
    return INT2NUM(_SELF(self)->xthickness);
}

static VALUE
rcstyle_set_xthickness(VALUE self, VALUE xthickness)
{
    _SELF(self)->xthickness = NUM2INT(xthickness);
    return self;
}

static VALUE
rcstyle_ythickness(VALUE self)
{
    return INT2NUM(_SELF(self)->ythickness);
}

static VALUE
rcstyle_set_ythickness(VALUE self, VALUE ythickness)
{
    _SELF(self)->ythickness = NUM2INT(ythickness);
    return self;
}

void 
Init_gtk_rcstyle(void)
{
    VALUE gRcStyle = G_DEF_CLASS(GTK_TYPE_RC_STYLE, "RcStyle", mGtk);
    rb_define_method(gRcStyle, "initialize", rcstyle_initialize, 0);
    rb_define_method(gRcStyle, "color_flags", rcstyle_color_flags, 1);
    rb_define_method(gRcStyle, "set_color_flags", rcstyle_set_color_flags, 2);
    rb_define_method(gRcStyle, "name", rcstyle_name, 0);
    rb_define_method(gRcStyle, "set_name", rcstyle_set_name, 1);
    rb_define_method(gRcStyle, "bg_pixmap_name", rcstyle_bg_pixmap_name, 1);
    rb_define_method(gRcStyle, "set_bg_pixmap_name", rcstyle_set_bg_pixmap_name, 2);
    rb_define_method(gRcStyle, "font_desc", rcstyle_font_desc, 0);
    rb_define_method(gRcStyle, "set_font_desc", rcstyle_set_font_desc, 1);
    rb_define_method(gRcStyle, "fg", rcstyle_fg, 1);
    rb_define_method(gRcStyle, "bg", rcstyle_bg, 1);
    rb_define_method(gRcStyle, "text", rcstyle_text, 1);
    rb_define_method(gRcStyle, "base", rcstyle_base, 1);
    rb_define_method(gRcStyle, "set_fg", rcstyle_set_fg, 4);
    rb_define_method(gRcStyle, "set_bg", rcstyle_set_bg, 4);
    rb_define_method(gRcStyle, "set_text", rcstyle_set_text, 4);
    rb_define_method(gRcStyle, "set_base", rcstyle_set_base, 4);
    rb_define_method(gRcStyle, "xthickness", rcstyle_xthickness, 0);
    rb_define_method(gRcStyle, "ythickness", rcstyle_ythickness, 0);
    rb_define_method(gRcStyle, "set_xthickness", rcstyle_set_xthickness, 1);
    rb_define_method(gRcStyle, "set_ythickness", rcstyle_set_ythickness, 1);

    G_DEF_SETTERS(gRcStyle);
}
