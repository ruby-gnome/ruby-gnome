/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rbgdk3private.h"

#ifdef GDK_WINDOWING_X11
#define RG_TARGET_NAMESPACE cX11Window
#define _SELF(self) (RVAL2GDKX11WINDOW(self))

static VALUE
rg_xid(VALUE self)
{
    return ULONG2NUM(gdk_x11_window_get_xid(_SELF(self)));
}

static VALUE
rg_move_to_current_desktop(VALUE self)
{
    gdk_x11_window_move_to_current_desktop(_SELF(self));

    return self;
}

static VALUE
rg_server_time(VALUE self)
{
    return UINT2NUM(gdk_x11_get_server_time(_SELF(self)));
}

static VALUE
rg_set_user_time(VALUE self, VALUE time)
{
    gdk_x11_window_set_user_time(_SELF(self), NUM2UINT(time));
    return Qnil;
}
#endif

void
Init_gdkx11_x11window(VALUE mGdkX11)
{
#ifdef GDK_WINDOWING_X11
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_X11_WINDOW, "X11Window", mGdkX11);

    RG_DEF_METHOD(xid, 0);
    RG_DEF_METHOD(move_to_current_desktop, 0);
    RG_DEF_METHOD(server_time, 0);
    RG_DEF_METHOD(set_user_time, 1);
#endif
}
