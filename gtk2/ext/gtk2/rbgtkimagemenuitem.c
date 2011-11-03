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

#include "global.h"

#define RG_TARGET_NAMESPACE cImageMenuItem

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE label, use_underline_or_accel_group;
    GtkWidget *widget = NULL;

    if (rb_scan_args(argc, argv, "02", &label, &use_underline_or_accel_group) > 0) {
        if (TYPE(label) == T_STRING){
            if (NIL_P(use_underline_or_accel_group) || RVAL2CBOOL(use_underline_or_accel_group)){
                widget = gtk_image_menu_item_new_with_mnemonic(RVAL2CSTR(label));
            } else {
                widget = gtk_image_menu_item_new_with_label(RVAL2CSTR(label));
            }
        } else if (TYPE(label) == T_SYMBOL){
            widget = gtk_image_menu_item_new_from_stock(rb_id2name(SYM2ID(label)),
                                                        NIL_P(use_underline_or_accel_group) ? NULL :
                                                        GTK_ACCEL_GROUP(RVAL2GOBJ(use_underline_or_accel_group)));
        } else {
            rb_raise(rb_eArgError, "invalid argument %s (expect Symbol(Gtk::Stock constants) or String)", 
                     rb_class2name(CLASS_OF(label)));
        }
    } else {
        widget = gtk_image_menu_item_new();
    }

    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_image_menu_item(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_IMAGE_MENU_ITEM, "ImageMenuItem", mGtk);

    RG_DEF_METHOD(initialize, -1);
}
