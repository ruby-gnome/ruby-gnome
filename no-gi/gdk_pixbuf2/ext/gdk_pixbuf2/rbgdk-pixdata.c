/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#include "rbgdk-pixbuf2private.h"

#define GDK_TYPE_PIXDATA (gdk_pixdata_get_type())
#define RG_TARGET_NAMESPACE cPixdata
#define _SELF(s) (RVAL2GDKPIXDATA(s))

static ID id_pixdata;

/*****************************************/
static GdkPixdata *
gdk_pixdata_copy(const GdkPixdata* src)
{
  GdkPixdata* data;
  g_return_val_if_fail (src != NULL, NULL);
  data = g_new(GdkPixdata, 1);
  *data = *src;
  return data;
}

static GType
gdk_pixdata_get_type(void)
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
rg_s_from_pixbuf(G_GNUC_UNUSED VALUE self, VALUE pixbuf, VALUE use_rle)
{
    GdkPixdata pixdata;
    gpointer rle_data = gdk_pixdata_from_pixbuf(&pixdata, RVAL2GDKPIXBUF(pixbuf), RVAL2CBOOL(use_rle));
    VALUE ret = GDKPIXDATA2RVAL(&pixdata);
    if (use_rle){
        /* need to manage the returned value */
        rb_ivar_set(ret, id_pixdata, Data_Wrap_Struct(rb_cData, NULL, g_free, rle_data));
    }
    return ret;
}

static VALUE
rg_to_pixbuf(VALUE self, VALUE copy_pixels)
{
    GError* error = NULL;
    GdkPixbuf* ret = gdk_pixbuf_from_pixdata(_SELF(self), RVAL2CBOOL(copy_pixels), &error);
    if (error)
        RAISE_GERROR(error);
    return GOBJ2RVAL(ret);
}

static VALUE
rg_serialize(VALUE self)
{
    guint stream_length;
    guint i;
    guint8* ret = gdk_pixdata_serialize(_SELF(self), &stream_length);
    VALUE ary = rb_ary_new2(stream_length);
    for (i = 0; i < stream_length; i++) {
        rb_ary_push(ary, UINT2NUM(ret[i]));
    }
    return ary;
}

static VALUE
rg_s_deserialize(VALUE self, VALUE rbstream)
{
    GdkPixdata pixdata;
    long n;
    guint8 *stream = RVAL2GUINT8S(rbstream, n);
    GError *error = NULL;

    /* TODO: Should we really be creating a new array here?  As far as I can
       tell, the data is copied. */
    if (!gdk_pixdata_deserialize(&pixdata, n, stream, &error))
        RAISE_GERROR(error);

    /* need to manage the returned value */
    rb_ivar_set(self, id_pixdata, Data_Wrap_Struct(rb_cData, NULL, g_free, stream));

    return GDKPIXDATA2RVAL(&pixdata);
}

static VALUE
rg_to_csource(VALUE self, VALUE name, VALUE dump_type)
{
    GString* str = gdk_pixdata_to_csource(_SELF(self), RVAL2CSTR(name), FIX2INT(dump_type));
    VALUE ret = CSTR2RVAL(str->str);
    g_string_free(str, TRUE);
    return ret;
}

/* GdkPixdata */
static VALUE
rg_magic(VALUE self)
{
    return UINT2NUM(_SELF(self)->magic);
}

static VALUE
rg_length(VALUE self)
{
    gint32 length = _SELF(self)->length;

    if(length > 0)
        length -= GDK_PIXDATA_HEADER_LENGTH;
    return INT2NUM(length);
}

static VALUE
rg_pixdata_type(VALUE self)
{
    return UINT2NUM(_SELF(self)->pixdata_type);
}

static VALUE
rg_rowstride(VALUE self)
{
    return INT2NUM(_SELF(self)->rowstride);
}

static VALUE
rg_width(VALUE self)
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
rg_height(VALUE self)
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
rg_pixel_data(VALUE self)
{
    gint i;
    guint8* ret = _SELF(self)->pixel_data;
    gint32 length = _SELF(self)->length - GDK_PIXDATA_HEADER_LENGTH;

    VALUE ary = rb_ary_new2(length);
    for (i = 0; i < length; i++) {
        rb_ary_push(ary, UINT2NUM(ret[i]));
    }
    return ary;
}

void
Init_gdk_pixdata(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_PIXDATA, "Pixdata", mGdk);    

    id_pixdata = rb_intern("pixdata");

    RG_DEF_SMETHOD(from_pixbuf, 2);
    RG_DEF_SMETHOD(deserialize, 1);
    RG_DEF_METHOD(to_pixbuf, 1);
    RG_DEF_METHOD(serialize, 0);
    RG_DEF_METHOD(to_csource, 2);
    RG_DEF_METHOD(magic, 0);
    RG_DEF_METHOD(length, 0);
    RG_DEF_METHOD(pixdata_type, 0);
    RG_DEF_METHOD(rowstride, 0);
    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(height, 0);
    RG_DEF_METHOD(pixel_data, 0);

    rb_define_const(RG_TARGET_NAMESPACE, "PIXBUF_MAGIC_NUMBER", INT2NUM(GDK_PIXBUF_MAGIC_NUMBER));
    rb_define_const(RG_TARGET_NAMESPACE, "HEADER_LENGTH", INT2NUM(GDK_PIXDATA_HEADER_LENGTH));

    /* GdkPixdataType */
    rb_define_const(RG_TARGET_NAMESPACE, "COLOR_TYPE_RGB", INT2FIX(GDK_PIXDATA_COLOR_TYPE_RGB));
    rb_define_const(RG_TARGET_NAMESPACE, "COLOR_TYPE_RGBA", INT2FIX(GDK_PIXDATA_COLOR_TYPE_RGBA));
    rb_define_const(RG_TARGET_NAMESPACE, "COLOR_TYPE_MASK", INT2FIX(GDK_PIXDATA_COLOR_TYPE_MASK));
    rb_define_const(RG_TARGET_NAMESPACE, "SAMPLE_WIDTH_8", INT2FIX(GDK_PIXDATA_SAMPLE_WIDTH_8));
    rb_define_const(RG_TARGET_NAMESPACE, "SAMPLE_WIDTH_MASK", INT2FIX(GDK_PIXDATA_SAMPLE_WIDTH_MASK));
    rb_define_const(RG_TARGET_NAMESPACE, "ENCODING_RAW", INT2FIX(GDK_PIXDATA_ENCODING_RAW));
    rb_define_const(RG_TARGET_NAMESPACE, "ENCODING_RLE", INT2FIX(GDK_PIXDATA_ENCODING_RLE));
    rb_define_const(RG_TARGET_NAMESPACE, "ENCODING_MASK", INT2FIX(GDK_PIXDATA_ENCODING_MASK));

    /* GdkPixdataDumpType */
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_PIXDATA_STREAM", INT2FIX(GDK_PIXDATA_DUMP_PIXDATA_STREAM));
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_PIXDATA_STRUCT", INT2FIX(GDK_PIXDATA_DUMP_PIXDATA_STRUCT));
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_MACROS", INT2FIX(GDK_PIXDATA_DUMP_MACROS));
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_GTYPES", INT2FIX(GDK_PIXDATA_DUMP_GTYPES));
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_CTYPES", INT2FIX(GDK_PIXDATA_DUMP_CTYPES));
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_STATIC", INT2FIX(GDK_PIXDATA_DUMP_STATIC));
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_CONST", INT2FIX(GDK_PIXDATA_DUMP_CONST));
    rb_define_const(RG_TARGET_NAMESPACE, "DUMP_RLE_DECODER", INT2FIX(GDK_PIXDATA_DUMP_RLE_DECODER));
}
