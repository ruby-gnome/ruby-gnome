/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcheckmenuitem.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

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

void 
Init_gtk_check_menu_item()
{
    VALUE gCMenuItem = G_DEF_CLASS(GTK_TYPE_CHECK_MENU_ITEM, "CheckMenuItem", mGtk);

    rb_define_method(gCMenuItem, "initialize", cmitem_initialize, -1);
    rb_define_method(gCMenuItem, "set_state", cmitem_set_state, 1);
    rb_define_method(gCMenuItem, "set_active", cmitem_set_active, 1);
    rb_define_method(gCMenuItem, "active=", cmitem_set_active, 1);
    rb_define_method(gCMenuItem, "active?", cmitem_is_active, 0);
    rb_define_method(gCMenuItem, "set_show_toggle", cmitem_set_show_toggle, 1);
    rb_define_method(gCMenuItem, "toggled", cmitem_toggled, 0);
}
