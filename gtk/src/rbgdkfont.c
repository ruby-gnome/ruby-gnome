/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkfont.c -

  $Author: mutoh $
  $Date: 2002/08/29 07:24:40 $

  Copyright (C) 2001 Neil Conway
************************************************/

#ifndef GTK_DISABLE_DEPRECATED

#include "global.h"

#define _SELF(f) ((GdkFont*)RVAL2BOXED(f))

static VALUE
font_load_font(self, name)
     VALUE self, name;
{
  GdkFont *font = gdk_font_load(STR2CSTR(name));
  return BOXED2RVAL(font, GDK_TYPE_FONT);
}

static VALUE
font_load_fontset(self, name)
     VALUE self, name;
{
  GdkFont *font = gdk_fontset_load(STR2CSTR(name));
  return BOXED2RVAL(font, GDK_TYPE_FONT);
}

static VALUE
font_string_width(self, str)
     VALUE self, str;
{
  return INT2NUM(gdk_string_width(_SELF(self), STR2CSTR(str)));
}

static VALUE
font_string_measure(self, str)
     VALUE self, str;
{
  return INT2NUM(gdk_string_measure(_SELF(self), STR2CSTR(str)));
}

static VALUE
font_string_height(self, str)
     VALUE self, str;
{
  return INT2NUM(gdk_string_height(_SELF(self), STR2CSTR(str)));
}

static VALUE
font_type(self)
     VALUE self;
{
  return INT2NUM(_SELF(self)->type);
}

static VALUE
font_ascent(self)
     VALUE self;
{
  return INT2NUM(_SELF(self)->ascent);
}

static VALUE
font_descent(self)
     VALUE self;
{
  return INT2NUM(_SELF(self)->descent);
}

static VALUE
font_equal(font1, font2)
     VALUE font1, font2;
{
  if (gdk_font_equal(_SELF(font1), _SELF(font2)))
	return Qtrue;
  return Qfalse;
}

static VALUE
font_string_extents(self, str)
     VALUE self, str;
{
  gint lbearing, rbearing, width, ascent, descent;

  gdk_string_extents(_SELF(self), STR2CSTR(str),
                     &lbearing, &rbearing, &width, &ascent, &descent);
  return rb_ary_new3(5,
                     INT2NUM(lbearing), INT2NUM(rbearing),
                     INT2NUM(width), INT2NUM(ascent), INT2NUM(descent));
}

static VALUE
font_char_width(self, character)
     VALUE self, character;
{
  return INT2NUM(gdk_char_width(_SELF(self), (gchar)NUM2INT(character)));
}

static VALUE
font_char_width_wc(self, character)
     VALUE self, character;
{
  return INT2NUM(gdk_char_width_wc(_SELF(self), NUM2INT(character)));
}

static VALUE
font_char_measure(self, character)
     VALUE self, character;
{
  return INT2NUM(gdk_char_measure(_SELF(self), (gchar)NUM2INT(character)));
}

static VALUE
font_char_height(self, character)
     VALUE self, character;
{
  return INT2NUM(gdk_char_height(_SELF(self), (gchar)NUM2INT(character)));
}

static VALUE
font_text_width(self, text, length)
     VALUE self, text, length;
{
  return INT2NUM(gdk_text_width(_SELF(self),
                                STR2CSTR(text),
                                NUM2INT(length)));
}

static VALUE
font_text_measure(self, text, length)
     VALUE self, text, length;
{
  return INT2NUM(gdk_text_measure(_SELF(self),
                                  STR2CSTR(text),
                                  NUM2INT(length)));
}

static VALUE
font_text_height(self, text, length)
     VALUE self, text, length;
{
  return INT2NUM(gdk_text_height(_SELF(self), 
                                 STR2CSTR(text),
                                 NUM2INT(length)));
}

void
Init_gtk_gdk_font()
{
  VALUE gdkFont = G_DEF_CLASS(GDK_TYPE_FONT, "Font", mGdk);

  rb_define_singleton_method(gdkFont, "font_load", font_load_font, 1);
  rb_define_singleton_method(gdkFont, "fontset_load", font_load_fontset, 1);
  rb_define_method(gdkFont, "string_width", font_string_width, 1);
  rb_define_method(gdkFont, "string_measure", font_string_measure, 1);
  rb_define_method(gdkFont, "string_height", font_string_height, 1);
  rb_define_method(gdkFont, "font_type", font_type, 0);
  rb_define_method(gdkFont, "ascent", font_ascent, 0);
  rb_define_method(gdkFont, "descent", font_descent, 0);
  rb_define_method(gdkFont, "==", font_equal, 1);
  rb_define_method(gdkFont, "string_extents", font_string_extents, 1);
  rb_define_method(gdkFont, "char_width", font_char_width, 1);
  rb_define_method(gdkFont, "char_width_wc", font_char_width_wc, 1);
  rb_define_method(gdkFont, "char_measure", font_char_measure, 1);
  rb_define_method(gdkFont, "char_height", font_char_height, 1);
  rb_define_method(gdkFont, "text_width", font_text_width, 2);
  rb_define_method(gdkFont, "text_measure", font_text_measure, 2);
  rb_define_method(gdkFont, "text_height", font_text_height, 2);
}

#endif
