/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkvseparator.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
vsep_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_vseparator_new());
    return Qnil;
}

void Init_gtk_vseparator()
{
    static RGObjClassInfo cinfo;

    gVSeparator = rb_define_class_under(mGtk, "VSeparator", gSeparator);
    cinfo.klass = gVSeparator;
    cinfo.gtype = GTK_TYPE_VSEPARATOR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gVSeparator, "initialize", vsep_initialize, 0);
}
