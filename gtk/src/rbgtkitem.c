/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkitem.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

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
    gtk_item_select(GTK_ITEM(RVAL2GOBJ(self)));
    return self;
}

static VALUE
item_deselect(self)
    VALUE self;
{
    gtk_item_deselect(GTK_ITEM(RVAL2GOBJ(self)));
    return self;
}

static VALUE
item_toggle(self)
    VALUE self;
{
    gtk_item_toggle(GTK_ITEM(RVAL2GOBJ(self)));
    return self;
}

void Init_gtk_item()
{
    static rbgtk_class_info cinfo;

    gItem = rb_define_class_under(mGtk, "Item", gBin);
    cinfo.klass = gItem;
    cinfo.gtype = GTK_TYPE_ITEM;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gItem, "SIGNAL_SELECT", rb_str_new2("select"));
    rb_define_const(gItem, "SIGNAL_DESELECT", rb_str_new2("deselect"));
    rb_define_const(gItem, "SIGNAL_TOGGLE", rb_str_new2("toggle"));

    rb_define_method(gItem, "select", item_select, 0);
    rb_define_method(gItem, "deselect", item_deselect, 0);
    rb_define_method(gItem, "toggle", item_toggle, 0);

    /* child initialization */
    Init_gtk_list_item();
    Init_gtk_menu_item();
#ifdef GTK_ENABLE_BROKEN
    Init_gtk_tree_item();
#endif
}
