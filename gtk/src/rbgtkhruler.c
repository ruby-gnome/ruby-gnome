/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhruler.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hruler_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_hruler_new());
    return Qnil;
}

void Init_gtk_hruler()
{
    static RGObjClassInfo cinfo;

    gHRuler = rb_define_class_under(mGtk, "HRuler", gRuler);
    cinfo.klass = gHRuler;
    cinfo.gtype = GTK_TYPE_HRULER;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gHRuler, "initialize", hruler_initialize, 0);
}
