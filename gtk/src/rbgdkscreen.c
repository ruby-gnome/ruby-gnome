/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplay.c -

  $Author: mutoh $
  $Date: 2003/07/04 19:09:55 $

  Copyright (C) 2003 Geoff Youngs
************************************************/

#include "global.h"

#if GTK_MINOR_VERSION >= 2
#define _SCREEN(i) GDK_SCREEN(RVAL2GOBJ(i))

static VALUE
gdkscreen_default(self)
    VALUE self;
{
      return GOBJ2RVAL(gdk_screen_get_default());
}

static VALUE
gdkscreen_number(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_number(_SCREEN(self)));
}
static VALUE
gdkscreen_width(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_width(_SCREEN(self)));
}
static VALUE
gdkscreen_height(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_height(_SCREEN(self)));
}
static VALUE
gdkscreen_width_mm(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_width_mm(_SCREEN(self)));
}
static VALUE
gdkscreen_height_mm(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_height_mm(_SCREEN(self)));
}
static VALUE
gdkscreen_n_monitors(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_n_monitors(_SCREEN(self)));
}
static VALUE
gdkscreen_monitor_geometry(self, num)
    VALUE self, num;
{
      GdkRectangle rect;
      gdk_screen_get_monitor_geometry(_SCREEN(self), NUM2INT(num), &rect);
      return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}
static VALUE
gdkscreen_monitor_at_point(self,x,y)
    VALUE self,x,y;
{
      return INT2NUM(gdk_screen_get_monitor_at_point(_SCREEN(self),NUM2INT(x),NUM2INT(y)));
}
static VALUE
gdkscreen_monitor_at_window(self,win)
    VALUE self,win;
{
      return INT2NUM(gdk_screen_get_monitor_at_window(_SCREEN(self),GDK_WINDOW(RVAL2GOBJ(win))));
}

static VALUE
gdkscreen_get_setting(self,name)
    VALUE self,name;
{
      GValue val;
      if (gdk_screen_get_setting(_SCREEN(self), RVAL2CSTR(name), &val))
      {
      	  return GVAL2RVAL(&val);
      }
      return Qnil;
}

#endif

void 
Init_gtk_gdk_screen()
{
#if GTK_MINOR_VERSION >= 2
    VALUE gdkScreen = G_DEF_CLASS(GDK_TYPE_SCREEN, "Screen", mGdk);

    rb_define_singleton_method(gdkScreen, "default", gdkscreen_default, 0);
    rb_define_method(gdkScreen, "number", gdkscreen_number, 0);
    rb_define_method(gdkScreen, "width", gdkscreen_width, 0);
    rb_define_method(gdkScreen, "height", gdkscreen_height, 0);
    rb_define_method(gdkScreen, "width_mm", gdkscreen_width_mm, 0);
    rb_define_method(gdkScreen, "height_mm", gdkscreen_height_mm, 0);
    rb_define_method(gdkScreen, "n_monitors", gdkscreen_n_monitors, 0);
    rb_define_method(gdkScreen, "monitor_geometry", gdkscreen_monitor_geometry, 1);
    rb_define_method(gdkScreen, "monitor_at_point", gdkscreen_monitor_at_point, 2);
    rb_define_method(gdkScreen, "monitor_at_window", gdkscreen_monitor_at_window, 1);
    rb_define_method(gdkScreen, "setting", gdkscreen_get_setting, 1);
#endif
}
