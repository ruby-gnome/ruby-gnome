/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cToolButton
#define _SELF(self) (RVAL2GTKTOOLBUTTON(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg;
    GtkToolItem *item = NULL;

    rb_scan_args(argc, argv, "01", &arg);
    if (NIL_P(arg)) {
        item = gtk_tool_button_new(NULL, NULL);
    } else if (TYPE(arg) == T_HASH) {
        VALUE icon_widget, label, stock_id, buffer;
        rbg_scan_options(arg,
                         "icon_widget", &icon_widget,
                         "label", &label,
                         "stock_id", &stock_id,
                         NULL);

        if (!NIL_P(icon_widget))
            item = gtk_tool_button_new(RVAL2GTKWIDGET(icon_widget),
                                       RVAL2CSTR_ACCEPT_NIL(label));
        else if (!NIL_P(stock_id))
            item = gtk_tool_button_new_from_stock(RVAL2GLIBID(stock_id, buffer));
    }
    if (!item)
        rb_raise(rb_eArgError, "Invalid arguments.");

    RBGTK_INITIALIZE(self, item);

    return Qnil;
}

void
Init_gtk_toolbutton(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOL_BUTTON, "ToolButton", mGtk);

    RG_DEF_METHOD(initialize, -1);

    RG_REG_GLIBID_SETTER("stock-id");
}
