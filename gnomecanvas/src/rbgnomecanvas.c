/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgnomecanvas.c -

  $Author: mutoh $
  $Date: 2005/10/01 19:44:15 $

  Copyright (C) 2002 KUBO Takehiro 
************************************************/

#include "rbgnomecanvas.h"

void
Init_gnomecanvas2(void)
{
    VALUE mGnome = rb_define_module("Gnome");

    Init_gnome_canvas(mGnome);
    Init_gnome_canvas_util(mGnome);
    Init_gnome_canvas_item(mGnome);
    Init_gnome_canvas_rich_text(mGnome);
    Init_gnome_canvas_path_def(mGnome);
    Init_gnome_canvas_buf(mGnome);
}
