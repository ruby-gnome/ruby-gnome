/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkmenuitem.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static VALUE
mitem_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "01", &label) == 1) {
		widget = gtk_menu_item_new_with_label(STR2CSTR(label));
    }
    else {
		widget = gtk_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
mitem_set_submenu(self, child)
    VALUE self, child;
{
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(RVAL2GOBJ(self)),
							  GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
mitem_select(self)
    VALUE self;
{
    gtk_menu_item_select(GTK_MENU_ITEM(RVAL2GOBJ(self)));
    return self;
}

static VALUE
mitem_deselect(self)
    VALUE self;
{
    gtk_menu_item_deselect(GTK_MENU_ITEM(RVAL2GOBJ(self)));
    return self;
}

static VALUE
mitem_activate(self)
    VALUE self;
{
    gtk_menu_item_activate(GTK_MENU_ITEM(RVAL2GOBJ(self)));
    return self;
}

static VALUE
mitem_right_justify(self)
    VALUE self;
{
    gtk_menu_item_right_justify(GTK_MENU_ITEM(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_menu_item()
{
    VALUE gMenuItem = G_DEF_CLASS(GTK_TYPE_MENU_ITEM, "MenuItem", mGtk);

    rb_define_const(gMenuItem, "SIGNAL_ACTIVATE", rb_str_new2("activate"));

    rb_define_method(gMenuItem, "initialize", mitem_initialize, -1);
    rb_define_method(gMenuItem, "set_submenu", mitem_set_submenu, 1);
    rb_define_method(gMenuItem, "select", mitem_select, 0);
    rb_define_method(gMenuItem, "deselect", mitem_deselect, 0);
    rb_define_method(gMenuItem, "activate", mitem_activate, 0);
    rb_define_method(gMenuItem, "right_justify", mitem_right_justify, 0);

    /* child initialize */
    Init_gtk_check_menu_item();
    Init_gtk_tearoff_menu_item();
}
