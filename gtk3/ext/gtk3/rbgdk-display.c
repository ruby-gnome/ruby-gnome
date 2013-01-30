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

#define RG_TARGET_NAMESPACE cDisplay
#define _SELF(self) (RVAL2GDKDISPLAYOBJECT(self))

static VALUE
rg_trigger_tooltip_query(VALUE self)
{
    gtk_tooltip_trigger_tooltip_query(_SELF(self));
    return self;
}

void 
Init_gtk_tooltip_gdk_display(void)
{
    VALUE RG_TARGET_NAMESPACE = GTYPE2CLASS(GDK_TYPE_DISPLAY);

    RG_DEF_METHOD(trigger_tooltip_query, 0);
}
