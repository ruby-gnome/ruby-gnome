/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvseparator.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vsep_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_vseparator_new());
    return Qnil;
}

void Init_gtk_vseparator()
{
    static rbgtk_class_info cinfo;

    gVSeparator = rb_define_class_under(mGtk, "VSeparator", gSeparator);
    cinfo.klass = gVSeparator;
    cinfo.gtype = GTK_TYPE_VSEPARATOR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gVSeparator, "initialize", vsep_initialize, 0);
}
