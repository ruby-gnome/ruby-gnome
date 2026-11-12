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

#define RG_TARGET_NAMESPACE rb_cGIConstantInfo
#define SELF(self) (RVAL2GI_CONSTANT_INFO(self))

static VALUE
rg_type(VALUE self)
{
    GIConstantInfo *info = SELF(self);
    return GOBJ2RVAL_UNREF(gi_constant_info_get_type_info(info));
}

void
rbgi_constant_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS(GI_TYPE_CONSTANT_INFO, "ConstantInfo", rb_mGLib);

    RG_DEF_METHOD(type, 0);
    /* TODO: This needs GIArgument -> Ruby conversion. */
    /* RG_DEF_METHOD(value, 0); */
}
#endif
