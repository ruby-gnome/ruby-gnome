/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cVScale

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2, arg3;
    GtkAdjustment *adj = NULL;
    GtkWidget *widget;

    rb_scan_args(argc, argv, "03", &arg1, &arg2, &arg3);

    if (NIL_P(arg1) || RVAL2GTYPE(arg1) == GTK_TYPE_ADJUSTMENT){
        if (!NIL_P(arg1)) adj = GTK_ADJUSTMENT(RVAL2GOBJ(arg1));
        widget = gtk_vscale_new(adj);
    } else {
        widget = gtk_vscale_new_with_range(NUM2DBL(arg1), 
                                           NUM2DBL(arg2), NUM2DBL(arg3));
    }
    RBGTK_INITIALIZE(self, widget);
    return Qnil;
}

void 
Init_gtk_vscale(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_VSCALE, "VScale", mGtk);
    RG_DEF_METHOD(initialize, -1);
}
