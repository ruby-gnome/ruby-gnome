/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplay.c -

  $Author: mutoh $
  $Date: 2004/02/26 17:46:19 $

  Copyright (C) 2003 Geoff Youngs
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,2,0)
#define _SELF(i) GDK_SCREEN(RVAL2GOBJ(i))

static VALUE
gdkscreen_default(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_default());
}

static VALUE
gdkscreen_get_default_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_default_colormap(_SELF(self)));
}

static VALUE
gdkscreen_set_default_colormap(self, colormap)
    VALUE self, colormap;
{
    gdk_screen_set_default_colormap(_SELF(self),
                                    GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
gdkscreen_get_system_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_system_colormap(_SELF(self)));
}

static VALUE
gdkscreen_get_system_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_system_visual(_SELF(self)));
}

static VALUE
gdkscreen_get_rgb_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_rgb_colormap(_SELF(self)));
}

static VALUE
gdkscreen_get_rgb_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_rgb_visual(_SELF(self)));
}

static VALUE
gdkscreen_get_root_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_root_window(_SELF(self)));
}

static VALUE
gdkscreen_get_display(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_screen_get_display(_SELF(self)));
}

static VALUE
gdkscreen_number(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_number(_SELF(self)));
}
static VALUE
gdkscreen_width(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_width(_SELF(self)));
}
static VALUE
gdkscreen_height(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_height(_SELF(self)));
}
static VALUE
gdkscreen_width_mm(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_width_mm(_SELF(self)));
}
static VALUE
gdkscreen_height_mm(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_height_mm(_SELF(self)));
}

static VALUE
gdkscreen_list_visuals(self)
    VALUE self;
{
    GList* list = gdk_screen_list_visuals(_SELF(self));
    VALUE ret = GLIST2ARY(list);
    g_list_free(list);
    return ret;
}

static VALUE
gdkscreen_get_toplevel_windows(self)
    VALUE self;
{
    GList* list = gdk_screen_get_toplevel_windows(_SELF(self));
    VALUE ret = GLIST2ARY(list);
    g_list_free(list);
    return ret;
}

static VALUE
gdkscreen_make_display_name(self)
    VALUE self;
{
    gchar* name = gdk_screen_make_display_name(_SELF(self));
    VALUE ret = CSTR2RVAL(name);
    g_free(name);
    return ret;
}

static VALUE
gdkscreen_n_monitors(self)
    VALUE self;
{
      return INT2NUM(gdk_screen_get_n_monitors(_SELF(self)));
}

static VALUE
gdkscreen_monitor_geometry(self, num)
    VALUE self, num;
{
      GdkRectangle rect;
      gdk_screen_get_monitor_geometry(_SELF(self), NUM2INT(num), &rect);
      return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
gdkscreen_get_monitor(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE arg1, arg2;
    VALUE ret;

    rb_scan_args(argc, argv, "11", &arg1, &arg2);
    if (argc == 2){
        ret = INT2NUM(gdk_screen_get_monitor_at_point(_SELF(self),
                                                      NUM2INT(arg1), NUM2INT(arg2)));
    } else if (argc == 1){
        ret = INT2NUM(gdk_screen_get_monitor_at_window(_SELF(self),
                                                       GDK_WINDOW(RVAL2GOBJ(arg1))));
    } else {
        rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
    }
    return ret;
}

static VALUE
gdkscreen_broadcast_client_message(self, event)
    VALUE self, event;
{
    gdk_screen_broadcast_client_message(_SELF(self), RVAL2GEV(event));
    return self;
}

static VALUE
gdkscreen_get_setting(self, name)
    VALUE self, name;
{
      GValue val;
      if (gdk_screen_get_setting(_SELF(self), RVAL2CSTR(name), &val))
      {
      	  return GVAL2RVAL(&val);
      }
      return Qnil;
}

#endif

void 
Init_gtk_gdk_screen()
{
#if GTK_CHECK_VERSION(2,2,0)
    VALUE gdkScreen = G_DEF_CLASS(GDK_TYPE_SCREEN, "Screen", mGdk);

    rb_define_singleton_method(gdkScreen, "default", gdkscreen_default, 0);
    rb_define_method(gdkScreen, "default_colormap", gdkscreen_get_default_colormap, 0);
    rb_define_method(gdkScreen, "set_default_colormap", gdkscreen_set_default_colormap, 1);
    rb_define_method(gdkScreen, "system_colormap", gdkscreen_get_system_colormap, 0);
    rb_define_method(gdkScreen, "system_visual", gdkscreen_get_system_visual, 0);
    rb_define_method(gdkScreen, "rgb_colormap", gdkscreen_get_rgb_colormap, 0);
    rb_define_method(gdkScreen, "rgb_visual", gdkscreen_get_rgb_visual, 0);
    rb_define_method(gdkScreen, "root_window", gdkscreen_get_root_window, 0);
    rb_define_method(gdkScreen, "display", gdkscreen_get_display, 0);
    rb_define_method(gdkScreen, "number", gdkscreen_number, 0);
    rb_define_method(gdkScreen, "width", gdkscreen_width, 0);
    rb_define_method(gdkScreen, "height", gdkscreen_height, 0);
    rb_define_method(gdkScreen, "width_mm", gdkscreen_width_mm, 0);
    rb_define_method(gdkScreen, "height_mm", gdkscreen_height_mm, 0);
    rb_define_method(gdkScreen, "visuals", gdkscreen_list_visuals, 0);
    rb_define_method(gdkScreen, "toplevel_windows", gdkscreen_get_toplevel_windows, 0);
    rb_define_method(gdkScreen, "display_name", gdkscreen_make_display_name, 0);
    rb_define_method(gdkScreen, "n_monitors", gdkscreen_n_monitors, 0);
    rb_define_method(gdkScreen, "monitor_geometry", gdkscreen_monitor_geometry, 1);
    rb_define_method(gdkScreen, "get_monitor", gdkscreen_get_monitor, -1);
    rb_define_method(gdkScreen, "broadcast_client_message", gdkscreen_broadcast_client_message, 1);
    rb_define_method(gdkScreen, "get_setting", gdkscreen_get_setting, 1);

    G_DEF_SETTERS(gdkScreen);

  #ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkScreenX11", "ScreenX11", mGdk);
  #endif
#endif
}
