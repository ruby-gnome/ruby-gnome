/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#include "global.h"

#define RG_TARGET_NAMESPACE cWindowGroup

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_window_group_new());
    return Qnil;
}

static VALUE
rg_add(VALUE self, VALUE window)
{
    gtk_window_group_add_window(GTK_WINDOW_GROUP(RVAL2GOBJ(self)), 
                                    GTK_WINDOW(RVAL2GOBJ(window)));
    return self;
}

static VALUE
rg_remove(VALUE self, VALUE window)
{
    gtk_window_group_remove_window(GTK_WINDOW_GROUP(RVAL2GOBJ(self)), 
                                       GTK_WINDOW(RVAL2GOBJ(window)));
    return self;
}

void 
Init_gtk_windowgroup(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_WINDOW_GROUP, "WindowGroup", mGtk);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(add, 1);
    RG_DEF_METHOD(remove, 1);
}
