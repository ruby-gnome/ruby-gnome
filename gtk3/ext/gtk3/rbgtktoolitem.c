/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004,2005 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cToolItem
#define _SELF(self) (RVAL2GTKTOOLITEM(self))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_tool_item_new());
    return Qnil;
}

static VALUE
rg_set_homogeneous(VALUE self, VALUE homogeneous)
{
    gtk_tool_item_set_homogeneous(_SELF(self), RVAL2CBOOL(homogeneous));
    return self;
}
static VALUE
rg_homogeneous_p(VALUE self)
{
    return CBOOL2RVAL(gtk_tool_item_get_homogeneous(_SELF(self)));
}

static VALUE
rg_set_expand(VALUE self, VALUE expand)
{
    gtk_tool_item_set_expand(_SELF(self), RVAL2CBOOL(expand));
    return self;
}
static VALUE
rg_expand_p(VALUE self)
{
    return CBOOL2RVAL(gtk_tool_item_get_expand(_SELF(self)));
}

/* deprecated
static VALUE
rg_set_tooltip(int argc, VALUE *argv, VALUE self)
{
    VALUE tooltip, tip_text, tip_private;

    rb_scan_args(argc, argv, "21", &tooltip, &tip_text, &tip_private);

    gtk_tool_item_set_tooltip(_SELF(self), RVAL2GTKTOOLTIPS(tooltip),
                              RVAL2CSTR(tip_text), 
                              RVAL2CSTR_ACCEPT_NIL(tip_private));
    return self;
}
*/

static VALUE
rg_set_use_drag_window(VALUE self, VALUE use_drag_window)
{
    gtk_tool_item_set_use_drag_window(_SELF(self), RVAL2CBOOL(use_drag_window));
    return self;
}
static VALUE
rg_use_drag_window_p(VALUE self)
{
    return CBOOL2RVAL(gtk_tool_item_get_use_drag_window(_SELF(self)));
}

static VALUE
rg_icon_size(VALUE self)
{
    return GTKICONSIZE2RVAL(gtk_tool_item_get_icon_size(_SELF(self)));
}

static VALUE
rg_orientation(VALUE self)
{
    return GTKORIENTATION2RVAL(gtk_tool_item_get_orientation(_SELF(self)));
}

static VALUE
rg_toolbar_style(VALUE self)
{
    return GTKTOOLBARSTYLE2RVAL(gtk_tool_item_get_toolbar_style(_SELF(self)));
}

static VALUE
rg_relief_style(VALUE self)
{
    return GTKRELIEFSTYLE2RVAL(gtk_tool_item_get_relief_style(_SELF(self)));
}

static VALUE
rg_retrieve_proxy_menu_item(VALUE self)
{
    return GOBJ2RVAL(gtk_tool_item_retrieve_proxy_menu_item(_SELF(self)));
}

static VALUE
rg_get_proxy_menu_item(VALUE self, VALUE menu_item_id)
{
    return GOBJ2RVAL(gtk_tool_item_get_proxy_menu_item(_SELF(self), RVAL2CSTR(menu_item_id)));
}

static VALUE
rg_set_proxy_menu_item(VALUE self, VALUE menu_item_id, VALUE menu_item)
{
    gtk_tool_item_set_proxy_menu_item(_SELF(self), RVAL2CSTR(menu_item_id),
                                      RVAL2GTKWIDGET(menu_item));
    return self;
}

static VALUE
rg_rebuild_menu(VALUE self)
{
    gtk_tool_item_rebuild_menu(_SELF(self));
    return self;
}

void 
Init_gtk_toolitem(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_TOOL_ITEM, "ToolItem", mGtk);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_homogeneous, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "homogenous");
    RG_DEF_METHOD_P(homogeneous, 0);
    RG_DEF_METHOD(set_expand, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "expand");
    RG_DEF_METHOD_P(expand, 0);
/* deprecated
    RG_DEF_METHOD(set_tooltip, -1);
*/
    RG_DEF_METHOD(set_use_drag_window, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "use_drag_window");
    RG_DEF_METHOD_P(use_drag_window, 0);
    RG_DEF_METHOD(icon_size, 0);
    RG_DEF_METHOD(orientation, 0);
    RG_DEF_METHOD(toolbar_style, 0);
    RG_DEF_METHOD(relief_style, 0);
    RG_DEF_METHOD(retrieve_proxy_menu_item, 0);
    RG_DEF_METHOD(get_proxy_menu_item, 1);
    RG_DEF_METHOD(set_proxy_menu_item, 2);
    RG_DEF_METHOD(rebuild_menu, 0);
}
