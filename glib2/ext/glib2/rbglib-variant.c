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

#define RG_TARGET_NAMESPACE cVariant

#define _SELF(s) (DATA_PTR(s))

static VALUE RG_TARGET_NAMESPACE;

VALUE
rbg_variant_to_ruby(GVariant *variant)
{
    const GVariantType *type;

    if (!variant) {
        return Qnil;
    }

    type = g_variant_get_type(variant);

    if (g_variant_type_equal(type, G_VARIANT_TYPE_BOOLEAN)) {
        return CBOOL2RVAL(g_variant_get_boolean(variant));
    } else {
        rb_raise(rb_eNotImpError,
                 "TODO: GVariant(%.*s) -> Ruby",
                 (int)g_variant_type_get_string_length(type),
                 g_variant_type_peek_string(type));
    }

    return Qnil;
}

GVariant *
rbg_variant_from_ruby(VALUE rb_variant)
{
    if (NIL_P(rb_variant)) {
        return NULL;
    } else {
        return _SELF(rb_variant);
    }
}

static void
rg_variant_free(gpointer object)
{
    GVariant *variant = object;

    g_variant_unref(variant);
}

static VALUE
rg_variant_allocate(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, rg_variant_free, NULL);
}

static VALUE
rg_initialize(VALUE self, VALUE rb_value)
{
    GVariant *variant;

    switch (rb_type(rb_value)) {
      case RUBY_T_STRING:
        variant = g_variant_new_take_string(g_strndup(RSTRING_PTR(rb_value),
                                                      RSTRING_LEN(rb_value)));
        break;
      default:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GVariant: %s",
                 RBG_INSPECT(rb_value));
        break;
    }
    g_variant_ref_sink(variant);
    DATA_PTR(self) = variant;

    return Qnil;
}

static VALUE
rg_value(VALUE self)
{
    GVariant *variant;

    variant = _SELF(self);

    return rbg_variant_to_ruby(variant);
}

void
Init_glib_variant(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_VARIANT, "Variant", mGLib);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rg_variant_allocate);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(value, 0);
}
