/* $Id: rbgnome-dock-layout.c,v 1.1 2002/05/19 12:29:57 mutoh Exp $ */

/* Gnome::DockLayout class for Ruby/Gnome
 * Copyright (C) 1999 Minoru Inachi <inachi@earth.interq.or.jp>
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

/*
 * Class Gnome::DockLayout
 *
 * Hierarchy:
 *
 *   Object
 *     +----Gtk::Object
 *            +----Gnome::DockLayout
 */
VALUE gnoDockLayout;

VALUE gnoDockLayoutItem;

static VALUE
docklayout_initialize(self)
    VALUE self;
{
    set_gobject(self, GTK_OBJECT(gnome_dock_layout_new()));
    return Qnil;
}

/*
 * Gnome::DockLayout#add_item(item, placement,
 *			      band_num, band_position, offset)
 *
 * Add item to layout with the specified parameters.
 *
 * arguments:
 *   item - The dock item to be added to @layout
 *   placement - Placement of @item in @layout
 *   band_num - Band number
 *   band_position - Position within the band
 *   offset - Distance from the previous element in the band
 */
static VALUE
docklayout_add_item(self, item, placement,
		    band_num, band_position, offset)
    VALUE self, item, placement;
    VALUE band_num, band_position, offset;
{
    gboolean result;
    result = gnome_dock_layout_add_item(GNOME_DOCK_LAYOUT(get_widget(self)),
					GNOME_DOCK_ITEM(get_widget(item)),
					NUM2INT(placement),
					NUM2INT(band_num),
					NUM2INT(band_position),
					NUM2INT(offset));

    if (!result) {
	rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

static VALUE
docklayout_add_floating_item(self, item, x, y, orientation)
    VALUE self, item, x, y, orientation;
{
    gboolean result;
    result = gnome_dock_layout_add_floating_item(
		GNOME_DOCK_LAYOUT(get_widget(self)),
		GNOME_DOCK_ITEM(get_widget(item)),
		NUM2INT(x),
		NUM2INT(y),
		NUM2INT(orientation));

    if (!result) {
	rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

static VALUE
docklayout_get_item(self, item)
    VALUE self, item;
{
    GnomeDockLayoutItem *result;

    if (TYPE(item) == T_STRING) {
	result = gnome_dock_layout_get_item_by_name(
		    GNOME_DOCK_LAYOUT(get_widget(self)),
		    STR2CSTR(item));
    } else {
	result = gnome_dock_layout_get_item(
		    GNOME_DOCK_LAYOUT(get_widget(self)),
		    GNOME_DOCK_ITEM(get_widget(item)));
    }
    return Data_Wrap_Struct(gnoDockLayoutItem, 0, 0, result);
}

static VALUE
docklayout_remove_item(self, item)
    VALUE self, item;
{
    gboolean result;

    if (TYPE(item) == T_STRING) {
	result = gnome_dock_layout_remove_item_by_name(
		    GNOME_DOCK_LAYOUT(get_widget(self)),
		    STR2CSTR(item));
    } else {
	result = gnome_dock_layout_remove_item(
		    GNOME_DOCK_LAYOUT(get_widget(self)),
		    GNOME_DOCK_ITEM(get_widget(item)));
    }

    if (!result) {
	rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

static VALUE
docklayout_create_string(self)
    VALUE self;
{
    gchar *result;
    result = gnome_dock_layout_create_string(
		GNOME_DOCK_LAYOUT(get_widget(self)));
    return rb_str_new2(result);
}

static VALUE
docklayout_parse_string(self, string)
    VALUE self, string;
{
    gboolean result;
    result = gnome_dock_layout_parse_string(
		GNOME_DOCK_LAYOUT(get_widget(self)),
		STR2CSTR(string));

    if (!result) {
	rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

static VALUE
docklayout_add_to_dock(self, dock)
    VALUE self, dock;
{
    gboolean result;
    result = gnome_dock_layout_add_to_dock(
		GNOME_DOCK_LAYOUT(get_widget(self)),
		GNOME_DOCK(get_widget(dock)));

    if (!result) {
	rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

void
Init_gnome_dock_layout()
{
    gnoDockLayout = rb_define_class_under(mGnome, "DockLayout", gObject);
    gnoDockLayoutItem = rb_define_class_under(mGnome, "DockLayoutItem",
					      rb_cData);

    /*
     * instance methods
     */
    rb_define_method(gnoDockLayout, "initialize", docklayout_initialize, 0);
    rb_define_method(gnoDockLayout, "add_item",
		     docklayout_add_item, 5);
    rb_define_method(gnoDockLayout, "add_floating_item",
		     docklayout_add_floating_item, 4);
    rb_define_method(gnoDockLayout, "get_item",
		     docklayout_get_item, 1);
    rb_define_method(gnoDockLayout, "remove_item",
		     docklayout_remove_item, 1);
    rb_define_method(gnoDockLayout, "create_string",
		     docklayout_create_string, 0);
    rb_define_method(gnoDockLayout, "parse_string",
		     docklayout_parse_string, 1);
    rb_define_method(gnoDockLayout, "add_to_dock",
		     docklayout_add_to_dock, 1);
}
