/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

#if GTK_CHECK_VERSION(2,2,0)
#define _SELF(obj) GDK_DISPLAY_MANAGER(RVAL2GOBJ(obj))

static VALUE
gdkdisplaymanager_get(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_display_manager_get());
}

static VALUE
gdkdisplaymanager_list_displays(VALUE self)
{
    return GSLIST2ARYF(gdk_display_manager_list_displays(_SELF(self)));
}

/* Move to Gdk::Display.
static VALUE
gdkdisplaymanager_get_core_pointer(VALUE self)
{
    return GOBJ2RVAL(gdk_display_get_core_pointer(_SELF(self)));
}
*/
#endif

void 
Init_gtk_gdk_display_manager(void)
{
#if GTK_CHECK_VERSION(2,2,0)
    VALUE gdkDisplayManager = G_DEF_CLASS(GDK_TYPE_DISPLAY_MANAGER, "DisplayManager", mGdk);

    rb_define_singleton_method(gdkDisplayManager, "get", gdkdisplaymanager_get, 0);
    rb_define_method(gdkDisplayManager, "displays", gdkdisplaymanager_list_displays, 0);
#endif
}
