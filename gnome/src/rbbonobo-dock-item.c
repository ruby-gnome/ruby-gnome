/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbbonobo-dock-item.c,v 1.3 2002/10/26 06:22:24 tkubo Exp $ */
/* based on libbonoboui/bonobo/bonobo-doc-item.h */
/* renamed from rbgnome-dock-item.c */

/* Bonobo::DockItem widget for Ruby/GNOME2
 * Copyright (C) 1999 Minoru Inachi <inachi@earth.interq.or.jp>
 *               2002 KUBO Takehiro <kubo@jiubao.org>
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

#include "rbbonobo.h"

#define _SELF(self) BONOBO_DOCK_ITEM(RVAL2GOBJ(self))

/*
 * Bonobo::DockItem::initialize(name, behavior)
 *
 * Initialize a new BonoboDockItem named @name, with the
 * specified @behavior.
 */
static VALUE
dockitem_initialize(self, name, behavior)
    VALUE self, name, behavior;
{
    RBGTK_INITIALIZE(self, bonobo_dock_item_new(RVAL2CSTR(name), NUM2INT(behavior)));
    return Qnil;
}

/*
 * Bonobo::DockItem::get_child
 *
 * Retrieve the child.
 */
static VALUE
dockitem_get_child(self)
    VALUE self;
{
    return GOBJ2RVAL(bonobo_dock_item_get_child(_SELF(self)));
}

/*
 * Bonobo::DockItem::get_name
 *
 * Retrieve the name.
 */
static VALUE
dockitem_get_name(self)
    VALUE self;
{
    char* result = bonobo_dock_item_get_name(_SELF(self));
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

/*
 * Bonobo::DockItem::set_shadow_type(type)
 *
 * Set the shadow type.
 */
static VALUE
dockitem_set_shadow_type(self, type)
    VALUE self, type;
{
    bonobo_dock_item_set_shadow_type(_SELF(self), NUM2INT(type));
    return self;
}

/*
 * Bonobo::DockItem#get_shadow_type
 *
 * Retrieve the shadow type.
 */
static VALUE
dockitem_get_shadow_type(self)
    VALUE self;
{
    GtkShadowType result;
    result = bonobo_dock_item_get_shadow_type(_SELF(self));
    return INT2FIX(result);
}

/*
 * Bonobo::DockItem#set_orientation(orientation)
 * 
 * Set the orientation.
 */
static VALUE
dockitem_set_orientation(self, orientation)
    VALUE self, orientation;
{
    gboolean result;
    result = bonobo_dock_item_set_orientation(_SELF(self),
                                              NUM2INT(orientation));
    if (!result) {
        rb_raise(rb_eRuntimeError, "operation failed\n");
    }
    return self;
}

/*
 * Bonobo::DockItem#get_orientation
 * 
 * Retrieve the orientation.
 */
static VALUE
dockitem_get_orientation(self)
    VALUE self;
{
    GtkOrientation result;
    result = bonobo_dock_item_get_orientation(_SELF(self));
    return INT2FIX(result);
}

/*
 * Bonobo::DockItem#get_behavior
 *
 * Retrieve the behavior.
 */
static VALUE
dockitem_get_behavior(self)
    VALUE self;
{
    BonoboDockItemBehavior result;
    result = bonobo_dock_item_get_behavior(_SELF(self));
    return INT2FIX(result);
}

void
Init_bonobo_dock_item(mBonobo)
    VALUE mBonobo;
{
    VALUE bnbDockItem = G_DEF_CLASS(BONOBO_TYPE_DOCK_ITEM, "DockItem", mBonobo);

    /* BonoboDockItemBehavior */
    rb_define_const(bnbDockItem, "BEH_NORMAL", INT2FIX(BONOBO_DOCK_ITEM_BEH_NORMAL));
    rb_define_const(bnbDockItem, "BEH_EXCLUSIVE", INT2FIX(BONOBO_DOCK_ITEM_BEH_EXCLUSIVE));
    rb_define_const(bnbDockItem, "BEH_NEVER_FLOATING", INT2FIX(BONOBO_DOCK_ITEM_BEH_NEVER_FLOATING));
    rb_define_const(bnbDockItem, "BEH_NEVER_VERTICAL", INT2FIX(BONOBO_DOCK_ITEM_BEH_NEVER_VERTICAL));
    rb_define_const(bnbDockItem, "BEH_NEVER_HORIZONTAL", INT2FIX(BONOBO_DOCK_ITEM_BEH_NEVER_HORIZONTAL));
    rb_define_const(bnbDockItem, "BEH_LOCKED", INT2FIX(BONOBO_DOCK_ITEM_BEH_LOCKED));

    /*
     * instance methods
     */
    rb_define_method(bnbDockItem, "initialize", dockitem_initialize, 2);
    rb_define_method(bnbDockItem, "child", dockitem_get_child, 0);
    rb_define_method(bnbDockItem, "name", dockitem_get_name, 0);
    rb_define_method(bnbDockItem, "set_shadow_type", dockitem_set_shadow_type, 1);
    rb_define_method(bnbDockItem, "shadow_type", dockitem_get_shadow_type, 0);
    rb_define_method(bnbDockItem, "set_orientation", dockitem_set_orientation, 1);
    rb_define_method(bnbDockItem, "orientation", dockitem_get_orientation, 0);
    rb_define_method(bnbDockItem, "behavior", dockitem_get_behavior, 0);

    G_DEF_SETTERS(bnbDockItem);
}
