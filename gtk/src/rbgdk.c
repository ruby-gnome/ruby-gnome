/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk.c -

  $Author: sakai $
  $Date: 2003/08/20 17:07:02 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE mGdk;

/*
void        gdk_init                        (gint *argc,
                                             gchar ***argv);
gboolean    gdk_init_check                  (gint *argc,
                                             gchar ***argv);
void        gdk_parse_args                  (gint *argc,
                                             gchar ***argv);
gchar*      gdk_get_display_arg_name        (void);
gchar*      gdk_set_locale                  (void);
void        gdk_set_sm_client_id            (const gchar *sm_client_id);
void        gdk_exit                        (gint error_code);

G_CONST_RETURN char* gdk_get_program_class  (void);
void        gdk_set_program_class           (const char *program_class);

gchar*      gdk_get_display                 (void);

*/

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

    disp = display ? DisplayString(display) : gdk_get_display_arg_name();
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
    rb_warn("Not supported in this environment.");
#endif
    return Qnil;
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
    rb_warn("Not supported in this environment.");
#endif
    return Qnil;
}

static VALUE
gdk_s_flush(self)
    VALUE self;
{
    gdk_flush();
    return Qnil;
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
    return INT2NUM(gdk_pointer_grab(GDK_WINDOW(RVAL2GOBJ(win)),
                     RTEST(owner_events),
                     NUM2INT(event_mask),
                     NIL_P(confine_to)?NULL:GDK_WINDOW(RVAL2GOBJ(confine_to)),
                     NIL_P(cursor)?NULL:(GdkCursor*)RVAL2BOXED(cursor, GDK_TYPE_CURSOR),
                     NUM2INT(time)));
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
    return INT2NUM(gdk_keyboard_grab(GDK_WINDOW(RVAL2GOBJ(win)), RTEST(owner_events), NUM2INT(time)));
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
    return Qnil;
}

/*
gboolean    gdk_get_use_xshm                (void);
void        gdk_set_use_xshm                (gboolean use_xshm);

void        gdk_error_trap_push             (void);
gint        gdk_error_trap_pop              (void);
*/
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
    return rb_str_new2(RUBY_GTK2_TARGET);
}

void
Init_gtk_gdk()
{
    mGdk = rb_define_module("Gdk");

#ifdef HAVE_X11_XLIB_H
    rb_define_module_function(mGdk, "set_x_error_handler", gdk_s_set_x_error_handler , 0);
    rb_define_module_function(mGdk, "set_x_io_error_handler", gdk_s_set_x_io_error_handler , 0);
#endif
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
    rb_define_module_function(mGdk, "windowing_x11?", gdk_s_windowing_x11, 0);
    rb_define_module_function(mGdk, "windowing_win32?", gdk_s_windowing_win32, 0);
    rb_define_module_function(mGdk, "windowing_fb?", gdk_s_windowing_fb, 0);

    rb_define_module_function(mGdk, "target", gdk_m_target, 0);
    
    /* GdkGrabStatus */
    G_DEF_CLASS(GDK_TYPE_GRAB_STATUS, "GrabStatus", mGdk);
    G_DEF_CONSTANTS(mGdk, GDK_TYPE_GRAB_STATUS, "GDK_");

    G_DEF_SETTERS(mGdk);
}

