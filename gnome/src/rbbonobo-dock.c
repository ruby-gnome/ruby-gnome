/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbbonobo-dock.c,v 1.2 2002/09/25 17:17:24 tkubo Exp $ */

/* Bonobo::Dock widget for Ruby/Gnome
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

#define _SELF(self) BONOBO_DOCK(RVAL2GOBJ(self))

static VALUE
dock_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, bonobo_dock_new());
    return Qnil;
}

/*
 * Bonobo::Dock#allow_floating_items(enable)
 */
static VALUE
dock_allow_floating_items(self, enable)
    VALUE self, enable;
{
    bonobo_dock_allow_floating_items(_SELF(self),
                                     RTEST(enable));
    return self;
}

/*
 * Bonobo::Dock#add_item(item, placement, band_num, position,
 *          offset, in_new_band)
 */ 
static VALUE
dock_add_item(self, item, placement, band_num, position, offset, in_new_band)
    VALUE self, item, placement, band_num, position, offset, in_new_band;
{
    bonobo_dock_add_item(_SELF(self),
                         BONOBO_DOCK_ITEM(RVAL2GOBJ(item)),
                         NUM2INT(placement),
                         NUM2INT(band_num),
                         NUM2INT(position),
                         NUM2INT(offset),
                         RTEST(in_new_band));
    return self;
}

/*
 * Bonobo::Dock#add_floating_item(widget, x, y, orientation)
 */
static VALUE
dock_add_floating_item(self, widget, x, y, orientation)
    VALUE self, widget, x, y, orientation;
{
    bonobo_dock_add_floating_item(_SELF(self),
                                  BONOBO_DOCK_ITEM(RVAL2GOBJ(widget)),
                                  NUM2INT(x), NUM2INT(y),
                                  NUM2INT(orientation));
    return self;
}

/*
 * Bonobo::Dock#set_client_area(widget)
 */
static VALUE
dock_set_client_area(self, widget)
    VALUE self, widget;
{
    bonobo_dock_set_client_area(_SELF(self),
                                GTK_WIDGET(RVAL2GOBJ(widget)));
    return self;
}

/*
 * Bonobo::Dock#get_client_area
 */
static VALUE
dock_get_client_area(self)
    VALUE self;
{
    GtkWidget* result;
    result = bonobo_dock_get_client_area(_SELF(self));
    return result ? GOBJ2RVAL(result) : Qnil;
}

/*
 * Bonobo::Dock#get_item_by_name(name)
 *
 * return:
 *   [ item, placement, num_band, band_position, offset ]
 */
static VALUE
dock_get_item_by_name(self, name)
    VALUE self, name;
{
    BonoboDockItem* result;
    BonoboDockPlacement placement;
    guint num_band;
    guint band_position;
    guint offset;
    VALUE ary;

    result = bonobo_dock_get_item_by_name(_SELF(self),
                                          RVAL2CSTR(name),
                                          &placement,
                                          &num_band,
                                          &band_position,
                                          &offset);
    ary = rb_ary_new2(5);
    rb_ary_push(ary, result ? GOBJ2RVAL(result) : Qnil);
    rb_ary_push(ary, INT2NUM(placement));
    rb_ary_push(ary, INT2NUM(num_band));
    rb_ary_push(ary, INT2NUM(band_position));
    rb_ary_push(ary, INT2NUM(offset));

    return ary;
}

/*
 * Bonobo::Dock#get_layout
 */
static VALUE
dock_get_layout(self)
    VALUE self;
{
    BonoboDockLayout *result;
    result = bonobo_dock_get_layout(_SELF(self));
    return result ? GOBJ2RVAL(result) : Qnil;
}

/*
 * Bonobo::Dock#add_from_layout(layout)
 */
static VALUE
dock_add_from_layout(self, layout)
    VALUE self, layout;
{
    gboolean result;
    result = bonobo_dock_add_from_layout(_SELF(self),
                                         BONOBO_DOCK_LAYOUT(RVAL2GOBJ(layout)));
    return result ? Qtrue : Qfalse;
}

void
Init_bonobo_dock(mBonobo)
    VALUE mBonobo;
{
    VALUE bnbDock = G_DEF_CLASS(BONOBO_TYPE_DOCK, "Dock", mBonobo);

    /* BonoboDockPlacement */
    rb_define_const(bnbDock, "TOP", INT2FIX(BONOBO_DOCK_TOP));
    rb_define_const(bnbDock, "RIGHT", INT2FIX(BONOBO_DOCK_RIGHT));
    rb_define_const(bnbDock, "BOTTOM", INT2FIX(BONOBO_DOCK_BOTTOM));
    rb_define_const(bnbDock, "LEFT", INT2FIX(BONOBO_DOCK_LEFT));
    rb_define_const(bnbDock, "FLOATING", INT2FIX(BONOBO_DOCK_FLOATING));

    /*
     * instance methods
     */
    rb_define_method(bnbDock, "initialize", dock_initialize, 0);
    rb_define_method(bnbDock, "allow_floating_items", dock_allow_floating_items, 1);
    rb_define_method(bnbDock, "add_item", dock_add_item, 6);
    rb_define_method(bnbDock, "add_floating_item", dock_add_floating_item, 4);
    rb_define_method(bnbDock, "set_client_area", dock_set_client_area, 1);
    rb_define_method(bnbDock, "get_client_area", dock_get_client_area, 0);
    rb_define_method(bnbDock, "get_item_by_name", dock_get_item_by_name, 1);
    rb_define_method(bnbDock, "get_layout", dock_get_layout, 0);
    rb_define_method(bnbDock, "add_from_layout", dock_add_from_layout, 1);
}
