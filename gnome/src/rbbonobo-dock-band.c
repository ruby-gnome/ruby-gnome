/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbbonobo-dock-band.c,v 1.1 2002/09/25 14:10:04 tkubo Exp $ */

/* Gnome::DockBand widget for Ruby/Gnome
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
 * Class Gnome::DockBand
 *
 * Hierarchy:
 *
 *   Object
 *     +----Gtk::Object
 *            +----Gtk::Widget
 *                   +----Gtk::Container
 *                          +----Gnome::DockBand
 */
VALUE gnoDockBand;

static VALUE
dockband_initialize(self)
    VALUE self;
{
    set_widget(self, gnome_dock_band_new());
    return Qnil;
}

static VALUE
dockband_set_orientation(self, orientation)
    VALUE self, orientation;
{
    gnome_dock_band_set_orientation(GNOME_DOCK_BAND(get_widget(self)),
                                    NUM2INT(orientation));
    return self;
}

static VALUE
dockband_get_orientation(self)
    VALUE self;
{
    GtkOrientation result;
    result = gnome_dock_band_get_orientation(
        GNOME_DOCK_BAND(get_widget(self)));
    return INT2FIX(result);
}

static VALUE
dockband_insert(self, child, offset, position)
    VALUE self, child, offset, position;
{
    gboolean result;
    result = gnome_dock_band_insert(
        GNOME_DOCK_BAND(get_widget(self)),
        GTK_WIDGET(get_widget(child)),
        NUM2INT(offset),
        NUM2INT(position));

    if (!result) {
        rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

static VALUE
dockband_prepend(self, child, offset)
    VALUE self, child, offset;
{
    gboolean result;
    result = gnome_dock_band_prepend(
        GNOME_DOCK_BAND(get_widget(self)),
        GTK_WIDGET(get_widget(child)),
        NUM2INT(offset));

    if (!result) {
        rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

static VALUE
dockband_append(self, child, offset)
    VALUE self, child, offset;
{
    gboolean result;
    result = gnome_dock_band_append(
        GNOME_DOCK_BAND(get_widget(self)),
        GTK_WIDGET(get_widget(child)),
        NUM2INT(offset));

    if (!result) {
        rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}
    
static VALUE
dockband_set_child_offset(self, child, offset)
    VALUE self, child, offset;
{
    gnome_dock_band_set_child_offset(
        GNOME_DOCK_BAND(get_widget(self)),
        GTK_WIDGET(get_widget(child)),
        NUM2INT(offset));

    return self;
}
    
static VALUE
dockband_get_child_offset(self, child)
    VALUE self, child;
{
    guint result;
    result = gnome_dock_band_get_child_offset(
        GNOME_DOCK_BAND(get_widget(self)),
        GTK_WIDGET(get_widget(child)));

    return INT2NUM(self);
}

#if 0
static VALUE
dockband_move_child(self, old_child, new_num)
    VALUE self, old_child, new_num;
{
    return self;
}
#endif

static VALUE
dockband_get_num_children(self)
    VALUE self;
{
    guint result;
    result = gnome_dock_band_get_num_children(
        GNOME_DOCK_BAND(get_widget(self)));
    return INT2NUM(result);
}

static VALUE
dockband_drag_begin(self, item)
    VALUE self, item;
{
    gnome_dock_band_drag_begin(
        GNOME_DOCK_BAND(get_widget(self)),
        GNOME_DOCK_ITEM(get_widget(item)));
    return self;
}

static VALUE
dockband_drag_to(self, item, x, y)
    VALUE self, item, x, y;
{
    gboolean result;
    result = gnome_dock_band_drag_to(
        GNOME_DOCK_BAND(get_widget(self)),
        GNOME_DOCK_ITEM(get_widget(item)),
        NUM2INT(x), NUM2INT(y));

    if (!result) {
        rb_raise(rb_eRuntimeError, "operation failed\n");
    }

    return self;
}

static VALUE
dockband_drag_end(self, item)
    VALUE self, item;
{
    gnome_dock_band_drag_end(
        GNOME_DOCK_BAND(get_widget(self)),
        GNOME_DOCK_ITEM(get_widget(item)));
    return self;
}

static VALUE
dockband_get_item_by_name(self, name)
    VALUE self, name;
{
    GnomeDockItem *result;
    guint position, offset;
    VALUE ary;

    result = gnome_dock_band_get_item_by_name(
        GNOME_DOCK_BAND(get_widget(self)), STR2CSTR(name),
        &position, &offset);

    ary = rb_ary_new2(3);
    rb_ary_push(ary, make_widget(gnoDockItem, GTK_WIDGET(result)));
    rb_ary_push(ary, INT2NUM(position));
    rb_ary_push(ary, INT2NUM(offset));

    return ary;
}

static VALUE
dockband_layout_add(self, layout, placement, band_num)
    VALUE self, layout, placement, band_num;
{
    gnome_dock_band_layout_add(
        GNOME_DOCK_BAND(get_widget(self)),
        GNOME_DOCK_LAYOUT(layout),
        NUM2INT(placement), NUM2INT(band_num));
    return self;
}

void
Init_gnome_dock_band()
{
    gnoDockBand = rb_define_class_under(mGnome, "DockBand", gContainer);

    /*
     * instance methods
     */
    rb_define_method(gnoDockBand, "initialize", dockband_initialize, 0);
    rb_define_method(gnoDockBand, "set_orientation", dockband_set_orientation, 1);
    rb_define_method(gnoDockBand, "get_orientation", dockband_get_orientation, 0);
    rb_define_method(gnoDockBand, "insert", dockband_insert, 3);
    rb_define_method(gnoDockBand, "prepend", dockband_prepend, 2);
    rb_define_method(gnoDockBand, "append", dockband_append, 2);
    rb_define_method(gnoDockBand, "set_child_offset", dockband_set_child_offset, 2);
    rb_define_method(gnoDockBand, "get_child_offset", dockband_get_child_offset, 1);
#if 0
    rb_define_method(gnoDockBand, "move_child", dockband_move_child, 2);
#endif
    rb_define_method(gnoDockBand, "get_num_children", dockband_get_num_children, 0);
    rb_define_method(gnoDockBand, "drag_begin", dockband_drag_begin, 1);
    rb_define_method(gnoDockBand, "drag_to", dockband_drag_to, 3);
    rb_define_method(gnoDockBand, "drag_end", dockband_drag_end, 1);
    rb_define_method(gnoDockBand, "get_item_by_name", dockband_get_item_by_name, 1);
    rb_define_method(gnoDockBand, "layout_add", dockband_layout_add, 3);
}
