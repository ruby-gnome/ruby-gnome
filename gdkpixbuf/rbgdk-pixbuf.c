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

#include "global.h"
#include <gdk-pixbuf/gdk-pixbuf.h>

VALUE gdkPixbuf;

static VALUE
make_pixbuf(buf)
    GdkPixbuf *buf;
{
    return Data_Wrap_Struct(gdkPixbuf, 0, gdk_pixbuf_unref, buf);
}

static GdkPixbuf*
get_pixbuf(buf)
    VALUE buf;
{
    GdkPixbuf *ptr;

    if (NIL_P(buf))
	rb_raise(rb_eArgError, "oops");

    if (!rb_obj_is_kind_of(buf, gdkPixbuf)) {
	rb_raise(rb_eTypeError, "not a GdkPixbuf");
    }

    Data_Get_Struct(buf, GdkPixbuf, ptr);

    return ptr;
}

static VALUE
new_from_file(self, file_name)
    VALUE self, file_name;
{
    GdkPixbuf *buf;

    if (TYPE(file_name) != T_STRING) {
	rb_raise(rb_eArgError, "argument must be string");
    }

    buf = gdk_pixbuf_new_from_file(RSTRING(file_name)->ptr);
    if (buf == NULL)
	rb_raise(rb_eRuntimeError, "error occurred in loading image");
    return make_pixbuf(buf);
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

    gdk_pixbuf_render_to_drawable(get_pixbuf(self),
				  get_gdkdrawable(args[0]),
				  get_gdkgc(args[1]),
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

    gdk_pixbuf_render_pixmap_and_mask(get_pixbuf(self),
				      &pixmap,
				      &mask,
				      alpha);
    return rb_ary_new3(2, new_gdkpixmap(pixmap), new_gdkbitmap(mask));
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

	ret = make_pixbuf(gdk_pixbuf_scale_simple(get_pixbuf(self),
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
    return INT2FIX(gdk_pixbuf_get_n_channels(get_pixbuf(self)));
}

static VALUE
get_has_alpha(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_has_alpha(get_pixbuf(self)));
}

static VALUE
get_bits_per_sample(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_bits_per_sample(get_pixbuf(self)));
}

static VALUE
get_width(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_width(get_pixbuf(self)));
}

static VALUE
get_height(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_height(get_pixbuf(self)));
}

static VALUE
get_rowstride(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_rowstride(get_pixbuf(self)));
}

void Init_gdk_pixbuf()
{
    gdk_rgb_init(); /* initialize it anyway */
    gdkPixbuf = rb_define_class_under(mGdk, "Pixbuf", rb_cData);

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
