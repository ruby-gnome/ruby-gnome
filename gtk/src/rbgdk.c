/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk.c -

  $Author: mutoh $
  $Date: 2005/05/07 19:12:52 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE mGdk;

/* We don't need them.
void        gdk_init                        (gint *argc,
                                             gchar ***argv);
gboolean    gdk_init_check                  (gint *argc,
                                             gchar ***argv);
void        gdk_parse_args                  (gint *argc,
                                             gchar ***argv);
*/

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdk_s_get_display_arg_name(self)
    VALUE self;
{
    return CSTR2RVAL(gdk_get_display_arg_name());
}
#endif

static VALUE
gdk_s_set_locale(self)
    VALUE self;
{
    return CSTR2RVAL(gdk_set_locale());
}

static VALUE
gdk_s_set_sm_client_id(self, id)
    VALUE self, id;
{
    gdk_set_sm_client_id(NIL_P(id) ? NULL : RVAL2CSTR(id));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdk_s_notify_startup_complete(self)
    VALUE self;
{
    gdk_notify_startup_complete();
    return self;
}
#endif

static VALUE
gdk_s_get_program_class(self)
    VALUE self;
{
    return CSTR2RVAL(gdk_get_program_class());
}

static VALUE
gdk_s_set_program_class(self, program_class)
    VALUE self, program_class;
{
    gdk_set_program_class(RVAL2CSTR(program_class));
    return self;
}

static VALUE
gdk_s_get_display(self)
    VALUE self;
{
    return CSTR2RVAL(gdk_get_display());
}

#ifdef HAVE_X11_XLIB_H 
#ifdef HAVE_XGETERRORTEXT
#include <X11/Xlib.h>
#include <errno.h>

static VALUE rb_x_error;
static VALUE rb_x_io_error;

static int
rbgdk_x_error(display, error) 
    Display* display;
    XErrorEvent* error;
{
     gchar buf[64];
                                                                                
     XGetErrorText(display, error->error_code, buf, 63);

     rb_funcall((VALUE)rb_x_error, id_call, 4, INT2NUM(error->serial),
                INT2NUM(error->error_code), INT2NUM(error->request_code),
                INT2NUM(error->minor_code));
     return 0;
}

static int
rbgdk_x_io_error(display) 
    Display* display;
{
    int errno_saved = errno;
    const gchar* disp;
    const gchar* error;

#if GTK_CHECK_VERSION(2,2,0)
    disp = display ? DisplayString(display) : gdk_get_display_arg_name();
#else
    disp = DisplayString(display);
#endif
    if (! disp)
        disp = "(none)";

    error = g_strerror(errno_saved);
    
    rb_funcall((VALUE)rb_x_io_error, id_call, 3, CSTR2RVAL(disp), 
               INT2NUM(errno), CSTR2RVAL(error));
    return 0;
}
#endif 
#endif 

static VALUE
gdk_s_set_x_error_handler(self)
    VALUE self;
{
#ifdef HAVE_XGETERRORTEXT
    rb_x_error = G_BLOCK_PROC();
    G_RELATIVE(self, rb_x_error);
    XSetErrorHandler(rbgdk_x_error);
#else
    rb_warn("Not supported on this environment.");
#endif
    return self;
}

static VALUE
gdk_s_set_x_io_error_handler(self)
    VALUE self;
{
#ifdef HAVE_XGETERRORTEXT
    rb_x_io_error = G_BLOCK_PROC();
    G_RELATIVE(self, rb_x_io_error);
    XSetIOErrorHandler(rbgdk_x_io_error);
#else
    rb_warn("Not supported on this environment.");
#endif
    return self;
}

static VALUE
gdk_s_flush(self)
    VALUE self;
{
    gdk_flush();
    return self;
}

static VALUE
gdk_s_screen_width(self)
    VALUE self;
{
    return INT2NUM(gdk_screen_width());
}

static VALUE
gdk_s_screen_width_mm(self)
    VALUE self;
{
    return INT2NUM(gdk_screen_width_mm());
}

static VALUE
gdk_s_screen_height(self)
   VALUE self;
{
    return INT2NUM(gdk_screen_height());
}

static VALUE
gdk_s_screen_height_mm(self)
    VALUE self;
{
    return INT2NUM(gdk_screen_height_mm());
}

static VALUE
gdk_s_pointer_grab(self, win, owner_events, event_mask, confine_to, cursor, time)
    VALUE self, win, owner_events, event_mask, confine_to, cursor, time;
{
    return GENUM2RVAL(gdk_pointer_grab(GDK_WINDOW(RVAL2GOBJ(win)),
                     RTEST(owner_events),
                     RVAL2GFLAGS(event_mask, GDK_TYPE_EVENT_MASK),
                     NIL_P(confine_to)?NULL:GDK_WINDOW(RVAL2GOBJ(confine_to)),
                     NIL_P(cursor)?NULL:(GdkCursor*)RVAL2BOXED(cursor, GDK_TYPE_CURSOR),
                     NUM2INT(time)), GDK_TYPE_GRAB_STATUS);
}

static VALUE
gdk_s_pointer_ungrab(self, time)
    VALUE self, time;
{
    gdk_pointer_ungrab(NUM2INT(time));
    return self;
}

static VALUE
gdk_s_keyboard_grab(self, win, owner_events, time)
    VALUE self, win, owner_events, time;
{
    return GENUM2RVAL(gdk_keyboard_grab(GDK_WINDOW(RVAL2GOBJ(win)), 
                                        RTEST(owner_events), NUM2INT(time)), 
                      GDK_TYPE_GRAB_STATUS);
}

static VALUE
gdk_s_keyboard_ungrab(self, time)
    VALUE self, time;
{
    gdk_keyboard_ungrab(NUM2INT(time));
    return self;
}

static VALUE
gdk_s_pointer_is_grabbed(self)
    VALUE self;
{
    return (gdk_pointer_is_grabbed() ? Qtrue : Qfalse);
}

static VALUE
gdk_s_set_double_click_time(self, msec)
    VALUE self, msec;
{
    gdk_set_double_click_time(NUM2UINT(msec));
    return self;
}

static VALUE
gdk_s_beep(self)
    VALUE self;
{
    gdk_beep();
    return self;
}

static VALUE
gdk_s_error_trap_push(self)
    VALUE self;
{
    gdk_error_trap_push();
    return self;
}

static VALUE
gdk_s_error_trap_pop(self)
    VALUE self;
{
    gdk_error_trap_pop();
    return self;
}

static VALUE
gdk_s_windowing_x11(self)
    VALUE self;
{
#ifdef GDK_WINDOWING_X11
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
gdk_s_windowing_win32(self)
    VALUE self;
{
#ifdef GDK_WINDOWING_WIN32
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
gdk_s_windowing_fb(self)
    VALUE self;
{
#ifdef GDK_WINDOWING_FB
    return Qtrue;
#else
    return Qfalse;
#endif
}

static VALUE
gdk_m_target(self)
    VALUE self;
{
    return CSTR2RVAL(RUBY_GTK2_TARGET);
}

void
Init_gtk_gdk()
{
    mGdk = rb_define_module("Gdk");

#if GTK_CHECK_VERSION(2,2,0)
    rb_define_module_function(mGdk, "display_arg_name", gdk_s_get_display_arg_name, 0);
#endif
    rb_define_module_function(mGdk, "set_locale", gdk_s_set_locale, 0);
    rb_define_module_function(mGdk, "set_sm_client_id", gdk_s_set_sm_client_id, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_module_function(mGdk, "notify_startup_complete", gdk_s_notify_startup_complete, 0);
#endif
    rb_define_module_function(mGdk, "program_class", gdk_s_get_program_class, 0);
    rb_define_module_function(mGdk, "set_program_class", gdk_s_set_program_class, 1);
    rb_define_module_function(mGdk, "display", gdk_s_get_display, 0);

    rb_define_module_function(mGdk, "set_x_error_handler", gdk_s_set_x_error_handler , 0);
    rb_define_module_function(mGdk, "set_x_io_error_handler", gdk_s_set_x_io_error_handler , 0);
    rb_define_module_function(mGdk, "screen_width", gdk_s_screen_width, 0);
    rb_define_module_function(mGdk, "screen_width_mm", gdk_s_screen_width_mm, 0);
    rb_define_module_function(mGdk, "screen_height", gdk_s_screen_height, 0);
    rb_define_module_function(mGdk, "screen_height_mm", gdk_s_screen_height_mm, 0);
    rb_define_module_function(mGdk, "beep", gdk_s_beep, 0);
    rb_define_module_function(mGdk, "flush", gdk_s_flush, 0);
    rb_define_module_function(mGdk, "set_double_click_time", gdk_s_set_double_click_time, 1);
    rb_define_module_function(mGdk, "pointer_grab", gdk_s_pointer_grab, 6);
    rb_define_module_function(mGdk, "pointer_ungrab", gdk_s_pointer_ungrab, 1);
    rb_define_module_function(mGdk, "keyboard_grab", gdk_s_keyboard_grab, 3);
    rb_define_module_function(mGdk, "keyboard_ungrab", gdk_s_keyboard_ungrab, 1);
    rb_define_module_function(mGdk, "pointer_is_grabbed?", gdk_s_pointer_is_grabbed, 0);
    rb_define_module_function(mGdk, "error_trap_push", gdk_s_error_trap_push, 0);
    rb_define_module_function(mGdk, "error_trap_pop", gdk_s_error_trap_pop, 0);
    rb_define_module_function(mGdk, "windowing_x11?", gdk_s_windowing_x11, 0);
    rb_define_module_function(mGdk, "windowing_win32?", gdk_s_windowing_win32, 0);
    rb_define_module_function(mGdk, "windowing_fb?", gdk_s_windowing_fb, 0);

    rb_define_module_function(mGdk, "target", gdk_m_target, 0);
    
    /* GdkGrabStatus */
    G_DEF_CLASS(GDK_TYPE_GRAB_STATUS, "GrabStatus", mGdk);
    G_DEF_CONSTANTS(mGdk, GDK_TYPE_GRAB_STATUS, "GDK_");

    G_DEF_SETTERS(mGdk);
}

