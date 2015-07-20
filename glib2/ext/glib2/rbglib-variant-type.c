/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

#include "rbgprivate.h"
#include "rbglib.h"

#define RG_TARGET_NAMESPACE mVariantType

#define _SELF(s) ((GVariantType *)RVAL2BOXED(s, G_TYPE_VARIANT_TYPE))

static VALUE
rg_s_valid_p(G_GNUC_UNUSED VALUE klass, VALUE rb_string)
{
    gboolean is_valid;

    is_valid = g_variant_type_string_is_valid(StringValueCStr(rb_string));

    return CBOOL2RVAL(is_valid);
}

static VALUE
rg_s_scan(G_GNUC_UNUSED VALUE klass, VALUE rb_string)
{
    gboolean found;
    const gchar *string;
    const gchar *end;

    string = StringValueCStr(rb_string);
    found = g_variant_type_string_scan(string, NULL, &end);

    if (!found) {
        return Qnil;
    }

    return CSTR2RVAL(end);
}

static VALUE
rg_initialize(VALUE self, VALUE rb_string)
{
    GVariantType *variant_type;
    const gchar *string;

    string = StringValueCStr(rb_string);
    if (!g_variant_type_string_is_valid(string)) {
        rb_raise(rb_eArgError,
                 "invalid type string: %s", rbg_inspect(rb_string));
    }

    variant_type = g_variant_type_new(string);
    G_INITIALIZE(self, variant_type);

    return Qnil;
}

static VALUE
rg_to_s(VALUE self)
{
    GVariantType *variant_type;
    const gchar *string;
    gsize string_length;

    variant_type = _SELF(self);
    string = g_variant_type_peek_string(variant_type);
    string_length = g_variant_type_get_string_length(variant_type);

    return CSTR2RVAL_LEN(string, string_length);
}

static VALUE
rg_definite_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_definite(variant_type));
}

static VALUE
rg_container_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_container(variant_type));
}

static VALUE
rg_basic_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_basic(variant_type));
}

void
Init_glib_variant_type(void)
{
    VALUE RG_TARGET_NAMESPACE =
        G_DEF_CLASS(G_TYPE_VARIANT_TYPE, "VariantType", mGLib);

    RG_DEF_SMETHOD_P(valid, 1);
    RG_DEF_SMETHOD(scan, 1);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(to_s, 0);
    RG_DEF_METHOD_P(definite, 0);
    RG_DEF_METHOD_P(container, 0);
    RG_DEF_METHOD_P(basic, 0);
}
