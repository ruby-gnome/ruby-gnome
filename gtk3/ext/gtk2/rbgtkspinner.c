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

#include "global.h"

#if GTK_CHECK_VERSION(2,20,0)

#define RG_TARGET_NAMESPACE cSpinner
#define _SELF(s) (GTK_SPINNER(RVAL2GOBJ(s)))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_spinner_new());
    return Qnil;
}

static VALUE
rg_start(VALUE self)
{
    gtk_spinner_start(_SELF(self));
    return self;
}

static VALUE
rg_stop(VALUE self)
{
    gtk_spinner_stop(_SELF(self));
    return self;
}

#endif

void 
Init_gtk_spinner(VALUE mGtk)
{
#if GTK_CHECK_VERSION(2,20,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_SPINNER, "Spinner", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(start, 0);
    RG_DEF_METHOD(stop, 0);
#endif
}
