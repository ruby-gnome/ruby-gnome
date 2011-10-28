/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "global.h"

#define RG_TARGET_NAMESPACE cMenuItem
#define _SELF(s) (GTK_MENU_ITEM(RVAL2GOBJ(s)))

static VALUE
mitem_initialize(int argc, VALUE *argv, VALUE self)
{
    const gchar *label = NULL;
    VALUE rb_label, use_underline;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "02", &rb_label, &use_underline);

    if (!NIL_P(rb_label))
        label = RVAL2CSTR(rb_label);
    if (label) {
        if (NIL_P(use_underline) || RVAL2CBOOL(use_underline)) {
            widget = gtk_menu_item_new_with_mnemonic(label);
        } else {
            widget = gtk_menu_item_new_with_label(label);
        }
    } else {
        widget = gtk_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

/* Defined as Properties
void                gtk_menu_item_set_right_justified   (GtkMenuItem *menu_item,
                                                         gboolean right_justified);
gboolean            gtk_menu_item_get_right_justified   (GtkMenuItem *menu_item);
GtkWidget *         gtk_menu_item_get_submenu           (GtkMenuItem *menu_item);
*/

static VALUE
mitem_set_submenu(VALUE self, VALUE child)
{
    GtkMenuItem *item;
    GtkWidget *submenu;

    item = _SELF(self);
    submenu = gtk_menu_item_get_submenu(item);
    if (submenu)
        G_CHILD_REMOVE(self, GOBJ2RVAL(submenu));

    gtk_menu_item_set_submenu(item, GTK_WIDGET(RVAL2GOBJ(child)));
    G_CHILD_ADD(self, child);
    return self;
}

/* Defined as Properties
void                gtk_menu_item_set_accel_path        (GtkMenuItem *menu_item,
                                                         const gchar *accel_path);
*/

static VALUE
mitem_remove_submenu(VALUE self)
{
    GtkMenuItem *item;
    GtkWidget *submenu;

    item = _SELF(self);
    submenu = gtk_menu_item_get_submenu(item);
    gtk_menu_item_remove_submenu(item);
    if (submenu)
        G_CHILD_REMOVE(self, GOBJ2RVAL(submenu));

    return self;
}

/* Defined as Signals
void                gtk_menu_item_select                (GtkMenuItem *menu_item);
void                gtk_menu_item_deselect              (GtkMenuItem *menu_item);
void                gtk_menu_item_activate              (GtkMenuItem *menu_item);
*/

static VALUE
mitem_toggle_size_request(VALUE self)
{
    gint requisition;
    gtk_menu_item_toggle_size_request(_SELF(self), &requisition);
    return INT2NUM(requisition);
}

static VALUE
mitem_toggle_size_allocate(VALUE self, VALUE allocation)
{
    gtk_menu_item_toggle_size_allocate(_SELF(self), NUM2INT(allocation));
    return self;
}

void 
Init_gtk_menu_item(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MENU_ITEM, "MenuItem", mGtk);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", mitem_initialize, -1);
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, "submenu", mitem_set_submenu, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "remove_submenu", mitem_remove_submenu, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "toggle_size_request", mitem_toggle_size_request, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "toggle_size_allocate", mitem_toggle_size_allocate, 1);
}
