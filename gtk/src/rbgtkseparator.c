/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkseparator.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void Init_gtk_separator()
{
    static rbgtk_class_info cinfo;

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
