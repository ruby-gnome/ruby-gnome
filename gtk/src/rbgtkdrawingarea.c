/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkdrawingarea.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:04 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
darea_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_drawing_area_new());
    return Qnil;
}

static VALUE
darea_size(self, w, h)
    VALUE self, w, h;
{
    gtk_drawing_area_size(GTK_DRAWING_AREA(get_widget(self)),
			  NUM2INT(w), NUM2INT(h));
    return self;
}

void Init_gtk_drawing_area()
{
    gDrawArea = rb_define_class_under(mGtk, "DrawingArea", gWidget);

    rb_define_method(gDrawArea, "initialize", darea_initialize, 0);
    rb_define_method(gDrawArea, "size", darea_size, 2);

    /* child initialization */
    Init_gtk_curve();
}
