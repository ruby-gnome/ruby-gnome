/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkradiomenuitem.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:41 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi,
                          dellin 
************************************************/

#include "global.h"

static VALUE
rmitem_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE arg1, arg2;
    GtkWidget *widget;
    GSList *list = NULL;
    char *label = NULL;
    
    if (rb_scan_args(argc, argv, "02", &arg1, &arg2) == 1 &&
	TYPE(arg1) == T_STRING) {
	label = RSTRING(arg1)->ptr;
    }
    else {
	if (!NIL_P(arg2)) {
	    label = STR2CSTR(arg2);
	}
	if (rb_obj_is_kind_of(arg1, gRMenuItem)) {
	    GtkWidget *b = get_widget(arg1);
	    list = GTK_RADIO_MENU_ITEM(b)->group;
	}
	else {
	    list = ary2gslist(arg1);
	}
    }
    if (label) {
	widget = gtk_radio_menu_item_new_with_label(list, label);
    }
    else {
	widget = gtk_radio_menu_item_new(list);
    }
    set_widget(self, widget);
    return Qnil;
}

static VALUE
rmitem_group(self)
    VALUE self;
{
    return gslist2ary(gtk_radio_menu_item_group(GTK_RADIO_MENU_ITEM(get_widget(self))));
}

static VALUE
rmitem_set_group(self, grp_ary)
    VALUE self, grp_ary;
{
    GtkRadioMenuItem *rmitem2add, *rmitem_orig;
    GSList *group;

    rmitem2add = GTK_RADIO_MENU_ITEM(get_widget(self));
    rmitem_orig = GTK_RADIO_MENU_ITEM(get_widget(rb_ary_entry(grp_ary, 0)));
    group = gtk_radio_menu_item_group(rmitem_orig);

    gtk_radio_menu_item_set_group(rmitem2add, group);

    return gslist2ary(gtk_radio_menu_item_group(rmitem2add));
}

void Init_gtk_radio_menu_item()
{
    gRMenuItem = rb_define_class_under(mGtk, "RadioMenuItem", gCMenuItem);

    rb_define_method(gRMenuItem, "initialize", rmitem_initialize, -1);
    rb_define_method(gRMenuItem, "group", rmitem_group, 0);
    rb_define_method(gRMenuItem, "set_group", rmitem_set_group, 1);
}
