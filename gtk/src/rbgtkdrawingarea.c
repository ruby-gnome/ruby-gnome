/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkdrawingarea.c -

  $Author: mutoh $
  $Date: 2003/02/01 16:46:23 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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
