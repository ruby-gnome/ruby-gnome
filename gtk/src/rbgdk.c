/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:09 $

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

/* Move to Gdk::Window.
GdkGrabStatus gdk_pointer_grab              (GdkWindow *window,
                                             gboolean owner_events,
                                             GdkEventMask event_mask,
                                             GdkWindow *confine_to,
                                             GdkCursor *cursor,
                                             guint32 time);
void        gdk_pointer_ungrab              (guint32 time);
GdkGrabStatus gdk_keyboard_grab             (GdkWindow *window,
                                             gboolean owner_events,
                                             guint32 time);
void        gdk_keyboard_ungrab             (guint32 time);

*/

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

void
Init_gtk_gdk()
{
    mGdk = rb_define_module("Gdk");

    rb_define_module_function(mGdk, "screen_width", gdk_s_screen_width, 0);
    rb_define_module_function(mGdk, "screen_width_mm", gdk_s_screen_width_mm, 0);
    rb_define_module_function(mGdk, "screen_height", gdk_s_screen_height, 0);
    rb_define_module_function(mGdk, "screen_height_mm", gdk_s_screen_height_mm, 0);
    rb_define_module_function(mGdk, "beep", gdk_s_beep, 0);
    rb_define_module_function(mGdk, "flush", gdk_s_flush, 0);
    rb_define_module_function(mGdk, "set_double_click_time", gdk_s_set_double_click_time, 1);
    rb_define_module_function(mGdk, "pointer_grabbed?", gdk_s_pointer_is_grabbed, 0);

    G_DEF_SETTERS(mGdk);
}
