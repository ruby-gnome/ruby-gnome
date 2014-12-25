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

#define RG_TARGET_NAMESPACE cAppChooserWidget
#define _SELF(self) (RVAL2GTKAPPCHOOSERWIDGET(self))

static VALUE
rg_initialize(VALUE self, VALUE content_type)
{
    RBGTK_INITIALIZE(self, gtk_app_chooser_widget_new(RVAL2CSTR(content_type)));

    return Qnil;
}

void
Init_gtk_appchooserwidget(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_APP_CHOOSER_WIDGET, "AppChooserWidget", mGtk);

    RG_DEF_METHOD(initialize, 1);
}
