/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkpixmap.c -

  $Author: mutoh $
  $Date: 2002/05/21 17:32:25 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED

#include "global.h"

static VALUE
pixmap_initialize(self, val, mask)
    VALUE self, val, mask;
{
    GdkPixmap *pixdata;
    GdkBitmap *maskdata;

    pixdata = get_gdkpixmap(val);
    maskdata = get_gdkbitmap(mask);
    set_widget(self, gtk_pixmap_new(pixdata, maskdata));
    return Qnil;
}

static VALUE
pixmap_set(self, val, mask)
    VALUE self, val, mask;
{
    GdkPixmap *pixdata;
    GdkBitmap *maskdata;

    pixdata = get_gdkpixmap(val);
    maskdata = get_gdkbitmap(mask);
    gtk_pixmap_set(GTK_PIXMAP(get_widget(self)), pixdata, maskdata);
    return self;
}

static VALUE
pixmap_get(self)
    VALUE self;
{
    GdkPixmap  *val;
    GdkBitmap *mask;

    gtk_pixmap_get(GTK_PIXMAP(get_widget(self)), &val, &mask);

    return rb_assoc_new(make_gdkpixmap(val),
			make_gdkbitmap(mask));
}

void Init_gtk_pixmap()
{
    gPixmap = rb_define_class_under(mGtk, "Pixmap", gMisc);

    rb_define_method(gPixmap, "initialize", pixmap_initialize, 2);
    rb_define_method(gPixmap, "set", pixmap_set, 2);
    rb_define_method(gPixmap, "get", pixmap_get, 0);
}
#endif
