/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktearoffmenuitem.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
tmitem_initialize(self)
    VALUE self;
{
    GtkWidget *widget;

    widget = gtk_tearoff_menu_item_new();
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_tearoff_menu_item()
{
    VALUE gTMenuItem = G_DEF_CLASS(GTK_TYPE_TEAROFF_MENU_ITEM, "TearoffMenuItem", mGtk);
    rb_define_method(gTMenuItem, "initialize", tmitem_initialize, 0);
}
