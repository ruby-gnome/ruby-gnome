/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkrgb.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:06 $

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
    GdkColormap *cmap;
    gdk_rgb_init();
    cmap = gdk_rgb_get_cmap();
    return make_gdkcmap(cmap);
}

static VALUE
rgb_get_visual(self)
    VALUE self;
{
    GdkVisual *visual;
    gdk_rgb_init();
    visual = gdk_rgb_get_visual();
    return make_gdkvisual(visual);
}

void
Init_gtk_gdk_rgb()
{
    mGdkRgb = rb_define_module_under(mGdk, "RGB");
    rb_define_const(mGdk, "Rgb", mGdkRgb);

    rb_define_module_function(mGdkRgb, "get_cmap", rgb_get_cmap, 0);
    rb_define_module_function(mGdkRgb, "get_visual", rgb_get_visual, 0);
}
