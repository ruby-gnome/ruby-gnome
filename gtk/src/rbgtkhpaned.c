/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhpaned.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hpaned_initialize(self)
    VALUE self;
{
    set_widget(self, gtk_hpaned_new());
    return Qnil;
}

void Init_gtk_hpaned()
{
    static rbgtk_class_info cinfo;

    gHPaned = rb_define_class_under(mGtk, "HPaned", gPaned);
    cinfo.klass = gHPaned;
    cinfo.gtype = GTK_TYPE_HPANED;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gHPaned, "initialize", hpaned_initialize, 0);
}
