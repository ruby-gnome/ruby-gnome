/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkcheckmenuitem.c -

  $Author: mutoh $
  $Date: 2002/06/22 19:50:57 $

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
	widget = gtk_check_menu_item_new_with_label(STR2CSTR(label));
    }
    else {
	widget = gtk_check_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
cmitem_set_state(self, state)
    VALUE self, state;
{
    gtk_check_menu_item_set_state(GTK_CHECK_MENU_ITEM(RVAL2GOBJ(self)), 
				  NUM2INT(state));
    return self;
}

static VALUE
cmitem_set_active(self, state)
    VALUE self, state;
{
    gtk_check_menu_item_set_state(GTK_CHECK_MENU_ITEM(RVAL2GOBJ(self)), 
				  RTEST(state));
    return self;
}

static VALUE
cmitem_is_active(self)
    VALUE self;
{
    return GTK_CHECK_MENU_ITEM(RVAL2GOBJ(self))->active==0?Qfalse:Qtrue;
}

static VALUE
cmitem_set_show_toggle(self, always)
    VALUE self, always;
{
    gtk_check_menu_item_set_show_toggle(GTK_CHECK_MENU_ITEM(RVAL2GOBJ(self)), 
					(gboolean)RTEST(always));
    return self;
}

static VALUE
cmitem_toggled(self)
    VALUE self;
{
    gtk_check_menu_item_toggled(GTK_CHECK_MENU_ITEM(RVAL2GOBJ(self)));
    return self;
}

void Init_gtk_check_menu_item()
{
    static rbgtk_class_info cinfo;

    gCMenuItem = rb_define_class_under(mGtk, "CheckMenuItem", gMenuItem);
    cinfo.klass = gCMenuItem;
    cinfo.gtype = GTK_TYPE_CHECK_MENU_ITEM;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_const(gCMenuItem, "SIGNAL_TOGGLED", rb_str_new2("toggled"));

    rb_define_method(gCMenuItem, "initialize", cmitem_initialize, -1);
    rb_define_method(gCMenuItem, "set_state", cmitem_set_state, 1);
    rb_define_method(gCMenuItem, "set_active", cmitem_set_active, 1);
    rb_define_method(gCMenuItem, "active=", cmitem_set_active, 1);
    rb_define_method(gCMenuItem, "active?", cmitem_is_active, 0);
    rb_define_method(gCMenuItem, "set_show_toggle", cmitem_set_show_toggle, 1);
    rb_define_method(gCMenuItem, "toggled", cmitem_toggled, 0);

    /* child initialization */
    Init_gtk_radio_menu_item();
}
