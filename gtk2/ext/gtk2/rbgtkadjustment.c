/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cAdjustment
#define _SELF(self) (GTK_ADJUSTMENT(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self, VALUE value, VALUE lower, VALUE upper, VALUE step_inc, VALUE page_inc, VALUE page_size)
{
    RBGTK_INITIALIZE(self, gtk_adjustment_new(NUM2DBL(value),
                                              NUM2DBL(lower),
                                              NUM2DBL(upper),
                                              NUM2DBL(step_inc),
                                              NUM2DBL(page_inc),
                                              NUM2DBL(page_size)));
    return Qnil;
}

static VALUE
rg_clamp_page(VALUE self, VALUE lower, VALUE upper)
{
    gtk_adjustment_clamp_page(_SELF(self), NUM2DBL(lower), NUM2DBL(upper));
    return self;
}

static VALUE
rg_changed(VALUE self)
{
    gtk_adjustment_changed(_SELF(self));
    return self;
}

static VALUE
rg_value_changed(VALUE self)
{
    gtk_adjustment_value_changed(_SELF(self));
    return self;
}

void 
Init_gtk_adjustment(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ADJUSTMENT, "Adjustment", mGtk);

    RG_DEF_METHOD(initialize, 6);
    RG_DEF_METHOD(clamp_page, 2);
    RG_DEF_METHOD(changed, 0);
    RG_DEF_METHOD(value_changed, 0);
}
