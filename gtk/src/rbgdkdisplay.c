/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplay.c -

  $Author: mutoh $
  $Date: 2003/07/24 16:35:05 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_MINOR_VERSION >= 2
#define _DISPLAY(i) GDK_DISPLAY_OBJECT(RVAL2GOBJ(i))

static VALUE
gdkdisplay_initialize(self, display_name)
    VALUE self, display_name;
{
    G_INITIALIZE(self, gdk_display_open(RVAL2CSTR(display_name)));
    return Qnil;
}

static VALUE
gdkdisplay_default(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_default());
}


static VALUE
gdkdisplay_name(self)
    VALUE self;
{
    return rb_str_new2(gdk_display_get_name(_DISPLAY(self)));
}

static VALUE
gdkdisplay_n_screens(self)
    VALUE self;
{
    return INT2NUM(gdk_display_get_n_screens(_DISPLAY(self)));
}

static VALUE
gdkdisplay_get_screen(self, num)
    VALUE self, num;
{
    return GOBJ2RVAL(gdk_display_get_screen(_DISPLAY(self), NUM2INT(num)));
}

static VALUE
gdkdisplay_default_screen(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_default_screen(_DISPLAY(self)));
}

static VALUE
gdkdisplay_pointer_ungrab(self,time)
    VALUE self,time;
{
    gdk_display_pointer_ungrab(_DISPLAY(self),NUM2ULONG(time));
    return self;
}

static VALUE
gdkdisplay_keyboard_ungrab(self,time)
    VALUE self,time;
{
    gdk_display_keyboard_ungrab(_DISPLAY(self),NUM2ULONG(time));
    return self;
}

static VALUE
gdkdisplay_pointer_grabbed(self)
    VALUE self;
{
    return gdk_display_pointer_is_grabbed(_DISPLAY(self)) ? Qtrue : Qfalse;
}

static VALUE
gdkdisplay_beep(self)
    VALUE self;
{
    gdk_display_beep(_DISPLAY(self));
    return self;
}

static VALUE
gdkdisplay_sync(self)
    VALUE self;
{
    gdk_display_sync(_DISPLAY(self));
    return self;
}

static VALUE
gdkdisplay_close(self)
    VALUE self;
{
    gdk_display_close(_DISPLAY(self));
    return self;
}

static VALUE
gdkdisplay_list_devices(self)
    VALUE self;
{
    return rbgutil_glist2ary(gdk_display_list_devices(_DISPLAY(self)));
}

static VALUE
gdkdisplay_get_pointer(self)
    VALUE self;
{
    GdkScreen *screen;
    int x,y;
    GdkModifierType mask;
    
    gdk_display_get_pointer(_DISPLAY(self), &screen, &x, &y, &mask);
    
    return rb_ary_new3(4, GOBJ2RVAL(screen), INT2NUM(x), INT2NUM(y), INT2NUM(mask));
}
static VALUE
gdkdisplay_get_window_at_pointer(self)
    VALUE self;
{
    GdkWindow *window;
    int x,y;
    
    window = gdk_display_get_window_at_pointer(_DISPLAY(self), &x, &y);
    
    return rb_ary_new3(3, GOBJ2RVAL(window), INT2NUM(x), INT2NUM(y));
}
#endif

void 
Init_gtk_gdk_display()
{
#if GTK_MINOR_VERSION >= 2
    VALUE gdkDisplay = G_DEF_CLASS(GDK_TYPE_DISPLAY, "Display", mGdk);

    rb_define_method(gdkDisplay, "initialize", gdkdisplay_initialize, 1);
    rb_define_singleton_method(gdkDisplay, "default", gdkdisplay_default, 0);
    rb_define_method(gdkDisplay, "name", gdkdisplay_name, 0);
    rb_define_method(gdkDisplay, "n_screens", gdkdisplay_n_screens, 0);
    rb_define_method(gdkDisplay, "get_screen", gdkdisplay_get_screen, 1);
    rb_define_alias(gdkDisplay, "[]", "get_screen");
    rb_define_alias(gdkDisplay, "screen", "get_screen");
    rb_define_method(gdkDisplay, "default_screen", gdkdisplay_default_screen, 0);
    
    rb_define_method(gdkDisplay, "pointer_ungrab", gdkdisplay_pointer_ungrab, 1);
    rb_define_method(gdkDisplay, "keyboard_ungrab", gdkdisplay_keyboard_ungrab, 1);
    rb_define_method(gdkDisplay, "pointer_grabbed?", gdkdisplay_pointer_grabbed, 0);
    
    rb_define_method(gdkDisplay, "beep", gdkdisplay_beep, 0);
    rb_define_method(gdkDisplay, "sync", gdkdisplay_sync, 0);
    rb_define_method(gdkDisplay, "close", gdkdisplay_close, 0);
    
    rb_define_method(gdkDisplay, "devices", gdkdisplay_list_devices, 0);
    rb_define_method(gdkDisplay, "pointer", gdkdisplay_get_pointer, 0);
    rb_define_method(gdkDisplay, "window_at_pointer", gdkdisplay_get_window_at_pointer, 0);
#endif
}
