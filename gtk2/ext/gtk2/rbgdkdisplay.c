/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
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

#if GTK_CHECK_VERSION(2,2,0)

#define RG_TARGET_NAMESPACE cDisplay
#define _SELF(i) GDK_DISPLAY_OBJECT(RVAL2GOBJ(i))

static VALUE
rg_close(VALUE self)
{
    gdk_display_close(_SELF(self));
    return self;
}

static VALUE
rg_s_open(G_GNUC_UNUSED VALUE self, VALUE display_name)
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
            rb_ensure(rb_yield, display, rg_close, display);
            return Qnil;
        } else {
            return display;
        }
    }
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    GdkDisplay* gdisplay = gdk_display_get_default();
     if (! gdisplay)
        rb_raise(rb_eRuntimeError, "No default display is found.");

    return GOBJ2RVAL(gdisplay);
}

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(gdk_display_get_name(_SELF(self)));
}

static VALUE
rg_n_screens(VALUE self)
{
    return INT2NUM(gdk_display_get_n_screens(_SELF(self)));
}

static VALUE
rg_get_screen(VALUE self, VALUE num)
{
    return GOBJ2RVAL(gdk_display_get_screen(_SELF(self), NUM2INT(num)));
}

static VALUE
rg_default_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_display_get_default_screen(_SELF(self)));
}

static VALUE
rg_pointer_ungrab(VALUE self, VALUE time)
{
    gdk_display_pointer_ungrab(_SELF(self), NUM2ULONG(time));
    return self;
}

static VALUE
rg_keyboard_ungrab(VALUE self, VALUE time)
{
    gdk_display_keyboard_ungrab(_SELF(self), NUM2ULONG(time));
    return self;
}

static VALUE
rg_pointer_grabbed_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_pointer_is_grabbed(_SELF(self)));
}

static VALUE
rg_beep(VALUE self)
{
    gdk_display_beep(_SELF(self));
    return self;
}

static VALUE
rg_sync(VALUE self)
{
    gdk_display_sync(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_flush(VALUE self)
{
    gdk_display_flush(_SELF(self));
    return self;
}
#endif

static VALUE
rg_devices(VALUE self)
{
    return rbgutil_glist2ary(gdk_display_list_devices(_SELF(self)));
}

static VALUE
rg_event(VALUE self)
{
    return GEV2RVAL(gdk_display_get_event(_SELF(self)));
}

static VALUE
rg_peek_event(VALUE self)
{
    return GEV2RVAL(gdk_display_peek_event(_SELF(self)));
}

static VALUE
rg_put_event(VALUE self, VALUE event)
{
    gdk_display_put_event(_SELF(self), RVAL2GEV(event));
    return self;
}

/* Don't implement this.
static GdkFilterReturn
filter_func(GdkXEvent xevent, GdkEvent event, gpointer func)
{
    return GENUM2RVAL(rb_funcall((VALUE)func, id_call, 2, 
                                 ????, GEV2RVAL(event)), 
                      GDK_TYPE_FILTER_RETURN);
}

static VALUE
gdkdisplay_add_client_message_filter(VALUE self, VALUE message_type)
{
    VALUE func = RB_BLOCK_PROC();
    G_RELATIVE(self, func);
    gdk_display_add_client_message_filter(_SELF(self), RVAL2ATOM(message_type),
                                          filter_func, func);
    return self;
}
*/

static VALUE
rg_set_double_click_time(VALUE self, VALUE msec)
{
    gdk_display_set_double_click_time(_SELF(self), NUM2UINT(msec));
    return self;
}

static VALUE
rg_double_click_time(VALUE self)
{
    return UINT2NUM(_SELF(self)->double_click_time);
}

static VALUE
rg_button_click_time(VALUE self)
{
    return rb_ary_new3(2,
                       UINT2NUM(_SELF(self)->button_click_time[0]),
                       UINT2NUM(_SELF(self)->button_click_time[1]));
}

static VALUE
rg_button_window(VALUE self)
{
    return rb_ary_new3(2,
                       GOBJ2RVAL(_SELF(self)->button_window[0]),
                       GOBJ2RVAL(_SELF(self)->button_window[1]));
}

static VALUE
rg_button_number(VALUE self)
{
    return rb_ary_new3(2,
                       INT2NUM(_SELF(self)->button_number[0]),
                       INT2NUM(_SELF(self)->button_number[1]));
}

static VALUE
rg_closed_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->closed);
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_button_x(VALUE self)
{
    return rb_ary_new3(2,
                       INT2NUM(_SELF(self)->button_x[0]),
                       INT2NUM(_SELF(self)->button_x[1]));
}

static VALUE
rg_button_y(VALUE self)
{
    return rb_ary_new3(2,
                       INT2NUM(_SELF(self)->button_y[0]),
                       INT2NUM(_SELF(self)->button_y[1]));
}

static VALUE
rg_set_double_click_distance(VALUE self, VALUE distance)
{
    gdk_display_set_double_click_distance(_SELF(self), NUM2UINT(distance));
    return self;
}

static VALUE
rg_double_click_distance(VALUE self)
{
    return UINT2NUM(_SELF(self)->double_click_distance);
}
#endif

static VALUE
rg_pointer(VALUE self)
{
    GdkScreen *screen;
    int x,y;
    GdkModifierType mask;

    gdk_display_get_pointer(_SELF(self), &screen, &x, &y, &mask);

    return rb_ary_new3(4, GOBJ2RVAL(screen), INT2NUM(x), INT2NUM(y), INT2NUM(mask));
}
static VALUE
rg_window_at_pointer(VALUE self)
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
rg_supports_cursor_color_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_supports_cursor_color(_SELF(self)));
}

static VALUE
rg_supports_cursor_alpha_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_supports_cursor_alpha(_SELF(self)));
}

static VALUE
rg_default_cursor_size(VALUE self)
{
    return UINT2NUM(gdk_display_get_default_cursor_size(_SELF(self)));
}

static VALUE
rg_maximal_cursor_size(VALUE self)
{
    guint width, height;
    gdk_display_get_maximal_cursor_size(_SELF(self), &width, &height);
    return rb_assoc_new(UINT2NUM(width), UINT2NUM(height));
}

static VALUE
rg_default_group(VALUE self)
{
    return GOBJ2RVAL(gdk_display_get_default_group(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_supports_selection_notification_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_supports_selection_notification(_SELF(self)));
}

static VALUE
rg_request_selection_notification_p(VALUE self, VALUE selection)
{
    return CBOOL2RVAL(gdk_display_request_selection_notification(_SELF(self), 
                                                                 RVAL2ATOM(selection)));
}

static VALUE
rg_supports_clipboard_persistence_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_supports_clipboard_persistence(_SELF(self)));
}

static VALUE
rg_store_clipboard(VALUE self, VALUE rbclipboard_window, VALUE rbtime_, VALUE rbtargets)
{
    GdkDisplay *display = _SELF(self);
    GdkWindow *clipboard_window = GDK_WINDOW(RVAL2GOBJ(rbclipboard_window));
    guint32 time_ = NUM2UINT(rbtime_);
    long n;
    GdkAtom *targets = RVAL2GDKATOMS(rbtargets, &n);

    gdk_display_store_clipboard(display, clipboard_window, time_, targets, n);

    g_free(targets);

    return self;
}
#endif

static VALUE
rg_core_pointer(VALUE self)
{
    return GOBJ2RVAL(gdk_display_get_core_pointer(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_warp_pointer(VALUE self, VALUE screen, VALUE x, VALUE y)
{
    gdk_display_warp_pointer(_SELF(self), RVAL2GOBJ(screen), NUM2INT(x), NUM2INT(y));
    return self;
}
#endif

#ifdef GDK_WINDOWING_X11
static VALUE
rg_grab(VALUE self)
{
    gdk_x11_display_grab(_SELF(self));
    return self;
}
static VALUE
rg_ungrab(VALUE self)
{
    gdk_x11_display_ungrab(_SELF(self));
    return self;
}
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_register_standard_event_type(VALUE self, VALUE event_base, VALUE n_events)
{
    gdk_x11_register_standard_event_type(_SELF(self),
                                         NUM2INT(event_base), NUM2INT(n_events));
    return self;
}
#endif
#if GTK_CHECK_VERSION(2,8,0)
static VALUE
rg_user_time(VALUE self)
{
    return UINT2NUM(gdk_x11_display_get_user_time(_SELF(self)));
}

static VALUE
rg_set_cursor_theme(VALUE self, VALUE theme, VALUE size)
{
    gdk_x11_display_set_cursor_theme(_SELF(self), RVAL2CSTR(theme), NUM2INT(size));
    return self;
}
#endif

#  if GTK_CHECK_VERSION(2, 12, 0)
/*
*** need gdk_x11_display_broadcast_startup_messagev() ***

typedef struct _StartupMessageParameterData {
    gchar **parameters;
    guint i;
} StartupMessageParameterData;

static int
collect_parameter(VALUE key, VALUE value, VALUE data)
{
    StartupMessageParameterData *parameter_data;
    parameter_data = (StartupMessageParameterData *)data;

    parameter_data->parameters[parameter_data->i] = RVAL2CSTR(key);
    parameter_data->i++;
    parameter_data->parameters[parameter_data->i] = RVAL2CSTR(value);
    parameter_data->i++;

    return ST_CONTINUE;
}

static VALUE
rg_broadcast_startup_message(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_message_type, rb_parameters;
    char *message_type;
    guint n_parameters;
    gchar **parameters;

    rb_scan_args(argc, argv, "11", &rb_message_type, &rb_parameters);

    message_type = RVAL2CSTR(rb_message_type);
    if (NIL_P(rb_parameters)) {
    n_parameters = 0;
    parameters = NULL;
    }
    else {
    StartupMessageParameterData data;

    Check_Type(rb_parameters, T_HASH);
    n_parameters = NUM2UINT(rb_funcall(rb_parameters, rb_intern("size"), 0));
    parameters = ALLOCA_N(gchar *, n_parameters * 2);
    data.i = 0;
    data.parameters = parameters;
    rb_hash_foreach(rb_parameters, collect_parameter, (VALUE)&data);
    }

    gdk_x11_display_broadcast_startup_messagev(_SELF(self),
                           message_type,
                           n_parameters,
                           parameters);

    return self;
}
*/

static VALUE
rg_startup_notification_id(VALUE self)
{
    return CSTR2RVAL(gdk_x11_display_get_startup_notification_id(_SELF(self)));
}
#  endif

#endif
#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_supports_shapes_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_supports_shapes(_SELF(self)));
}

static VALUE
rg_supports_input_shapes_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_supports_input_shapes(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2, 12, 0)
static VALUE
rg_supports_composite_p(VALUE self)
{
    return CBOOL2RVAL(gdk_display_supports_composite(_SELF(self)));
}

static VALUE
rg_trigger_tooltip_query(VALUE self)
{
    gtk_tooltip_trigger_tooltip_query(_SELF(self));
    return self;
}
#endif

void 
Init_gtk_gdk_display(VALUE mGdk)
{
#if GTK_CHECK_VERSION(2,2,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_DISPLAY, "Display", mGdk);

    RG_DEF_SMETHOD(open, 1);
    RG_DEF_SMETHOD(default, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(n_screens, 0);
    RG_DEF_METHOD(get_screen, 1);
    RG_DEF_ALIAS("[]", "get_screen");
    RG_DEF_METHOD(default_screen, 0);

    RG_DEF_METHOD(pointer_ungrab, 1);
    RG_DEF_METHOD(keyboard_ungrab, 1);
    RG_DEF_METHOD_P(pointer_grabbed, 0);

    RG_DEF_METHOD(beep, 0);
    RG_DEF_METHOD(sync, 0);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(flush, 0);
#endif
    RG_DEF_METHOD(close, 0);

    RG_DEF_METHOD(devices, 0);
    RG_DEF_METHOD(event, 0);
    RG_DEF_METHOD(peek_event, 0);
    RG_DEF_METHOD(put_event, 1);
    RG_DEF_METHOD(set_double_click_time, 1);
    RG_DEF_METHOD(double_click_time, 0);
    RG_DEF_METHOD(button_click_time, 0);
    RG_DEF_METHOD(button_window, 0);
    RG_DEF_METHOD(button_number, 0);
    RG_DEF_METHOD_P(closed, 0);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(button_x, 0);
    RG_DEF_METHOD(button_y, 0);
    RG_DEF_METHOD(set_double_click_distance, 1);
    RG_DEF_METHOD(double_click_distance, 0);
#endif
    RG_DEF_METHOD(pointer, 0);
    RG_DEF_METHOD(window_at_pointer, 0);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD_P(supports_cursor_color, 0);
    RG_DEF_METHOD_P(supports_cursor_alpha, 0);
    RG_DEF_METHOD(default_cursor_size, 0);
    RG_DEF_METHOD(maximal_cursor_size, 0);
    RG_DEF_METHOD(default_group, 0);
#endif
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD_P(supports_selection_notification, 0);
    RG_DEF_METHOD_P(request_selection_notification, 1);
    RG_DEF_METHOD_P(supports_clipboard_persistence, 0);
    RG_DEF_METHOD(store_clipboard, 3);
#endif
    RG_DEF_METHOD(core_pointer, 0);
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(warp_pointer, 3);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#ifdef GDK_WINDOWING_X11
    RG_DEF_METHOD(grab, 0);
    RG_DEF_METHOD(ungrab, 0);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(register_standard_event_type, 2);
#endif
#if GTK_CHECK_VERSION(2,8,0)
    RG_DEF_METHOD(user_time, 0);
    RG_DEF_METHOD(set_cursor_theme, 2);
#endif
#  if GTK_CHECK_VERSION(2, 12, 0)
/*
    RG_DEF_METHOD(broadcast_startup_message, -1);
*/
    RG_DEF_METHOD(startup_notification_id, 0);
#  endif
    G_DEF_CLASS3("GdkDisplayX11", "DisplayX11", mGdk);
#endif
#endif

#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD_P(supports_shapes, 0);
    RG_DEF_METHOD_P(supports_input_shapes, 0);
#endif
#if GTK_CHECK_VERSION(2, 12, 0)
    RG_DEF_METHOD_P(supports_composite, 0);
    RG_DEF_METHOD(trigger_tooltip_query, 0);
#endif
}
