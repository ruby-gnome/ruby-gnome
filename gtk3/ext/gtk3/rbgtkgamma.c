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

#define RG_TARGET_NAMESPACE cGammaCurve

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_gamma_curve_new());
    return Qnil;
}

static VALUE
rg_gamma(VALUE self)
{
    return rb_float_new(GTK_GAMMA_CURVE(RVAL2GOBJ(self))->gamma);
}

static VALUE
rg_curve(VALUE self)
{
    return GOBJ2RVAL(GTK_GAMMA_CURVE(RVAL2GOBJ(self))->curve);
}

void 
Init_gtk_gamma_curve(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_GAMMA_CURVE, "GammaCurve", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(gamma, 0);
    RG_DEF_METHOD(curve, 0);
}
