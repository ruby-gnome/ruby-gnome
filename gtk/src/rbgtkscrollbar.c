/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscrollbar.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void Init_gtk_scrollbar()
{
  static rbgtk_class_info cinfo;

  gScrollbar = rb_define_class_under(mGtk, "Scrollbar", gRange);
  cinfo.klass = gScrollbar;
  cinfo.gtype = GTK_TYPE_SCROLLBAR;
  cinfo.mark = 0;
  cinfo.free = 0;
  rbgtk_register_class(&cinfo);

  /* child init */
  Init_gtk_hscrollbar();
  Init_gtk_vscrollbar();
}
