/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkseparator.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void Init_gtk_separator()
{
    static RGObjClassInfo cinfo;

    gSeparator = rb_define_class_under(mGtk, "Separator", gWidget);
    cinfo.klass = gSeparator;
    cinfo.gtype = GTK_TYPE_SEPARATOR;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /* child init */
    Init_gtk_hseparator();
    Init_gtk_vseparator();
}
