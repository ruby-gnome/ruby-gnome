/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkhseparator.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
hsep_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_hseparator_new());
    return Qnil;
}

void Init_gtk_hseparator()
{
    static RGObjClassInfo cinfo;

    gHSeparator = rb_define_class_under(mGtk, "HSeparator", gSeparator);
    cinfo.klass = gHSeparator;
    cinfo.gtype = GTK_TYPE_HSEPARATOR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gHSeparator, "initialize", hsep_initialize, 0);
}
