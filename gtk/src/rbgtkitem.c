/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkitem.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:04 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Item
 */
static VALUE
item_select(self)
    VALUE self;
{
    gtk_item_select(GTK_ITEM(get_widget(self)));
    return self;
}

static VALUE
item_deselect(self)
    VALUE self;
{
    gtk_item_deselect(GTK_ITEM(get_widget(self)));
    return self;
}

static VALUE
item_toggle(self)
    VALUE self;
{
    gtk_item_toggle(GTK_ITEM(get_widget(self)));
    return self;
}

void Init_gtk_item()
{
    gItem = rb_define_class_under(mGtk, "Item", gBin);

    rb_define_const(gItem, "SIGNAL_SELECT", rb_str_new2("select"));
    rb_define_const(gItem, "SIGNAL_DESELECT", rb_str_new2("deselect"));
    rb_define_const(gItem, "SIGNAL_TOGGLE", rb_str_new2("toggle"));

    rb_define_method(gItem, "select", item_select, 0);
    rb_define_method(gItem, "deselect", item_deselect, 0);
    rb_define_method(gItem, "toggle", item_toggle, 0);

    /* child initialization */
    Init_gtk_list_item();
    Init_gtk_menu_item();
    Init_gtk_tree_item();
}
