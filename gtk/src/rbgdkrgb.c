/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkrgb.c -

  $Author: geoff_youngs $
  $Date: 2004/11/06 10:47:47 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define RVAL2DRAW(s) GDK_DRAWABLE(RVAL2GOBJ(s))


static VALUE
rgb_draw_rgb_image(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE win, gc, x, y, w, h, dither, buf, rowstride, xdith, ydith;

    rb_scan_args(argc, argv, "92", &win, &gc, &x, &y, &w, &h, &dither, 
                 &buf, &rowstride, &xdith, &ydith);

    if (argc == 9){
        gdk_draw_rgb_image(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
                           NUM2INT(x), NUM2INT(y),
                           NUM2INT(w), NUM2INT(h),
                           RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
                           RVAL2CSTR(buf),
                           NUM2INT(rowstride));
    } else {
        gdk_draw_rgb_image_dithalign(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
                                     NUM2INT(x), NUM2INT(y),
                                     NUM2INT(w), NUM2INT(h),
                                     RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
                                     RVAL2CSTR(buf),
                                     NUM2INT(rowstride),
                                     NUM2INT(xdith), NUM2INT(ydith));
    }

    return self;
}

static VALUE
rgb_draw_indexed_image(self, win, gc, x, y, w, h, dither, buf, rowstride, colors)
    VALUE self, win, gc, x, y, w, h, dither, buf, rowstride, colors;
{
    GdkRgbCmap* cmap;
    guint32* gcolors;
    gint i, n_colors;

    n_colors = RARRAY(colors)->len;

    if (n_colors > 255)
        rb_raise(rb_eArgError, "colors: out of range (0 - 255)");

    gcolors = g_new(guint32, n_colors);
    for (i = 0; i < n_colors; i++) {
        gcolors[i] = NUM2UINT(RARRAY(colors)->ptr[i]);
    }

    cmap = gdk_rgb_cmap_new(gcolors, n_colors);
    
    gdk_draw_indexed_image(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
                           NUM2INT(x), NUM2INT(y),
                           NUM2INT(w), NUM2INT(h),
                           RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
                           RVAL2CSTR(buf), NUM2INT(rowstride), cmap);
    gdk_rgb_cmap_free(cmap);
    return self;
}

static VALUE
rgb_draw_gray_image(self, win, gc, x, y, w, h, dither, buf, rowstride)
    VALUE self, win, gc, x, y, w, h, dither, buf, rowstride;
{
    gdk_draw_gray_image(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
						NUM2INT(x), NUM2INT(y),
						NUM2INT(w), NUM2INT(h),
						RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
						RVAL2CSTR(buf),
						NUM2INT(rowstride));
    return self;
}

static VALUE
rgb_draw_rgb_32_image(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE win, gc, x, y, w, h, dither, buf, rowstride, xdith, ydith;

    rb_scan_args(argc, argv, "92", &win, &gc, &x, &y, &w, &h, &dither, 
                 &buf, &rowstride, &xdith, &ydith);

    if (argc == 9){
        gdk_draw_rgb_32_image(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
                              NUM2INT(x), NUM2INT(y),
                              NUM2INT(w), NUM2INT(h),
                              RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
                              RVAL2CSTR(buf),
                              NUM2INT(rowstride));
    } else {
        gdk_draw_rgb_32_image_dithalign(RVAL2DRAW(win), GDK_GC(RVAL2GOBJ(gc)),
                              NUM2INT(x), NUM2INT(y),
                              NUM2INT(w), NUM2INT(h),
                              RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
                              RVAL2CSTR(buf),
                              NUM2INT(rowstride), NUM2INT(xdith), NUM2INT(ydith));
    }
    return self;
}

static VALUE
rgb_find_color(self, colormap, color)
    VALUE self, colormap, color;
{
    gdk_rgb_find_color(GDK_COLORMAP(RVAL2GOBJ(colormap)),
                       (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
    return self;
}

static VALUE
rgb_set_install(self, install)
    VALUE self, install;
{
    gdk_rgb_set_install(RTEST(install));
    return self;
}

static VALUE
rgb_set_min_colors(self, min_colors)
    VALUE self, min_colors;
{
    gdk_rgb_set_min_colors(NUM2INT(min_colors));
    return self;
}

static VALUE
rgb_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_rgb_get_visual());
}

static VALUE
rgb_get_cmap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_rgb_get_colormap());
}

static VALUE
rgb_ditherable(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_rgb_ditherable());
}

static VALUE
rgb_set_verbose(self, verbose)
    VALUE self, verbose;
{
    gdk_rgb_set_verbose(RTEST(verbose));
    return self;
}

void
Init_gtk_gdk_rgb()
{
    VALUE mGdkRgb = rb_define_module_under(mGdk, "RGB");

    rb_define_module_function(mGdkRgb, "draw_rgb_image", rgb_draw_rgb_image, -1);
    rb_define_module_function(mGdkRgb, "draw_indexed_image", rgb_draw_indexed_image, 10);
    rb_define_module_function(mGdkRgb, "draw_gray_image", rgb_draw_gray_image, 9);
    rb_define_module_function(mGdkRgb, "draw_rgb_32_image", rgb_draw_rgb_32_image, -1);
    rb_define_module_function(mGdkRgb, "find_color", rgb_find_color, 2);
    rb_define_module_function(mGdkRgb, "set_install", rgb_set_install, 1);
    rb_define_module_function(mGdkRgb, "set_min_colors", rgb_set_min_colors, 0);
    rb_define_module_function(mGdkRgb, "visual", rgb_get_visual, 0);
    rb_define_module_function(mGdkRgb, "colormap", rgb_get_cmap, 0);
    rb_define_module_function(mGdkRgb, "ditherable?", rgb_ditherable, 0);
    rb_define_module_function(mGdkRgb, "set_verbose", rgb_set_verbose, 1);

    /* GdkRgbDither */
    G_DEF_CLASS(GDK_TYPE_RGB_DITHER, "Dither", mGdkRgb);
    G_DEF_CONSTANTS(mGdkRgb, GDK_TYPE_RGB_DITHER, "GDK_RGB_");
}
