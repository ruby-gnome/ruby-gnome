/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkimage.c -

  $Author: mutoh $
  $Date: 2002/05/19 13:59:10 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
image_initialize(self, val, mask)
    VALUE self, val, mask;
{
#if GTK_MAJOR_VERSION < 2
    set_widget(self, gtk_image_new(get_gdkimage(val),
				   (GdkBitmap*)get_gdkpixmap(mask)));
#else
    set_widget(self, gtk_image_new_from_image(
        get_gdkimage(val),
        (GdkBitmap*)get_gdkpixmap(mask)));
#endif
    return Qnil;
}

static VALUE
image_set(self, val, mask)
    VALUE self, val, mask;
{
    gtk_image_set(GTK_IMAGE(get_widget(self)), get_gdkimage(val),
		  get_gdkpixmap(mask));
    return self;
}

static VALUE
image_get(self)
    VALUE self;
{
    GdkImage  *val;
    GdkBitmap *mask;

    gtk_image_get(GTK_IMAGE(get_widget(self)), &val, &mask);

    return rb_assoc_new(make_gdkimage(val),
			make_gdkpixmap(mask));
}

void Init_gtk_image()
{
    gImage = rb_define_class_under(mGtk, "Image", gMisc);

    rb_define_method(gImage, "initialize", image_initialize, 2);
    rb_define_method(gImage, "set", image_set, 2);
    rb_define_method(gImage, "get", image_get, 0);
}
