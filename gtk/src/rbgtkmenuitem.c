/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenuitem.c -

  $Author: mutoh $
  $Date: 2002/12/01 04:33:45 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_MENU_ITEM(RVAL2GOBJ(s)))

static VALUE
mitem_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE label, use_underline;
    GtkWidget *widget = NULL;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline) > 0) {
        if (NIL_P(use_underline) || RTEST(use_underline)){
            widget = gtk_menu_item_new_with_mnemonic(RVAL2CSTR(label));
        } else {
            widget = gtk_menu_item_new_with_label(RVAL2CSTR(label));
        }
    } else {
        widget = gtk_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
mitem_set_right_justified(self, right_justified)
    VALUE self, right_justified;
{
    gtk_menu_item_set_right_justified(_SELF(self), RTEST(right_justified));
    return self;
}

static VALUE
mitem_set_submenu(self, child)
    VALUE self, child;
{
    gtk_menu_item_set_submenu(_SELF(self), GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
mitem_set_accel_path(self, accel_path)
    VALUE self, accel_path;
{
    gtk_menu_item_set_accel_path(_SELF(self), RVAL2CSTR(accel_path));
    return self;
}

static VALUE
mitem_remove_submenu(self)
    VALUE self;
{
    gtk_menu_item_remove_submenu(_SELF(self));
    return self;
}

static VALUE
mitem_select(self)
    VALUE self;
{
    gtk_menu_item_select(_SELF(self));
    return self;
}

static VALUE
mitem_deselect(self)
    VALUE self;
{
    gtk_menu_item_deselect(_SELF(self));
    return self;
}

static VALUE
mitem_activate(self)
    VALUE self;
{
    gtk_menu_item_activate(_SELF(self));
    return self;
}

static VALUE
mitem_toggle_size_request(self)
    VALUE self;
{
    gint requisition;
    gtk_menu_item_toggle_size_request(_SELF(self), &requisition);
    return INT2NUM(requisition);
}

static VALUE
mitem_toggle_size_allocate(self, allocation)
    VALUE self, allocation;
{
    gtk_menu_item_toggle_size_allocate(_SELF(self), NUM2INT(allocation));
    return self;
}

static VALUE
mitem_get_right_justified(self)
    VALUE self;
{
    return gtk_menu_item_get_right_justified(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
mitem_get_submenu(self)
    VALUE self;
{
    GtkWidget* submenu = gtk_menu_item_get_submenu(_SELF(self));
    return submenu ? GOBJ2RVAL(submenu) : Qnil;
}

void 
Init_gtk_menu_item()
{
    VALUE gMenuItem = G_DEF_CLASS(GTK_TYPE_MENU_ITEM, "MenuItem", mGtk);

    rb_define_method(gMenuItem, "initialize", mitem_initialize, -1);
    rb_define_method(gMenuItem, "set_right_justified", mitem_set_right_justified, 1);
    rb_define_method(gMenuItem, "set_submenu", mitem_set_submenu, 1);
    rb_define_method(gMenuItem, "set_accel_path", mitem_set_accel_path, 1);
    rb_define_method(gMenuItem, "remove_submenu", mitem_remove_submenu, 0);
    rb_define_method(gMenuItem, "select", mitem_select, 0);
    rb_define_method(gMenuItem, "deselect", mitem_deselect, 0);
    rb_define_method(gMenuItem, "activate", mitem_activate, 0);
    rb_define_method(gMenuItem, "toggle_size_request", mitem_toggle_size_request, 0);
    rb_define_method(gMenuItem, "toggle_size_allocate", mitem_toggle_size_allocate, 1);
    rb_define_method(gMenuItem, "right_justified?", mitem_get_right_justified, 0);
    rb_define_method(gMenuItem, "submenu", mitem_get_submenu, 0);

    G_DEF_SETTERS(gMenuItem);
}
