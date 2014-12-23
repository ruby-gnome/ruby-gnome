/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2014  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cActionBar
#define _SELF(self) (RVAL2GTKACTIONBAR(self))

#if GTK_CHECK_VERSION(3, 12, 0)
static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_action_bar_new());
    return Qnil;
}

static VALUE
rg_pack_start(VALUE self, VALUE child)
{
    gtk_action_bar_pack_start(_SELF(self), RVAL2GOBJ(child));
    return self;
}

static VALUE
rg_pack_end(VALUE self, VALUE child)
{
    gtk_action_bar_pack_end(_SELF(self), RVAL2GOBJ(child));
    return self;
}

static VALUE
rg_center_widget(VALUE self)
{
    return GOBJ2RVAL(gtk_action_bar_get_center_widget(_SELF(self)));
}

static VALUE
rg_set_center_widget(VALUE self, VALUE child)
{
    gtk_action_bar_set_center_widget(_SELF(self), RVAL2GOBJ(child));
    return self;
}

void
Init_gtk_action_bar(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GTK_TYPE_ACTION_BAR, "ActionBar", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(pack_start, 1);
    RG_DEF_METHOD(pack_end, 1);
    RG_DEF_METHOD(center_widget, 0);
    RG_DEF_METHOD(set_center_widget, 1);
}
#endif
