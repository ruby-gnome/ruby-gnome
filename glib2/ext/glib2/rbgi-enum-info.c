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

#define RG_TARGET_NAMESPACE rb_cGIEnumInfo
#define SELF(self) (RVAL2GI_ENUM_INFO(self))

static VALUE
rg_n_values(VALUE self)
{
    GIEnumInfo *info = SELF(self);
    return UINT2NUM(gi_enum_info_get_n_values(info));
}

static VALUE
rg_get_value(VALUE self, VALUE rb_n)
{
    GIEnumInfo *info = SELF(self);
    unsigned int n = NUM2UINT(rb_n);
    return GOBJ2RVAL_UNREF(gi_enum_info_get_value(info, n));
}

static VALUE
rg_n_methods(VALUE self)
{
    GIEnumInfo *info = SELF(self);
    return UINT2NUM(gi_enum_info_get_n_methods(info));
}

static VALUE
rg_get_method(VALUE self, VALUE rb_n)
{
    GIEnumInfo *info = SELF(self);
    unsigned int n = NUM2UINT(rb_n);
    return GOBJ2RVAL_UNREF(gi_enum_info_get_method(info, n));
}

static VALUE
rg_storage_type(VALUE self)
{
    GIEnumInfo *info = SELF(self);
    return GI_TYPE_TAG2RVAL(gi_enum_info_get_storage_type(info));
}

static VALUE
rg_error_domain(VALUE self)
{
    GIEnumInfo *info = SELF(self);
    return CSTR2RVAL(gi_enum_info_get_error_domain(info));
}

void
rbgi_enum_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GI_TYPE_ENUM_INFO, "EnumInfo", rb_mGLib);

    RG_DEF_METHOD(n_values, 0);
    RG_DEF_METHOD(get_value, 1);
    RG_DEF_METHOD(n_methods, 0);
    RG_DEF_METHOD(get_method, 1);
    RG_DEF_METHOD(storage_type, 0);
    RG_DEF_METHOD(error_domain, 0);
}
#endif
