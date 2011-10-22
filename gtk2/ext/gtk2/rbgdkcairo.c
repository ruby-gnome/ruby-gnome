/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Kouhei Sutou
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#if GTK_CHECK_VERSION(2,8,0) && defined(HAVE_RB_CAIRO_H)
#  define CAIRO_AVAILABLE 1
#else
#  define CAIRO_AVAILABLE 0
#endif

#if CAIRO_AVAILABLE
#include <gdk/gdkcairo.h>
#include <rb_cairo.h>

#define _SELF(self) RVAL2CRCONTEXT(self)

static VALUE
gdkdraw_cairo_set_source_color(VALUE self, VALUE color)
{
    gdk_cairo_set_source_color(_SELF(self), RVAL2GDKCOLOR(color));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

static VALUE
gdkdraw_cairo_set_source_pixbuf(int argc, VALUE *argv, VALUE self)
{
    VALUE pixbuf, pixbuf_x, pixbuf_y;

    rb_scan_args(argc, argv, "12", &pixbuf, &pixbuf_x, &pixbuf_y);

    gdk_cairo_set_source_pixbuf(_SELF(self),
                                GDK_PIXBUF(RVAL2GOBJ(pixbuf)),
                                NIL_P(pixbuf_x) ? 0 : NUM2DBL(pixbuf_x),
                                NIL_P(pixbuf_y) ? 0 : NUM2DBL(pixbuf_y));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
gdkdraw_cairo_set_source_pixmap(VALUE self, VALUE pixmap, VALUE pixmap_x, VALUE pixmap_y)
{
    gdk_cairo_set_source_pixmap(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(pixmap)),
                                NUM2DBL(pixmap_x), NUM2DBL(pixmap_y));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}
#endif

static VALUE
gdkdraw_cairo_rectangle(VALUE self, VALUE rectangle)
{
    gdk_cairo_rectangle(_SELF(self), 
                        (GdkRectangle*)RVAL2BOXED(rectangle, GDK_TYPE_RECTANGLE));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}

static VALUE
gdkdraw_cairo_region(VALUE self, VALUE region)
{
    gdk_cairo_region(_SELF(self), (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION));
    rb_cairo_check_status(cairo_status(_SELF(self)));
    return self;
}
#endif

static VALUE
cairo_available_p(G_GNUC_UNUSED VALUE self)
{
#if CAIRO_AVAILABLE
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_gtk_gdk_cairo(void)
{
#if CAIRO_AVAILABLE
    rb_define_method(rb_cCairo_Context, "set_source_gdk_color",
                     gdkdraw_cairo_set_source_color, 1);
    rb_define_method(rb_cCairo_Context, "set_source_pixbuf",
                     gdkdraw_cairo_set_source_pixbuf, -1);
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(rb_cCairo_Context, "set_source_pixmap", gdkdraw_cairo_set_source_pixmap, 3);
#endif
    rb_define_method(rb_cCairo_Context, "gdk_rectangle", gdkdraw_cairo_rectangle, 1);
    rb_define_method(rb_cCairo_Context, "gdk_region", gdkdraw_cairo_region, 1);

    G_DEF_SETTERS(rb_cCairo_Context);
#endif

    rb_define_module_function(mGdk, "cairo_available?", cairo_available_p, 0);
}

