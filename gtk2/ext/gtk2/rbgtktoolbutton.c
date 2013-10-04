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

#include "global.h"

#define RG_TARGET_NAMESPACE cToolButton
#define _SELF(self) (GTK_TOOL_BUTTON(RVAL2GOBJ(self)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE obj, label;
    GtkToolItem* item;

    rb_scan_args(argc, argv, "02", &obj, &label);

    if (TYPE(obj) == T_SYMBOL){
        item = gtk_tool_button_new_from_stock(rb_id2name(SYM2ID(obj)));
    } else if (TYPE(obj) == T_STRING){
        item = gtk_tool_button_new_from_stock(RVAL2CSTR(obj));
    } else {
        item = gtk_tool_button_new(GTK_WIDGET(RVAL2GOBJ(obj)), 
                                   NIL_P(label) ? NULL : RVAL2CSTR(label));
    }
    RBGTK_INITIALIZE(self, item);
    return Qnil;
}

void 
Init_gtk_toolbutton(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOL_BUTTON, "ToolButton", mGtk);

    RG_DEF_METHOD(initialize, -1);
}
