/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdk.c -

  $Author: mutoh $
  $Date: 2002/09/09 14:24:50 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE mGdk;

/*
 * Gdk
 */
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
gdk_s_beep(self)
    VALUE self;
{
    gdk_beep();
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
gdk_s_events_pending(self)
    VALUE self;
{
    return gdk_events_pending() ? Qtrue : Qfalse;
}

extern void Init_gtk_gdk_const();
extern void Init_gtk_gdk_colormap();
extern void Init_gtk_gdk_color();
extern void Init_gtk_gdk_cursor();
extern void Init_gtk_gdk_visual();
extern void Init_gtk_gdk_draw();
extern void Init_gtk_gdk_event();
#ifndef GTK_DISABLE_DEPRECATED
extern void Init_gtk_gdk_font();
#endif
extern void Init_gtk_gdk_gc();
extern void Init_gtk_gdk_image();
extern void Init_gtk_gdk_keyval();
extern void Init_gtk_gdk_pixmap();
extern void Init_gtk_gdk_rectangle();
extern void Init_gtk_gdk_region();
extern void Init_gtk_gdk_rgb();
extern void Init_gtk_gdk_window();
extern void Init_gtk_gdk_geometry();
extern void Init_gtk_gdkkeysyms();
extern void Init_gtk_gdk_dnd();
extern void Init_gtk_gdk_atom();
extern void Init_gtk_gdk_selection();

void
Init_gtk_gdk()
{
    /* Gdk */
    mGdk = rb_define_module("Gdk");

    rb_define_singleton_method(mGdk, "screen_width", gdk_s_screen_width, 0);
    rb_define_singleton_method(mGdk, "screen_width_mm", gdk_s_screen_width_mm, 0);
    rb_define_singleton_method(mGdk, "screen_height", gdk_s_screen_height, 0);
    rb_define_singleton_method(mGdk, "screen_height_mm", gdk_s_screen_height_mm, 0);
    rb_define_singleton_method(mGdk, "beep", gdk_s_beep, 0);
    rb_define_singleton_method(mGdk, "flush", gdk_s_flush, 0);
    rb_define_singleton_method(mGdk, "events_pending", gdk_s_events_pending, 0);

    Init_gtk_gdk_const();
    Init_gtk_gdk_colormap();
    Init_gtk_gdk_color();
    Init_gtk_gdk_cursor();
    Init_gtk_gdk_draw();
    Init_gtk_gdk_event();
#ifndef GTK_DISABLE_DEPRECATED
    Init_gtk_gdk_font();
#endif
    Init_gtk_gdk_gc();
    Init_gtk_gdk_dnd();
    Init_gtk_gdk_image();
    Init_gtk_gdk_keyval();
    Init_gtk_gdk_visual();
    Init_gtk_gdk_pixmap();
    Init_gtk_gdk_window();
    Init_gtk_gdk_geometry();
    Init_gtk_gdk_rectangle();
    Init_gtk_gdk_region();
    Init_gtk_gdk_rgb();
    Init_gtk_gdkkeysyms();
    Init_gtk_gdk_atom();
    Init_gtk_gdk_selection();
}
