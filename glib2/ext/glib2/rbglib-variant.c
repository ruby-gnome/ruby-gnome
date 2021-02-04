/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

#define _SELF(s) (RTYPEDDATA_DATA(s))

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
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_BYTE)) {
        return UINT2NUM(g_variant_get_byte(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_INT16)) {
        return INT2NUM(g_variant_get_int16(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_UINT16)) {
        return UINT2NUM(g_variant_get_uint16(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_INT32)) {
        return INT2NUM(g_variant_get_int32(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_UINT32)) {
        return UINT2NUM(g_variant_get_uint32(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_INT64)) {
        return rbglib_int64_to_num(g_variant_get_int64(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_UINT64)) {
        return rbglib_uint64_to_num(g_variant_get_uint64(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_DOUBLE)) {
        return rb_float_new(g_variant_get_double(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_STRING) ||
               g_variant_type_equal(type, G_VARIANT_TYPE_OBJECT_PATH) ||
               g_variant_type_equal(type, G_VARIANT_TYPE_SIGNATURE)) {
        const gchar *string;
        gsize string_length;
        string = g_variant_get_string(variant, &string_length);
        return CSTR2RVAL_LEN(string, string_length);
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_HANDLE)) {
        return INT2NUM(g_variant_get_handle(variant));
    } else if (g_variant_type_equal(type, G_VARIANT_TYPE_VARIANT)) {
        GVariant *val = g_variant_get_variant(variant);
        VALUE value = rbg_variant_to_ruby(val);
        g_variant_unref(val);
        return value;
    } else if (g_variant_type_is_array(type)) {
        gsize i, len = g_variant_n_children(variant);
        VALUE ary = rb_ary_new2(len);
        for (i = 0; i < len; i++) {
            GVariant *val = g_variant_get_child_value(variant, i);
            rb_ary_store(ary, i, rbg_variant_to_ruby(val));
            g_variant_unref(val);
        }
        return ary;
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

static GVariant *
rg_ruby_to_variant(VALUE rb_value, VALUE rb_variant_type)
{
    const GVariantType *variant_type;

    if (NIL_P(rb_variant_type)) {
        switch (rb_type(rb_value)) {
          case RUBY_T_TRUE:
          case RUBY_T_FALSE:
            variant_type = G_VARIANT_TYPE_BOOLEAN;
            break;
          case RUBY_T_FIXNUM:
            variant_type = G_VARIANT_TYPE_INT64;
            break;
          case RUBY_T_FLOAT:
            variant_type = G_VARIANT_TYPE_DOUBLE;
            break;
          case RUBY_T_STRING:
            variant_type = G_VARIANT_TYPE_STRING;
            break;
          case RUBY_T_ARRAY:
            variant_type = G_VARIANT_TYPE_ARRAY;
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
        return g_variant_new_boolean(RVAL2CBOOL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_BYTE)) {
        return g_variant_new_byte(NUM2UINT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_INT16)) {
        return g_variant_new_int16(NUM2INT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_UINT16)) {
        return g_variant_new_uint16(NUM2UINT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_INT32)) {
        return g_variant_new_int32(NUM2INT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_UINT32)) {
        return g_variant_new_uint32(NUM2UINT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_INT64)) {
        return g_variant_new_int64(NUM2LONG(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_UINT64)) {
        return g_variant_new_uint64(NUM2ULONG(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_HANDLE)) {
        return g_variant_new_handle(NUM2INT(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_DOUBLE)) {
        return g_variant_new_double(NUM2DBL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_STRING)) {
        return g_variant_new_string(RVAL2CSTR_ACCEPT_NIL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_OBJECT_PATH)) {
        return g_variant_new_object_path(RVAL2CSTR_ACCEPT_NIL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_SIGNATURE)) {
        return g_variant_new_signature(RVAL2CSTR_ACCEPT_NIL(rb_value));
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_VARIANT)) {
        return g_variant_new_variant(rbg_variant_from_ruby(rb_value));
    } else if (g_variant_type_equal(variant_type,
                                    G_VARIANT_TYPE_STRING_ARRAY)) {
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
        return g_variant_new_strv(strings, length);
    } else if (g_variant_type_equal(variant_type,
                                    G_VARIANT_TYPE_OBJECT_PATH_ARRAY)) {
        const gchar **paths;
        gssize length;
        if (NIL_P(rb_value)) {
            paths = NULL;
            length = 0;
        } else {
            gssize i;

            length = RARRAY_LEN(rb_value);
            paths = ALLOCA_N(const gchar *, length);
            for (i = 0; i < length; i++) {
                VALUE rb_path = RARRAY_PTR(rb_value)[i];
                paths[i] = RVAL2CSTR_ACCEPT_NIL(rb_path);
            }
        }
        return g_variant_new_objv(paths, length);
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_ARRAY)) {
        int i;
        GVariantBuilder builder;
        g_variant_builder_init(&builder, G_VARIANT_TYPE_ARRAY);
        for (i = 0; i < RARRAY_LEN(rb_value); i++) {
            GVariant *val = rg_ruby_to_variant(rb_ary_entry(rb_value, i), Qnil);
            g_variant_builder_add_value(&builder, val);
        }
        return g_variant_builder_end(&builder);
    } else if (g_variant_type_equal(variant_type, G_VARIANT_TYPE_BYTESTRING)) {
        return g_variant_new_bytestring(RVAL2CSTR_RAW_ACCEPT_NIL(rb_value));
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
        return g_variant_new_bytestring_array(strings, length);
    } else {
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GVariant(%.*s): %s",
                 (int)g_variant_type_get_string_length(variant_type),
                 g_variant_type_peek_string(variant_type),
                 RBG_INSPECT(rb_value));
    }
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GVariant *variant;
    VALUE rb_value;
    VALUE rb_variant_type;

    rb_scan_args(argc, argv, "11", &rb_value, &rb_variant_type);
    variant = rg_ruby_to_variant(rb_value, rb_variant_type);
    g_variant_ref_sink(variant);
    RTYPEDDATA_DATA(self) = variant;

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
