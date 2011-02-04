/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixbuf.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:28 $

  Copyright (C) 2002-2004 Masao Mutoh
  Copyright (C) 2000 Yasushi Shoji
************************************************/

#include "rbgdk-pixbuf.h"
#include <string.h>

#ifdef HAVE_GDK_PIXBUF_GDK_PIXBUF_IO_H
#define GDK_PIXBUF_ENABLE_BACKEND
#include <gdk-pixbuf/gdk-pixbuf-io.h>
#endif

#define _SELF(s) GDK_PIXBUF(RVAL2GOBJ(s)) 

#define NOMEM_ERROR(error) g_set_error(error,\
                             GDK_PIXBUF_ERROR,\
                             GDK_PIXBUF_ERROR_INSUFFICIENT_MEMORY,\
                             "Insufficient memory to load image file");

static ID id_pixdata;

/****************************************************/
/* The GdkPixbuf Structure */
static VALUE
get_colorspace(VALUE self)
{
    return GENUM2RVAL(gdk_pixbuf_get_colorspace(_SELF(self)), GDK_TYPE_COLORSPACE);
}

static VALUE
get_n_channels(VALUE self)
{
    return INT2FIX(gdk_pixbuf_get_n_channels(_SELF(self)));
}

static VALUE
get_has_alpha(VALUE self)
{
    return CBOOL2RVAL(gdk_pixbuf_get_has_alpha(_SELF(self)));
}

static VALUE
get_bits_per_sample(VALUE self)
{
    return INT2FIX(gdk_pixbuf_get_bits_per_sample(_SELF(self)));
}

static int
pixels_size(GdkPixbuf *pixbuf)
{
    int height, width, rowstride, n_channels, bits_per_sample;

    height = gdk_pixbuf_get_height(pixbuf);
    width = gdk_pixbuf_get_width(pixbuf);
    rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    bits_per_sample = gdk_pixbuf_get_bits_per_sample(pixbuf);

    return ((height - 1) * rowstride +
            width * ((n_channels * bits_per_sample + 7) / 8));
}

static VALUE
get_pixels(VALUE self)
{
    GdkPixbuf *pixbuf = _SELF(self);
    int size;

    size = pixels_size(pixbuf);
    return rb_str_new((const char*)gdk_pixbuf_get_pixels(pixbuf), size);
}

static VALUE
set_pixels(VALUE self, VALUE pixels)
{
    GdkPixbuf *pixbuf = _SELF(self);
    int size;
    int arg_size;

    size = pixels_size(pixbuf);

    Check_Type(pixels, T_STRING);
    arg_size = RSTRING_LEN(pixels);
    if (arg_size != size)
      rb_raise(rb_eRangeError,
               "Pixels are %i bytes, %i bytes supplied.",
               size, arg_size);

    /* The user currently cannot get a pointer to the actual
     * pixels, the data is copied to a String. */
    memcpy(gdk_pixbuf_get_pixels(pixbuf),
           RSTRING_PTR(pixels), MIN(RSTRING_LEN(pixels), size));

  return pixels;
}

static VALUE
get_width(VALUE self)
{
    return INT2FIX(gdk_pixbuf_get_width(_SELF(self)));
}

static VALUE
get_height(VALUE self)
{
    return INT2FIX(gdk_pixbuf_get_height(_SELF(self)));
}

static VALUE
get_rowstride(VALUE self)
{
    return INT2FIX(gdk_pixbuf_get_rowstride(_SELF(self)));
}

static VALUE
get_option(VALUE self, VALUE key)
{
    const gchar* ret = gdk_pixbuf_get_option(_SELF(self), RVAL2CSTR(key));
    return ret ? CSTR2RVAL(ret) : Qnil;
}

/****************************************************/
/* File opening */
/* Image Data in Memory */
static VALUE
initialize(int argc, VALUE *argv, VALUE self)
{
    GdkPixbuf* buf;
    GError* error = NULL;
    VALUE arg1, arg2, arg3, arg4, arg5, arg6, arg7;

    rb_scan_args(argc, argv, "16", &arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7);

    if (argc == 7){
        buf = gdk_pixbuf_new_from_data((const guchar*)RVAL2CSTR(arg1), 
                                       RVAL2GENUM(arg2, GDK_TYPE_COLORSPACE),
                                       RVAL2CBOOL(arg3),   NUM2INT(arg4),
                                       NUM2INT(arg5), NUM2INT(arg6),
                                       NUM2INT(arg7), NULL, NULL);
        if (buf == NULL){
            rb_gc();
            buf = gdk_pixbuf_new_from_data((const guchar*)RVAL2CSTR(arg1), 
                                           RVAL2GENUM(arg2, GDK_TYPE_COLORSPACE),
                                           RVAL2CBOOL(arg3),   NUM2INT(arg4),
                                           NUM2INT(arg5), NUM2INT(arg6),
                                           NUM2INT(arg7), NULL, NULL);
            if (buf == NULL) NOMEM_ERROR(&error);
        }
        // Save a reference to the string because the pixbuf doesn't copy it.
        G_RELATIVE(self, arg1);
    } else if (argc == 5){
        if (rb_obj_is_kind_of(arg1, GTYPE2CLASS(GDK_TYPE_PIXBUF))){
            buf = gdk_pixbuf_new_subpixbuf(_SELF(arg1), 
                                           NUM2INT(arg2), NUM2INT(arg3), 
                                           NUM2INT(arg4), NUM2INT(arg5));
            if (buf == NULL){
                rb_gc();
                buf = gdk_pixbuf_new_subpixbuf(_SELF(arg1), 
                                               NUM2INT(arg2), NUM2INT(arg3), 
                                               NUM2INT(arg4), NUM2INT(arg5));
                if (buf == NULL) NOMEM_ERROR(&error);
            }
        } else if (rb_obj_is_kind_of(arg1, GTYPE2CLASS(GDK_TYPE_COLORSPACE))){
            buf = gdk_pixbuf_new(RVAL2GENUM(arg1, GDK_TYPE_COLORSPACE),
                                 RVAL2CBOOL(arg2), NUM2INT(arg3),
                                 NUM2INT(arg4), NUM2INT(arg5));
            if (buf == NULL){
                rb_gc();
                buf = gdk_pixbuf_new(RVAL2GENUM(arg1, GDK_TYPE_COLORSPACE),
                                     RVAL2CBOOL(arg2), NUM2INT(arg3),
                                     NUM2INT(arg4), NUM2INT(arg5));
                if (buf == NULL) NOMEM_ERROR(&error);
            }
        } else {
            rb_raise(rb_eArgError, "Wrong type of 1st argument or wrong number of arguments");
        }
    } else if (argc == 4) {
#if RBGDK_PIXBUF_CHECK_VERSION(2,6,0)
        int width = NUM2INT(arg2);
        int height = NUM2INT(arg3);
#if ! RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
        if (width < 0 || height < 0)
            rb_warning("For scaling on load, a negative value for width or height are not supported in GTK+ < 2.8.0");
#endif
        buf = gdk_pixbuf_new_from_file_at_scale(RVAL2CSTR(arg1),
                                                width, height,
                                                RVAL2CBOOL(arg4), &error);
        if (buf == NULL){
            rb_gc();
            error = NULL;
            buf = gdk_pixbuf_new_from_file_at_scale(RVAL2CSTR(arg1),
                                                    NUM2INT(arg2), NUM2INT(arg3), 
                                                    RVAL2CBOOL(arg4), &error);
        }
#else
        rb_warning("Scaling on load not supported in GTK+ < 2.6.0");
        buf = gdk_pixbuf_new_from_file(RVAL2CSTR(arg1), &error);
        if (buf == NULL){
            error = NULL;
            rb_gc();
            buf = gdk_pixbuf_new_from_file(RVAL2CSTR(arg1), &error);
        }
#endif
    } else if (argc == 3) {
#if RBGDK_PIXBUF_CHECK_VERSION(2,4,0)
        buf = gdk_pixbuf_new_from_file_at_size(RVAL2CSTR(arg1),
                                               NUM2INT(arg2), NUM2INT(arg3), &error);
        if (buf == NULL){
            rb_gc();
            error = NULL;
            buf = gdk_pixbuf_new_from_file_at_size(RVAL2CSTR(arg1),
                                                   NUM2INT(arg2), NUM2INT(arg3), &error);
        }
#else
        rb_warning("Sizing on load not supported in GTK+ < 2.4.0");
        buf = gdk_pixbuf_new_from_file(RVAL2CSTR(arg1), &error);
        if (buf == NULL){
            error = NULL;
            rb_gc();
            buf = gdk_pixbuf_new_from_file(RVAL2CSTR(arg1), &error);
        }
#endif
    } else if (argc == 2){
        int i;
        int len = RARRAY_LEN(arg1); 
        guint8 *gstream = g_new(guint8, len);
        for (i = 0; i < len; i++){
            gstream[i] = (guint8)NUM2UINT(RARRAY_PTR(arg1)[i]);
        }      
        buf = gdk_pixbuf_new_from_inline(len, gstream, RVAL2CBOOL(arg2), &error);
        if (buf == NULL){
            rb_gc();
            error = NULL;
            buf = gdk_pixbuf_new_from_inline(len, gstream, RVAL2CBOOL(arg2), &error);
        }
        /* need to manage the returned value */
        rb_ivar_set(self, id_pixdata, Data_Wrap_Struct(rb_cData, NULL, g_free, gstream));
    } else if (argc == 1){
        if (TYPE(arg1) == T_STRING) {
            buf = gdk_pixbuf_new_from_file(RVAL2CSTR(arg1), &error);
            if (buf == NULL){
                rb_gc();
                error = NULL;
                buf = gdk_pixbuf_new_from_file(RVAL2CSTR(arg1), &error);
            }
        } else if (TYPE(arg1) == T_ARRAY) {
            int i;
            gchar** data = ALLOCA_N(gchar*, RARRAY_LEN(arg1));
            for (i=0; i < RARRAY_LEN(arg1); i++) {
		data[i] = RVAL2CSTR(RARRAY_PTR(arg1)[i]);
            }
            buf = gdk_pixbuf_new_from_xpm_data((const gchar**)data);
            if (buf == NULL){
                rb_gc();
                buf = gdk_pixbuf_new_from_xpm_data((const gchar**)data);
                if (buf == NULL) NOMEM_ERROR(&error);
            }
        } else {
            rb_raise(rb_eArgError, "Wrong type of 1st argument or wrong number of arguments");
        }
    } else {
        rb_raise(rb_eArgError, "Wrong number of arguments");
    }

    if (error || ! buf) RAISE_GERROR(error);
    
    G_INITIALIZE(self, buf);
    return Qnil;
}

static VALUE
copy(VALUE self)
{
    VALUE ret;
    GdkPixbuf* dest = gdk_pixbuf_copy(_SELF(self));
    if (dest == NULL)
        return Qnil;
    ret = GOBJ2RVAL(dest);
    g_object_unref(dest);
    return ret;
}

#if RBGDK_PIXBUF_CHECK_VERSION(2,4,0)
static VALUE
get_file_info(VALUE self, VALUE filename)
{
    gint width, height;

    GdkPixbufFormat* format = gdk_pixbuf_get_file_info(RVAL2CSTR(filename), 
                                                       &width, &height);
    return format ? rb_ary_new3(3, BOXED2RVAL(format, GDK_TYPE_PIXBUF_FORMAT), INT2NUM(width), INT2NUM(height)) : Qnil;
}

#endif

static VALUE
save_to(VALUE self, gchar *filename, gchar *type, VALUE options)
{
    VALUE result = self;
    GError *error = NULL;
    gchar **keys = NULL;
    gchar **values = NULL;

    if (!NIL_P(options)) {
        VALUE ary, key, value;
        ID to_s;
        gint len, i;

        Check_Type(options, T_HASH);
        to_s = rb_intern("to_s");

        ary = rb_funcall(options, rb_intern("to_a"), 0);
        len = RARRAY_LEN(ary);
        keys = ALLOCA_N(gchar *, len + 1);
        values = ALLOCA_N(gchar *, len + 1);
        for (i = 0; i < len; i++) {
            key = RARRAY_PTR(RARRAY_PTR(ary)[i])[0];
            if (SYMBOL_P(key)) {
                const char *const_key;
                const_key = rb_id2name(SYM2ID(key));
                keys[i] = (gchar *)const_key;
            } else {
                keys[i] = RVAL2CSTR(key);
            }
            value = rb_funcall(RARRAY_PTR(RARRAY_PTR(ary)[i])[1], to_s, 0);
            values[i] = RVAL2CSTR(value);
        }
        keys[len] = NULL;
        values[len] = NULL;
    }

    if (filename) {
        gdk_pixbuf_savev(_SELF(self), filename, type, keys, values, &error);
    }
#if RBGDK_PIXBUF_CHECK_VERSION(2,4,0)
    else {
        gchar *buffer;
        gsize buffer_size;
        if (gdk_pixbuf_save_to_bufferv(_SELF(self), &buffer, &buffer_size,
                                       type, keys, values, &error))
            result = rb_str_new(buffer, buffer_size);
    }
#endif

    if (error)
        RAISE_GERROR(error);

    return result;
}

/****************************************************/
/* File saving */
static VALUE
save(int argc, VALUE *argv, VALUE self)
{
    VALUE filename, type, options;

    rb_scan_args(argc, argv, "21", &filename, &type, &options);

    return save_to(self, RVAL2CSTR(filename), RVAL2CSTR(type), options);
}

#if RBGDK_PIXBUF_CHECK_VERSION(2,4,0)
/* XXX
gboolean    gdk_pixbuf_save_to_callbackv    (GdkPixbuf *pixbuf,
                                             GdkPixbufSaveFunc save_func,
                                             gpointer user_data,
                                             const char *type,
                                             char **option_keys,
                                             char **option_values,
                                             GError **error);
*/

static VALUE
save_to_buffer(int argc, VALUE *argv, VALUE self)
{
    VALUE type, options;

    rb_scan_args(argc, argv, "11", &type, &options);

    return save_to(self, NULL, RVAL2CSTR(type), options);
}
#endif

/****************************************************/
/* Scaling */
static VALUE
scale_simple(int argc, VALUE *argv, VALUE self)
{
    GdkPixbuf* dest;
    VALUE dest_width, dest_height, interp_type, ret;
    GdkInterpType type = GDK_INTERP_BILINEAR;

    rb_scan_args(argc, argv, "21", &dest_width, &dest_height,
                 &interp_type);

    if (!NIL_P(interp_type))
        type = RVAL2GENUM(interp_type, GDK_TYPE_INTERP_TYPE);
    
    dest = gdk_pixbuf_scale_simple(_SELF(self),
                                   NUM2INT(dest_width),
                                   NUM2INT(dest_height),
                                   type);
    if (dest == NULL)
        return Qnil;

    ret = GOBJ2RVAL(dest);
    g_object_unref(dest);
    return ret;
}

static VALUE
scale(int argc, VALUE *argv, VALUE self)
{
    GdkInterpType type = GDK_INTERP_BILINEAR;

    VALUE src, src_x, src_y, src_width, src_height; 
    VALUE offset_x, offset_y, scale_x, scale_y, interp_type;

    rb_scan_args(argc, argv, "91", &src, &src_x, &src_y, 
                 &src_width, &src_height, &offset_x, &offset_y, 
                 &scale_x, &scale_y, &interp_type);

    if (!NIL_P(interp_type))
        type = RVAL2GENUM(interp_type, GDK_TYPE_INTERP_TYPE);

    gdk_pixbuf_scale(_SELF(src), _SELF(self), 
                     NUM2INT(src_x), NUM2INT(src_y), 
                     NUM2INT(src_width), NUM2INT(src_height),
                     NUM2DBL(offset_x), NUM2DBL(offset_y),
                     NUM2DBL(scale_x), NUM2DBL(scale_y), type);
    return self;
}

static VALUE
composite_simple(VALUE self, VALUE dest_width, VALUE dest_height, VALUE interp_type, VALUE overall_alpha, VALUE check_size, VALUE color1, VALUE color2)
{
    GdkPixbuf* dest;
    VALUE ret;
    GdkInterpType type = GDK_INTERP_BILINEAR;

    if (!NIL_P(interp_type))
        type = RVAL2GENUM(interp_type, GDK_TYPE_INTERP_TYPE);

    dest = gdk_pixbuf_composite_color_simple(
        _SELF(self), NUM2INT(dest_width), NUM2INT(dest_height), 
        type, NUM2INT(overall_alpha), NUM2INT(check_size),
        NUM2UINT(color1), NUM2UINT(color2));

    if (dest == NULL)
        return Qnil;

    ret = GOBJ2RVAL(dest);
    g_object_unref(dest);
    return ret;
}

static VALUE
composite(int argc, VALUE *argv, VALUE self)
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
	    interp_type = RVAL2GENUM(args[9], GDK_TYPE_INTERP_TYPE);

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
	    interp_type = RVAL2GENUM(args[9], GDK_TYPE_INTERP_TYPE);

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

#if RBGDK_PIXBUF_CHECK_VERSION(2,6,0)
static VALUE
rotate_simple(VALUE self, VALUE angle)
{
    VALUE ret;
    GdkPixbuf* dest = gdk_pixbuf_rotate_simple(_SELF(self), RVAL2GENUM(angle, GDK_TYPE_PIXBUF_ROTATION));
    if (dest == NULL)
        return Qnil;
    ret = GOBJ2RVAL(dest);
    g_object_unref(dest);
    return ret;
}

static VALUE
flip(VALUE self, VALUE horizontal)
{
    VALUE ret;
    GdkPixbuf* dest = gdk_pixbuf_flip(_SELF(self), RVAL2CBOOL(horizontal));
    if (dest == NULL)
        return Qnil;
    ret = GOBJ2RVAL(dest);
    g_object_unref(dest);
    return ret;
}
#endif

static VALUE
add_alpha(VALUE self, VALUE substitute_color, VALUE r, VALUE g, VALUE b)
{
    VALUE ret;
    GdkPixbuf* dest = gdk_pixbuf_add_alpha(_SELF(self),
                                           RVAL2CBOOL(substitute_color),
                                           FIX2INT(r), FIX2INT(g), FIX2INT(b));
    if (dest == NULL)
        return Qnil;
    ret = GOBJ2RVAL(dest);
    g_object_unref(dest);
    return ret;
}

static VALUE
copy_area(VALUE self, VALUE src_x, VALUE src_y, VALUE width, VALUE height, VALUE dest, VALUE dest_x, VALUE dest_y)
{
    gdk_pixbuf_copy_area(_SELF(self), NUM2INT(src_x), NUM2INT(src_y),
                         NUM2INT(width), NUM2INT(height),
                         _SELF(dest), NUM2INT(dest_x), NUM2INT(dest_y));
    return dest;
}

static VALUE
saturate_and_pixelate(VALUE self, VALUE staturation, VALUE pixelate)
{
    GdkPixbuf* dest = gdk_pixbuf_copy(_SELF(self));
    gdk_pixbuf_saturate_and_pixelate(_SELF(self), dest, 
                                     NUM2DBL(staturation), RVAL2CBOOL(pixelate));
    return GOBJ2RVAL(dest);
}

static VALUE
fill(VALUE self, VALUE pixel)
{
    gdk_pixbuf_fill(_SELF(self), NUM2UINT(pixel));
    return self;
}

/* From Module Interface */
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
static VALUE
get_formats(VALUE self)
{
    return GSLIST2ARY2(gdk_pixbuf_get_formats(), GDK_TYPE_PIXBUF_FORMAT);
}

static VALUE
set_option(VALUE self, VALUE key, VALUE value)
{
#if HAVE_GDK_PIXBUF_SET_OPTION
    return CBOOL2RVAL(gdk_pixbuf_set_option(_SELF(self), 
                                            RVAL2CSTR(key), RVAL2CSTR(value)));
#else
    rb_warning("not supported in this version of GTK+");
    return Qfalse;
#endif
}
#endif

void 
Init_gdk_pixbuf2()
{
    VALUE mGdk = rb_define_module("Gdk");
    VALUE gdkPixbuf = G_DEF_CLASS(GDK_TYPE_PIXBUF, "Pixbuf", mGdk);    

    id_pixdata = rb_intern("pixdata");
   
    /*
    gdk_rgb_init();*/ /* initialize it anyway */
    

    /*
     * Initialization and Versions 
     */
/* Removed. This crashes Ruby/GTK on Windows + GTK+-2.4.x.
   Pointed out by Laurent.
#ifdef HAVE_GDK_PIXBUF_VERSION
    rb_define_const(gdkPixbuf, "VERSION", CSTR2RVAL(gdk_pixbuf_version));
#endif
*/
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
    rb_define_method(gdkPixbuf, "pixels", get_pixels, 0);
    rb_define_method(gdkPixbuf, "pixels=", set_pixels, 1);
    rb_define_method(gdkPixbuf, "width", get_width, 0);
    rb_define_method(gdkPixbuf, "height", get_height, 0);
    rb_define_method(gdkPixbuf, "rowstride", get_rowstride, 0);
    rb_define_method(gdkPixbuf, "get_option", get_option, 1);

    /* GdkPixbufError */
    G_DEF_ERROR(GDK_PIXBUF_ERROR, "PixbufError", mGdk, rb_eRuntimeError, GDK_TYPE_PIXBUF_ERROR);

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
    rb_define_method(gdkPixbuf, "dup", copy, 0);
#if RBGDK_PIXBUF_CHECK_VERSION(2,4,0)
    rb_define_singleton_method(gdkPixbuf, "get_file_info", get_file_info, 1);
#endif

    /*
     * File saving
     */
    rb_define_method(gdkPixbuf, "save", save, -1);
#if RBGDK_PIXBUF_CHECK_VERSION(2,4,0)
    rb_define_method(gdkPixbuf, "save_to_buffer", save_to_buffer, -1);
#endif

    /*
     * Scaling
     */
    rb_define_method(gdkPixbuf, "scale", scale_simple, -1);
    rb_define_method(gdkPixbuf, "scale!", scale, -1);
    rb_define_method(gdkPixbuf, "composite", composite_simple, 7);
    rb_define_method(gdkPixbuf, "composite!", composite, -1);
#if RBGDK_PIXBUF_CHECK_VERSION(2,6,0)
    rb_define_method(gdkPixbuf, "rotate", rotate_simple, 1);
    rb_define_method(gdkPixbuf, "flip", flip, 1);
#endif

    /* GdkInterpType */
    G_DEF_CLASS(GDK_TYPE_INTERP_TYPE, "InterpType", gdkPixbuf);
    G_DEF_CONSTANTS(gdkPixbuf, GDK_TYPE_INTERP_TYPE, "GDK_");

#if RBGDK_PIXBUF_CHECK_VERSION(2,6,0)
    /* GdkPixbufRotation */
    G_DEF_CLASS(GDK_TYPE_PIXBUF_ROTATION, "GdkPixbufRotation", gdkPixbuf);
    G_DEF_CONSTANTS(gdkPixbuf, GDK_TYPE_PIXBUF_ROTATION, "GDK_PIXBUF_");
#endif
    /*
     * Utilities
     */
    rb_define_method(gdkPixbuf, "add_alpha", add_alpha, 4);
    rb_define_method(gdkPixbuf, "copy_area", copy_area, 7);
    rb_define_method(gdkPixbuf, "saturate_and_pixelate", saturate_and_pixelate, 2);
    rb_define_method(gdkPixbuf, "fill!", fill, 1);

    /*
     * Module Interface
     */
#if RBGDK_PIXBUF_CHECK_VERSION(2,2,0)
    rb_define_singleton_method(gdkPixbuf, "formats", get_formats, 0);
    rb_define_method(gdkPixbuf, "set_option", set_option, 2);
#endif

    Init_gdk_pixbuf_animation(mGdk);
#if RBGDK_PIXBUF_CHECK_VERSION(2,8,0)
    Init_gdk_pixbuf_simpleanim(mGdk);
#endif
    Init_gdk_pixdata(mGdk);
    Init_gdk_pixbuf_loader(mGdk);
    Init_gdk_pixbuf_format(mGdk);
}
