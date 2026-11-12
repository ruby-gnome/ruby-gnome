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

#define RG_TARGET_NAMESPACE rb_cGIPropertyInfo
#define SELF(self) (RVAL2GI_PROPERTY_INFO(self))

static VALUE
rg_flags(VALUE self)
{
    GIPropertyInfo *info = SELF(self);
    return UINT2NUM(gi_property_info_get_flags(info));
}

static VALUE
rg_type(VALUE self)
{
    GIPropertyInfo *info = SELF(self);
    return GOBJ2RVAL_UNREF(gi_property_info_get_type_info(info));
}

static VALUE
rg_ownership_transfer(VALUE self)
{
    GIPropertyInfo *info = SELF(self);
    return GI_TRANSFER2RVAL(gi_property_info_get_ownership_transfer(info));
}

static VALUE
rg_getter(VALUE self)
{
    GIPropertyInfo *info = SELF(self);
    return GOBJ2RVAL_UNREF(gi_property_info_get_getter(info));
}

static VALUE
rg_setter(VALUE self)
{
    GIPropertyInfo *info = SELF(self);
    return GOBJ2RVAL_UNREF(gi_property_info_get_setter(info));
}

void
rbgi_property_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GI_TYPE_PROPERTY_INFO, "PropertyInfo", rb_mGLib);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(ownership_transfer, 0);
    RG_DEF_METHOD(getter, 0);
    RG_DEF_METHOD(setter, 0);
}
#endif
