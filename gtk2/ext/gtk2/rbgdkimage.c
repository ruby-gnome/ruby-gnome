/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define _SELF(i) GDK_IMAGE(RVAL2GOBJ(i))

static VALUE
gdkimage_initialize(VALUE self, VALUE type, VALUE visual, VALUE w, VALUE h)
{
    GdkImage* image = gdk_image_new((GdkImageType)RVAL2GENUM(type, GDK_TYPE_IMAGE_TYPE),
                                    GDK_VISUAL(RVAL2GOBJ(visual)),
                                    NUM2INT(w), NUM2INT(h));

    if (image)
        G_INITIALIZE(self, image);
    else
        rb_raise(rb_eArgError, "The image could not be created.");

    return Qnil;
}

static VALUE
gdkimage_get_colormap(VALUE self)
{
    return GOBJ2RVAL(gdk_image_get_colormap(_SELF(self)));
}

static VALUE
gdkimage_set_colormap(VALUE self, VALUE colormap)
{
    gdk_image_set_colormap(_SELF(self), RVAL2GOBJ(colormap));
    return self;
}

static VALUE
gdkimage_put_pixel(VALUE self, VALUE x, VALUE y, VALUE pix)
{
    gdk_image_put_pixel(_SELF(self),
                        NUM2INT(x), NUM2INT(y), NUM2INT(pix));
    return self;
}

static VALUE
gdkimage_get_pixel(VALUE self, VALUE x, VALUE y)
{
    return INT2NUM(gdk_image_get_pixel(_SELF(self),
                                       NUM2INT(x), NUM2INT(y)));
}

static VALUE
gdkimage_image_type(VALUE self)
{
    return GENUM2RVAL((_SELF(self))->type, GDK_TYPE_IMAGE_TYPE);
}

static VALUE
gdkimage_visual(VALUE self)
{
    return GOBJ2RVAL((_SELF(self))->visual);
}

static VALUE
gdkimage_byte_order(VALUE self)
{
    return GENUM2RVAL((_SELF(self))->byte_order, GDK_TYPE_BYTE_ORDER);
}

static VALUE
gdkimage_bits_per_pixel(VALUE self)
{
    return INT2NUM((_SELF(self))->bits_per_pixel);
}

static VALUE
gdkimage_width(VALUE self)
{
    return INT2NUM((_SELF(self))->width);
}

static VALUE
gdkimage_height(VALUE self)
{
    return INT2NUM((_SELF(self))->height);
}

static VALUE
gdkimage_depth(VALUE self)
{
    return INT2NUM((_SELF(self))->depth);
}

static VALUE
gdkimage_bpp(VALUE self)
{
    return INT2NUM((_SELF(self))->bpp);
}

static VALUE
gdkimage_bpl(VALUE self)
{
    return INT2NUM((_SELF(self))->bpl);
}

void 
Init_gtk_gdk_image(void)
{
    VALUE gdkImage = G_DEF_CLASS(GDK_TYPE_IMAGE, "Image", mGdk);

    rb_define_method(gdkImage, "initialize", gdkimage_initialize, 4);
    rb_define_method(gdkImage, "colormap", gdkimage_get_colormap, 0);
    rb_define_method(gdkImage, "set_colormap", gdkimage_set_colormap, 1);
    rb_define_method(gdkImage, "put_pixel", gdkimage_put_pixel, 3);
    rb_define_method(gdkImage, "get_pixel", gdkimage_get_pixel, 2);
    rb_define_method(gdkImage, "image_type", gdkimage_image_type, 0);
    rb_define_method(gdkImage, "visual", gdkimage_visual, 0);
    rb_define_method(gdkImage, "byte_order", gdkimage_byte_order, 0);
    rb_define_method(gdkImage, "bits_per_pixel", gdkimage_bits_per_pixel, 0);
    rb_define_method(gdkImage, "width", gdkimage_width, 0);
    rb_define_method(gdkImage, "height", gdkimage_height, 0);
    rb_define_method(gdkImage, "depth", gdkimage_depth, 0);
    rb_define_method(gdkImage, "bpp", gdkimage_bpp, 0);
    rb_define_method(gdkImage, "bpl", gdkimage_bpl, 0);

    G_DEF_SETTERS(gdkImage);

    /* GdkImageType */
    G_DEF_CLASS(GDK_TYPE_IMAGE_TYPE, "Type", gdkImage);
    G_DEF_CONSTANTS(gdkImage, GDK_TYPE_IMAGE_TYPE, "GDK_IMAGE_");
}
