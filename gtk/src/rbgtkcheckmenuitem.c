/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcheckmenuitem.c -

  $Author: mutoh $
  $Date: 2002/10/21 17:29:30 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
cmitem_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &label) == 1) {
	widget = gtk_check_menu_item_new_with_label(RVAL2CSTR(label));
    }
    else {
	widget = gtk_check_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

/*
GtkWidget*  gtk_check_menu_item_new_with_mnemonic
                                            (const gchar *label);
*/

static VALUE
cmitem_toggled(self)
    VALUE self;
{
    gtk_check_menu_item_toggled(GTK_CHECK_MENU_ITEM(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_check_menu_item()
{
    VALUE gCMenuItem = G_DEF_CLASS(GTK_TYPE_CHECK_MENU_ITEM, "CheckMenuItem", mGtk);

    rb_define_method(gCMenuItem, "initialize", cmitem_initialize, -1);
    rb_define_method(gCMenuItem, "toggled", cmitem_toggled, 0);
}
