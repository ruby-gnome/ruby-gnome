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

#define RG_TARGET_NAMESPACE cItem

/*
 * Item
 */
static VALUE
rg_select(VALUE self)
{
    gtk_item_select(GTK_ITEM(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_deselect(VALUE self)
{
    gtk_item_deselect(GTK_ITEM(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_toggle(VALUE self)
{
    gtk_item_toggle(GTK_ITEM(RVAL2GOBJ(self)));
    return self;
}

void 
Init_gtk_item(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ITEM, "Item", mGtk);
    RG_DEF_METHOD(select, 0);
    RG_DEF_METHOD(deselect, 0);
    RG_DEF_METHOD(toggle, 0);
}
