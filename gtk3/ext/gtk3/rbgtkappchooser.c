/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE mAppChooser
#define _SELF(self) (RVAL2GTKAPPCHOOSER(self))

static VALUE
rg_app_info(VALUE self)
{
    return GOBJ2RVAL_UNREF(gtk_app_chooser_get_app_info(_SELF(self)));
}

static VALUE
rg_refresh(VALUE self)
{
    gtk_app_chooser_refresh(_SELF(self));

    return self;
}

void
Init_gtk_appchooser(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(GTK_TYPE_APP_CHOOSER, "AppChooser", mGtk);

    RG_DEF_METHOD(app_info, 0);
    RG_DEF_METHOD(refresh, 0);
}
