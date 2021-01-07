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

#define RG_TARGET_NAMESPACE cVariantType

#define _SELF(s) (RVAL2GVARIANTTYPE(s))

static VALUE RG_TARGET_NAMESPACE;

GVariantType *
rbg_variant_type_from_ruby(VALUE rb_variant_type)
{
    if (NIL_P(rb_variant_type)) {
        return NULL;
    }

    if (RB_TYPE_P(rb_variant_type, RUBY_T_STRING)) {
        rb_variant_type = rb_funcall(RG_TARGET_NAMESPACE,
                                     rb_intern("new"),
                                     1,
                                     rb_variant_type);
    }

    return RVAL2BOXED(rb_variant_type, G_TYPE_VARIANT_TYPE);
}

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

static VALUE
rg_maybe_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_maybe(variant_type));
}

static VALUE
rg_array_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_array(variant_type));
}

static VALUE
rg_tuple_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_tuple(variant_type));
}

static VALUE
rg_dict_entry_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_dict_entry(variant_type));
}

static VALUE
rg_variant_p(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return CBOOL2RVAL(g_variant_type_is_variant(variant_type));
}

static VALUE
rg_operator_eq(VALUE self, VALUE other)
{
    GVariantType *variant_type1;
    GVariantType *variant_type2;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(other, RG_TARGET_NAMESPACE)))
        return Qfalse;

    variant_type1 = _SELF(self);
    variant_type2 = _SELF(other);
    return CBOOL2RVAL(g_variant_type_equal(variant_type1, variant_type2));
}

static VALUE
rg_hash(VALUE self)
{
    GVariantType *variant_type;

    variant_type = _SELF(self);
    return UINT2NUM(g_variant_type_hash(variant_type));
}

static VALUE
rg_is_subtype_of_p(VALUE self, VALUE rb_subtype)
{
    GVariantType *variant_type;
    GVariantType *sub_variant_type;

    if (!RVAL2CBOOL(rb_obj_is_kind_of(rb_subtype, RG_TARGET_NAMESPACE)))
        return Qfalse;

    variant_type = _SELF(self);
    sub_variant_type = _SELF(rb_subtype);
    return CBOOL2RVAL(g_variant_type_is_subtype_of(variant_type,
                                                   sub_variant_type));
}

static VALUE
rg_element(VALUE self)
{
    GVariantType *variant_type;
    const GVariantType *element;

    variant_type = _SELF(self);
    if (!(g_variant_type_is_array(variant_type) ||
          g_variant_type_is_maybe(variant_type))) {
        rb_raise(rb_eArgError,
                 "must be array or maybe type: <%.*s>",
                 (int)g_variant_type_get_string_length(variant_type),
                 g_variant_type_peek_string(variant_type));
    }

    element = g_variant_type_element(variant_type);
    return GVARIANTTYPE2RVAL((GVariantType *)element);
}

void
Init_glib_variant_type(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_VARIANT_TYPE, "VariantType", mGLib);

    RG_DEF_SMETHOD_P(valid, 1);
    RG_DEF_SMETHOD(scan, 1);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(to_s, 0);

    RG_DEF_METHOD_P(definite, 0);
    RG_DEF_METHOD_P(container, 0);
    RG_DEF_METHOD_P(basic, 0);
    RG_DEF_METHOD_P(maybe, 0);
    RG_DEF_METHOD_P(array, 0);
    RG_DEF_METHOD_P(tuple, 0);
    RG_DEF_METHOD_P(dict_entry, 0);
    RG_DEF_METHOD_P(variant, 0);

    RG_DEF_METHOD_OPERATOR("==", eq, 1);

    RG_DEF_METHOD(hash, 0);
    RG_DEF_ALIAS("eql?", "==");

    RG_DEF_METHOD_P(is_subtype_of, 1);

    RG_DEF_METHOD(element, 0);

    {
        ID id_new;

        CONST_ID(id_new, "new");

#define DEF_TYPE(name) do {                                             \
            const GVariantType *type = G_VARIANT_TYPE_ ## name;         \
            const gchar *type_string;                                   \
            gsize type_string_length;                                   \
            VALUE rb_type_string;                                       \
                                                                        \
            type_string = g_variant_type_peek_string(type);             \
            type_string_length = g_variant_type_get_string_length(type); \
            rb_type_string = rb_str_new(type_string,                    \
                                        type_string_length);            \
            rb_define_const(RG_TARGET_NAMESPACE, G_STRINGIFY(name),     \
                            rb_funcall(RG_TARGET_NAMESPACE, id_new, 1,  \
                                       rb_type_string));                \
        } while (0)

        DEF_TYPE(BOOLEAN);
        DEF_TYPE(BYTE);
        DEF_TYPE(INT16);
        DEF_TYPE(UINT16);
        DEF_TYPE(INT32);
        DEF_TYPE(UINT32);
        DEF_TYPE(INT64);
        DEF_TYPE(UINT64);
        DEF_TYPE(HANDLE);
        DEF_TYPE(DOUBLE);
        DEF_TYPE(STRING);
        DEF_TYPE(OBJECT_PATH);
        DEF_TYPE(SIGNATURE);
        DEF_TYPE(VARIANT);
        DEF_TYPE(ANY);
        DEF_TYPE(BASIC);
        DEF_TYPE(MAYBE);
        DEF_TYPE(ARRAY);
        DEF_TYPE(TUPLE);
        DEF_TYPE(UNIT);
        DEF_TYPE(DICT_ENTRY);
        DEF_TYPE(DICTIONARY);
        DEF_TYPE(STRING_ARRAY);
        DEF_TYPE(OBJECT_PATH_ARRAY);
        DEF_TYPE(BYTESTRING);
        DEF_TYPE(BYTESTRING_ARRAY);
        DEF_TYPE(VARDICT);

#undef DEF_TYPE
    }
}
