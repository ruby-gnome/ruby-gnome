/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkpixmap.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED

#include "global.h"

static VALUE
pixmap_initialize(self, pixmap, mask)
    VALUE self, pixmap, mask;
{
    RBGTK_INITIALIZE(self, gtk_pixmap_new(GDK_PIXMAP(RVAL2GOBJ(pixmap)),
										  GDK_BITMAP(RVAL2GOBJ(mask))));
    return Qnil;
}

static VALUE
pixmap_set(self, pixmap, mask)
    VALUE self, pixmap, mask;
{
    gtk_pixmap_set(GTK_PIXMAP(RVAL2GOBJ(self)), 
				   GDK_PIXMAP(RVAL2GOBJ(pixmap)),
				   GDK_BITMAP(RVAL2GOBJ(mask)));
	return self;
}

static VALUE
pixmap_get(self)
    VALUE self;
{
    GdkPixmap  *val;
    GdkBitmap *mask;

    gtk_pixmap_get(GTK_PIXMAP(RVAL2GOBJ(self)), &val, &mask);
	return rb_assoc_new(GOBJ2RVAL(val),GOBJ2RVAL(mask));
}

void 
Init_gtk_pixmap()  /* GTK_DISABLE_DEPRECATED */
{
    VALUE gPixmap = G_DEF_CLASS(GTK_TYPE_PIXMAP, "Pixmap", mGtk);

    rb_define_method(gPixmap, "initialize", pixmap_initialize, 2);
    rb_define_method(gPixmap, "set", pixmap_set, 2);
    rb_define_method(gPixmap, "get", pixmap_get, 0);
}
#endif
