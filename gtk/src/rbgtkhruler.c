/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhruler.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hruler_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_hruler_new());
    return Qnil;
}

void Init_gtk_hruler()
{
    static rbgtk_class_info cinfo;

    gHRuler = rb_define_class_under(mGtk, "HRuler", gRuler);
    cinfo.klass = gHRuler;
    cinfo.gtype = GTK_TYPE_HRULER;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gHRuler, "initialize", hruler_initialize, 0);
}
