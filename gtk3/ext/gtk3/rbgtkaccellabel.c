/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 2001  Neil Conway
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

#define RG_TARGET_NAMESPACE cAccelLabel

static VALUE
rg_initialize(VALUE self, VALUE string)
{
    RBGTK_INITIALIZE(self, gtk_accel_label_new(RVAL2CSTR(string)));
    return Qnil;
}

static VALUE
rg_accel_width(VALUE self)
{
    return INT2NUM(gtk_accel_label_get_accel_width(GTK_ACCEL_LABEL(RVAL2GOBJ(self))));
}

static VALUE
rg_refetch(VALUE self)
{
    gtk_accel_label_refetch(GTK_ACCEL_LABEL(RVAL2GOBJ(self)));
    return Qfalse; // always returns false
}

void
Init_gtk_accel_label(VALUE mGtk)
{
  VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ACCEL_LABEL, "AccelLabel", mGtk);

  RG_DEF_METHOD(initialize, 1);
  RG_DEF_METHOD(accel_width, 0);
  RG_DEF_METHOD(refetch, 0);
}
