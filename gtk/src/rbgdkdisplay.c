/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdisplay.c -

  $Author: ggc $
  $Date: 2007/07/13 14:27:08 $

  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
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
    GdkDisplay* gdisplay = gdk_display_open(RVAL2CSTR(display_name));
    if (! gdisplay) {
        rb_raise(rb_eRuntimeError, "The display `%s' could not be opened.", 
                 RVAL2CSTR(display_name));
    } else {
        VALUE display;
        g_object_ref(gdisplay);
        display = GOBJ2RVAL(gdisplay);

        if (rb_block_given_p()) {
            rb_ensure(rb_yield, display, gdkdisplay_close, display);
            return Qnil;
        } else {
            return display;
        }
    }
}

static VALUE
gdkdisplay_s_get_default(self)
    VALUE self;
{
    GdkDisplay* gdisplay = gdk_display_get_default();
     if (! gdisplay)
        rb_raise(rb_eRuntimeError, "No default display is found.");
    
    return GOBJ2RVAL(gdisplay);
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
    return CBOOL2RVAL(gdk_display_pointer_is_grabbed(_SELF(self)));
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

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkdisplay_flush(self)
    VALUE self;
{
    gdk_display_flush(_SELF(self));
    return self;
}
#endif

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
gdkdisplay_get_double_click_time(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->double_click_time);
}

static VALUE
gdkdisplay_get_button_click_time(self)
    VALUE self;
{
    return rb_ary_new3(2,
                       UINT2NUM(_SELF(self)->button_click_time[0]),
                       UINT2NUM(_SELF(self)->button_click_time[1]));
}

static VALUE
gdkdisplay_get_button_window(self)
    VALUE self;
{
    return rb_ary_new3(2,
                       GOBJ2RVAL(_SELF(self)->button_window[0]),
                       GOBJ2RVAL(_SELF(self)->button_window[1]));
}

static VALUE
gdkdisplay_get_button_number(self)
    VALUE self;
{
    return rb_ary_new3(2,
                       INT2NUM(_SELF(self)->button_number[0]),
                       INT2NUM(_SELF(self)->button_number[1]));
}

static VALUE
gdkdisplay_get_closed(self)
    VALUE self;
{
    return CBOOL2RVAL(_SELF(self)->closed);
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkdisplay_get_button_x(self)
    VALUE self;
{
    return rb_ary_new3(2,
                       INT2NUM(_SELF(self)->button_x[0]),
                       INT2NUM(_SELF(self)->button_x[1]));
}

static VALUE
gdkdisplay_get_button_y(self)
    VALUE self;
{
    return rb_ary_new3(2,
                       INT2NUM(_SELF(self)->button_y[0]),
                       INT2NUM(_SELF(self)->button_y[1]));
}

static VALUE
gdkdisplay_set_double_click_distance(self, distance)
    VALUE self, distance;
{
    gdk_display_set_double_click_distance(_SELF(self), NUM2UINT(distance));
    return self;
}

static VALUE
gdkdisplay_get_double_click_distance(self, distance)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->double_click_distance);
}
#endif

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

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkdisplay_supports_cursor_color(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_display_supports_cursor_color(_SELF(self)));
}

static VALUE
gdkdisplay_supports_cursor_alpha(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_display_supports_cursor_alpha(_SELF(self)));
}

static VALUE
gdkdisplay_get_default_cursor_size(self)
    VALUE self;
{
    return UINT2NUM(gdk_display_get_default_cursor_size(_SELF(self)));
}

static VALUE
gdkdisplay_get_maximal_cursor_size(self)
    VALUE self;
{
    guint width, height;
    gdk_display_get_maximal_cursor_size(_SELF(self), &width, &height);
    return rb_assoc_new(UINT2NUM(width), UINT2NUM(height));
}

static VALUE
gdkdisplay_get_default_group(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_default_group(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
gdkdisplay_supports_selection_notification(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_display_supports_selection_notification(_SELF(self)));
}

static VALUE
gdkdisplay_request_selection_notification(self, selection)
    VALUE self, selection;
{
    return CBOOL2RVAL(gdk_display_request_selection_notification(_SELF(self), 
                                                                 RVAL2ATOM(selection)));
}

static VALUE
gdkdisplay_supports_clipboard_persistence(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_display_supports_clipboard_persistence(_SELF(self)));
}

static VALUE
gdkdisplay_store_clipboard(self, clipboard_window, time_, targets)
    VALUE self, clipboard_window, time_, targets;
{
    gint i;
    gint n_targets = RARRAY(targets)->len;
    GdkAtom* gtargets = g_new(GdkAtom, n_targets);

    for (i = 0; i < n_targets; i++){
        gtargets[i] = RVAL2ATOM(RARRAY(targets)->ptr[i]);
    }

    gdk_display_store_clipboard(_SELF(self), GDK_WINDOW(RVAL2GOBJ(clipboard_window)),
                                NUM2UINT(time_), gtargets, n_targets);
    return self;
}
#endif

static VALUE
gdkdisplay_get_core_pointer(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_display_get_core_pointer(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
gdkdisplay_warp_pointer(self, screen, x, y)
    VALUE self, screen, x, y;
{
    gdk_display_warp_pointer(_SELF(self), RVAL2GOBJ(screen), NUM2INT(x), NUM2INT(y));
    return self;
}
#endif

#ifdef GDK_WINDOWING_X11
static VALUE
gdkdisplay_grab(self)
    VALUE self;
{
    gdk_x11_display_grab(_SELF(self));
    return self;
}
static VALUE
gdkdisplay_ungrab(self)
    VALUE self;
{
    gdk_x11_display_ungrab(_SELF(self));
    return self;
}
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkdisplay_register_standard_event_type(self, event_base, n_events)
    VALUE self, event_base, n_events;
{
    gdk_x11_register_standard_event_type(_SELF(self),
                                         NUM2INT(event_base), NUM2INT(n_events));
    return self;
}
#endif
#if GTK_CHECK_VERSION(2,8,0)
static VALUE
gdkdisplay_get_user_time(self)
    VALUE self;
{
    return UINT2NUM(gdk_x11_display_get_user_time(_SELF(self)));
}

static VALUE
gdkdisplay_set_cursor_theme(self, theme, size)
    VALUE self, theme, size;
{
    gdk_x11_display_set_cursor_theme(_SELF(self), RVAL2CSTR(theme), NUM2INT(size));
    return self;
}
#endif
#endif
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gdkdisplay_supports_shapes(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_display_supports_shapes(_SELF(self)));
}

static VALUE
gdkdisplay_supports_input_shapes(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_display_supports_input_shapes(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
gdkdisplay_supports_composite(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_display_supports_composite(_SELF(self)));
}

static VALUE
gdkdisplay_trigger_tooltip_query(self)
    VALUE self;
{
    gtk_tooltip_trigger_tooltip_query(_SELF(self));
    return self;
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
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkDisplay, "flush", gdkdisplay_flush, 0);
#endif
    rb_define_method(gdkDisplay, "close", gdkdisplay_close, 0);
    
    rb_define_method(gdkDisplay, "devices", gdkdisplay_list_devices, 0);
    rb_define_method(gdkDisplay, "event", gdkdisplay_get_event, 0);
    rb_define_method(gdkDisplay, "peek_event", gdkdisplay_peek_event, 0);
    rb_define_method(gdkDisplay, "put_event", gdkdisplay_put_event, 1);
    rb_define_method(gdkDisplay, "set_double_click_time", gdkdisplay_set_double_click_time, 1);
    rb_define_method(gdkDisplay, "double_click_time",  gdkdisplay_get_double_click_time, 0);
    rb_define_method(gdkDisplay, "button_click_time",  gdkdisplay_get_button_click_time, 0);
    rb_define_method(gdkDisplay, "button_window",  gdkdisplay_get_button_window, 0);
    rb_define_method(gdkDisplay, "button_number",  gdkdisplay_get_button_number, 0);
    rb_define_method(gdkDisplay, "closed?",  gdkdisplay_get_closed, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkDisplay, "button_x",  gdkdisplay_get_button_x, 0);
    rb_define_method(gdkDisplay, "button_y",  gdkdisplay_get_button_y, 0);
    rb_define_method(gdkDisplay, "set_double_click_distance", gdkdisplay_set_double_click_distance, 1);
    rb_define_method(gdkDisplay, "double_click_distance", gdkdisplay_get_double_click_distance, 0);
#endif
    rb_define_method(gdkDisplay, "pointer", gdkdisplay_get_pointer, 0);
    rb_define_method(gdkDisplay, "window_at_pointer", gdkdisplay_get_window_at_pointer, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkDisplay, "supports_cursor_color?", gdkdisplay_supports_cursor_color, 0);
    rb_define_method(gdkDisplay, "supports_cursor_alpha?", gdkdisplay_supports_cursor_alpha, 0);
    rb_define_method(gdkDisplay, "default_cursor_size", gdkdisplay_get_default_cursor_size, 0);
    rb_define_method(gdkDisplay, "maximal_cursor_size", gdkdisplay_get_maximal_cursor_size, 0);
    rb_define_method(gdkDisplay, "default_group", gdkdisplay_get_default_group, 0);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gdkDisplay, "supports_selection_notification?", gdkdisplay_supports_selection_notification, 0);
    rb_define_method(gdkDisplay, "request_selection_notification?", gdkdisplay_request_selection_notification, 1);
    rb_define_method(gdkDisplay, "supports_clipboard_persistence?", gdkdisplay_supports_clipboard_persistence, 0);
    rb_define_method(gdkDisplay, "store_clipboard", gdkdisplay_store_clipboard, 3);
#endif
    rb_define_method(gdkDisplay, "core_pointer", gdkdisplay_get_core_pointer, 0);
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gdkDisplay, "warp_pointer", gdkdisplay_warp_pointer, 3);
#endif

    G_DEF_SETTERS(gdkDisplay);
#ifdef GDK_WINDOWING_X11
    rb_define_method(gdkDisplay, "grab", gdkdisplay_grab, 0);
    rb_define_method(gdkDisplay, "ungrab", gdkdisplay_ungrab, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkDisplay, "register_standard_event_type", gdkdisplay_register_standard_event_type, 2);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    rb_define_method(gdkDisplay, "user_time", gdkdisplay_get_user_time, 0);
    rb_define_method(gdkDisplay, "set_cursor_theme", gdkdisplay_set_cursor_theme, 2);
#endif
    G_DEF_CLASS3("GdkDisplayX11", "DisplayX11", mGdk);
#endif
#endif

#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gdkDisplay, "supports_shapes", gdkdisplay_supports_shapes, 0);
    rb_define_method(gdkDisplay, "supports_input_shapes", gdkdisplay_supports_input_shapes, 0);
#endif
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gdkDisplay, "supports_composite?", gdkdisplay_supports_composite, 0);
    rb_define_method(gdkDisplay, "trigger_tooltip_query", gdkdisplay_trigger_tooltip_query, 0);
#endif
}
