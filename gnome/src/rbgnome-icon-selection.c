/* $Id: rbgnome-icon-selection.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::IconSelection widget for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

VALUE gnoIconSelection;

static VALUE
icon_sel_init(self)
    VALUE self;
{
    GtkWidget* icon_selection = gnome_icon_selection_new();
    set_widget(self, icon_selection);
    return Qnil;
}

static VALUE
icon_sel_add_defaults(self)
    VALUE self;
{
    gnome_icon_selection_add_defaults(GNOME_ICON_SELECTION(get_widget(self)));
    return Qnil;
}

static VALUE
icon_sel_add_directory(self, dir)
    VALUE self, dir;
{
    gnome_icon_selection_add_directory(GNOME_ICON_SELECTION(get_widget(self)),
                                       STR2CSTR(dir));
    return Qnil;
}

static VALUE
icon_sel_show_icons(self)
    VALUE self;
{
    gnome_icon_selection_show_icons(GNOME_ICON_SELECTION(get_widget(self)));
    return Qnil;
}

static VALUE
icon_sel_clear(self, not_shown)
    VALUE self, not_shown;
{
    gnome_icon_selection_clear(GNOME_ICON_SELECTION(get_widget(self)),
                               RTEST(not_shown));
    return Qnil;
}

static VALUE
icon_sel_get_icon(self, full_path)
    VALUE self, full_path;
{
    return rb_str_new2(
        gnome_icon_selection_get_icon(GNOME_ICON_SELECTION(get_widget(self)),
                                      RTEST(full_path)));
}

static VALUE
icon_sel_select_icon(self, filename)
    VALUE self, filename;
{
    gnome_icon_selection_select_icon(GNOME_ICON_SELECTION(get_widget(self)),
                                     STR2CSTR(filename));
    return Qnil;
}

void
Init_gnome_icon_selection()
{
    gnoIconSelection = rb_define_class_under(mGnome, "IconSelection", gVBox);
    rb_define_method(gnoIconSelection, "initialize", icon_sel_init, 0);
    rb_define_method(gnoIconSelection, "add_defaults",
            icon_sel_add_defaults, 0);
    rb_define_method(gnoIconSelection, "add_directory",
            icon_sel_add_directory, 1);
    rb_define_method(gnoIconSelection, "show_icons",
            icon_sel_show_icons, 0);
    rb_define_method(gnoIconSelection, "clear",
            icon_sel_clear, 1);
    rb_define_method(gnoIconSelection, "get_icon",
            icon_sel_get_icon, 1);
    rb_define_method(gnoIconSelection, "select_icon",
            icon_sel_select_icon, 1);
}
