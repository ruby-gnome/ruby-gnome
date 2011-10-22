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

static VALUE
tbtn_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE label, use_underline;
    GtkWidget *widget;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline) > 0) {
        if (TYPE(label) == T_STRING){
            if (NIL_P(use_underline) || RVAL2CBOOL(use_underline)){
                widget = gtk_toggle_button_new_with_mnemonic(RVAL2CSTR(label));
            } else {
                widget = gtk_toggle_button_new_with_label(RVAL2CSTR(label));
            }
        } else if (TYPE(label) == T_SYMBOL){
            widget = gtk_toggle_button_new_with_label(rb_id2name(SYM2ID(label)));
            gtk_button_set_use_stock(GTK_BUTTON(widget), TRUE);
        } else {
            rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                     rb_class2name(CLASS_OF(label)));
        }
    } else {
	widget = gtk_toggle_button_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

static VALUE
tbtn_set_mode(VALUE self, VALUE mode)
{
    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)),
			       RVAL2CBOOL(mode));
    return self;
}

static VALUE
tbtn_get_mode(VALUE self)
{
    return CBOOL2RVAL(gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self))));
}

static VALUE
tbtn_toggled(VALUE self)
{
    gtk_toggle_button_toggled(GTK_TOGGLE_BUTTON(RVAL2GOBJ(self)));
    return self;
}


void 
Init_gtk_toggle_button(void)
{
    VALUE gTButton = G_DEF_CLASS(GTK_TYPE_TOGGLE_BUTTON, "ToggleButton", mGtk);

    rb_define_method(gTButton, "initialize", tbtn_initialize, -1);
    rb_define_method(gTButton, "set_mode", tbtn_set_mode, 1);
    G_DEF_SETTER(gTButton, "mode");
    rb_define_method(gTButton, "mode?", tbtn_get_mode, 0);
    rb_define_method(gTButton, "toggled", tbtn_toggled, 0);
}
