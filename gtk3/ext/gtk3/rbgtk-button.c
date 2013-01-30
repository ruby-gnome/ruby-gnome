/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cButton
#define _SELF(self) (RVAL2GTKBUTTON(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE button;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "01", &button);
    if (NIL_P(button)) {
        widget = gtk_button_new();
    } else if (TYPE(button) == T_HASH) {
        VALUE label, mnemonic, stock_id, buffer;
        rbg_scan_options(button,
                         "label", &label,
                         "mnemonic", &mnemonic,
                         "stock_id", &stock_id,
                         NULL);

        if (!NIL_P(label))
            widget = gtk_button_new_with_label(RVAL2CSTR(label));
        else if (!NIL_P(mnemonic))
            widget = gtk_button_new_with_mnemonic(RVAL2CSTR(mnemonic));
        else if (!NIL_P(stock_id))
            widget = gtk_button_new_from_stock(RVAL2GLIBID(stock_id, buffer));
    }
    if (!widget)
        rb_raise(rb_eArgError, "Invalid arguments.");

    RBGTK_INITIALIZE(self, widget); 

    return Qnil;
}

static VALUE
rg_set_alignment(VALUE self, VALUE xalign, VALUE yalign)
{
    gtk_button_set_alignment(_SELF(self), NUM2DBL(xalign), NUM2DBL(yalign));
    return self;
}

static VALUE
rg_alignment(VALUE self)
{
    gfloat xalign, yalign;
    gtk_button_get_alignment(_SELF(self), &xalign, &yalign);

    return rb_assoc_new(rb_float_new(xalign), rb_float_new(yalign));
}

static VALUE
rg_event_window(VALUE self)
{
    return GOBJ2RVAL(gtk_button_get_event_window(_SELF(self)));
}

void
Init_gtk_button(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_BUTTON, "Button", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_alignment, 2);
    RG_DEF_METHOD(alignment, 0);
    RG_DEF_METHOD(event_window, 0);
}
