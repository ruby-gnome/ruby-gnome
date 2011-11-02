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

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

#define RG_TARGET_NAMESPACE cMenuToolButton
#define _SELF(self) (GTK_MENU_TOOL_BUTTON(RVAL2GOBJ(self)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GtkToolItem* item;

    if (argc == 0){
        item = gtk_menu_tool_button_new((GtkWidget*)NULL, (const gchar*)NULL);
    } else if (TYPE(argv[0]) == T_SYMBOL || TYPE(argv[0]) == T_STRING){
        VALUE stock_id;
        rb_scan_args(argc, argv, "10", &stock_id);

        if (TYPE(stock_id) == T_SYMBOL){
            item = gtk_menu_tool_button_new_from_stock(rb_id2name(SYM2ID(stock_id)));
        } else {
            item = gtk_menu_tool_button_new_from_stock(RVAL2CSTR(stock_id));
        }
    } else {
        VALUE icon_widget, label;
        rb_scan_args(argc, argv, "11", &icon_widget, &label);

        item = gtk_menu_tool_button_new(GTK_WIDGET(RVAL2GOBJ(icon_widget)),
                                        NIL_P(label) ? (const gchar*)NULL : RVAL2CSTR(label));
    }

    RBGTK_INITIALIZE(self, item);
    return Qnil;
}

static VALUE
rg_set_arrow_tooltip(int argc, VALUE *argv, VALUE self)
{
    VALUE tooltips, tip_text, tip_private;

    rb_scan_args(argc, argv, "12", &tooltips, &tip_text, &tip_private);
    gtk_menu_tool_button_set_arrow_tooltip(_SELF(self), GTK_TOOLTIPS(RVAL2GOBJ(self)),
                                           NIL_P(tip_text) ? NULL : RVAL2CSTR(tip_text),
                                           NIL_P(tip_private) ? NULL : RVAL2CSTR(tip_private));
    return self;
}
#endif

void 
Init_gtk_menutoolbutton(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_MENU_TOOL_BUTTON, "MenuToolButton", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_arrow_tooltip, -1);
#endif
}
