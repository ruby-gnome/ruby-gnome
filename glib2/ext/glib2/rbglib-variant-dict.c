/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2024  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE cVariantDict

#define _SELF(s) (RVAL2GVARIANTDICT(s))

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_variant;
    rb_scan_args(argc, argv, "01", &rb_variant);
    GVariant *variant = NULL;
    if (!RB_NIL_P(rb_variant)) {
        variant = RVAL2GVARIANT(rb_variant);
    }
    GVariantDict *dict = g_variant_dict_new(variant);
    G_INITIALIZE(self, dict);

    return Qnil;
}

static VALUE
rg_insert(VALUE self, VALUE rb_key, VALUE rb_value)
{
    GVariantDict *dict = _SELF(self);
    const gchar *key = RVAL2CSTR(rb_key);
    GVariant *value = RVAL2GVARIANT(rb_value);
    g_variant_dict_insert_value(dict, key, value);
    return Qnil;
}

static VALUE
rg_lookup(VALUE self, VALUE rb_key, VALUE rb_expected_type)
{
    GVariantDict *dict = _SELF(self);
    const gchar *key = RVAL2CSTR(rb_key);
    const GVariantType *expected_type = RVAL2GVARIANTTYPE(rb_expected_type);
    GVariant *value = g_variant_dict_lookup_value(dict, key, expected_type);
    return GVARIANT2RVAL(value);
}

static VALUE
rg_contains_p(VALUE self, VALUE rb_key)
{
    GVariantDict *dict = _SELF(self);
    const gchar *key = RVAL2CSTR(rb_key);
    gboolean contains = g_variant_dict_contains(dict, key);
    return CBOOL2RVAL(contains);
}

void
Init_glib_variant_dict(void)
{
    RG_TARGET_NAMESPACE =
        G_DEF_CLASS(G_TYPE_VARIANT_DICT, "VariantDict", rbg_mGLib());

    RG_DEF_METHOD(initialize, -1);

    RG_DEF_METHOD(insert, 2);
    RG_DEF_METHOD(lookup, 2);
    RG_DEF_METHOD_P(contains, 1);
}
