/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#include "rbatkprivate.h"

#define RG_TARGET_NAMESPACE mValue
#define _SELF(s) (RVAL2ATKVALUE(s))

static VALUE
rg_current(VALUE self)
{
    GValue gval = G_VALUE_INIT;
    atk_value_get_current_value(_SELF(self), &gval);

    return GVAL2RVAL(&gval);
}

static VALUE
rg_max(VALUE self)
{
    GValue gval = G_VALUE_INIT;
    atk_value_get_maximum_value(_SELF(self), &gval);

    return GVAL2RVAL(&gval);
}

static VALUE
rg_min(VALUE self)
{
    GValue gval = G_VALUE_INIT;
    atk_value_get_minimum_value(_SELF(self), &gval);

    return GVAL2RVAL(&gval);
}

static VALUE
rg_set_current(VALUE self, VALUE value)
{
    GValue gval = G_VALUE_INIT;
    g_value_init(&gval, RVAL2GTYPE(value));

    rbgobj_rvalue_to_gvalue(value, &gval);

    if (! atk_value_set_current_value(_SELF(self), &gval)){
        rb_raise(rb_eRuntimeError, "Can't set the current value.");
    }

    return self;
}

void
Init_atk_value(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_VALUE, "Value", mAtk);

    RG_DEF_METHOD(current, 0);
    RG_DEF_METHOD(max, 0);
    RG_DEF_METHOD(min, 0);
    RG_DEF_METHOD(set_current, 1);
}
