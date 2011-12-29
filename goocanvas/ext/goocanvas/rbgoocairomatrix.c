/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Vincent Isambart <vincent.isambart@gmail.com>
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

#include "rbgoocanvasprivate.h"

#define RG_TARGET_NAMESPACE cMatrix

static VALUE
rg_to_goo(VALUE self)
{
    GValue gval = {0,};
    VALUE result;

    g_value_init(&gval, GOO_TYPE_CAIRO_MATRIX);
    g_value_set_boxed(&gval, RVAL2CRMATRIX(self));

    result = rbgobj_gvalue_to_rvalue(&gval);
    g_value_unset(&gval);

    return result;
}

void
Init_goocairomatrix(VALUE mCairo)
{
    VALUE RG_TARGET_NAMESPACE = rb_const_get(mCairo, rb_intern("Matrix"));

    RG_DEF_METHOD(to_goo, 0);
}
