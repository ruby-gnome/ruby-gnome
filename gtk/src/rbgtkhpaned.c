/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhpaned.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hpaned_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_hpaned_new());
    return Qnil;
}

void Init_gtk_hpaned()
{
    static RGObjClassInfo cinfo;

    gHPaned = rb_define_class_under(mGtk, "HPaned", gPaned);
    cinfo.klass = gHPaned;
    cinfo.gtype = GTK_TYPE_HPANED;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gHPaned, "initialize", hpaned_initialize, 0);
}
