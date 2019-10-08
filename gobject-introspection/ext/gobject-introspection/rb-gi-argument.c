/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2019  Ruby-GNOME Project Team
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

#include "rb-gi-private.h"

static VALUE rb_cGLibBoxed = Qnil;
static VALUE rb_cGLibBytes = Qnil;
static VALUE rb_cGLibObject = Qnil;
static VALUE rb_cGLibValue = Qnil;

static void
array_c_to_ruby_sized_interface(gconstpointer *elements,
                                gint64 n_elements,
                                GITypeInfo *element_type_info,
                                VALUE rb_array)
{
    gint64 i;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;
    const char *interface_name;

    interface_info = g_type_info_get_interface(element_type_info);
    interface_type = g_base_info_get_type(interface_info);
    gtype = g_registered_type_info_get_g_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        interface_name = g_info_type_to_string(interface_type);
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array,
                            rb_gi_struct_info_to_ruby(interface_info,
                                                      (gpointer)elements[i],
                                                      TRUE));
            }
            g_base_info_unref(interface_info);
            g_base_info_unref(element_type_info);
        } else {
            /* TODO: Should we check caller_allocates? */
            gsize struct_size = g_struct_info_get_size(interface_info);
            for (i = 0; i < n_elements; i++) {
                gpointer element = ((gchar *)elements) + struct_size * i;
                rb_ary_push(rb_array, BOXED2RVAL(element, gtype));
            }
            g_base_info_unref(interface_info);
            g_base_info_unref(element_type_info);
        }
        break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
        interface_name = g_info_type_to_string(interface_type);
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_OBJECT:
        for (i = 0; i < n_elements; i++) {
            rb_ary_push(rb_array, GOBJ2RVAL((GObject *)(elements[i])));
        }
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        break;
    case GI_INFO_TYPE_INTERFACE:
    case GI_INFO_TYPE_CONSTANT:
    case GI_INFO_TYPE_INVALID_0:
    case GI_INFO_TYPE_UNION:
    case GI_INFO_TYPE_VALUE:
    case GI_INFO_TYPE_SIGNAL:
    case GI_INFO_TYPE_VFUNC:
    case GI_INFO_TYPE_PROPERTY:
    case GI_INFO_TYPE_FIELD:
    case GI_INFO_TYPE_ARG:
    case GI_INFO_TYPE_TYPE:
    case GI_INFO_TYPE_UNRESOLVED:
        interface_name = g_info_type_to_string(interface_type);
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    default:
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }
}

static void
array_c_to_ruby_sized(gconstpointer *elements,
                      gint64 n_elements,
                      GITypeInfo *type_info,
                      VALUE rb_array)
{
    gint64 i;
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_BOOLEAN:
        g_base_info_unref(element_type_info);
        {
            const gboolean *booleans = (const gboolean *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, CBOOL2RVAL(booleans[i]));
            }
        }
        break;
    case GI_TYPE_TAG_INT8:
        g_base_info_unref(element_type_info);
        {
            const gint8 *numbers = (const gint8 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, INT2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_UINT8:
        g_base_info_unref(element_type_info);
        {
            const guint8 *numbers = (const guint8 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, UINT2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_INT16:
        g_base_info_unref(element_type_info);
        {
            const gint16 *numbers = (const gint16 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, INT2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_UINT16:
        g_base_info_unref(element_type_info);
        {
            const guint16 *numbers = (const guint16 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, UINT2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_INT32:
        g_base_info_unref(element_type_info);
        {
            const gint32 *numbers = (const gint32 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, INT2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_UINT32:
        g_base_info_unref(element_type_info);
        {
            const guint32 *numbers = (const guint32 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, UINT2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_INT64:
        g_base_info_unref(element_type_info);
        {
            const gint64 *numbers = (const gint64 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, LL2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_UINT64:
        g_base_info_unref(element_type_info);
        {
            const guint64 *numbers = (const guint64 *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, ULL2NUM(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_FLOAT:
        g_base_info_unref(element_type_info);
        {
            const gfloat *numbers = (const gfloat *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, rb_float_new(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_DOUBLE:
        g_base_info_unref(element_type_info);
        {
            const gdouble *numbers = (const gdouble *)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, rb_float_new(numbers[i]));
            }
        }
        break;
    case GI_TYPE_TAG_GTYPE:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_UTF8:
        g_base_info_unref(element_type_info);
        {
            const gchar **strings = (const gchar **)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, CSTR2RVAL(strings[i]));
            }
        }
        break;
    case GI_TYPE_TAG_FILENAME:
        g_base_info_unref(element_type_info);
        {
            const gchar **filenames = (const gchar **)elements;
            for (i = 0; i < n_elements; i++) {
                rb_ary_push(rb_array, CSTRFILENAME2RVAL(filenames[i]));
            }
        }
        break;
    case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        array_c_to_ruby_sized_interface(elements, n_elements, element_type_info,
                                        rb_array);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c][%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }
}

static void
array_c_to_ruby(GIArgument *array, GITypeInfo *type_info, gint64 n_elements,
                VALUE rb_array)
{
    gconstpointer *elements;
    gint fixed_size;
    gboolean zero_terminated_p;

    elements = array->v_pointer;
    if (!elements) {
        return;
    }

    fixed_size = g_type_info_get_array_fixed_size(type_info);
    zero_terminated_p = g_type_info_is_zero_terminated(type_info);

    if (n_elements != -1) {
        array_c_to_ruby_sized(elements, n_elements, type_info, rb_array);
    } else if (zero_terminated_p) {
        const gchar **strings = (const gchar **)elements;
        for (; *strings; strings++) {
            rb_ary_push(rb_array, CSTR2RVAL(*strings));
        }
    } else {
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c] -> Ruby: "
                 "zero-terminated: %s "
                 "fixed-size: %d "
                 "length: %" G_GINT64_FORMAT,
                 zero_terminated_p ? "true" : "false",
                 fixed_size,
                 n_elements);
    }
}

static void
array_array_interface_to_ruby(GIArgument *array,
                              G_GNUC_UNUSED GITypeInfo *array_type_info,
                              GITypeInfo *element_type_info,
                              VALUE rb_array)
{
    GArray *elements;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;
    const char *interface_name;

    elements = array->v_pointer;

    interface_info = g_type_info_get_interface(element_type_info);
    interface_type = g_base_info_get_type(interface_info);
    gtype = g_registered_type_info_get_g_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        interface_name = g_info_type_to_string(interface_type);
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[array][interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            guint i;
            guint element_size;

            element_size = g_array_get_element_size(elements);
            for (i = 0; i < elements->len; i++) {
                gpointer element;
                element = elements->data + (element_size * i);
                rb_ary_push(rb_array,
                            rb_gi_struct_info_to_ruby(interface_info,
                                                      element,
                                                      FALSE));
            }
        } else {
            interface_name = g_info_type_to_string(interface_type);
            g_base_info_unref(interface_info);
            g_base_info_unref(element_type_info);
            rb_raise(rb_eNotImpError,
                     "TODO: GIArgument(array)[array][interface(%s)](%s) -> Ruby",
                     interface_name,
                     g_type_name(gtype));
        }
        break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
    case GI_INFO_TYPE_CONSTANT:
    case GI_INFO_TYPE_INVALID_0:
    case GI_INFO_TYPE_UNION:
    case GI_INFO_TYPE_VALUE:
    case GI_INFO_TYPE_SIGNAL:
    case GI_INFO_TYPE_VFUNC:
    case GI_INFO_TYPE_PROPERTY:
    case GI_INFO_TYPE_FIELD:
    case GI_INFO_TYPE_ARG:
    case GI_INFO_TYPE_TYPE:
    case GI_INFO_TYPE_UNRESOLVED:
        interface_name = g_info_type_to_string(interface_type);
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[array][interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    default:
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(interface_info);
}

static void
array_array_to_ruby(GIArgument *array,
                    GITypeInfo *array_type_info,
                    VALUE rb_array)
{
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;
    GArray *elements;

    elements = array->v_pointer;
    if (!elements) {
        return;
    }

    element_type_info = g_type_info_get_param_type(array_type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
    case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[array][%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        array_array_interface_to_ruby(array,
                                      array_type_info,
                                      element_type_info,
                                      rb_array);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[array][%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(element_type_info);
}

static gint64
get_array_length(GIArgument *argument, GITypeInfo *type_info)
{
    GITypeTag type_tag;
    gint64 length = -1;

    if (!argument) {
        return length;
    }

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid array length argument?: <%s>",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_INT8:
        length = argument->v_int8;
        break;
    case GI_TYPE_TAG_UINT8:
        length = argument->v_uint8;
        break;
    case GI_TYPE_TAG_INT16:
        length = argument->v_int16;
        break;
    case GI_TYPE_TAG_UINT16:
        length = argument->v_uint16;
        break;
    case GI_TYPE_TAG_INT32:
        length = argument->v_int32;
        break;
    case GI_TYPE_TAG_UINT32:
        length = argument->v_uint32;
        break;
    case GI_TYPE_TAG_INT64:
        length = argument->v_int64;
        break;
    case GI_TYPE_TAG_UINT64:
        length = argument->v_uint64;
        break;
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
    case GI_TYPE_TAG_ARRAY:
    case GI_TYPE_TAG_INTERFACE:
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid array length argument?: <%s>",
                 g_type_tag_to_string(type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }

    return length;
}

static VALUE
rb_gi_array_argument_to_ruby(GIArgument *array_argument,
                             GIArgument *length_argument,
                             GITypeInfo *array_type_info,
                             GITypeInfo *length_type_info)
{
    VALUE rb_array;
    GIArrayType array_type;
    gint64 n_elements;

    array_type = g_type_info_get_array_type(array_type_info);
    n_elements = get_array_length(length_argument, length_type_info);
    if (n_elements == -1) {
        rb_array = rb_ary_new();
    } else {
        rb_array = rb_ary_new2(n_elements);
    }
    switch (array_type) {
    case GI_ARRAY_TYPE_C:
        array_c_to_ruby(array_argument, array_type_info, n_elements, rb_array);
        break;
    case GI_ARRAY_TYPE_ARRAY:
        array_array_to_ruby(array_argument, array_type_info, rb_array);
        break;
    case GI_ARRAY_TYPE_PTR_ARRAY:
        rb_raise(rb_eNotImpError, "TODO: GIArgument(array)[ptr-array] -> Ruby");
        break;
    case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError, "TODO: GIArgument(array)[byte-array] -> Ruby");
        break;
    default:
        g_assert_not_reached();
        break;
    }

    return rb_array;
}

static VALUE
rb_gi_argument_to_ruby_interface(GIArgument *argument,
                                 gboolean duplicate,
                                 GITypeInfo *type_info)
{
    VALUE rb_interface;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    gtype = g_registered_type_info_get_g_type(interface_info);
    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[invalid] -> Ruby");
        break;
    case GI_INFO_TYPE_FUNCTION:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[function] -> Ruby");
        break;
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[callback] -> Ruby");
        break;
    case GI_INFO_TYPE_STRUCT:
        rb_interface = rb_gi_struct_info_to_ruby(interface_info,
                                                 argument->v_pointer,
                                                 !duplicate);
        break;
    case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[boxed] -> Ruby");
        break;
    case GI_INFO_TYPE_ENUM:
        if (gtype == G_TYPE_NONE) {
            rb_interface = INT2NUM(argument->v_int32);
        } else {
            rb_interface = GENUM2RVAL(argument->v_int32, gtype);
        }
        break;
    case GI_INFO_TYPE_FLAGS:
        if (gtype == G_TYPE_NONE) {
            rb_interface = INT2NUM(argument->v_int32);
        } else {
            rb_interface = GFLAGS2RVAL(argument->v_int32, gtype);
        }
        break;
    case GI_INFO_TYPE_OBJECT:
        rb_interface = GOBJ2RVAL(argument->v_pointer);
        break;
    case GI_INFO_TYPE_INTERFACE:
        rb_interface = GOBJ2RVAL(argument->v_pointer);
        break;
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[constant] -> Ruby");
        break;
    case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
    case GI_INFO_TYPE_UNION:
        rb_interface = BOXED2RVAL(argument->v_pointer, gtype);
        break;
    case GI_INFO_TYPE_VALUE:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[value] -> Ruby");
        break;
    case GI_INFO_TYPE_SIGNAL:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[signal] -> Ruby");
        break;
    case GI_INFO_TYPE_VFUNC:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[vfunc] -> Ruby");
        break;
    case GI_INFO_TYPE_PROPERTY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[property] -> Ruby");
        break;
    case GI_INFO_TYPE_FIELD:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[field] -> Ruby");
        break;
    case GI_INFO_TYPE_ARG:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[arg] -> Ruby");
        break;
    case GI_INFO_TYPE_TYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[type] -> Ruby");
        break;
    case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(interface)[unresolved] -> Ruby");
        break;
    default:
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(interface_info);

    return rb_interface;
}

static void
normalize_out_array_length(GIArgument *normalized_argument,
                           GIArgument *argument,
                           RBGIArgMetadata *metadata)
{
    switch (metadata->type.tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid out array length argument?: <%s>",
                 g_type_tag_to_string(metadata->type.tag));
        break;
    case GI_TYPE_TAG_INT8:
        normalized_argument->v_int8 = *((gint8 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_UINT8:
        normalized_argument->v_uint8 = *((guint8 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_INT16:
        normalized_argument->v_int16 = *((gint16 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_UINT16:
        normalized_argument->v_uint16 = *((guint16 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_INT32:
        normalized_argument->v_int32 = *((gint32 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_UINT32:
        normalized_argument->v_uint32 = *((guint32 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_INT64:
        normalized_argument->v_int64 = *((gint64 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_UINT64:
        normalized_argument->v_uint64 = *((guint64 *)argument->v_pointer);
        break;
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
    case GI_TYPE_TAG_ARRAY:
    case GI_TYPE_TAG_INTERFACE:
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid out array length argument?: <%s>",
                 g_type_tag_to_string(metadata->type.tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static VALUE
rb_gi_argument_to_ruby_array(GIArgument *array_argument,
                             GITypeInfo *array_type_info,
                             GArray *in_args,
                             GArray *out_args,
                             GPtrArray *args_metadata)
{
    VALUE rb_array;
    gint length_index;
    GIArgument *length_argument = NULL;
    GIArgument normalized_length_argument;
    GITypeInfo *length_type_info = NULL;

    length_index = g_type_info_get_array_length(array_type_info);
    if (length_index != -1) {
        RBGIArgMetadata *length_metadata;
        GIArgument *raw_length_argument = NULL;

        length_metadata = g_ptr_array_index(args_metadata, length_index);
        length_type_info = length_metadata->type.info;
        if (length_metadata->direction == GI_DIRECTION_OUT) {
            raw_length_argument = &g_array_index(out_args, GIArgument,
                                                 length_metadata->out_arg_index);
        } else if (length_metadata->direction == GI_DIRECTION_INOUT) {
            raw_length_argument = &g_array_index(in_args, GIArgument,
                                                 length_metadata->in_arg_index);
        }

        if (raw_length_argument) {
            if (length_metadata->array_metadata &&
                length_metadata->array_metadata->output_buffer_p) {
                length_argument = raw_length_argument;
            } else {
                normalize_out_array_length(&normalized_length_argument,
                                           raw_length_argument,
                                           length_metadata);
                length_argument = &normalized_length_argument;
            }
        } else {
            length_argument = &g_array_index(in_args, GIArgument,
                                             length_metadata->in_arg_index);
        }
    }

    rb_array = rb_gi_array_argument_to_ruby(array_argument,
                                            length_argument,
                                            array_type_info,
                                            length_type_info);

    return rb_array;
}

static VALUE
rb_gi_argument_to_ruby_glist_interface(GIArgument *argument,
                                       G_GNUC_UNUSED GITypeInfo *type_info,
                                       GITypeInfo *element_type_info)
{
    VALUE rb_argument = Qnil;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    const gchar *interface_name;
    GType gtype;

    interface_info = g_type_info_get_interface(element_type_info);
    interface_type = g_base_info_get_type(interface_info);
    interface_name = g_info_type_to_string(interface_type);
    gtype = g_registered_type_info_get_g_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            GList *node;
            rb_argument = rb_ary_new();
            for (node = argument->v_pointer; node; node = g_list_next(node)) {
                rb_ary_push(rb_argument,
                            rb_gi_struct_info_to_ruby(interface_info,
                                                      node->data,
                                                      TRUE));
            }
        } else {
            rb_argument = BOXEDGLIST2RVAL(argument->v_pointer, gtype);
        }
        break;
    case GI_INFO_TYPE_BOXED:
        rb_argument = BOXEDGLIST2RVAL(argument->v_pointer, gtype);
        break;
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
        rb_argument = GOBJGLIST2RVAL(argument->v_pointer);
        break;
    case GI_INFO_TYPE_CONSTANT:
    case GI_INFO_TYPE_INVALID_0:
    case GI_INFO_TYPE_UNION:
    case GI_INFO_TYPE_VALUE:
    case GI_INFO_TYPE_SIGNAL:
    case GI_INFO_TYPE_VFUNC:
    case GI_INFO_TYPE_PROPERTY:
    case GI_INFO_TYPE_FIELD:
    case GI_INFO_TYPE_ARG:
    case GI_INFO_TYPE_TYPE:
    case GI_INFO_TYPE_UNRESOLVED:
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    default:
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(interface_info);
    g_base_info_unref(element_type_info);

    return rb_argument;
}

static VALUE
rb_gi_argument_to_ruby_glist(GIArgument *argument, GITypeInfo *type_info)
{
    VALUE rb_argument;
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_UTF8:
        g_base_info_unref(element_type_info);
        rb_argument = CSTRGLIST2RVAL(argument->v_pointer);
        break;
    case GI_TYPE_TAG_FILENAME:
    case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_argument = rb_gi_argument_to_ruby_glist_interface(argument,
                                                             type_info,
                                                             element_type_info);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GList)[%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }

    return rb_argument;
}

static VALUE
rb_gi_argument_to_ruby_gslist_interface(GIArgument *argument,
                                        G_GNUC_UNUSED GITypeInfo *type_info,
                                        GITypeInfo *element_type_info)
{
    VALUE rb_argument = Qnil;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    const gchar *interface_name;
    GType gtype;

    interface_info = g_type_info_get_interface(element_type_info);
    interface_type = g_base_info_get_type(interface_info);
    interface_name = g_info_type_to_string(interface_type);
    gtype = g_registered_type_info_get_g_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            GSList *node;
            rb_argument = rb_ary_new();
            for (node = argument->v_pointer; node; node = g_slist_next(node)) {
                rb_ary_push(rb_argument,
                            rb_gi_struct_info_to_ruby(interface_info,
                                                      node->data,
                                                      TRUE));
            }
        } else {
            rb_argument = BOXEDGLIST2RVAL(argument->v_pointer, gtype);
        }
        break;
    case GI_INFO_TYPE_BOXED:
        rb_argument = BOXEDGLIST2RVAL(argument->v_pointer, gtype);
        break;
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
        rb_argument = GOBJGLIST2RVAL(argument->v_pointer);
        break;
    case GI_INFO_TYPE_CONSTANT:
    case GI_INFO_TYPE_INVALID_0:
    case GI_INFO_TYPE_UNION:
    case GI_INFO_TYPE_VALUE:
    case GI_INFO_TYPE_SIGNAL:
    case GI_INFO_TYPE_VFUNC:
    case GI_INFO_TYPE_PROPERTY:
    case GI_INFO_TYPE_FIELD:
    case GI_INFO_TYPE_ARG:
    case GI_INFO_TYPE_TYPE:
    case GI_INFO_TYPE_UNRESOLVED:
        g_base_info_unref(interface_info);
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[interface(%s)](%s) -> Ruby",
                 interface_name,
                 g_type_name(gtype));
        break;
    default:
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(interface_info);
    g_base_info_unref(element_type_info);

    return rb_argument;
}

static VALUE
rb_gi_argument_to_ruby_gslist(GIArgument *argument, GITypeInfo *type_info)
{
    VALUE rb_argument;
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_UTF8:
        g_base_info_unref(element_type_info);
        rb_argument = CSTRGSLIST2RVAL(argument->v_pointer);
        break;
    case GI_TYPE_TAG_FILENAME:
        g_base_info_unref(element_type_info);
        rb_argument = FILENAMEGSLIST2RVAL(argument->v_pointer);
        break;
    case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_argument = rb_gi_argument_to_ruby_gslist_interface(argument,
                                                              type_info,
                                                              element_type_info);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GSList)[%s] -> Ruby",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }

    return rb_argument;
}

typedef struct {
    GIArgument *argument;
    VALUE rb_table;
    GITypeInfo *key_type_info;
    GITypeTag key_type_tag;
    GITypeInfo *value_type_info;
    GITypeTag value_type_tag;
} GHashToRubyData;

static void
rb_gi_argument_to_ruby_ghash_foreach_body(gpointer key,
                                          gpointer value,
                                          gpointer user_data)
{
    GHashToRubyData *data = user_data;
    VALUE rb_key;
    VALUE rb_value;

    switch (data->key_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
      case GI_TYPE_TAG_INT16:
      case GI_TYPE_TAG_UINT16:
      case GI_TYPE_TAG_INT32:
      case GI_TYPE_TAG_UINT32:
      case GI_TYPE_TAG_INT64:
      case GI_TYPE_TAG_UINT64:
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_type_tag),
                 g_type_tag_to_string(data->value_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        rb_key = CSTR2RVAL(key);
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_type_tag),
                 g_type_tag_to_string(data->value_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            GIArgument key_argument;
            key_argument.v_pointer = key;
            rb_key = rb_gi_argument_to_ruby_interface(&key_argument,
                                                      FALSE,
                                                      data->key_type_info);
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_type_tag),
                 g_type_tag_to_string(data->value_type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    switch (data->value_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
      case GI_TYPE_TAG_INT16:
      case GI_TYPE_TAG_UINT16:
      case GI_TYPE_TAG_INT32:
      case GI_TYPE_TAG_UINT32:
      case GI_TYPE_TAG_INT64:
      case GI_TYPE_TAG_UINT64:
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_type_tag),
                 g_type_tag_to_string(data->value_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        rb_value = CSTR2RVAL(value);
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_type_tag),
                 g_type_tag_to_string(data->value_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            GIArgument value_argument;
            value_argument.v_pointer = value;
            rb_value = rb_gi_argument_to_ruby_interface(&value_argument,
                                                        FALSE,
                                                        data->value_type_info);
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(GHash)[%s][%s] -> Ruby",
                 g_type_tag_to_string(data->key_type_tag),
                 g_type_tag_to_string(data->value_type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    rb_hash_aset(data->rb_table, rb_key, rb_value);
}

static VALUE
rb_gi_argument_to_ruby_ghash_body(VALUE user_data)
{
    GHashToRubyData *data = (GHashToRubyData *)user_data;

    g_hash_table_foreach(data->argument->v_pointer,
                         rb_gi_argument_to_ruby_ghash_foreach_body,
                         data);

    return data->rb_table;
}

static VALUE
rb_gi_argument_to_ruby_ghash_ensure(VALUE user_data)
{
    GHashToRubyData *data = (GHashToRubyData *)user_data;

    g_base_info_unref(data->key_type_info);
    g_base_info_unref(data->value_type_info);

    return Qnil;
}

static VALUE
rb_gi_argument_to_ruby_ghash(GIArgument *argument, GITypeInfo *type_info)
{
    GHashToRubyData data;

    data.argument = argument;
    data.rb_table = rb_hash_new();

    data.key_type_info = g_type_info_get_param_type(type_info, 0);
    data.key_type_tag = g_type_info_get_tag(data.key_type_info);

    data.value_type_info = g_type_info_get_param_type(type_info, 1);
    data.value_type_tag = g_type_info_get_tag(data.value_type_info);

    return rb_ensure(rb_gi_argument_to_ruby_ghash_body, (VALUE)&data,
                     rb_gi_argument_to_ruby_ghash_ensure, (VALUE)&data);
}

static VALUE
rb_gi_argument_to_ruby_unichar(GIArgument *argument)
{
    VALUE rb_argument;
    gunichar ucs4_character;
    gchar *utf8_string;
    GError *error = NULL;

    ucs4_character = argument->v_uint32;
    utf8_string = g_ucs4_to_utf8(&ucs4_character, 1, NULL, NULL, &error);
    if (error) {
        RG_RAISE_ERROR(error);
    }
    rb_argument = CSTR2RVAL_FREE(utf8_string);

    return rb_argument;
}

VALUE
rb_gi_argument_to_ruby(GIArgument *argument,
                       gboolean duplicate,
                       GITypeInfo *type_info,
                       GArray *in_args,
                       GArray *out_args,
                       GPtrArray *args_metadata)
{
    VALUE rb_argument = Qnil;
    GITypeTag type_tag;

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
        if (g_type_info_is_pointer(type_info)) {
            rb_argument = ULONG2NUM(GPOINTER_TO_UINT(argument->v_pointer));
        } else {
            rb_argument = Qnil;
        }
        break;
    case GI_TYPE_TAG_BOOLEAN:
        rb_argument = CBOOL2RVAL(argument->v_boolean);
        break;
    case GI_TYPE_TAG_INT8:
        rb_argument = INT2NUM(argument->v_int8);
        break;
    case GI_TYPE_TAG_UINT8:
        rb_argument = UINT2NUM(argument->v_uint8);
        break;
    case GI_TYPE_TAG_INT16:
        rb_argument = INT2NUM(argument->v_int16);
        break;
    case GI_TYPE_TAG_UINT16:
        rb_argument = UINT2NUM(argument->v_uint16);
        break;
    case GI_TYPE_TAG_INT32:
        rb_argument = INT2NUM(argument->v_int32);
        break;
    case GI_TYPE_TAG_UINT32:
        rb_argument = UINT2NUM(argument->v_uint32);
        break;
    case GI_TYPE_TAG_INT64:
        rb_argument = LL2NUM(argument->v_int64);
        break;
    case GI_TYPE_TAG_UINT64:
        rb_argument = ULL2NUM(argument->v_uint64);
        break;
    case GI_TYPE_TAG_FLOAT:
        rb_argument = DBL2NUM(argument->v_float);
        break;
    case GI_TYPE_TAG_DOUBLE:
        rb_argument = DBL2NUM(argument->v_double);
        break;
    case GI_TYPE_TAG_GTYPE:
        if (argument->v_size == G_TYPE_INVALID) {
            rb_argument = Qnil;
        } else {
            rb_argument = rbgobj_gtype_new(argument->v_size);
        }
        break;
    case GI_TYPE_TAG_UTF8:
        rb_argument = CSTR2RVAL(argument->v_string);
        break;
    case GI_TYPE_TAG_FILENAME:
        /* TODO: set encoding */
        rb_argument = CSTR2RVAL(argument->v_string);
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_argument = rb_gi_argument_to_ruby_array(argument,
                                                   type_info,
                                                   in_args,
                                                   out_args,
                                                   args_metadata);
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_argument = rb_gi_argument_to_ruby_interface(argument,
                                                       duplicate,
                                                       type_info);
        break;
    case GI_TYPE_TAG_GLIST:
        rb_argument = rb_gi_argument_to_ruby_glist(argument, type_info);
        break;
    case GI_TYPE_TAG_GSLIST:
        rb_argument = rb_gi_argument_to_ruby_gslist(argument, type_info);
        break;
    case GI_TYPE_TAG_GHASH:
        rb_argument = rb_gi_argument_to_ruby_ghash(argument, type_info);
        break;
    case GI_TYPE_TAG_ERROR:
        rb_argument = GERROR2RVAL(argument->v_pointer);
        break;
    case GI_TYPE_TAG_UNICHAR:
        rb_argument = rb_gi_argument_to_ruby_unichar(argument);
        break;
    default:
        g_assert_not_reached();
        break;
    }

    return rb_argument;
}

static void
rb_gi_return_argument_free_container(GIArgument *argument,
                                     GITypeInfo *type_info)
{
    GITypeTag type_tag;

    type_tag = g_type_info_get_tag(type_info);

    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) as container",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_ARRAY:
        g_free(argument->v_pointer);
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) as container",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_GLIST:
        g_list_free(argument->v_pointer);
        break;
    case GI_TYPE_TAG_GSLIST:
        g_slist_free(argument->v_pointer);
        break;
    case GI_TYPE_TAG_GHASH:
        g_hash_table_unref(argument->v_pointer);
        break;
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) as container",
                 g_type_tag_to_string(type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_return_argument_free_everything_array_c(GIArgument *argument,
                                              GITypeInfo *type_info)
{
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);
    g_base_info_unref(element_type_info);

    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[c][%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_BOOLEAN:
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
        g_free(argument->v_pointer);
        break;
    case GI_TYPE_TAG_GTYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[c][%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_UTF8:
        g_strfreev(argument->v_pointer);
        break;
    case GI_TYPE_TAG_FILENAME:
        g_strfreev(argument->v_pointer);
        break;
    case GI_TYPE_TAG_ARRAY:
    case GI_TYPE_TAG_INTERFACE:
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[c][%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_return_argument_free_everything_array(GIArgument *argument,
                                            GITypeInfo *type_info)
{
    switch (g_type_info_get_array_type(type_info)) {
    case GI_ARRAY_TYPE_C:
        rb_gi_return_argument_free_everything_array_c(argument, type_info);
        break;
    case GI_ARRAY_TYPE_ARRAY:
        g_array_free(argument->v_pointer, TRUE);
        break;
    case GI_ARRAY_TYPE_PTR_ARRAY:
        g_ptr_array_free(argument->v_pointer, TRUE);
        break;
    case GI_ARRAY_TYPE_BYTE_ARRAY:
        g_ptr_array_free(argument->v_pointer, TRUE);
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_return_argument_free_everything_interface_struct(GIArgument *argument,
                                                       GType       gtype)
{
    if (!argument->v_pointer) {
        return;
    }

    if (!gtype) {
        xfree(argument->v_pointer);
    }

    if (G_TYPE_IS_BOXED(gtype)) {
        g_boxed_free(gtype, argument->v_pointer);
    } else {
        rbgobj_instance_unref(argument->v_pointer);
    }
}

static void
rb_gi_return_argument_free_everything_interface(GIArgument *argument,
                                                GITypeInfo *type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);
    gtype = g_registered_type_info_get_g_type(interface_info);
    g_base_info_unref(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[invalid] everything");
        break;
    case GI_INFO_TYPE_FUNCTION:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[function] everything");
        break;
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[callback] everything");
        break;
    case GI_INFO_TYPE_STRUCT:
        rb_gi_return_argument_free_everything_interface_struct(argument, gtype);
        break;
    case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[boxed] everything");
        break;
    case GI_INFO_TYPE_ENUM:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[enum] everything");
        break;
    case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[flags] everything");
        break;
    case GI_INFO_TYPE_OBJECT:
        if (argument->v_pointer) {
            GObject *object = argument->v_pointer;
            if (g_object_is_floating(object)) {
                g_object_ref_sink(object);
            }
            g_object_unref(object);
        }
        break;
    case GI_INFO_TYPE_INTERFACE:
        if (argument->v_pointer) {
            g_object_unref(argument->v_pointer);
        }
        break;
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[constant] everything");
        break;
    case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
    case GI_INFO_TYPE_UNION:
      if (gtype == G_TYPE_NONE) {
          rb_raise(rb_eNotImpError,
                   "TODO: free GIArgument(interface)[union] everything");
      } else {
          g_boxed_free(gtype, argument->v_pointer);
      }
      break;
    case GI_INFO_TYPE_VALUE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[value] everything");
        break;
    case GI_INFO_TYPE_SIGNAL:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[signal] everything");
        break;
    case GI_INFO_TYPE_VFUNC:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[vfunc] everything");
        break;
    case GI_INFO_TYPE_PROPERTY:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[property] everything");
        break;
    case GI_INFO_TYPE_FIELD:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[field] everything");
        break;
    case GI_INFO_TYPE_ARG:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[arg] everything");
        break;
    case GI_INFO_TYPE_TYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[type] everything");
        break;
    case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[unresolved] everything");
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_boxed_free_callback(gpointer boxed, gpointer user_data)
{
    GType *gtype = user_data;

    g_boxed_free(*gtype, boxed);
}


static void
rb_gi_return_argument_free_everything_glist_interface(GIArgument *argument,
                                                      G_GNUC_UNUSED GITypeInfo *type_info,
                                                      GITypeInfo *element_type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    const gchar *interface_name;
    GType gtype;

    interface_info = g_type_info_get_interface(element_type_info);
    interface_type = g_base_info_get_type(interface_info);
    interface_name = g_info_type_to_string(interface_type);
    gtype = g_registered_type_info_get_g_type(interface_info);
    g_base_info_unref(interface_info);
    g_base_info_unref(element_type_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                     interface_name,
                     g_type_name(gtype));
        } else {
            g_list_foreach(argument->v_pointer,
                           rb_gi_boxed_free_callback,
                           &gtype);
            g_list_free(argument->v_pointer);
        }
        break;
    case GI_INFO_TYPE_BOXED:
        g_list_foreach(argument->v_pointer, rb_gi_boxed_free_callback, &gtype);
        g_list_free(argument->v_pointer);
        break;
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
        g_list_foreach(argument->v_pointer, (GFunc)g_object_unref, NULL);
        g_list_free(argument->v_pointer);
        break;
    case GI_INFO_TYPE_CONSTANT:
    case GI_INFO_TYPE_INVALID_0:
    case GI_INFO_TYPE_UNION:
    case GI_INFO_TYPE_VALUE:
    case GI_INFO_TYPE_SIGNAL:
    case GI_INFO_TYPE_VFUNC:
    case GI_INFO_TYPE_PROPERTY:
    case GI_INFO_TYPE_FIELD:
    case GI_INFO_TYPE_ARG:
    case GI_INFO_TYPE_TYPE:
    case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[interface(%s)](%s) everything",
                 interface_name,
                 g_type_name(gtype));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_return_argument_free_everything_glist(GIArgument *argument,
                                            GITypeInfo *type_info)
{
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_UTF8:
        g_base_info_unref(element_type_info);
        g_list_foreach(argument->v_pointer, (GFunc)g_free, NULL);
        g_list_free(argument->v_pointer);
        break;
    case GI_TYPE_TAG_FILENAME:
    case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_gi_return_argument_free_everything_glist_interface(argument,
                                                              type_info,
                                                              element_type_info);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_return_argument_free_everything_gslist_interface(GIArgument *argument,
                                                       G_GNUC_UNUSED GITypeInfo *type_info,
                                                       GITypeInfo *element_type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    const gchar *interface_name;
    GType gtype;

    interface_info = g_type_info_get_interface(element_type_info);
    interface_type = g_base_info_get_type(interface_info);
    interface_name = g_info_type_to_string(interface_type);
    gtype = g_registered_type_info_get_g_type(interface_info);
    g_base_info_unref(interface_info);
    g_base_info_unref(element_type_info);

    switch (interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                 interface_name,
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_STRUCT:
        if (gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                     interface_name,
                     g_type_name(gtype));
        } else {
            g_slist_foreach(argument->v_pointer,
                            rb_gi_boxed_free_callback,
                            &gtype);
            g_slist_free(argument->v_pointer);
        }
        break;
      case GI_INFO_TYPE_BOXED:
        g_slist_foreach(argument->v_pointer, rb_gi_boxed_free_callback, &gtype);
        g_slist_free(argument->v_pointer);
        break;
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                 interface_name,
                 g_type_name(gtype));
        break;
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        g_slist_foreach(argument->v_pointer, (GFunc)g_object_unref, NULL);
        g_slist_free(argument->v_pointer);
        break;
      case GI_INFO_TYPE_CONSTANT:
      case GI_INFO_TYPE_INVALID_0:
      case GI_INFO_TYPE_UNION:
      case GI_INFO_TYPE_VALUE:
      case GI_INFO_TYPE_SIGNAL:
      case GI_INFO_TYPE_VFUNC:
      case GI_INFO_TYPE_PROPERTY:
      case GI_INFO_TYPE_FIELD:
      case GI_INFO_TYPE_ARG:
      case GI_INFO_TYPE_TYPE:
      case GI_INFO_TYPE_UNRESOLVED:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[interface(%s)](%s) everything",
                 interface_name,
                 g_type_name(gtype));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_return_argument_free_everything_gslist(GIArgument *argument,
                                             GITypeInfo *type_info)
{
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    if (!argument->v_pointer)
        return;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
        g_base_info_unref(element_type_info);
        g_slist_foreach(argument->v_pointer, (GFunc)g_free, NULL);
        g_slist_free(argument->v_pointer);
        break;
    case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_gi_return_argument_free_everything_gslist_interface(argument,
                                                               type_info,
                                                               element_type_info);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(GSList)[%s] everything",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_return_argument_free_everything(GIArgument *argument,
                                      GITypeInfo *type_info)
{
    GITypeTag type_tag;

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        break;
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
      case GI_TYPE_TAG_INT16:
      case GI_TYPE_TAG_UINT16:
      case GI_TYPE_TAG_INT32:
      case GI_TYPE_TAG_UINT32:
      case GI_TYPE_TAG_INT64:
      case GI_TYPE_TAG_UINT64:
      case GI_TYPE_TAG_FLOAT:
      case GI_TYPE_TAG_DOUBLE:
      case GI_TYPE_TAG_GTYPE:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) everything",
                 g_type_tag_to_string(type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        g_free(argument->v_string);
        break;
      case GI_TYPE_TAG_FILENAME:
        g_free(argument->v_string);
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_return_argument_free_everything_array(argument, type_info);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_return_argument_free_everything_interface(argument, type_info);
        break;
      case GI_TYPE_TAG_GLIST:
        rb_gi_return_argument_free_everything_glist(argument, type_info);
        break;
      case GI_TYPE_TAG_GSLIST:
        rb_gi_return_argument_free_everything_gslist(argument, type_info);
        break;
      case GI_TYPE_TAG_GHASH:
        g_hash_table_unref(argument->v_pointer);
        break;
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) everything",
                 g_type_tag_to_string(type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

VALUE
rb_gi_return_argument_to_ruby(GICallableInfo *callable_info,
                              GIArgument *argument,
                              GArray *in_args,
                              GArray *out_args,
                              GPtrArray *args_metadata)
{
    VALUE rb_argument;
    gboolean may_return_null;
    GITypeInfo return_value_info;

    may_return_null = g_callable_info_may_return_null(callable_info);
    if (may_return_null && !argument->v_pointer) {
        return Qnil;
    }

    g_callable_info_load_return_type(callable_info, &return_value_info);
    rb_argument = rb_gi_argument_to_ruby(argument, FALSE, &return_value_info,
                                         in_args, out_args, args_metadata);
    switch (g_callable_info_get_caller_owns(callable_info)) {
    case GI_TRANSFER_NOTHING:
        break;
    case GI_TRANSFER_CONTAINER:
        rb_gi_return_argument_free_container(argument, &return_value_info);
        break;
    case GI_TRANSFER_EVERYTHING:
        rb_gi_return_argument_free_everything(argument, &return_value_info);
        break;
    default:
        g_assert_not_reached();
        break;
    }
    return rb_argument;
}

void
rb_gi_argument_init(void)
{
    rb_cGLibBoxed = rb_const_get(mGLib, rb_intern("Boxed"));
    rb_cGLibBytes = rb_const_get(mGLib, rb_intern("Bytes"));
    rb_cGLibObject = rb_const_get(mGLib, rb_intern("Object"));
    rb_cGLibValue = rb_const_get(mGLib, rb_intern("Value"));
}
