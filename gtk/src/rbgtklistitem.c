/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtklistitem.c -

  $Author: mutoh $
  $Date: 2002/09/14 15:43:41 $

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
		widget = gtk_list_item_new_with_label(RVAL2CSTR(label));
    }
    else {
		widget = gtk_list_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_list_item()
{
    VALUE gListItem = G_DEF_CLASS(GTK_TYPE_LIST_ITEM, "ListItem", mGtk);

    rb_define_method(gListItem, "initialize", litem_initialize, -1);
}
#endif
