/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkrgb.c -

  $Author: sakai $
  $Date: 2003/07/20 05:05:08 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define RVAL2DRAW(s) GDK_DRAWABLE(RVAL2GOBJ(s))

static VALUE
rgb_get_cmap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_rgb_get_colormap());
}

static VALUE
rgb_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_rgb_get_visual());
}

static VALUE
gdkdraw_draw_rgb_image(self, win, gc, x, y, w, h, dither, buf, rowstride)
    VALUE self, win, gc, x, y, w, h, dither, buf, rowstride;
{
    gdk_draw_rgb_image(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
					   NUM2INT(x), NUM2INT(y),
					   NUM2INT(w), NUM2INT(h),
					   NUM2INT(dither),
					   RVAL2CSTR(buf),
					   NUM2INT(rowstride));
    return self;
}

static VALUE
gdkdraw_draw_rgb_image_dithalign(self, win, gc, x, y, w, h, dither, buf, rowstride, 
								 xdith, ydith)
    VALUE self, win, gc, x, y, w, h, dither, buf, rowstride, xdith, ydith;
{
    gdk_draw_rgb_image_dithalign(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
								 NUM2INT(x), NUM2INT(y),
								 NUM2INT(w), NUM2INT(h),
								 NUM2INT(dither),
								 RVAL2CSTR(buf),
								 NUM2INT(rowstride),
								 NUM2INT(xdith), NUM2INT(ydith));
    return self;
}

static VALUE
gdkdraw_draw_gray_image(self, win, gc, x, y, w, h, dither, buf, rowstride)
    VALUE self, win, gc, x, y, w, h, dither, buf, rowstride;
{
    gdk_draw_gray_image(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
						NUM2INT(x), NUM2INT(y),
						NUM2INT(w), NUM2INT(h),
						NUM2INT(dither),
						RVAL2CSTR(buf),
						NUM2INT(rowstride));
    return self;
}

static VALUE
gdkdraw_draw_rgb_32_image(self, win, gc, x, y, w, h, dither, buf, rowstride)
    VALUE self, win, gc, x, y, w, h, dither, buf, rowstride;
{
    gdk_draw_rgb_32_image(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
						  NUM2INT(x), NUM2INT(y),
						  NUM2INT(w), NUM2INT(h),
						  NUM2INT(dither),
						  RVAL2CSTR(buf),
						  NUM2INT(rowstride));
    return self;
}

void
Init_gtk_gdk_rgb()
{
    VALUE mGdkRgb = rb_define_module_under(mGdk, "RGB");

    rb_define_module_function(mGdkRgb, "cmap", rgb_get_cmap, 0);
    rb_define_module_function(mGdkRgb, "visual", rgb_get_visual, 0);
    rb_define_module_function(mGdkRgb, "draw_rgb_image", gdkdraw_draw_rgb_image, 9);
    rb_define_module_function(mGdkRgb, "draw_rgb_image_dithalign", gdkdraw_draw_rgb_image_dithalign, 11);
    rb_define_module_function(mGdkRgb, "draw_gray_image", gdkdraw_draw_gray_image, 9);
    rb_define_module_function(mGdkRgb, "draw_rgb_32_image", gdkdraw_draw_rgb_32_image, 9);

    /* GdkRgbDither */
    G_DEF_CONSTANTS(mGdkRgb, GDK_TYPE_RGB_DITHER, "GDK_RGB_");
}
