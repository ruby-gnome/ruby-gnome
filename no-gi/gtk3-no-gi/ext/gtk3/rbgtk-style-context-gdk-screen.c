/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2013  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cScreen
#define _SELF(self) (RVAL2GDKSCREEN(self))

static VALUE
rg_add_provider(VALUE self, VALUE provider, VALUE priority)
{
    gtk_style_context_add_provider_for_screen(_SELF(self),
                                              RVAL2GTKSTYLEPROVIDER(provider),
                                              NUM2UINT(priority));

    return self;
}

static VALUE
rg_remove_provider(VALUE self, VALUE provider)
{
    gtk_style_context_remove_provider_for_screen(_SELF(self),
                                                 RVAL2GTKSTYLEPROVIDER(provider));

    return self;
}

static VALUE
rg_reset_widgets(VALUE self)
{
    gtk_style_context_reset_widgets(_SELF(self));

    return self;
}

void
Init_gtk_style_context_gdk_screen(void)
{
    VALUE RG_TARGET_NAMESPACE = GTYPE2CLASS(GDK_TYPE_SCREEN);

    RG_DEF_METHOD(add_provider, 2);
    RG_DEF_METHOD(remove_provider, 1);
    RG_DEF_METHOD(reset_widgets, 0);
}
