/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscrollbar.c -

  $Author: mutoh $
  $Date: 2002/09/10 17:43:30 $

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
