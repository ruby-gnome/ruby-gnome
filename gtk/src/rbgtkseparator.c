/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkseparator.c -

  $Author: sakai $
  $Date: 2002/08/01 17:46:19 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void 
Init_gtk_separator()
{
    G_DEF_CLASS(GTK_TYPE_SEPARATOR, "Separator", mGtk);
}
