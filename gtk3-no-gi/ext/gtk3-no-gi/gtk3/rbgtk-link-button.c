/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cLinkButton
#define _SELF(self) (RVAL2GTKLINKBUTTON(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE uri, label;
    GtkWidget *widget = NULL;

    rb_scan_args(argc, argv, "11", &uri, &label);
    if (NIL_P(label)){
        widget = gtk_link_button_new(RVAL2CSTR(uri));
    } else {
        widget = gtk_link_button_new_with_label(RVAL2CSTR(uri), RVAL2CSTR(label));
    }
    RBGTK_INITIALIZE(self, widget); 
    return Qnil;
}

void
Init_gtk_link_button(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_LINK_BUTTON, "LinkButton", mGtk);

    RG_DEF_METHOD(initialize, -1);
}
