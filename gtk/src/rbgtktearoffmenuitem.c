/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtktearoffmenuitem.c -

  $Author: mutoh $
  $Date: 2002/06/23 16:13:32 $

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

void Init_gtk_tearoff_menu_item()
{
    static RGObjClassInfo cinfo;

    gTMenuItem = rb_define_class_under(mGtk, "TearoffMenuItem", gMenuItem);
    cinfo.klass = gTMenuItem;
    cinfo.gtype = GTK_TYPE_TEAROFF_MENU_ITEM;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_method(gTMenuItem, "initialize", tmitem_initialize, 0);
}
