/* -*- c-file-style: "ruby" -*- */
/* 
 * Copyright (C) 2000 Yasushi Shoji
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkpixbuf.h>
#include "rbgobject.h"
#include "rbgtk.h"

static VALUE
new_from_file(self, file_name)
    VALUE self, file_name;
{
    GdkPixbuf *buf;

    if (TYPE(file_name) != T_STRING) {
	rb_raise(rb_eArgError, "argument must be string");
    }

    buf = gdk_pixbuf_new_from_file(RSTRING(file_name)->ptr, NULL);
    if (buf == NULL)
	rb_raise(rb_eRuntimeError, "error occurred in loading image");
    return GOBJ2RVAL(buf);
}

static VALUE
render_to_drawable(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE args[11];
    int i;
    int dither = GDK_RGB_DITHER_NONE;
    int x_dither = 0;
    int y_dither = 0;

    rb_scan_args(argc, argv, "83", &args[0], &args[1], &args[2], &args[3], &args[4], &args[5], &args[6], &args[7], &args[8], &args[9], &args[10]);

    for (i=0; i<8; i++) {
	if (NIL_P(args[i]))
	    rb_raise(rb_eArgError, "arguments %d must be non nil", i);
    }

    if (!NIL_P(args[8]))
	dither = FIX2INT(args[8]);
    if (!NIL_P(args[9]))
	x_dither = NUM2INT(args[9]);
    if (!NIL_P(args[10]))
	y_dither = NUM2INT(args[10]);

    gdk_pixbuf_render_to_drawable(GDK_PIXBUF(RVAL2GOBJ(self)),
				  GDK_DRAWABLE(RVAL2GOBJ(args[0])),
				  GDK_GC(RVAL2GOBJ(args[1])),
				  NUM2INT(args[2]),
				  NUM2INT(args[3]),
				  NUM2INT(args[4]),
				  NUM2INT(args[5]),
				  NUM2INT(args[6]),
				  NUM2INT(args[7]),
				  dither, x_dither, y_dither);
    return self;
}

static VALUE
render_pm(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE alpha_value;
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    int alpha;

    rb_scan_args(argc, argv, "01", &alpha_value);

    if (NIL_P(alpha_value))
	alpha = 0;
    else
	alpha = NUM2INT(alpha_value);

    gdk_pixbuf_render_pixmap_and_mask(GDK_PIXBUF(RVAL2GOBJ(self)),
				      &pixmap,
				      &mask,
				      alpha);
    return rb_ary_new3(2,
                       pixmap ? GOBJ2RVAL(pixmap) : Qnil,
                       mask ? GOBJ2RVAL(mask) : Qnil);
}

static VALUE
scale(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE ret;
    VALUE args[10];
    int dest_width, dest_height;
    GdkInterpType interp_type = GDK_INTERP_NEAREST;

    rb_scan_args(argc, argv, "28", &args[0], &args[1], &args[2], &args[3], &args[4], &args[5], &args[6], &args[7], &args[8], &args[9]);

    switch (argc) {
      case 2:
      case 3:
	dest_width = NUM2INT(args[0]);
	dest_height = NUM2INT(args[1]);
	if (!NIL_P(args[2]))
	    interp_type = NUM2INT(args[2]);

	ret = GOBJ2RVAL(gdk_pixbuf_scale_simple(GDK_PIXBUF(RVAL2GOBJ(self)),
						  dest_width,
						  dest_height,
						  interp_type));
	break;
      case 7:
	rb_raise(rb_eArgError, "Full version is not yet supported\n");
	break;
      default:
	rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
	break;
    }
    return ret;
}

static VALUE
get_n_channels(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_n_channels(GDK_PIXBUF(RVAL2GOBJ(self))));
}

static VALUE
get_has_alpha(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_has_alpha(GDK_PIXBUF(RVAL2GOBJ(self))));
}

static VALUE
get_bits_per_sample(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_bits_per_sample(GDK_PIXBUF(RVAL2GOBJ(self))));
}

static VALUE
get_width(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_width(GDK_PIXBUF(RVAL2GOBJ(self))));
}

static VALUE
get_height(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_height(GDK_PIXBUF(RVAL2GOBJ(self))));
}

static VALUE
get_rowstride(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_rowstride(GDK_PIXBUF(RVAL2GOBJ(self))));
}

void Init_gdk_pixbuf2()
{
    VALUE gdkPixbuf;
    gdk_rgb_init(); /* initialize it anyway */
    gdkPixbuf = G_DEF_CLASS(GDK_TYPE_PIXBUF, "Pixbuf", mGdk);    

    rb_define_singleton_method(gdkPixbuf, "new", new_from_file, 1);

    /* structure */
    /* rb_define_method(gdkPixbuf, "get_format", ..., 0); */
    rb_define_method(gdkPixbuf, "get_n_channels", get_n_channels, 0);
    rb_define_method(gdkPixbuf, "get_has_alpha", get_has_alpha, 0);
    rb_define_method(gdkPixbuf, "get_bits_per_sample", get_bits_per_sample, 0);
    /* rb_define_method(gdkPixbuf, "get_pixels", ..., 0); */
    rb_define_method(gdkPixbuf, "get_width", get_width, 0);
    rb_define_method(gdkPixbuf, "get_height", get_height, 0);
    rb_define_method(gdkPixbuf, "get_rowstride", get_rowstride, 0);

    /* rendering */
    /* rb_define_method(gdkPixbuf, "render_to_drawable_alpha", ..., ...,); */
    rb_define_method(gdkPixbuf, "render_to_drawable", render_to_drawable, -1);
    /* rb_define_method(gdkPixbuf, "render_threshold_alpha", ..., ...); */
    rb_define_method(gdkPixbuf, "render_pixmap_and_mask", render_pm, -1);

    /* scaling */
    rb_define_method(gdkPixbuf, "scale", scale, -1);
    /* rb_define_method(gdkPixbuf, "composite", composite, -1); */

    /* constants */
    rb_define_const(gdkPixbuf, "ALPHA_BILEVEL",
		    INT2FIX(GDK_PIXBUF_ALPHA_BILEVEL));
    rb_define_const(gdkPixbuf, "ALPHA_FULL",
		    INT2FIX(GDK_PIXBUF_ALPHA_FULL));
    /* aliases */
    rb_define_alias(gdkPixbuf, "to_pixmap_and_mask", "render_pixmap_and_mask");
}
