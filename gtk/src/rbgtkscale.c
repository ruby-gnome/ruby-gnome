/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkscale.c -

  $Author: mutoh $
  $Date: 2002/10/25 17:51:25 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void 
Init_gtk_scale()
{
    G_DEF_CLASS(GTK_TYPE_SCALE, "Scale", mGtk);
}
