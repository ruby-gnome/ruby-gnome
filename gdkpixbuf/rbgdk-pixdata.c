/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdk-pixdata.c -

  $Author: mutoh $
  $Date: 2004/08/14 07:00:50 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/
#include "rbgdk-pixbuf.h"

#define GDK_TYPE_PIXDATA (gdk_pixdata_get_type())
#define _SELF(s) ((GdkPixdata*)(RVAL2BOXED(s, GDK_TYPE_PIXDATA)))
#define PIXDATA2RVAL(pix) (BOXED2RVAL(pix, GDK_TYPE_PIXDATA))

static ID id_pixdata;

/*****************************************/
GdkPixdata*
gdk_pixdata_copy(const GdkPixdata* src)
{
  GdkPixdata* data;
  g_return_val_if_fail (src != NULL, NULL);
  data = g_new(GdkPixdata, 1);
  *data = *src;
  return data;
}

GType
gdk_pixdata_get_type (void)
{
  static GType our_type = 0;
  if (our_type == 0)
      our_type = g_boxed_type_register_static ("GdkPixdata",
                                               (GBoxedCopyFunc)gdk_pixdata_copy,
                                               (GBoxedFreeFunc)g_free);
  return our_type;
}
/*****************************************/
static VALUE
pixdata_s_from_pixbuf(self, pixbuf, use_rle)
    VALUE self, pixbuf, use_rle;
{
    GdkPixdata pixdata;
    gpointer rle_data = gdk_pixdata_from_pixbuf(&pixdata, RVAL2GOBJ(pixbuf), RTEST(use_rle));
    VALUE ret = PIXDATA2RVAL(&pixdata);
    if (use_rle){
        /* need to manage the returned value */
        rb_ivar_set(ret, id_pixdata, Data_Wrap_Struct(rb_cData, NULL, g_free, rle_data));
    }
    return ret;
}

static VALUE
pixdata_to_pixbuf(self, copy_pixels)
    VALUE self, copy_pixels;
{
    GError* error = NULL;
    GdkPixbuf* ret = gdk_pixbuf_from_pixdata(_SELF(self), RTEST(copy_pixels), &error);
    if (error)
        RAISE_GERROR(error);
    return GOBJ2RVAL(ret);
}

static VALUE
pixdata_serialize(self)
    VALUE self;
{
    guint stream_length;
    gint i;
    guint8* ret = gdk_pixdata_serialize(_SELF(self), &stream_length);
    VALUE ary = rb_ary_new2(stream_length);
    for (i = 0; i < stream_length; i++) {
        rb_ary_push(ary, UINT2NUM(ret[i]));
    }
    return ary;
}

static VALUE
pixdata_s_deserialize(self, stream)
    VALUE self, stream;
{
    GdkPixdata pixdata;
    gboolean ret;
    guint8 *gstream;
    GError* error = NULL;
    gint i, len;

    len = RARRAY(stream)->len;
    gstream = g_new(guint8, len);
    //   gstream = ALLOCA_N(guint8, len);
    for (i = 0; i < len; i++){
        gstream[i] = (guint8)NUM2UINT(RARRAY(stream)->ptr[i]);
    }
    ret = gdk_pixdata_deserialize(&pixdata, len, gstream, &error);

    /* need to manage the returned value */
    rb_ivar_set(ret, id_pixdata, Data_Wrap_Struct(rb_cData, NULL, g_free, gstream));

    if (ret != TRUE) RAISE_GERROR(error);

    return PIXDATA2RVAL(&pixdata);
}

static VALUE
pixdata_to_csource(self, name, dump_type)
    VALUE self, name, dump_type;
{
    GString* str = gdk_pixdata_to_csource(_SELF(self), RVAL2CSTR(name), FIX2INT(dump_type));
    VALUE ret = CSTR2RVAL(str->str);
    g_string_free(str, TRUE);
    return ret;
}

/* GdkPixdata */
static VALUE
pixdata_magic(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->magic);
}

static VALUE
pixdata_length(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->length);
}

static VALUE
pixdata_pixdata_type(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->pixdata_type);
}

static VALUE
pixdata_rowstride(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->rowstride);
}

static VALUE
pixdata_width(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
pixdata_height(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
pixdata_pixel_data(self)
    VALUE self;
{
    gint i;
    guint8* ret = _SELF(self)->pixel_data;

    VALUE ary = rb_ary_new2(_SELF(self)->length);
    for (i = 0; i < _SELF(self)->length; i++) {
        rb_ary_push(ary, UINT2NUM(ret[i]));
    }
    return ary;
}


void
Init_gdk_pixdata(VALUE mGdk)
{
    VALUE pixdata = G_DEF_CLASS(GDK_TYPE_PIXDATA, "Pixdata", mGdk);    

    id_pixdata = rb_intern("pixdata");

    rb_define_singleton_method(pixdata, "from_pixbuf", pixdata_s_from_pixbuf, 2);
    rb_define_singleton_method(pixdata, "deserialize", pixdata_s_deserialize, 1);
    rb_define_method(pixdata, "to_pixbuf", pixdata_to_pixbuf, 1);
    rb_define_method(pixdata, "serialize", pixdata_serialize, 0);
    rb_define_method(pixdata, "to_csource", pixdata_to_csource, 2);
    rb_define_method(pixdata, "magic", pixdata_magic, 0);
    rb_define_method(pixdata, "length", pixdata_length, 0);
    rb_define_method(pixdata, "pixdata_type", pixdata_pixdata_type, 0);
    rb_define_method(pixdata, "rowstride", pixdata_rowstride, 0);
    rb_define_method(pixdata, "width", pixdata_width, 0);
    rb_define_method(pixdata, "height", pixdata_height, 0);
    rb_define_method(pixdata, "pixel_data", pixdata_pixel_data, 0);

    rb_define_const(pixdata, "PIXBUF_MAGIC_NUMBER", INT2NUM(GDK_PIXBUF_MAGIC_NUMBER));
    rb_define_const(pixdata, "HEADER_LENGTH", INT2NUM(GDK_PIXDATA_HEADER_LENGTH));

    /* GdkPixdataType */
    rb_define_const(pixdata, "COLOR_TYPE_RGB", INT2FIX(GDK_PIXDATA_COLOR_TYPE_RGB));
    rb_define_const(pixdata, "COLOR_TYPE_RGBA", INT2FIX(GDK_PIXDATA_COLOR_TYPE_RGBA));
    rb_define_const(pixdata, "COLOR_TYPE_MASK", INT2FIX(GDK_PIXDATA_COLOR_TYPE_MASK));
    rb_define_const(pixdata, "SAMPLE_WIDTH_8", INT2FIX(GDK_PIXDATA_SAMPLE_WIDTH_8));
    rb_define_const(pixdata, "SAMPLE_WIDTH_MASK", INT2FIX(GDK_PIXDATA_SAMPLE_WIDTH_MASK));
    rb_define_const(pixdata, "ENCODING_RAW", INT2FIX(GDK_PIXDATA_ENCODING_RAW));
    rb_define_const(pixdata, "ENCODING_RLE", INT2FIX(GDK_PIXDATA_ENCODING_RLE));
    rb_define_const(pixdata, "ENCODING_MASK", INT2FIX(GDK_PIXDATA_ENCODING_MASK));

    /* GdkPixdataDumpType */
    rb_define_const(pixdata, "DUMP_PIXDATA_STREAM", INT2FIX(GDK_PIXDATA_DUMP_PIXDATA_STREAM));
    rb_define_const(pixdata, "DUMP_PIXDATA_STRUCT", INT2FIX(GDK_PIXDATA_DUMP_PIXDATA_STRUCT));
    rb_define_const(pixdata, "DUMP_MACROS", INT2FIX(GDK_PIXDATA_DUMP_MACROS));
    rb_define_const(pixdata, "DUMP_GTYPES", INT2FIX(GDK_PIXDATA_DUMP_GTYPES));
    rb_define_const(pixdata, "DUMP_CTYPES", INT2FIX(GDK_PIXDATA_DUMP_CTYPES));
    rb_define_const(pixdata, "DUMP_STATIC", INT2FIX(GDK_PIXDATA_DUMP_STATIC));
    rb_define_const(pixdata, "DUMP_CONST", INT2FIX(GDK_PIXDATA_DUMP_CONST));
    rb_define_const(pixdata, "DUMP_RLE_DECODER", INT2FIX(GDK_PIXDATA_DUMP_RLE_DECODER));
}
