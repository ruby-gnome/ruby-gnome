/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplay.c -

  $Author: sakai $
  $Date: 2003/11/20 18:27:54 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,2,0)
#define _SELF(i) GDK_DISPLAY_OBJECT(RVAL2GOBJ(i))


static VALUE
gdkdisplay_close(self)
    VALUE self;
{
    gdk_display_close(_SELF(self));
    return self;
}

static VALUE
gdkdisplay_s_open(self, display_name)
    VALUE self, display_name;
{
    VALUE display = GOBJ2RVAL(gdk_display_open(RVAL2CSTR(display_name)));
    if (rb_block_given_p()) {
        rb_ensure(rb_yield, display, gdkdisplay_close, display);
        return Qnil;
    } else {
        return display;
    }
}

static VALUE
gdkdisplay_s_get_default(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_default());
}


static VALUE
gdkdisplay_name(self)
    VALUE self;
{
    return CSTR2RVAL(gdk_display_get_name(_SELF(self)));
}

static VALUE
gdkdisplay_n_screens(self)
    VALUE self;
{
    return INT2NUM(gdk_display_get_n_screens(_SELF(self)));
}

static VALUE
gdkdisplay_get_screen(self, num)
    VALUE self, num;
{
    return GOBJ2RVAL(gdk_display_get_screen(_SELF(self), NUM2INT(num)));
}

static VALUE
gdkdisplay_default_screen(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_default_screen(_SELF(self)));
}

static VALUE
gdkdisplay_pointer_ungrab(self, time)
    VALUE self, time;
{
    gdk_display_pointer_ungrab(_SELF(self), NUM2ULONG(time));
    return self;
}

static VALUE
gdkdisplay_keyboard_ungrab(self, time)
    VALUE self, time;
{
    gdk_display_keyboard_ungrab(_SELF(self), NUM2ULONG(time));
    return self;
}

static VALUE
gdkdisplay_pointer_grabbed(self)
    VALUE self;
{
    return gdk_display_pointer_is_grabbed(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
gdkdisplay_beep(self)
    VALUE self;
{
    gdk_display_beep(_SELF(self));
    return self;
}

static VALUE
gdkdisplay_sync(self)
    VALUE self;
{
    gdk_display_sync(_SELF(self));
    return self;
}

static VALUE
gdkdisplay_list_devices(self)
    VALUE self;
{
    return rbgutil_glist2ary(gdk_display_list_devices(_SELF(self)));
}

static VALUE
gdkdisplay_get_event(self)
    VALUE self;
{
    return GEV2RVAL(gdk_display_get_event(_SELF(self)));
}

static VALUE
gdkdisplay_peek_event(self)
    VALUE self;
{
    return GEV2RVAL(gdk_display_peek_event(_SELF(self)));
}

static VALUE
gdkdisplay_put_event(self, event)
    VALUE self, event;
{
    gdk_display_put_event(_SELF(self), RVAL2GEV(event));
    return self;
}

/* Don't implement this.
static GdkFilterReturn
filter_func(xevent, event, func)
    GdkXEvent xevent;
    GdkEvent event;
    gpointer func;
{
    return GENUM2RVAL(rb_funcall((VALUE)func, id_call, 2, 
                                 ????, GEV2RVAL(event)), 
                      GDK_TYPE_FILTER_RETURN);
}

static VALUE
gdkdisplay_add_client_message_filter(self, message_type)
    VALUE self, message_type;
{
    VALUE func = RB_BLOCK_PROC();
    G_RELATIVE(self, func);
    gdk_display_add_client_message_filter(_SELF(self), RVAL2ATOM(message_type),
                                          filter_func, func);
    return self;
}
*/

static VALUE
gdkdisplay_set_double_click_time(self, msec)
    VALUE self, msec;
{
    gdk_display_set_double_click_time(_SELF(self), NUM2UINT(msec));
    return self;
}

static VALUE
gdkdisplay_get_pointer(self)
    VALUE self;
{
    GdkScreen *screen;
    int x,y;
    GdkModifierType mask;
    
    gdk_display_get_pointer(_SELF(self), &screen, &x, &y, &mask);
    
    return rb_ary_new3(4, GOBJ2RVAL(screen), INT2NUM(x), INT2NUM(y), INT2NUM(mask));
}
static VALUE
gdkdisplay_get_window_at_pointer(self)
    VALUE self;
{
    GdkWindow *window;
    int x,y;
    
    window = gdk_display_get_window_at_pointer(_SELF(self), &x, &y);
    
    return rb_ary_new3(3, GOBJ2RVAL(window), INT2NUM(x), INT2NUM(y));
}

/* Don't implement this.
GdkDisplayPointerHooks* gdk_display_set_pointer_hooks
                                            (GdkDisplay *display,
                                             const GdkDisplayPointerHooks *new_hooks);
 */

static VALUE
gdkdisplay_get_core_pointer(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_core_pointer(_SELF(self)));
}

#endif

void 
Init_gtk_gdk_display()
{
#if GTK_CHECK_VERSION(2,2,0)
    VALUE gdkDisplay = G_DEF_CLASS(GDK_TYPE_DISPLAY, "Display", mGdk);

    rb_define_singleton_method(gdkDisplay, "open", gdkdisplay_s_open, 1);
    rb_define_singleton_method(gdkDisplay, "default", gdkdisplay_s_get_default, 0);
    rb_define_method(gdkDisplay, "name", gdkdisplay_name, 0);
    rb_define_method(gdkDisplay, "n_screens", gdkdisplay_n_screens, 0);
    rb_define_method(gdkDisplay, "get_screen", gdkdisplay_get_screen, 1);
    rb_define_alias(gdkDisplay, "[]", "get_screen");
    rb_define_method(gdkDisplay, "default_screen", gdkdisplay_default_screen, 0);
    
    rb_define_method(gdkDisplay, "pointer_ungrab", gdkdisplay_pointer_ungrab, 1);
    rb_define_method(gdkDisplay, "keyboard_ungrab", gdkdisplay_keyboard_ungrab, 1);
    rb_define_method(gdkDisplay, "pointer_grabbed?", gdkdisplay_pointer_grabbed, 0);
    
    rb_define_method(gdkDisplay, "beep", gdkdisplay_beep, 0);
    rb_define_method(gdkDisplay, "sync", gdkdisplay_sync, 0);
    rb_define_method(gdkDisplay, "close", gdkdisplay_close, 0);
    
    rb_define_method(gdkDisplay, "devices", gdkdisplay_list_devices, 0);
    rb_define_method(gdkDisplay, "event", gdkdisplay_get_event, 0);
    rb_define_method(gdkDisplay, "peek_event", gdkdisplay_peek_event, 0);
    rb_define_method(gdkDisplay, "put_event", gdkdisplay_put_event, 0);
    rb_define_method(gdkDisplay, "set_double_click_time", gdkdisplay_set_double_click_time, 2);
    rb_define_method(gdkDisplay, "pointer", gdkdisplay_get_pointer, 0);
    rb_define_method(gdkDisplay, "window_at_pointer", gdkdisplay_get_window_at_pointer, 0);
    rb_define_method(gdkDisplay, "core_pointer", gdkdisplay_get_core_pointer, 0);
#endif
}
