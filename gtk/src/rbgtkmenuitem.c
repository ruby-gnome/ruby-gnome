/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenuitem.c -

  $Author: mutoh $
  $Date: 2002/10/21 17:29:30 $

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
        widget = gtk_menu_item_new_with_label(RVAL2CSTR(label));
    }
    else {
        widget = gtk_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

/*
GtkWidget*  gtk_menu_item_new_with_mnemonic (const gchar *label);
*/

static VALUE
mitem_set_right_justified(self, right_justified)
    VALUE self, right_justified;
{
    gtk_menu_item_set_right_justified(GTK_MENU_ITEM(RVAL2GOBJ(self)), RTEST(right_justified));
    return self;
}

static VALUE
mitem_set_submenu(self, child)
    VALUE self, child;
{
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(RVAL2GOBJ(self)),
                              GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}
/*
void        gtk_menu_item_set_accel_path    (GtkMenuItem *menu_item,
                                             const gchar *accel_path);
void        gtk_menu_item_remove_submenu    (GtkMenuItem *menu_item);
*/

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

/*
void        gtk_menu_item_toggle_size_request
                                            (GtkMenuItem *menu_item,
                                             gint *requisition);
*/

static VALUE
mitem_toggle_size_allocate(self, allocation)
    VALUE self, allocation;
{
    gtk_menu_item_toggle_size_allocate(GTK_MENU_ITEM(RVAL2GOBJ(self)), NUM2INT(allocation));
    return self;
}

static VALUE
mitem_get_right_justified(self)
    VALUE self;
{
    return gtk_menu_item_get_right_justified(GTK_MENU_ITEM(RVAL2GOBJ(self))) ? Qtrue : Qfalse;
}

static VALUE
mitem_get_submenu(self)
    VALUE self;
{
    GtkWidget* submenu = gtk_menu_item_get_submenu(GTK_MENU_ITEM(RVAL2GOBJ(self)));
    return submenu ? GOBJ2RVAL(submenu) : Qnil;
}

void 
Init_gtk_menu_item()
{
    VALUE gMenuItem = G_DEF_CLASS(GTK_TYPE_MENU_ITEM, "MenuItem", mGtk);

    rb_define_method(gMenuItem, "initialize", mitem_initialize, -1);
    rb_define_method(gMenuItem, "set_right_justified", mitem_set_right_justified, 1);
    rb_define_method(gMenuItem, "set_submenu", mitem_set_submenu, 1);
    rb_define_method(gMenuItem, "select", mitem_select, 0);
    rb_define_method(gMenuItem, "deselect", mitem_deselect, 0);
    rb_define_method(gMenuItem, "activate", mitem_activate, 0);
    rb_define_method(gMenuItem, "toggle_size_allocate", mitem_toggle_size_allocate, 1);
    rb_define_method(gMenuItem, "right_justified?", mitem_get_right_justified, 0);
    rb_define_method(gMenuItem, "submenu", mitem_get_submenu, 0);

    G_DEF_SETTERS(gMenuItem);
}
