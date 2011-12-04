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

#define RG_TARGET_NAMESPACE cOverlay
#define _SELF(self) (RVAL2GTKOVERLAY(self))

#if GTK_CHECK_VERSION(3, 2, 0)
static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_overlay_new());

    return Qnil;
}

static VALUE
rg_add_overlay(VALUE self, VALUE widget)
{
    gtk_overlay_add_overlay(_SELF(self), RVAL2GTKWIDGET(widget));

    return self;
}
#endif

void
Init_gtk_overlay(VALUE mGtk)
{
#if GTK_CHECK_VERSION(3, 2, 0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_OVERLAY, "Overlay", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(add_overlay, 1);
#endif
}
