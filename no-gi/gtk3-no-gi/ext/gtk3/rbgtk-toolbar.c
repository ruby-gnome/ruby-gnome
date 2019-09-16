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

#define RG_TARGET_NAMESPACE cToolbar
#define _SELF(self) (RVAL2GTKTOOLBAR(self))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_toolbar_new());

    return Qnil;
}

static VALUE
rg_get_drop_index(VALUE self, VALUE x, VALUE y)
{
    return INT2NUM(gtk_toolbar_get_drop_index(_SELF(self), NUM2INT(x), NUM2INT(y)));
}

static VALUE
rg_get_item_index(VALUE self, VALUE item)
{
    return INT2NUM(gtk_toolbar_get_item_index(_SELF(self), RVAL2GTKTOOLITEM(item)));
}

static VALUE
rg_n_items(VALUE self)
{
    return INT2NUM(gtk_toolbar_get_n_items(_SELF(self)));
}

static VALUE
rg_get_nth_item(VALUE self, VALUE n)
{
    return GOBJ2RVAL(gtk_toolbar_get_nth_item(_SELF(self), NUM2INT(n)));
}

static VALUE
rg_relief_style(VALUE self)
{
    return GTKRELIEFSTYLE2RVAL(gtk_toolbar_get_relief_style(_SELF(self)));
}

static VALUE
rg_insert(VALUE self, VALUE item, VALUE pos)
{
    gtk_toolbar_insert(_SELF(self), RVAL2GTKTOOLITEM(item), NUM2INT(pos));

    return self;
}

static VALUE
rg_set_drop_highlight_item(int argc, VALUE *argv, VALUE self)
{
    VALUE tool_item, index_;

    rb_scan_args(argc, argv, "11", &tool_item, &index_);
    gtk_toolbar_set_drop_highlight_item(_SELF(self),
                                        NIL_P(tool_item) ? NULL : RVAL2GTKTOOLITEM(tool_item),
                                        NIL_P(tool_item) ? 0 : NUM2INT(index_));

    return self;
}

static VALUE
rg_unset_icon_size(VALUE self)
{
    gtk_toolbar_unset_icon_size(_SELF(self));

    return self;
}

static VALUE
rg_unset_style(VALUE self)
{
    gtk_toolbar_unset_style(_SELF(self));

    return self;
}

void
Init_gtk_toolbar(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOLBAR, "Toolbar", mGtk);
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_SPACE_STYLE, "SpaceStyle", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GTK_TYPE_TOOLBAR_STYLE, "Style", RG_TARGET_NAMESPACE);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(get_drop_index, 2);
    RG_DEF_METHOD(get_item_index, 1);
    RG_DEF_METHOD(get_nth_item, 1);
    RG_DEF_METHOD(insert, 2);
    RG_DEF_METHOD(n_items, 0);
    RG_DEF_METHOD(relief_style, 0);
    RG_DEF_METHOD(set_drop_highlight_item, -1);
    RG_DEF_METHOD(unset_icon_size, 0);
    RG_DEF_METHOD(unset_style, 0);
}
