/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdk.c -

  $Author: mutoh $
  $Date: 2002/09/10 17:42:53 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE mGdk;

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
}
