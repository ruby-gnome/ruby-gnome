/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbuf.c -

  $Author: sakai $
  $Date: 2003/08/21 07:43:58 $

  Copyright (C) 2002,2003 Masao Mutoh
  Copyright (C) 2000 Yasushi Shoji
************************************************/
#include "rbgdk-pixbuf.h"

#define _SELF(s) GDK_PIXBUF(RVAL2GOBJ(s)) 

/****************************************************/
/* The GdkPixbuf Structure */
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

    rb_scan_args(argc, argv, "14", &arg1, &arg2,
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
            rb_raise(rb_eArgError, "Wrong type of 1st arguments");
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
    VALUE filename, type, options, key, str;
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
            str = rb_funcall(RARRAY(RARRAY(ary)->ptr[i])->ptr[1], to_s, 0);
            values[i] = RVAL2CSTR(str);
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
/* Scaling */
static VALUE
scale_simple(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE dest_width, dest_height, interp_type;
    GdkInterpType type = GDK_INTERP_BILINEAR;

    rb_scan_args(argc, argv, "21", &dest_width, &dest_height,
                 &interp_type);

    if (!NIL_P(interp_type))
        type = FIX2INT(interp_type);
    
    return GOBJ2RVAL(gdk_pixbuf_scale_simple(_SELF(self),
                                             NUM2INT(dest_width),
                                             NUM2INT(dest_height),
                                             type));
}

static VALUE
scale(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    GdkInterpType type = GDK_INTERP_BILINEAR;

    VALUE dest, dest_x, dest_y, dest_width, dest_height; 
    VALUE offset_x, offset_y, scale_x, scale_y, interp_type;

    rb_scan_args(argc, argv, "91", &dest, &dest_x, &dest_y, 
                 &dest_width, &dest_height, &offset_x, &offset_y, 
                 &scale_x, &scale_y, &interp_type);

    if (!NIL_P(interp_type))
        type = FIX2INT(interp_type);

    gdk_pixbuf_scale(_SELF(dest), _SELF(self),
                     NUM2INT(dest_x), NUM2INT(dest_y), 
                     NUM2INT(dest_width), NUM2INT(dest_height),
                     NUM2DBL(offset_x), NUM2DBL(offset_y),
                     NUM2DBL(scale_x), NUM2DBL(scale_y), type);
    return self;
}

static VALUE
composite_simple(self, dest_width, dest_height, interp_type, overall_alpha,
                 check_size, color1, color2)
    VALUE self, dest_width, dest_height, interp_type, overall_alpha,
    check_size, color1, color2;
{
    GdkInterpType type = GDK_INTERP_BILINEAR;

    if (!NIL_P(interp_type))
        type = FIX2INT(interp_type);

    return GOBJ2RVAL(gdk_pixbuf_composite_color_simple(
                         _SELF(self), NUM2INT(dest_width), NUM2INT(dest_height), 
                         type, NUM2INT(overall_alpha), NUM2INT(check_size),
                         NUM2UINT(color1), NUM2UINT(color2)));
}

static VALUE
composite(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE ret;
    VALUE args[16];
    GdkInterpType interp_type = GDK_INTERP_BILINEAR;

    rb_scan_args(argc, argv, "97", 
                 &args[0], &args[1], &args[2], &args[3], &args[4], 
                 &args[5], &args[6], &args[7], &args[8], &args[9],
                 &args[10], &args[11], &args[12], &args[13], &args[14],
                 &args[15]);

    switch (argc) {
      case 11:
	if (!NIL_P(args[9]))
	    interp_type = FIX2INT(args[9]);

        gdk_pixbuf_composite(_SELF(args[0]), _SELF(self), 
                             NUM2INT(args[1]), NUM2INT(args[2]),
                             NUM2INT(args[3]), NUM2INT(args[4]),
                             NUM2DBL(args[5]), NUM2DBL(args[6]),
                             NUM2DBL(args[7]), NUM2DBL(args[8]),
                             interp_type, NUM2INT(args[10]));
        ret = self;
        break;
      case 16:
	if (!NIL_P(args[9]))
	    interp_type = FIX2INT(args[9]);

        gdk_pixbuf_composite_color(_SELF(args[0]), _SELF(self),
                                   NUM2INT(args[1]), NUM2INT(args[2]),
                                   NUM2INT(args[3]), NUM2INT(args[4]),
                                   NUM2DBL(args[5]), NUM2DBL(args[6]),
                                   NUM2DBL(args[7]), NUM2DBL(args[8]),
                                   interp_type, NUM2INT(args[10]),
                                   NUM2INT(args[11]), NUM2INT(args[12]),
                                   NUM2INT(args[13]), NUM2UINT(args[14]),
                                   NUM2UINT(args[15]));
        ret = self;
        break;
      default:
	rb_raise(rb_eArgError, "Wrong number of arguments: %d", argc);
	break;
    }
    return ret;
}

static VALUE
add_alpha(self, substitute_color, r, g, b)
    VALUE self, substitute_color, r, g, b;
{
    return GOBJ2RVAL(gdk_pixbuf_add_alpha(_SELF(self),
                                          RTEST(substitute_color),
                                          FIX2INT(r), FIX2INT(g), FIX2INT(b)));
}

static VALUE
copy_area(self, src_x, src_y, width, height, dest, dest_x, dest_y)
    VALUE self, src_x, src_y, width, height, dest, dest_x, dest_y;
{
    gdk_pixbuf_copy_area(_SELF(self), NUM2INT(src_x), NUM2INT(src_y),
                         NUM2INT(width), NUM2INT(height),
                         _SELF(dest), NUM2INT(dest_x), NUM2INT(dest_y));
    return dest;
}

static VALUE
saturate_and_pixelate(self, staturation, pixelate)
    VALUE self, staturation, pixelate;
{
    GdkPixbuf* dest = gdk_pixbuf_copy(_SELF(self));
    gdk_pixbuf_saturate_and_pixelate(_SELF(self), dest, 
                                     NUM2DBL(staturation), RTEST(pixelate));
    return GOBJ2RVAL(dest);
}

static VALUE
fill(self, pixel)
    VALUE self, pixel;
{
    gdk_pixbuf_fill(_SELF(self), NUM2UINT(pixel));
    return self;
}

void 
Init_gdk_pixbuf2()
{
    VALUE mGdk = rb_define_module("Gdk");
    VALUE gdkPixbuf = G_DEF_CLASS(GDK_TYPE_PIXBUF, "Pixbuf", mGdk);    
    /*
    gdk_rgb_init();*/ /* initialize it anyway */
    

    /*
     * Initialization and Versions 
     */
#ifdef HAVE_GDK_PIXBUF_VERSION
    rb_define_const(gdkPixbuf, "VERSION", CSTR2RVAL(gdk_pixbuf_version));
#endif
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
    rb_define_method(gdkPixbuf, "get_option", get_option, 1);

    /* GdkPixbufError(not yet) */

    /* GdkColorspace */
    G_DEF_CLASS(GDK_TYPE_COLORSPACE, "ColorSpace", gdkPixbuf);
    G_DEF_CONSTANTS(gdkPixbuf, GDK_TYPE_COLORSPACE, "GDK_");

    /* GdkPixbufAlphaMode */
    G_DEF_CLASS(GDK_TYPE_PIXBUF_ALPHA_MODE, "AlphaMode", gdkPixbuf);
    G_DEF_CONSTANTS(gdkPixbuf, GDK_TYPE_PIXBUF_ALPHA_MODE, "GDK_PIXBUF_");

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
     * Scaling
     */
    rb_define_method(gdkPixbuf, "scale", scale_simple, -1);
    rb_define_method(gdkPixbuf, "scale!", scale, -1);
    rb_define_method(gdkPixbuf, "composite", composite_simple, 7);
    rb_define_method(gdkPixbuf, "composite!", composite, -1);
    /* GdkInterpType */
    G_DEF_CLASS(GDK_TYPE_INTERP_TYPE, "InterpType", gdkPixbuf);
    G_DEF_CONSTANTS(gdkPixbuf, GDK_TYPE_INTERP_TYPE, "GDK_");

    /*
     * Utilities
     */
    rb_define_method(gdkPixbuf, "add_alpha", add_alpha, 4);
    rb_define_method(gdkPixbuf, "copy_area", copy_area, 7);
    rb_define_method(gdkPixbuf, "saturate_and_pixelate", saturate_and_pixelate, 2);
    rb_define_method(gdkPixbuf, "fill!", fill, 1);

    Init_gdk_pixbuf_animation(mGdk);
    Init_gdk_pixdata(mGdk);
    Init_gdk_pixbuf_loader(mGdk);
}
