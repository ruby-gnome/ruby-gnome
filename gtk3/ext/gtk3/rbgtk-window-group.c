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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cWindowGroup
#define _SELF(self) (RVAL2GTKWINDOWGROUP(self))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_window_group_new());
    return Qnil;
}

static VALUE
rg_add(VALUE self, VALUE window)
{
    gtk_window_group_add_window(_SELF(self), 
                                RVAL2GTKWINDOW(window));
    return self;
}

static VALUE
rg_remove(VALUE self, VALUE window)
{
    gtk_window_group_remove_window(_SELF(self), 
                                   RVAL2GTKWINDOW(window));
    return self;
}

static VALUE
rg_get_current_device_grab(VALUE self, VALUE device)
{
    return GOBJ2RVAL(gtk_window_group_get_current_device_grab(_SELF(self),
                                                              NIL_P(device) ? NULL : RVAL2GDKDEVICE(device)));
}

static VALUE
rg_current_grab(VALUE self)
{
    return GOBJ2RVAL(gtk_window_group_get_current_grab(_SELF(self)));
}

static VALUE
rg_windows(VALUE self)
{
    /* TODO: need free? */
    return GOBJGLIST2RVAL(gtk_window_group_list_windows(_SELF(self)));
}

void 
Init_gtk_windowgroup(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_WINDOW_GROUP, "WindowGroup", mGtk);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(add, 1);
    RG_DEF_METHOD(remove, 1);
    RG_DEF_METHOD(get_current_device_grab, 1);
    RG_DEF_METHOD(current_grab, 0);
    RG_DEF_METHOD(windows, 0);
}
