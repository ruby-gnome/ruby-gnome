/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkseparatormenuitem.c -

  $Author: mutoh $
  $Date: 2002/11/15 13:27:23 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

static VALUE
smitem_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_separator_menu_item_new());
    return Qnil;
}

void 
Init_gtk_separator_menu_item()
{
    VALUE gSMenuItem = G_DEF_CLASS(GTK_TYPE_SEPARATOR_MENU_ITEM, "SeparatorMenuItem", mGtk);

    rb_define_method(gSMenuItem, "initialize", smitem_initialize, -1);
}
