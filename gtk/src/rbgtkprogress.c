/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkprogress.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void 
Init_gtk_progress()
{
    G_DEF_CLASS(GTK_TYPE_PROGRESS, "Progress", mGtk);
}
