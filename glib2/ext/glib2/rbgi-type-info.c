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

#define RG_TARGET_NAMESPACE rb_cGITypeInfo
#define SELF(self) (RVAL2GI_TYPE_INFO(self))

static VALUE
rg_pointer_p(VALUE self)
{
    GITypeInfo *info = SELF(self);
    return CBOOL2RVAL(gi_type_info_is_pointer(info));
}

static VALUE
rg_tag(VALUE self)
{
    GITypeInfo *info = SELF(self);
    return GI_TYPE_TAG2RVAL(gi_type_info_get_tag(info));
}

static VALUE
rg_get_param_type(VALUE self, VALUE rb_n)
{
    GITypeInfo *info = SELF(self);
    gint n = NUM2INT(rb_n);
    return GOBJ2RVAL_UNREF(gi_type_info_get_param_type(info, n));
}

static VALUE
rg_interface(VALUE self)
{
    GITypeInfo *info = SELF(self);
    return GOBJ2RVAL_UNREF(gi_type_info_get_interface(info));
}

static VALUE
rg_array_length_index(VALUE self)
{
    GITypeInfo *info = SELF(self);
    unsigned int length;
    if (gi_type_info_get_array_length_index(info, &length)) {
        return INT2NUM(length);
    } else {
        return Qnil;
    }
}

static VALUE
rg_array_fixed_size(VALUE self)
{
    GITypeInfo *info = SELF(self);
    size_t size;
    if (gi_type_info_get_array_fixed_size(info, &size)) {
        return SIZET2NUM(size);
    } else {
        return Qnil;
    }
}

static VALUE
rg_zero_terminated_p(VALUE self)
{
    GITypeInfo *info = SELF(self);
    return CBOOL2RVAL(gi_type_info_is_zero_terminated(info));
}

static VALUE
rg_array_type(VALUE self)
{
    GITypeInfo *info = SELF(self);
    return GI_ARRAY_TYPE2RVAL(gi_type_info_get_array_type(info));
}

void
rbgi_type_info_init(VALUE rb_mGLib)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS(GI_TYPE_TYPE_INFO, "TypeInfo", rb_mGLib);

    RG_DEF_METHOD_P(pointer, 0);
    RG_DEF_METHOD(tag, 0);
    RG_DEF_METHOD(get_param_type, 1);
    RG_DEF_METHOD(interface, 0);
    RG_DEF_METHOD(array_length_index, 0);
    RG_DEF_METHOD(array_fixed_size, 0);
    RG_DEF_METHOD_P(zero_terminated, 0);
    RG_DEF_METHOD(array_type, 0);

    rb_undef_method(RG_TARGET_NAMESPACE, "name");
}
#endif
