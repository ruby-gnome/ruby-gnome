/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkrgb.c -

  $Author: mutoh $
  $Date: 2002/07/06 20:56:15 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Gdk::Rgb
 */

VALUE mGdkRgb;

static VALUE
rgb_get_cmap(self)
    VALUE self;
{
    gdk_rgb_init();
    return GOBJ2RVAL(gdk_rgb_get_cmap());
}

static VALUE
rgb_get_visual(self)
    VALUE self;
{
    gdk_rgb_init();
    return GOBJ2RVAL(gdk_rgb_get_visual());
}

void
Init_gtk_gdk_rgb()
{
    mGdkRgb = rb_define_module_under(mGdk, "RGB");
    rb_define_const(mGdk, "Rgb", mGdkRgb);

    rb_define_module_function(mGdkRgb, "cmap", rgb_get_cmap, 0);
    rb_define_module_function(mGdkRgb, "visual", rgb_get_visual, 0);
}
