/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkdrawingarea.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:18 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
darea_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_drawing_area_new());
    return Qnil;
}

static VALUE
darea_size(self, w, h)
    VALUE self, w, h;
{
    gtk_drawing_area_size(GTK_DRAWING_AREA(RVAL2GOBJ(self)),
			  NUM2INT(w), NUM2INT(h));
    return self;
}

void 
Init_gtk_drawing_area()
{
    VALUE gDrawArea = G_DEF_CLASS(GTK_TYPE_DRAWING_AREA, "DrawingArea", mGtk);

    rb_define_method(gDrawArea, "initialize", darea_initialize, 0);
    rb_define_method(gDrawArea, "size", darea_size, 2);
}
