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

#define RG_TARGET_NAMESPACE cCurve

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_curve_new());
    return Qnil;
}

static VALUE
rg_reset(VALUE self)
{
    gtk_curve_reset(GTK_CURVE(RVAL2GOBJ(self)));
    return self;
}

static VALUE
rg_set_gamma(VALUE self, VALUE gamma)
{
    gtk_curve_set_gamma(GTK_CURVE(RVAL2GOBJ(self)), NUM2DBL(gamma));
    return self;
}

static VALUE
rg_set_range(VALUE self, VALUE min_x, VALUE max_x, VALUE min_y, VALUE max_y)
{
    gtk_curve_set_range(GTK_CURVE(RVAL2GOBJ(self)),
                        NUM2DBL(min_x), NUM2DBL(max_x),
                        NUM2DBL(min_y), NUM2DBL(max_y));
    return self;
}

static VALUE
rg_set_vector(VALUE self, VALUE length, VALUE vector)
{
    gint len = NUM2INT(length);
    gfloat *c_vec;
    gint i;

    c_vec = ALLOCA_N(gfloat, len);
    for (i = 0; i < len; i++)
      c_vec[i] = NUM2DBL(RARRAY_PTR(vector)[i]);

    gtk_curve_set_vector(GTK_CURVE(RVAL2GOBJ(self)), len, c_vec);

    return self;
}

static VALUE
rg_get_vector(VALUE self, VALUE length)
{
    gint len = NUM2INT(length);
    gfloat *c_vec;
    gint i;
    VALUE vector = rb_ary_new2(len);

    c_vec = ALLOCA_N(gfloat, len);
    gtk_curve_get_vector(GTK_CURVE(RVAL2GOBJ(self)), len, c_vec);
    for (i = 0; i < len; i++)
      rb_ary_push(vector, rb_float_new(c_vec[i]));

    return vector;
}

void 
Init_gtk_curve(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_CURVE, "Curve", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(reset, 0);
    RG_DEF_METHOD(set_gamma, 1);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "gamma");
    RG_DEF_METHOD(set_range, 4);
    RG_DEF_METHOD(set_vector, 2);
    G_DEF_SETTER(RG_TARGET_NAMESPACE, "vector");
    /* NOTE: For backwards compatability. */
    RG_DEF_METHOD(get_vector, 1);
    RG_DEF_ALIAS("vector", "get_vector");

    /* GtkCurveType(from standard constants) */

    G_DEF_CLASS(GTK_TYPE_CURVE_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GTK_TYPE_CURVE_TYPE, "GTK_CURVE_");
}
