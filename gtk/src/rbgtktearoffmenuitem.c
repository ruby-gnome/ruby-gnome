/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtktearoffmenuitem.c -

  $Author: mutoh $
  $Date: 2002/10/21 17:29:30 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
tmitem_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_tearoff_menu_item_new());
    return Qnil;
}

void 
Init_gtk_tearoff_menu_item()
{
    VALUE gTMenuItem = G_DEF_CLASS(GTK_TYPE_TEAROFF_MENU_ITEM, "TearoffMenuItem", mGtk);
    rb_define_method(gTMenuItem, "initialize", tmitem_initialize, 0);
}
