/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkimage.c -

  $Author: mutoh $
  $Date: 2002/08/29 13:07:01 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
image_initialize(self, val, mask)
    VALUE self, val, mask;
{
    RBGTK_INITIALIZE(self, gtk_image_new_from_image(
						 GDK_IMAGE(RVAL2GOBJ(val)),
						 GDK_BITMAP(RVAL2GOBJ(mask))));
    return Qnil;
}

static VALUE
image_set(self, val, mask)
    VALUE self, val, mask;
{
    gtk_image_set(GTK_IMAGE(RVAL2GOBJ(self)), 
                  GDK_IMAGE(RVAL2GOBJ(val)),
				  GDK_PIXMAP(RVAL2GOBJ(mask)));
    return self;
}

static VALUE
image_get(self)
    VALUE self;
{
    GdkImage  *val;
    GdkBitmap *mask;

    gtk_image_get(GTK_IMAGE(RVAL2GOBJ(self)), &val, &mask);
    return rb_assoc_new(GOBJ2RVAL(val),GOBJ2RVAL(mask));
}

void 
Init_gtk_image()
{
    VALUE gImage = G_DEF_CLASS(GTK_TYPE_IMAGE, "Image", mGtk);

    rb_define_method(gImage, "initialize", image_initialize, 2);
    rb_define_method(gImage, "set", image_set, 2);
    rb_define_method(gImage, "get", image_get, 0);
}
