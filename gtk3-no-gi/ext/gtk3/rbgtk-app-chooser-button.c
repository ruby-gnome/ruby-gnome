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

#define RG_TARGET_NAMESPACE cAppChooserButton
#define _SELF(self) (RVAL2GTKAPPCHOOSERBUTTON(self))

static VALUE
rg_initialize(VALUE self, VALUE content_type)
{
    RBGTK_INITIALIZE(self, gtk_app_chooser_button_new(RVAL2CSTR(content_type)));

    return Qnil;
}

static VALUE
rg_append_custom_item(VALUE self, VALUE name, VALUE label, VALUE icon)
{
    gtk_app_chooser_button_append_custom_item(_SELF(self),
                                              RVAL2CSTR(name),
                                              RVAL2CSTR(label),
                                              RVAL2GICON(icon));

    return self;
}

static VALUE
rg_append_separator(VALUE self)
{
    gtk_app_chooser_button_append_separator(_SELF(self));

    return self;
}

static VALUE
rg_set_active_custom_item(VALUE self, VALUE name)
{
    gtk_app_chooser_button_set_active_custom_item(_SELF(self), RVAL2CSTR(name));

    return self;
}

void
Init_gtk_appchooserbutton(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_APP_CHOOSER_BUTTON, "AppChooserButton", mGtk);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(append_custom_item, 3);
    RG_DEF_METHOD(append_separator, 0);
    RG_DEF_METHOD(set_active_custom_item, 1);
}
