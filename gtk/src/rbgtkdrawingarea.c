/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkdrawingarea.c -

  $Author: mutoh $
  $Date: 2002/10/25 17:51:24 $

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

void 
Init_gtk_drawing_area()
{
    VALUE gDrawArea = G_DEF_CLASS(GTK_TYPE_DRAWING_AREA, "DrawingArea", mGtk);

    rb_define_method(gDrawArea, "initialize", darea_initialize, 0);
}
