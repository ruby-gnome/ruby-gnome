/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhseparator.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hsep_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_hseparator_new());
    return Qnil;
}

void Init_gtk_hseparator()
{
    static rbgtk_class_info cinfo;

    gHSeparator = rb_define_class_under(mGtk, "HSeparator", gSeparator);
    cinfo.klass = gHSeparator;
    cinfo.gtype = GTK_TYPE_HSEPARATOR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gHSeparator, "initialize", hsep_initialize, 0);
}
