/* $Id: rbgnome-dock.c,v 1.1 2002/05/19 12:29:54 mutoh Exp $ */

/* Gnome::Dock widget for Ruby/Gnome
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

VALUE gnoDock;

static VALUE
dock_initialize(self)
    VALUE self;
{
    set_widget(self, gnome_dock_new());
    return Qnil;
}

/*
 * Gnome::Dock#allow_floating_items(enable)
 */
static VALUE
dock_allow_floating_items(self, enable)
    VALUE self, enable;
{
    gnome_dock_allow_floating_items(GNOME_DOCK(get_widget(self)),
				    RTEST(enable));
    return self;
}

/*
 * Gnome::Dock#add_item(item, placement, band_num, position,
 *			offset, in_new_band)
 */ 
static VALUE
dock_add_item(self, item, placement, band_num, position, offset, in_new_band)
    VALUE self, item, placement, band_num, position, offset, in_new_band;
{
    gnome_dock_add_item(GNOME_DOCK(get_widget(self)),
			GNOME_DOCK_ITEM(get_widget(item)),
			NUM2INT(placement),
			NUM2INT(band_num),
			NUM2INT(position),
			NUM2INT(offset),
			RTEST(in_new_band));
    return self;
}

/*
 * Gnome::Dock#add_floating_item(widget, x, y, orientation)
 */
static VALUE
dock_add_floating_item(self, widget, x, y, orientation)
    VALUE self, widget, x, y, orientation;
{
    gnome_dock_add_floating_item(GNOME_DOCK(get_widget(self)),
				 GNOME_DOCK_ITEM(get_widget(widget)),
				 NUM2INT(x), NUM2INT(y),
				 NUM2INT(orientation));
    return self;
}

/*
 * Gnome::Dock#set_client_area(widget)
 */
static VALUE
dock_set_client_area(self, widget)
    VALUE self, widget;
{
    gnome_dock_set_client_area(GNOME_DOCK(get_widget(self)),
			       GTK_WIDGET(get_widget(widget)));
    return self;
}

/*
 * Gnome::Dock#get_client_area
 */
static VALUE
dock_get_client_area(self)
    VALUE self;
{
    GtkWidget* result;
    result = gnome_dock_get_client_area(GNOME_DOCK(get_widget(self)));
    return result?make_gnobject_auto_type(GTK_OBJECT(result)):Qnil;
}

/*
 * Gnome::Dock#get_item_by_name(name)
 *
 * return:
 *   [ item, placement, num_band, band_position, offset ]
 */
static VALUE
dock_get_item_by_name(self, name)
    VALUE self, name;
{
    GnomeDockItem* result;
    GnomeDockPlacement placement;
    guint num_band;
    guint band_position;
    guint offset;
    VALUE ary;

    result = gnome_dock_get_item_by_name(GNOME_DOCK(get_widget(self)),
					 STR2CSTR(name),
					 &placement,
					 &num_band,
					 &band_position,
					 &offset);
    ary = rb_ary_new2(5);
    rb_ary_push(ary, result?make_widget(gnoDockItem, GTK_WIDGET(result)):Qnil);
    rb_ary_push(ary, INT2NUM(placement));
    rb_ary_push(ary, INT2NUM(num_band));
    rb_ary_push(ary, INT2NUM(band_position));
    rb_ary_push(ary, INT2NUM(offset));

    return ary;
}

/*
 * Gnome::Dock#get_layout
 */
static VALUE
dock_get_layout(self)
    VALUE self;
{
    GnomeDockLayout *result;
    result = gnome_dock_get_layout(GNOME_DOCK(get_widget(self)));
    return result?make_widget(gnoDockLayout, GTK_WIDGET(result)):Qnil;
}

/*
 * Gnome::Dock#add_from_layout(layout)
 */
static VALUE
dock_add_from_layout(self, layout)
	VALUE self, layout;
{
    gboolean result;
    result = gnome_dock_add_from_layout(GNOME_DOCK(get_widget(self)),
					GNOME_DOCK_LAYOUT(layout));
    return result?Qtrue:Qfalse;
}

void
Init_gnome_dock()
{
    gnoDock = rb_define_class_under(mGnome, "Dock", gContainer);

    /* GnomeDockPlacement */
    rb_define_const(gnoDock, "TOP", INT2FIX(GNOME_DOCK_TOP));
    rb_define_const(gnoDock, "RIGHT", INT2FIX(GNOME_DOCK_RIGHT));
    rb_define_const(gnoDock, "BOTTOM", INT2FIX(GNOME_DOCK_BOTTOM));
    rb_define_const(gnoDock, "LEFT", INT2FIX(GNOME_DOCK_LEFT));
    rb_define_const(gnoDock, "FLOATING", INT2FIX(GNOME_DOCK_FLOATING));

    /*
     * instance methods
     */
    rb_define_method(gnoDock, "initialize", dock_initialize, 0);
    rb_define_method(gnoDock, "allow_floating_items", dock_allow_floating_items, 1);
    rb_define_method(gnoDock, "add_item", dock_add_item, 6);
    rb_define_method(gnoDock, "add_floating_item", dock_add_floating_item, 4);
    rb_define_method(gnoDock, "set_client_area", dock_set_client_area, 1);
    rb_define_method(gnoDock, "get_client_area", dock_get_client_area, 0);
    rb_define_method(gnoDock, "get_item_by_name", dock_get_item_by_name, 1);
    rb_define_method(gnoDock, "get_layout", dock_get_layout, 0);
    rb_define_method(gnoDock, "add_from_layout", dock_add_from_layout, 1);

    /*
     * Signal
     */
    rb_define_const(gnoDock, "SIGNAL_LAYOUT_CHANGED", rb_str_new2("layout_changed"));
}
