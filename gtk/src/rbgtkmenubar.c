/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenubar.c -

  $Author: mutoh $
  $Date: 2002/10/25 17:51:24 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
mbar_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_menu_bar_new());
    return Qnil;
}

void 
Init_gtk_menu_bar()
{
    VALUE gMenuBar = G_DEF_CLASS(GTK_TYPE_MENU_BAR, "MenuBar", mGtk);
    rb_define_method(gMenuBar, "initialize", mbar_initialize, 0);
}
