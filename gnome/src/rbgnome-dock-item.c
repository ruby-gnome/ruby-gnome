/* $Id: rbgnome-dock-item.c,v 1.1 2002/05/19 12:39:26 mutoh Exp $ */

/* Gnome::DockItem widget for Ruby/Gnome
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

VALUE gnoDockItem;

/*
 * Gnome::DockItem::initialize(name, behavior)
 *
 * Initialize a new GnomeDockItem named @name, with the
 * specified @behavior.
 */
static VALUE
dockitem_initialize(self, name, behavior)
    VALUE self, name, behavior;
{
    set_widget(self, gnome_dock_item_new(STR2CSTR(name), NUM2INT(behavior)));
    return Qnil;
}

/*
 * Gnome::DockItem::get_child
 *
 * Retrieve the child.
 */
static VALUE
dockitem_get_child(self)
    VALUE self;
{
    GtkWidget* result;
    result = gnome_dock_item_get_child(GNOME_DOCK_ITEM(get_widget(self)));
    return make_gnobject_auto_type((GtkObject*)result);
}

/*
 * Gnome::DockItem::get_name
 *
 * Retrieve the name.
 */
static VALUE
dockitem_get_name(self)
    VALUE self;
{
    char* result;
    result = gnome_dock_item_get_name(GNOME_DOCK_ITEM(get_widget(self)));
    return rb_str_new2(result);
}

/*
 * Gnome::DockItem::set_shadow_type(type)
 *
 * Set the shadow type.
 */
static VALUE
dockitem_set_shadow_type(self, type)
    VALUE self, type;
{
    gnome_dock_item_set_shadow_type(GNOME_DOCK_ITEM(get_widget(self)),
				    NUM2INT(type));
    return self;
}

/*
 * Gnome::DockItem#get_shadow_type
 *
 * Retrieve the shadow type.
 */
static VALUE
dockitem_get_shadow_type(self)
	VALUE self;
{
    GtkShadowType result;
    result = gnome_dock_item_get_shadow_type(
		 GNOME_DOCK_ITEM(get_widget(self)));
    return INT2FIX(result);
}

/*
 * Gnome::DockItem#set_orientation(orientation)
 * 
 * Set the orientation.
 */
static VALUE
dockitem_set_orientation(self, orientation)
    VALUE self, orientation;
{
    gboolean result;
    result = gnome_dock_item_set_orientation(GNOME_DOCK_ITEM(get_widget(self)),
					     NUM2INT(orientation));
    if (!result) {
	rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

/*
 * Gnome::DockItem#get_orientation
 * 
 * Retrieve the orientation.
 */
static VALUE
dockitem_get_orientation(self)
    VALUE self;
{
    GtkOrientation result;
    result = gnome_dock_item_get_orientation(
		GNOME_DOCK_ITEM(get_widget(self)));
    return INT2FIX(result);
}

/*
 * Gnome::DockItem#get_behavior
 *
 * Retrieve the behavior.
 */
static VALUE
dockitem_get_behavior(self)
    VALUE self;
{
    GnomeDockItemBehavior result;
    result = gnome_dock_item_get_behavior(
		GNOME_DOCK_ITEM(get_widget(self)));
    return INT2FIX(result);
}

void
Init_gnome_dock_item()
{
    gnoDockItem = rb_define_class_under(mGnome, "DockItem", gBin);

    /* GnomeDockItemBehavior */
    rb_define_const(gnoDockItem, "BEH_NORMAL",
		    INT2FIX(GNOME_DOCK_ITEM_BEH_NORMAL));
    rb_define_const(gnoDockItem, "BEH_EXCLUSIVE",
		    INT2FIX(GNOME_DOCK_ITEM_BEH_EXCLUSIVE));
    rb_define_const(gnoDockItem, "BEH_NEVER_FLOATING",
		    INT2FIX(GNOME_DOCK_ITEM_BEH_NEVER_FLOATING));
    rb_define_const(gnoDockItem, "BEH_NEVER_VERTICAL",
		    INT2FIX(GNOME_DOCK_ITEM_BEH_NEVER_VERTICAL));
    rb_define_const(gnoDockItem, "BEH_NEVER_HORIZONTAL",
		    INT2FIX(GNOME_DOCK_ITEM_BEH_NEVER_HORIZONTAL));
    rb_define_const(gnoDockItem, "BEH_LOCKED",
		    INT2FIX(GNOME_DOCK_ITEM_BEH_LOCKED));

    /*
     * instance methods
     */
    rb_define_method(gnoDockItem, "initialize",
		     dockitem_initialize, 2);
    rb_define_method(gnoDockItem, "get_child",
		     dockitem_get_child, 0);
    rb_define_method(gnoDockItem, "get_name",
		     dockitem_get_name, 0);
    rb_define_method(gnoDockItem, "set_shadow_type",
		     dockitem_set_shadow_type, 1);
    rb_define_method(gnoDockItem, "get_shadow_type",
		     dockitem_get_shadow_type, 0);
    rb_define_method(gnoDockItem, "set_orientation",
		     dockitem_set_orientation, 1);
    rb_define_method(gnoDockItem, "get_orientation",
		     dockitem_get_orientation, 0);
    rb_define_method(gnoDockItem, "get_behavior",
		     dockitem_get_behavior, 0);

    /* Signals */
    rb_define_const(gnoDockItem, "SIGNAL_DOCK_DRAG_BEGIN",
		    rb_str_new2("dock_drag_begin"));
    rb_define_const(gnoDockItem, "SIGNAL_DOCK_DRAG_MOTION",
		    rb_str_new2("dock_drag_motion"));
    rb_define_const(gnoDockItem, "SIGNAL_DOCK_DRAG_END",
		    rb_str_new2("dock_drag_end"));
    rb_define_const(gnoDockItem, "SIGNAL_DOCK_DETACH",
		    rb_str_new2("dock_detach"));
}
