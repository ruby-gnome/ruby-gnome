/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkseparator.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:35 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void Init_gtk_separator()
{
    gSeparator = rb_define_class_under(mGtk, "Separator", gWidget);

    /* child init */
    Init_gtk_hseparator();
    Init_gtk_vseparator();
}
