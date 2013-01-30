/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cMenuToolButton
#define _SELF(self) (RVAL2GTKMENUTOOLBUTTON(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg;
    GtkToolItem *item = NULL;

    rb_scan_args(argc, argv, "01", &arg);
    if (NIL_P(arg)) {
        item = gtk_menu_tool_button_new(NULL, NULL);
    } else if (TYPE(arg) == T_HASH) {
        VALUE icon_widget, label, stock_id, buffer;
        rbg_scan_options(arg,
                         "icon_widget", &icon_widget,
                         "label", &label,
                         "stock_id", &stock_id,
                         NULL);

        if (!NIL_P(icon_widget))
            item = gtk_menu_tool_button_new(RVAL2GTKWIDGET(icon_widget),
                                              RVAL2CSTR_ACCEPT_NIL(label));
        else if (!NIL_P(stock_id))
            item = gtk_menu_tool_button_new_from_stock(RVAL2GLIBID(stock_id, buffer));
    }
    if (!item)
        rb_raise(rb_eArgError, "Invalid arguments.");

    RBGTK_INITIALIZE(self, item);

    return Qnil;
}

/* deprecated
static VALUE
rg_set_arrow_tooltip(int argc, VALUE *argv, VALUE self)
{
    VALUE tooltips, tip_text, tip_private;

    rb_scan_args(argc, argv, "12", &tooltips, &tip_text, &tip_private);
    gtk_menu_tool_button_set_arrow_tooltip(_SELF(self), RVAL2GTKTOOLTIPS(self),
                                           RVAL2CSTR_ACCEPT_NIL(tip_text),
                                           RVAL2CSTR_ACCEPT_NIL(tip_private));
    return self;
}
*/

void 
Init_gtk_menutoolbutton(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MENU_TOOL_BUTTON, "MenuToolButton", mGtk);

    RG_DEF_METHOD(initialize, -1);
/* deprecated
    RG_DEF_METHOD(set_arrow_tooltip, -1);
*/
}
