/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkimage.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:35 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

VALUE gdkImage;

VALUE
make_gdkimage(image)
    GdkImage *image;
{
    return Data_Wrap_Struct(gdkImage, 0, gdk_image_destroy, image);
}

GdkImage*
get_gdkimage(image)
    VALUE image;
{
    GdkImage *gimage;

    if (NIL_P(image)) return NULL;

    if (!rb_obj_is_instance_of(image, gdkImage)) {
	rb_raise(rb_eTypeError, "not a GdkImage");
    }
    Data_Get_Struct(image, GdkImage, gimage);
    if (gimage == 0) {
	rb_raise(rb_eArgError, "destroyed GdkImage");
    }

    return gimage;
}

static VALUE
gdkimage_s_newbmap(klass, visual, data, w, h)
    VALUE klass, visual, data, w, h;
{
    int width = NUM2INT(w);
    int height = NUM2INT(h);
    Check_Type(data, T_STRING);
    if (RSTRING(data)->len < (width * height)) {
        rb_raise(rb_eArgError, "data too short");
    }
    return make_gdkimage(gdk_image_new_bitmap(get_gdkvisual(visual),
                         RSTRING(data)->ptr,
                         width, height));
}

static VALUE
gdkimage_s_new(klass, type, visual, w, h)
    VALUE klass, type, visual, w, h;
{
    return make_gdkimage(gdk_image_new((GdkImageType)NUM2INT(type),
                         get_gdkvisual(visual),
                         NUM2INT(w), NUM2INT(h)));
}

static VALUE
gdkimage_s_get(klass, win, x, y, w, h)
    VALUE klass, win, x, y, w, h;
{
    return make_gdkimage(gdk_image_get(get_gdkdrawable(win),
                         NUM2INT(x), NUM2INT(y),
                         NUM2INT(w), NUM2INT(h)));
}

static VALUE
gdkimage_put_pixel(self, x, y, pix)
    VALUE self, x, y, pix;
{
    gdk_image_put_pixel(get_gdkimage(self),
                        NUM2INT(x), NUM2INT(y), NUM2INT(pix));
    return self;
}

static VALUE
gdkimage_get_pixel(self, x, y)
    VALUE self, x, y;
{
    return INT2NUM(gdk_image_get_pixel(get_gdkimage(self),
                                       NUM2INT(x), NUM2INT(y)));
}

static VALUE
gdkimage_destroy(self)
    VALUE self;
{
    gdk_image_destroy(get_gdkimage(self));
    DATA_PTR(self) = 0;
    return Qnil;
}

static VALUE
gdkimage_width(self)
    VALUE self;
{
    return INT2NUM((get_gdkimage(self))->width);
}

static VALUE
gdkimage_height(self)
    VALUE self;
{
    return INT2NUM((get_gdkimage(self))->height);
}

static VALUE
gdkimage_depth(self)
    VALUE self;
{
    return INT2NUM((get_gdkimage(self))->depth);
}

static VALUE
gdkimage_bpp(self)
    VALUE self;
{
    return INT2NUM((get_gdkimage(self))->bpp);
}

static VALUE
gdkimage_bpl(self)
    VALUE self;
{
    return INT2NUM((get_gdkimage(self))->bpl);
}

void Init_gtk_gdk_image() {
    gdkImage = rb_define_class_under(mGdk, "Image", rb_cData);

    rb_define_singleton_method(gdkImage, "new_bitmap", gdkimage_s_newbmap, 4);
    rb_define_singleton_method(gdkImage, "new", gdkimage_s_new, 4);
    rb_define_singleton_method(gdkImage, "get", gdkimage_s_get, 5);

    rb_define_method(gdkImage, "put_pixel", gdkimage_put_pixel, 3);
    rb_define_method(gdkImage, "get_pixel", gdkimage_get_pixel, 2);
    rb_define_method(gdkImage, "destroy", gdkimage_destroy, 0);
    rb_define_method(gdkImage, "width", gdkimage_width, 0);
    rb_define_method(gdkImage, "height", gdkimage_height, 0);
    rb_define_method(gdkImage, "depth", gdkimage_depth, 0);
    rb_define_method(gdkImage, "bpp", gdkimage_bpp, 0);
    rb_define_method(gdkImage, "bpl", gdkimage_bpl, 0);
}
