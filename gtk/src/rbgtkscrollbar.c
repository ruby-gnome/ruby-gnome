/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkscrollbar.c -

  $Author: mutoh $
  $Date: 2003/01/19 14:28:25 $

  Copyright (C) 2002,2003 The Ruby-GNOME2 Project
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void 
Init_gtk_scrollbar()
{
    G_DEF_CLASS(GTK_TYPE_SCROLLBAR, "Scrollbar", mGtk);
}
