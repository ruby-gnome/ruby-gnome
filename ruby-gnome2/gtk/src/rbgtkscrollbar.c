/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscrollbar.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:37 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void Init_gtk_scrollbar()
{
  gScrollbar = rb_define_class_under(mGtk, "Scrollbar", gRange);

  /* child init */
  Init_gtk_hscrollbar();
  Init_gtk_vscrollbar();
}
