/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkitem.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

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

void 
Init_gtk_item()
{
    VALUE gItem = G_DEF_CLASS(GTK_TYPE_ITEM, "Item", mGtk);
    rb_define_method(gItem, "select", item_select, 0);
    rb_define_method(gItem, "deselect", item_deselect, 0);
    rb_define_method(gItem, "toggle", item_toggle, 0);
}
