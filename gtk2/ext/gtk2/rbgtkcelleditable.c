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

#define RG_TARGET_NAMESPACE mCellEditable
#define _SELF(s) (GTK_CELL_EDITABLE(RVAL2GOBJ(s)))

static VALUE
rg_start_editing(VALUE self, VALUE event)
{
    gtk_cell_editable_start_editing(_SELF(self), 
                                    NIL_P(event) ? NULL : RVAL2GEV(event));
    return self;
}

static VALUE
rg_editing_done(VALUE self)
{
    gtk_cell_editable_editing_done(_SELF(self));
    return self;
}

static VALUE
rg_remove_widget(VALUE self)
{
    gtk_cell_editable_remove_widget(_SELF(self));
    return self;
}

void
Init_gtk_celleditable(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_CELL_EDITABLE, "CellEditable", mGtk);

    RG_DEF_METHOD(start_editing, 1);
    RG_DEF_METHOD(editing_done, 0);
    RG_DEF_METHOD(remove_widget, 0);
}
