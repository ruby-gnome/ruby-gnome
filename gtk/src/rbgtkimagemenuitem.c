/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkimagemenuitem.c -

  $Author: mutoh $
  $Date: 2002/11/15 13:27:23 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "global.h"

static VALUE
imitem_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &label) == 1) {
	widget = gtk_image_menu_item_new_with_label(RVAL2CSTR(label));
    }
    else {
	widget = gtk_image_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

/*
GtkWidget*  gtk_image_menu_item_new_from_stock
                                            (const gchar *stock_id,
                                             GtkAccelGroup *accel_group);
GtkWidget*  gtk_image_menu_item_new_with_mnemonic
                                            (const gchar *label);
*/

void 
Init_gtk_image_menu_item()
{
    VALUE gIMenuItem = G_DEF_CLASS(GTK_TYPE_IMAGE_MENU_ITEM, "ImageMenuItem", mGtk);

    rb_define_method(gIMenuItem, "initialize", imitem_initialize, -1);
}
