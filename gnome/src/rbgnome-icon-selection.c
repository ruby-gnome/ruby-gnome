/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-icon-selection.c,v 1.6 2007/07/13 16:07:30 ggc Exp $ */
/* based on libgnomeui/gnome-icon-sel.h */

/* Gnome::IconSelection widget for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgnome.h"

#define _SELF(self) GNOME_ICON_SELECTION(RVAL2GOBJ(self))

static VALUE
icon_sel_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gnome_icon_selection_new());
    return Qnil;
}

static VALUE
icon_sel_add_defaults(self)
    VALUE self;
{
    gnome_icon_selection_add_defaults(_SELF(self));
    return self;
}

static VALUE
icon_sel_add_directory(self, dir)
    VALUE self, dir;
{
    gnome_icon_selection_add_directory(_SELF(self),
                                       RVAL2CSTR(dir));
    return self;
}

static VALUE
icon_sel_show_icons(self)
    VALUE self;
{
    gnome_icon_selection_show_icons(_SELF(self));
    return self;
}

static VALUE
icon_sel_clear(self, not_shown)
    VALUE self, not_shown;
{
    gnome_icon_selection_clear(_SELF(self),
                               RVAL2CBOOL(not_shown));
    return self;
}

static VALUE
icon_sel_get_icon(self, full_path)
    VALUE self, full_path;
{
    gchar *filename;
    VALUE obj;

    filename = gnome_icon_selection_get_icon(_SELF(self), RVAL2CBOOL(full_path));
    SET_STR_AND_GFREE(obj, filename);
    return obj;
}

static VALUE
icon_sel_select_icon(self, filename)
    VALUE self, filename;
{
    gnome_icon_selection_select_icon(_SELF(self), RVAL2CSTR(filename));
    return self;
}

static VALUE
icon_sel_stop_loading(self)
    VALUE self;
{
    gnome_icon_selection_stop_loading(_SELF(self));
    return self;
}

static VALUE
icon_sel_get_gil(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_icon_selection_get_gil(_SELF(self)));
}

static VALUE
icon_sel_get_box(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_icon_selection_get_box(_SELF(self)));
}

void
Init_gnome_icon_selection(mGnome)
    VALUE mGnome;
{
    VALUE gnoIconSelection = G_DEF_CLASS(GNOME_TYPE_ICON_SELECTION, "IconSelection", mGnome);
    rb_define_method(gnoIconSelection, "initialize", icon_sel_initialize, 0);
    rb_define_method(gnoIconSelection, "add_defaults", icon_sel_add_defaults, 0);
    rb_define_method(gnoIconSelection, "add_directory", icon_sel_add_directory, 1);
    rb_define_method(gnoIconSelection, "show_icons", icon_sel_show_icons, 0);
    rb_define_method(gnoIconSelection, "clear", icon_sel_clear, 1);
    rb_define_method(gnoIconSelection, "get_icon", icon_sel_get_icon, 1);
    rb_define_method(gnoIconSelection, "select_icon", icon_sel_select_icon, 1);
    rb_define_method(gnoIconSelection, "stop_loading", icon_sel_stop_loading, 0);
    rb_define_method(gnoIconSelection, "gil", icon_sel_get_gil, 0);
    rb_define_method(gnoIconSelection, "box", icon_sel_get_box, 0);
}
