/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvpaned.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vpaned_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_vpaned_new());
    return Qnil;
}

void Init_gtk_vpaned()
{
    static rbgtk_class_info cinfo;

    gVPaned = rb_define_class_under(mGtk, "VPaned", gPaned);
    cinfo.klass = gVPaned;
    cinfo.gtype = GTK_TYPE_VPANED;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gVPaned, "initialize", vpaned_initialize, 0);
}
