/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkcairo.c -

  $Author: ktou $
  $Date: 2006/12/17 07:55:05 $

  Copyright (C) 2005 Kouhei Sutou
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,8,0)
#  ifdef HAVE_RB_CAIRO_H

#include <rb_cairo.h>

#define _SELF(self) RVAL2CRCONTEXT(self)

static VALUE
gdkdraw_cairo_set_source_color(self, color)
    VALUE self, color;
{
    gdk_cairo_set_source_color(_SELF(self), 
                               (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

static VALUE
gdkdraw_cairo_set_source_pixbuf(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE pixbuf, pixbuf_x, pixbuf_y;

    rb_scan_args(argc, argv, "12", &pixbuf, &pixbuf_x, &pixbuf_y);

    gdk_cairo_set_source_pixbuf(_SELF(self), GDK_PIXBUF(RVAL2GOBJ(pixbuf)),
                                NIL_P(pixbuf_x) ? 0 : NUM2DBL(pixbuf_x),
                                NIL_P(pixbuf_y) ? 0 : NUM2DBL(pixbuf_y));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gdkdraw_cairo_set_source_pixmap(self, pixmap, pixmap_x, pixmap_y)
    VALUE self, pixmap, pixmap_x, pixmap_y;
{
    gdk_cairo_set_source_pixmap(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                                NUM2DBL(pixmap_x), NUM2DBL(pixmap_y));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}
#endif

static VALUE
gdkdraw_cairo_rectangle(self, rectangle)
    VALUE self, rectangle;
{
    gdk_cairo_rectangle(_SELF(self), 
                        (GdkRectangle*)RVAL2BOXED(rectangle, GDK_TYPE_RECTANGLE));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

static VALUE
gdkdraw_cairo_region(self, region)
    VALUE self, region;
{
    gdk_cairo_region(_SELF(self), (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}
#  endif
#endif

void
Init_gtk_gdk_cairo()
{
#if GTK_CHECK_VERSION(2,8,0)
#  ifdef HAVE_RB_CAIRO_H
    rb_define_method(rb_cCairo_Context, "set_source_color", gdkdraw_cairo_set_source_color, 1);
    rb_define_method(rb_cCairo_Context, "set_source_pixbuf", gdkdraw_cairo_set_source_pixbuf, -1);
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(rb_cCairo_Context, "set_source_pixmap", gdkdraw_cairo_set_source_pixmap, 3);
#endif
    rb_define_method(rb_cCairo_Context, "gdk_rectangle", gdkdraw_cairo_rectangle, 1);
    rb_define_method(rb_cCairo_Context, "gdk_region", gdkdraw_cairo_region, 1);

    G_DEF_SETTERS(rb_cCairo_Context);
#  endif
#endif
}

