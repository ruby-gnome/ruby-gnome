/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkimage.c -

  $Author: sakai $
  $Date: 2003/07/20 05:05:08 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(i) GDK_IMAGE(RVAL2GOBJ(i))

static VALUE
gdkimage_initialize(self, type, visual, w, h)
    VALUE self, type, visual, w, h;
{
    G_INITIALIZE(self, gdk_image_new((GdkImageType)NUM2INT(type),
                                          GDK_VISUAL(RVAL2GOBJ(visual)),
                                          NUM2INT(w), NUM2INT(h)));
    return Qnil;
}

static VALUE
gdkimage_put_pixel(self, x, y, pix)
    VALUE self, x, y, pix;
{
    gdk_image_put_pixel(_SELF(self),
                        NUM2INT(x), NUM2INT(y), NUM2INT(pix));
    return self;
}

static VALUE
gdkimage_get_pixel(self, x, y)
    VALUE self, x, y;
{
    return INT2NUM(gdk_image_get_pixel(_SELF(self),
                                       NUM2INT(x), NUM2INT(y)));
}

static VALUE
gdkimage_width(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->width);
}

static VALUE
gdkimage_height(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->height);
}

static VALUE
gdkimage_depth(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->depth);
}

static VALUE
gdkimage_bpp(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->bpp);
}

static VALUE
gdkimage_bpl(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->bpl);
}

void 
Init_gtk_gdk_image()
{
    VALUE gdkImage = G_DEF_CLASS(GDK_TYPE_IMAGE, "Image", mGdk);

    rb_define_method(gdkImage, "initialize", gdkimage_initialize, 4);
    rb_define_method(gdkImage, "put_pixel", gdkimage_put_pixel, 3);
    rb_define_method(gdkImage, "get_pixel", gdkimage_get_pixel, 2);
    rb_define_method(gdkImage, "width", gdkimage_width, 0);
    rb_define_method(gdkImage, "height", gdkimage_height, 0);
    rb_define_method(gdkImage, "depth", gdkimage_depth, 0);
    rb_define_method(gdkImage, "bpp", gdkimage_bpp, 0);
    rb_define_method(gdkImage, "bpl", gdkimage_bpl, 0);

    /* GdkImageType */
    G_DEF_CONSTANTS(gdkImage, GDK_TYPE_IMAGE_TYPE, "GDK_IMAGE_");
}
