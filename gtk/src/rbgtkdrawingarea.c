/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkdrawingarea.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

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
    static rbgtk_class_info cinfo;

    gDrawArea = rb_define_class_under(mGtk, "DrawingArea", gWidget);
    cinfo.klass = gDrawArea;
    cinfo.gtype = GTK_TYPE_DRAWING_AREA;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gDrawArea, "initialize", darea_initialize, 0);
    rb_define_method(gDrawArea, "size", darea_size, 2);

    /* child initialization */
    Init_gtk_curve();
}
