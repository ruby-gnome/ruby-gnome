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

#define _SELF(s) GDK_PIXBUF(RVAL2GOBJ(s))

/****************************************************/
/* Common */

/****************************************************/
/* File opening */
/* Image Data in Memory */
static VALUE
initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GdkPixbuf* buf;
    GError* error = NULL;
    VALUE arg1, arg2, arg3, arg4, arg5;
    gchar** data;
    int i;

    rb_scan_args(argc, argv, "15", &arg1, &arg2,
                 &arg3, &arg4, &arg5);

    if (argc == 5){
        buf = gdk_pixbuf_new(FIX2INT(arg1),
                             RTEST(arg2), NUM2INT(arg3),
                             NUM2INT(arg4), NUM2INT(arg5));
    } else {
        if (TYPE(arg1) == T_STRING) {
            buf = gdk_pixbuf_new_from_file(RSTRING(arg1)->ptr, &error);
            if (buf == NULL)
                RAISE_GERROR(error);
        } else if (TYPE(arg1) == T_ARRAY) {
            data = ALLOCA_N(char*, RARRAY(arg1)->len);
            for (i=0; i < RARRAY(arg1)->len; i++) {
		data[i] = RVAL2CSTR(RARRAY(arg1)->ptr[i]);
            }
            buf = gdk_pixbuf_new_from_xpm_data((const char**)data);
        } else {
            rb_raise(rb_eArgError, "1st argument is not correct.");
        }
    }
    
    G_INITIALIZE(self, buf);
    return Qnil;
}

static VALUE
new_subpixbuf(self, src_x, src_y, width, height)
    VALUE self, src_x, src_y, width, height;
{
    return GOBJ2RVAL(gdk_pixbuf_new_subpixbuf(_SELF(self), 
                                              NUM2INT(src_x), NUM2INT(src_y), 
                                              NUM2INT(width), NUM2INT(height)));
}
  
static VALUE
copy(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_pixbuf_copy(_SELF(self)));
}

/****************************************************/
/* File saving */
static VALUE
save(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE filename, type, options, key;
    GError* error = NULL;
    gboolean result;
    gchar** keys = NULL;
    gchar** values = NULL;
    gint len, i;
    ID to_s = rb_intern("to_s");
    VALUE ary;

    rb_scan_args(argc, argv, "21", &filename, &type, &options);

    if (options != Qnil){
        Check_Type(options, T_HASH);
        ary = rb_funcall(options, rb_intern("to_a"), 0);
        len = RARRAY(ary)->len;
        keys = ALLOCA_N(gchar*, len + 1);
        values = ALLOCA_N(gchar*, len + 1);
        for (i = 0; i < len; i++) {
            key = RARRAY(RARRAY(ary)->ptr[i])->ptr[0];
            if (SYMBOL_P(key)) {
               keys[i] = rb_id2name(SYM2ID(key));
            } else {
                keys[i] = RVAL2CSTR(key);
            }
            values[i] = RVAL2CSTR(rb_funcall(RARRAY(RARRAY(ary)->ptr[i])->ptr[1], to_s, 0));
        }
        keys[len] = NULL;
        values[len] = NULL;
    }
    result = gdk_pixbuf_savev(_SELF(self), RVAL2CSTR(filename),
                              RVAL2CSTR(type), keys, values, &error);
    if (! result)
        RAISE_GERROR(error);

    return self;
}

/****************************************************/
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

    gdk_pixbuf_render_to_drawable(_SELF(self),
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
    VALUE colormap_or_alpha, alpha;
    GdkPixmap *pixmap;
    GdkBitmap *mask;

    rb_scan_args(argc, argv, "01", &colormap_or_alpha, &alpha);

    if (rb_obj_is_kind_of(colormap_or_alpha, GTYPE2CLASS(GDK_TYPE_COLORMAP))){
        gdk_pixbuf_render_pixmap_and_mask_for_colormap(_SELF(self),
                                                       RVAL2GOBJ(colormap_or_alpha),
                                                       &pixmap,
                                                       &mask,
                                                       NIL_P(alpha)?0:NUM2INT(alpha));
    } else {
        gdk_pixbuf_render_pixmap_and_mask(_SELF(self),
                                          &pixmap,
                                          &mask,
                                          NIL_P(colormap_or_alpha)?
                                          0:NUM2INT(colormap_or_alpha));
    }

    return rb_ary_new3(2,
                       pixmap ? GOBJ2RVAL(pixmap) : Qnil,
                       mask ? GOBJ2RVAL(mask) : Qnil);
}

/****************************************************/
/* Scaling */
static VALUE
scale(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE ret;
    VALUE args[10];
    int dest_width, dest_height;
    GdkInterpType interp_type = GDK_INTERP_BILINEAR;
    GdkPixbuf* dest = NULL;

    rb_scan_args(argc, argv, "28", &args[0], &args[1], &args[2], &args[3], &args[4], 
                 &args[5], &args[6], &args[7], &args[8], &args[9]);

    switch (argc) {
      case 2:
      case 3:
	dest_width = NUM2INT(args[0]);
	dest_height = NUM2INT(args[1]);
	if (!NIL_P(args[2]))
	    interp_type = FIX2INT(args[2]);

	ret = GOBJ2RVAL(gdk_pixbuf_scale_simple(_SELF(self),
						  dest_width,
						  dest_height,
						  interp_type));
	break;
      case 8:
      case 9:
        if (!NIL_P(args[9]))
            interp_type = FIX2INT(args[9]);

        gdk_pixbuf_scale(_SELF(self), dest, 
                         NUM2INT(args[0]), NUM2INT(args[1]), 
                         NUM2INT(args[2]), NUM2INT(args[3]),
                         NUM2DBL(args[4]), NUM2DBL(args[5]),
                         NUM2DBL(args[6]), NUM2DBL(args[7]),
                         interp_type);
        ret = GOBJ2RVAL(dest);
	break;
      default:
	rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
	break;
    }
    return ret;
}

static VALUE
get_colorspace(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_colorspace(_SELF(self)));
}

static VALUE
get_n_channels(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_n_channels(_SELF(self)));
}

static VALUE
get_has_alpha(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_has_alpha(_SELF(self)));
}

static VALUE
get_bits_per_sample(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_bits_per_sample(_SELF(self)));
}
/*
guchar*     gdk_pixbuf_get_pixels           (const GdkPixbuf *pixbuf);
*/

static VALUE
get_width(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_width(_SELF(self)));
}

static VALUE
get_height(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_height(_SELF(self)));
}

static VALUE
get_rowstride(self)
    VALUE self;
{
    return INT2FIX(gdk_pixbuf_get_rowstride(_SELF(self)));
}

static VALUE
get_option(self, key)
    VALUE self, key;
{
    const gchar* ret = gdk_pixbuf_get_option(_SELF(self), RVAL2CSTR(key));
    return ret ? CSTR2RVAL(ret) : Qnil;
}

void Init_gdk_pixbuf2()
{
    VALUE gdkPixbuf;
    gdk_rgb_init(); /* initialize it anyway */
    gdkPixbuf = G_DEF_CLASS(GDK_TYPE_PIXBUF, "Pixbuf", mGdk);    

    /*
     * Initialization and Versions 
     */
    rb_define_const(gdkPixbuf, "VERSION", INT2FIX(gdk_pixbuf_version));
    rb_define_const(gdkPixbuf, "MAJOR", INT2FIX(GDK_PIXBUF_MAJOR));
    rb_define_const(gdkPixbuf, "MINOR", INT2FIX(GDK_PIXBUF_MINOR));
    rb_define_const(gdkPixbuf, "MICRO", INT2FIX(GDK_PIXBUF_MICRO));

    /* 
     * The GdkPixbuf Structure 
     */
    rb_define_method(gdkPixbuf, "colorspace", get_colorspace, 0);
    rb_define_method(gdkPixbuf, "n_channels", get_n_channels, 0);
    rb_define_method(gdkPixbuf, "has_alpha?", get_has_alpha, 0);
    rb_define_method(gdkPixbuf, "bits_per_sample", get_bits_per_sample, 0);
    /* rb_define_method(gdkPixbuf, "pixels", ..., 0); */
    rb_define_method(gdkPixbuf, "width", get_width, 0);
    rb_define_method(gdkPixbuf, "height", get_height, 0);
    rb_define_method(gdkPixbuf, "rowstride", get_rowstride, 0);
    rb_define_method(gdkPixbuf, "option", get_option, 0);
    /* GdkPixbufError(not yet) */
    /* GdkColorspace */
    rb_define_const(gdkPixbuf, "COLORSPACE_RGB", INT2FIX(GDK_COLORSPACE_RGB));
    /* GdkPixbufAlphaMode */
    rb_define_const(gdkPixbuf, "ALPHA_BILEVEL", INT2FIX(GDK_PIXBUF_ALPHA_BILEVEL));
    rb_define_const(gdkPixbuf, "ALPHA_FULL", INT2FIX(GDK_PIXBUF_ALPHA_FULL));

    /* 
     * File Loading, Image Data in Memory
     */
    rb_define_method(gdkPixbuf, "initialize", initialize, -1);
    rb_define_method(gdkPixbuf, "get_subpixbuf", new_subpixbuf, 4);
    rb_define_method(gdkPixbuf, "dup", copy, 0);

    /*
     * File saving
     */
    rb_define_method(gdkPixbuf, "save", save, -1);

    /* 
     * Inline data
     */
    /*
     * Scaling
     */
    rb_define_method(gdkPixbuf, "scale", scale, -1);
    /* rb_define_method(gdkPixbuf, "composite", composite, -1); */
    /* GdkInterpType */
    rb_define_const(gdkPixbuf, "INTERP_NEAREST", INT2FIX(GDK_INTERP_NEAREST));
    rb_define_const(gdkPixbuf, "INTERP_TILES", INT2FIX(GDK_INTERP_TILES));
    rb_define_const(gdkPixbuf, "INTERP_BILINEAR", INT2FIX(GDK_INTERP_BILINEAR));
    rb_define_const(gdkPixbuf, "INTERP_HYPER", INT2FIX(GDK_INTERP_HYPER));

    /* 
     * Rendering
     * The functions to render pixbufs to GDK drawables are contained in GDK.
     * But in Ruby/GdkPixbuf contained them, not Ruby/GDK.
     */
    /* rb_define_method(gdkPixbuf, "render_to_drawable_alpha", ..., ...,); */
    rb_define_method(gdkPixbuf, "render_to_drawable", render_to_drawable, -1);
    /* rb_define_method(gdkPixbuf, "render_threshold_alpha", ..., ...); */
    rb_define_method(gdkPixbuf, "render_pixmap_and_mask", render_pm, -1);

    /*
     * Drawables to Pixbufs
     * The functions to render pixbufs to GDK drawables are contained in GDK.
     * But in Ruby/GdkPixbuf contained them, not Ruby/GDK.
     */
    /*
     * Utilities
     */
}
