/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcheckmenuitem.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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
    VALUE label, use_underline;
    GtkWidget *widget = NULL;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline) > 0) {
        if (NIL_P(use_underline) || RVAL2CBOOL(use_underline)){
            widget = gtk_check_menu_item_new_with_mnemonic(RVAL2CSTR(label));
        } else {
            widget = gtk_check_menu_item_new_with_label(RVAL2CSTR(label));
        }
    }
    else {
	widget = gtk_check_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

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
