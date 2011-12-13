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

#define RG_TARGET_NAMESPACE cApplication
#define _SELF(self) (RVAL2GTKAPPLICATION(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE application_id, flags;

    rb_scan_args(argc, argv, "11", &application_id, &flags);
    G_INITIALIZE(self, gtk_application_new(RVAL2CSTR(application_id),
                                           NIL_P(flags) ? 0 : RVAL2GAPPLICATIONFLAGS(flags)));

    return Qnil;
}

static VALUE
rg_add_window(VALUE self, VALUE window)
{
    gtk_application_add_window(_SELF(self), RVAL2GTKWINDOW(window));

    return self;
}

static VALUE
rg_windows(VALUE self)
{
    return GOBJGLIST2RVAL(gtk_application_get_windows(_SELF(self)));
}

static VALUE
rg_remove_window(VALUE self, VALUE window)
{
    gtk_application_remove_window(_SELF(self), RVAL2GTKWINDOW(window));

    return self;
}

void
Init_gtk_application(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_APPLICATION, "Application", mGtk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(add_window, 1);
    RG_DEF_METHOD(windows, 0);
    RG_DEF_METHOD(remove_window, 1);
}
