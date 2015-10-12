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
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_STRING)) {
        const gchar *string;
        gsize string_length;
        string = g_variant_get_string(variant, &string_length);
        return CSTR2RVAL_LEN(string, string_length);
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
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GVariant *variant;
    const GVariantType *variant_type;
    VALUE rb_value;
    VALUE rb_variant_type;

    rb_scan_args(argc, argv, "11", &rb_value, &rb_variant_type);

    if (NIL_P(rb_variant_type)) {
        switch (rb_type(rb_value)) {
          case RUBY_T_TRUE:
          case RUBY_T_FALSE:
            variant_type = G_VARIANT_TYPE_BOOLEAN;
            break;
          case RUBY_T_FIXNUM:
            variant_type = G_VARIANT_TYPE_INT32;
            break;
          case RUBY_T_FLOAT:
            variant_type = G_VARIANT_TYPE_DOUBLE;
            break;
          case RUBY_T_STRING:
            variant_type = G_VARIANT_TYPE_STRING;
            break;
          default:
            rb_raise(rb_eNotImpError,
                     "TODO: Ruby -> GVariantType: %s",
                     RBG_INSPECT(rb_value));
            break;
        }
    } else {
        variant_type = RVAL2GVARIANTTYPE(rb_variant_type);
    }

    if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_BOOLEAN)) {
        variant = g_variant_new_boolean(RVAL2CBOOL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_BYTE)) {
        variant = g_variant_new_byte(NUM2UINT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_INT16)) {
        variant = g_variant_new_int16(NUM2INT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_UINT16)) {
        variant = g_variant_new_uint16(NUM2UINT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_INT32)) {
        variant = g_variant_new_int32(NUM2INT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_UINT32)) {
        variant = g_variant_new_uint32(NUM2UINT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_INT64)) {
        variant = g_variant_new_int64(NUM2LONG(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_UINT64)) {
        variant = g_variant_new_uint64(NUM2ULONG(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_HANDLE)) {
        variant = g_variant_new_handle(NUM2INT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_DOUBLE)) {
        variant = g_variant_new_double(NUM2DBL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_STRING)) {
        variant = g_variant_new_string(RVAL2CSTR_ACCEPT_NIL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_OBJECT_PATH)) {
        variant = g_variant_new_object_path(RVAL2CSTR_ACCEPT_NIL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_SIGNATURE)) {
        variant = g_variant_new_signature(RVAL2CSTR_ACCEPT_NIL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_VARIANT)) {
        variant = g_variant_new_variant(rbg_variant_from_ruby(rb_value));
    } else if (g_variant_type_equal(variant_type,
                                    G_VARIANT_TYPE_STRING_ARRAY) ||
               g_variant_type_equal(variant_type,
                                    G_VARIANT_TYPE_OBJECT_PATH_ARRAY)) {
        const gchar **strings;
        gssize length;
        if (NIL_P(rb_value)) {
            strings = NULL;
            length = 0;
        } else {
            gssize i;

            length = RARRAY_LEN(rb_value);
            strings = ALLOCA_N(const gchar *, length);
            for (i = 0; i < length; i++) {
                VALUE rb_string = RARRAY_PTR(rb_value)[i];
                strings[i] = RVAL2CSTR_ACCEPT_NIL(rb_string);
            }
        }
        if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_STRING_ARRAY)) {
            variant = g_variant_new_strv(strings, length);
        } else {
            variant = g_variant_new_objv(strings, length);
        }
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_BYTESTRING)) {
        variant = g_variant_new_bytestring(RVAL2CSTR_RAW_ACCEPT_NIL(rb_value));
    } else if (g_variant_type_equal(variant_type,
                                    G_VARIANT_TYPE_BYTESTRING_ARRAY)) {
        const gchar **strings;
        gssize length;
        if (NIL_P(rb_value)) {
            strings = NULL;
            length = 0;
        } else {
            gssize i;

            length = RARRAY_LEN(rb_value);
            strings = ALLOCA_N(const gchar *, length);
            for (i = 0; i < length; i++) {
                VALUE rb_string = RARRAY_PTR(rb_value)[i];
                strings[i] = RVAL2CSTR_RAW_ACCEPT_NIL(rb_string);
            }
        }
        variant = g_variant_new_bytestring_array(strings, length);
    } else {
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GVariant(%.*s): %s",
                 (int)g_variant_type_get_string_length(variant_type),
                 g_variant_type_peek_string(variant_type),
                 RBG_INSPECT(rb_value));
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

static VALUE
rg_type(VALUE self)
{
    GVariant *variant;

    variant = _SELF(self);

    return GVARIANTTYPE2RVAL((GVariantType *)g_variant_get_type(variant));
}

void
Init_glib_variant(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_VARIANT, "Variant", mGLib);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rg_variant_allocate);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(value, 0);
    RG_DEF_METHOD(type, 0);
}
