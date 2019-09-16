/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cToolItemGroup
#define _SELF(self) (RVAL2GTKTOOLITEMGROUP(self))

static VALUE
rg_initialize(VALUE self, VALUE label)
{
    RBGTK_INITIALIZE(self, gtk_tool_item_group_new(RVAL2CSTR(label)));

    return Qnil;
}

static VALUE
rg_get_drop_item(VALUE self, VALUE x, VALUE y)
{
    return GOBJ2RVAL(gtk_tool_item_group_get_drop_item(_SELF(self), NUM2INT(x), NUM2INT(y)));
}

static VALUE
rg_n_items(VALUE self)
{
    return UINT2NUM(gtk_tool_item_group_get_n_items(_SELF(self)));
}

static VALUE
rg_get_nth_item(VALUE self, VALUE index)
{
    return GOBJ2RVAL(gtk_tool_item_group_get_nth_item(_SELF(self), NUM2UINT(index)));
}

static VALUE
rg_insert(VALUE self, VALUE item, VALUE position)
{
    gtk_tool_item_group_insert(_SELF(self), RVAL2GTKTOOLITEM(item), NUM2INT(position));

    return self;
}

void
Init_gtk_toolitemgroup(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOL_ITEM_GROUP, "ToolItemGroup", mGtk);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(get_drop_item, 2);
    RG_DEF_METHOD(get_nth_item, 1);
    RG_DEF_METHOD(insert, 2);
    RG_DEF_METHOD(n_items, 0);
}
