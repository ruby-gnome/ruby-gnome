/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cAccessible
#define _SELF(self) (RVAL2GTKACCESSIBLE(self))

static VALUE
rg_connect_widget_destroyed(VALUE self)
{
    gtk_accessible_connect_widget_destroyed(RVAL2GTKACCESSIBLE(self));
    return self;
}

static VALUE
rg_widget(VALUE self)
{
    return GOBJ2RVAL(gtk_accessible_get_widget(_SELF(self)));
}

static VALUE
rg_set_widget(VALUE self, VALUE widget)
{
    gtk_accessible_set_widget(_SELF(self), RVAL2GTKWIDGET(widget));

    return self;
}

void
Init_gtk_accessible(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ACCESSIBLE, "Accessible", mGtk);

    RG_DEF_METHOD(connect_widget_destroyed, 0);
    RG_DEF_METHOD(widget, 0);
    RG_DEF_METHOD(set_widget, 1);
}
