/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtklistitem.c -

  $Author: igapy $
  $Date: 2002/05/30 00:46:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#ifndef GTK_DISABLE_DEPRECATED
#include "global.h"

static VALUE
litem_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &label) == 1) {
	widget = gtk_list_item_new_with_label(STR2CSTR(label));
    }
    else {
	widget = gtk_list_item_new();
    }

    set_widget(self, widget);
    return Qnil;
}

void Init_gtk_list_item()
{
    static rbgtk_class_info cinfo;

    gListItem = rb_define_class_under(mGtk, "ListItem", gItem);
    cinfo.klass = gListItem;
    cinfo.gtype = GTK_TYPE_LIST_ITEM;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    /*
      rb_define_const(gListItem, "SIGNAL_", rb_str_new2(""));
    */

    rb_define_method(gListItem, "initialize", litem_initialize, -1);
}
#endif
