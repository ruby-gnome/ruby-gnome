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

#define RG_TARGET_NAMESPACE rb_cGIRegisteredTypeInfo
#define SELF(self) (RVAL2GI_REGISTERED_TYPE_INFO(self))

static VALUE
rg_type_name(VALUE self)
{
    GIRegisteredTypeInfo *info = SELF(self);
    return CSTR2RVAL(gi_registered_type_info_get_type_name(info));
}

static VALUE
rg_type_init_function_name(VALUE self)
{
    GIRegisteredTypeInfo *info = SELF(self);
    return CSTR2RVAL(gi_registered_type_info_get_type_init_function_name(info));
}

static VALUE
rg_gtype(VALUE self)
{
    GIRegisteredTypeInfo *info = SELF(self);
    return rbgobj_gtype_new(gi_registered_type_info_get_g_type(info));
}

static VALUE
rg_boxed_p(VALUE self)
{
    GIRegisteredTypeInfo *info = SELF(self);
    return CBOOL2RVAL(gi_registered_type_info_is_boxed(info));
}

void
rbgi_registered_type_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GI_TYPE_REGISTERED_TYPE_INFO,
                    "RegisteredTypeInfo",
                    rb_mGLib);

    RG_DEF_METHOD(type_name, 0);
    RG_DEF_METHOD(type_init_function_name, 0);
    RG_DEF_METHOD(gtype, 0);
    RG_DEF_METHOD_P(boxed, 0);
}
#endif
