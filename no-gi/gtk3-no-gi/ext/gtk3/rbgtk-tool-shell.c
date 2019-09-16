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

#define RG_TARGET_NAMESPACE mToolShell
#define _SELF(self) (RVAL2GTKTOOLSHELL(self))

static VALUE
rg_ellipsize_mode(VALUE self)
{
    return PANGOELLIPSIZEMODE2RVAL(gtk_tool_shell_get_ellipsize_mode(_SELF(self)));
}

static VALUE
rg_icon_size(VALUE self)
{
    return GTKICONSIZE2RVAL(gtk_tool_shell_get_icon_size(_SELF(self)));
}

static VALUE
rg_orientation(VALUE self)
{
    return GTKORIENTATION2RVAL(gtk_tool_shell_get_orientation(_SELF(self)));
}

static VALUE
rg_relief_style(VALUE self)
{
    return GTKRELIEFSTYLE2RVAL(gtk_tool_shell_get_relief_style(_SELF(self)));
}

static VALUE
rg_style(VALUE self)
{
    return GTKTOOLBARSTYLE2RVAL(gtk_tool_shell_get_style(_SELF(self)));
}

static VALUE
rg_text_alignment(VALUE self)
{
    return DBL2NUM(gtk_tool_shell_get_text_alignment(_SELF(self)));
}

static VALUE
rg_text_orientation(VALUE self)
{
    return GTKORIENTATION2RVAL(gtk_tool_shell_get_text_orientation(_SELF(self)));
}

static VALUE
rg_text_size_group(VALUE self)
{
    return GOBJ2RVAL(gtk_tool_shell_get_text_size_group(_SELF(self)));
}

static VALUE
rg_rebuild_menu(VALUE self)
{
    gtk_tool_shell_rebuild_menu(_SELF(self));

    return self;
}

void
Init_gtk_toolshell(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_TOOL_SHELL, "ToolShell", mGtk);

    RG_DEF_METHOD(ellipsize_mode, 0);
    RG_DEF_METHOD(icon_size, 0);
    RG_DEF_METHOD(orientation, 0);
    RG_DEF_METHOD(relief_style, 0);
    RG_DEF_METHOD(style, 0);
    RG_DEF_METHOD(text_alignment, 0);
    RG_DEF_METHOD(text_orientation, 0);
    RG_DEF_METHOD(text_size_group, 0);
    RG_DEF_METHOD(rebuild_menu, 0);
}
