/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkscrollbar.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

void Init_gtk_scrollbar()
{
  static RGObjClassInfo cinfo;

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
