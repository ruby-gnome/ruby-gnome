/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkseparator.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void 
Init_gtk_separator()
{
    G_DEF_CLASS(GTK_TYPE_SEPARATOR, "Separator", mGtk);

    /* child init */
    Init_gtk_hseparator();
    Init_gtk_vseparator();
}
