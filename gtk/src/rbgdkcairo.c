/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkcairo.c -

  $Author: ktou $
  $Date: 2005/10/09 16:24:27 $

  Copyright (C) 2005 Kouhei Sutou
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,8,0)
#  ifdef HAVE_RB_CAIRO_H

#include <rb_cairo.h>

#define _SELF(self) RVAL2CRCONTEXT(self)

#define RVAL2COLOR(obj) ((GdkColor*)RVAL2BOXED(obj, GDK_TYPE_COLOR))
#define RVAL2PIXBUF(obj) GDK_PIXBUF(RVAL2GOBJ(obj))
#define RVAL2RECTANGLE(obj) ((GdkRectangle*)RVAL2BOXED(obj, GDK_TYPE_RECTANGLE))
#define RVAL2REGION(obj) ((GdkRegion*)RVAL2BOXED(obj, GDK_TYPE_REGION))

static VALUE
gdkdraw_cairo_set_source_color(self, color)
    VALUE self, color;
{
    gdk_cairo_set_source_color(_SELF(self), RVAL2COLOR(color));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

static VALUE
gdkdraw_cairo_set_source_pixbuf(self, pixbuf, pixbuf_x, pixbuf_y)
    VALUE self, pixbuf, pixbuf_x, pixbuf_y;
{
    gdk_cairo_set_source_pixbuf(_SELF(self), RVAL2PIXBUF(pixbuf),
                                NUM2DBL(pixbuf_x), NUM2DBL(pixbuf_y));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

static VALUE
gdkdraw_cairo_rectangle(self, rectangle)
    VALUE self, rectangle;
{
    gdk_cairo_rectangle(_SELF(self), RVAL2RECTANGLE(rectangle));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

static VALUE
gdkdraw_cairo_region(self, region)
    VALUE self, region;
{
    gdk_cairo_region(_SELF(self), RVAL2REGION(region));
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
    rb_define_method(rb_cCairo_Context, "set_source_color",
                     gdkdraw_cairo_set_source_color, 1);
    rb_define_method(rb_cCairo_Context, "set_source_pixbuf",
                     gdkdraw_cairo_set_source_pixbuf, 3);
    rb_define_method(rb_cCairo_Context, "gdk_rectangle",
                     gdkdraw_cairo_rectangle, 1);
    rb_define_method(rb_cCairo_Context, "gdk_region",
                     gdkdraw_cairo_region, 1);

    G_DEF_SETTERS(rb_cCairo_Context);
#  endif
#endif
}

