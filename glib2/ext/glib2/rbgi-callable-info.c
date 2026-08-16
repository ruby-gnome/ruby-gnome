/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2026  Ruby-GNOME Project Team
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

#ifdef HAVE_GIREPOSITORY
#include "rbgi-private.h"

#define RG_TARGET_NAMESPACE rb_cGICallableInfo
#define SELF(self) RVAL2GI_CALLABLE_INFO(self)

static VALUE
rg_n_args(VALUE self)
{
    GICallableInfo *info = SELF(self);
    return INT2NUM(gi_callable_info_get_n_args(info));
}

static VALUE
rg_get_arg(VALUE self, VALUE rb_n)
{
    GICallableInfo *info = SELF(self);
    gint n = NUM2INT(rb_n);
    return GOBJ2RVAL_UNREF(gi_callable_info_get_arg(info, n));
}

void
rbgi_callable_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GI_TYPE_CALLABLE_INFO, "CallableInfo", rb_mGLib);

    /* TODO: More methods */
    RG_DEF_METHOD(n_args, 0);
    RG_DEF_METHOD(get_arg, 1);
}
#endif
