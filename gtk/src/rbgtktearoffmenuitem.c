/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktearoffmenuitem.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:05 $

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
    set_widget(self, widget);
    return Qnil;
}

void Init_gtk_tearoff_menu_item()
{
    gTMenuItem = rb_define_class_under(mGtk, "TearoffMenuItem", gMenuItem);

    rb_define_method(gTMenuItem, "initialize", tmitem_initialize, 0);
}
