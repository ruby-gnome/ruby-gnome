/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cImageMenuItem

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE image;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "01", &image);
    if (NIL_P(image)) {
        widget = gtk_image_menu_item_new();
    } else if (TYPE(image) == T_HASH) {
        VALUE label, mnemonic, stock_id, accel_group, buffer;
        rbg_scan_options(image,
                         "label", &label,
                         "mnemonic", &mnemonic,
                         "stock_id", &stock_id,
                         "accel_group", &accel_group,
                         NULL);

        if (!NIL_P(label))
            widget = gtk_image_menu_item_new_with_label(RVAL2CSTR(label));
        else if (!NIL_P(mnemonic))
            widget = gtk_image_menu_item_new_with_mnemonic(RVAL2CSTR(mnemonic));
        else if (!NIL_P(stock_id))
            widget = gtk_image_menu_item_new_from_stock(RVAL2GLIBID(stock_id, buffer),
                                                        NIL_P(accel_group) ? NULL :
                                                        RVAL2GTKACCELGROUP(accel_group));
    }
    if (!widget)
        rb_raise(rb_eArgError, "Invalid arguments.");

    RBGTK_INITIALIZE(self, widget);

    return Qnil;
}

void 
Init_gtk_image_menu_item(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_IMAGE_MENU_ITEM, "ImageMenuItem", mGtk);

    RG_DEF_METHOD(initialize, -1);
}
