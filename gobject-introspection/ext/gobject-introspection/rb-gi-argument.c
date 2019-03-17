/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2019  Ruby-GNOME2 Project Team
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

static VALUE rb_cGLibValue = Qnil;
static VALUE rb_cGLibBytes = Qnil;
static VALUE rb_cGLibBoxed = Qnil;

static gboolean
rb_gi_arg_info_may_be_null(GIArgInfo *arg_info)
{
#if GI_CHECK_VERSION(1, 42, 0)
    return g_arg_info_may_be_null(arg_info);
#else
    /*
      GObject Introspection < 1.42 doesn't support "(nullable)" yet.
      So, we assume that all argument may be NULL. It's danger but
      convenient.
    */
    return TRUE;
#endif
}

static const gchar *
rb_gi_transfer_to_string(GITransfer transfer)
{
    switch(transfer) {
      case GI_TRANSFER_NOTHING:
        return "nothing";
      case GI_TRANSFER_CONTAINER:
        return "container";
      case GI_TRANSFER_EVERYTHING:
        return "everything";
      default:
        return "unknown";
    }
}

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
                           GITypeInfo *type_info)
{
    GITypeTag type_tag;

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid out array length argument?: <%s>",
                 g_type_tag_to_string(type_tag));
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
                 g_type_tag_to_string(type_tag));
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
    GITypeInfo raw_length_type_info;
    GITypeInfo *length_type_info = NULL;

    length_index = g_type_info_get_array_length(array_type_info);
    if (length_index != -1) {
        RBGIArgMetadata *length_metadata;
        GIArgInfo *length_arg_info = NULL;
        GIArgument *raw_length_argument = NULL;

        length_metadata = g_ptr_array_index(args_metadata, length_index);
        length_arg_info = &(length_metadata->arg_info);

        g_arg_info_load_type(length_arg_info, &raw_length_type_info);
        length_type_info = &raw_length_type_info;

        if (length_metadata->direction == GI_DIRECTION_OUT) {
            raw_length_argument = &g_array_index(out_args, GIArgument,
                                                 length_metadata->out_arg_index);
        } else if (length_metadata->direction == GI_DIRECTION_INOUT) {
            raw_length_argument = &g_array_index(in_args, GIArgument,
                                                 length_metadata->in_arg_index);
        }

        if (raw_length_argument) {
            normalize_out_array_length(&normalized_length_argument,
                                       raw_length_argument,
                                       length_type_info);
            length_argument = &normalized_length_argument;
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
rb_gi_out_argument_init_array_c(GIArgument *argument,
                                G_GNUC_UNUSED GIArgInfo *arg_info,
                                G_GNUC_UNUSED GITypeInfo *array_type_info,
                                GITypeInfo *element_type_info)
{
    GITypeTag element_type_tag;

    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UINT8:
        argument->v_pointer = xmalloc(sizeof(guint8 *));
        break;
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
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        argument->v_pointer = xmalloc(sizeof(gchar **));
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        argument->v_pointer = xmalloc(sizeof(gpointer *));
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[c][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
      default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_out_argument_init_array_array_interface(GIArgument *argument,
                                              G_GNUC_UNUSED GIArgInfo *arg_info,
                                              GITypeInfo *array_type_info,
                                              GITypeInfo *element_type_info)
{
    gboolean zero_terminated_p;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;
    const char *interface_name;

    zero_terminated_p = g_type_info_is_zero_terminated(array_type_info);
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
    {
        gsize raw_struct_size;
        guint struct_size;

        raw_struct_size = g_struct_info_get_size(interface_info);
        struct_size = (guint)raw_struct_size;
        argument->v_pointer = g_array_new(zero_terminated_p,
                                          TRUE,
                                          struct_size);
        break;
    }
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
rb_gi_out_argument_init_array_array(GIArgument *argument,
                                    GIArgInfo *arg_info,
                                    GITypeInfo *array_type_info,
                                    GITypeInfo *element_type_info)
{
    GITypeTag element_type_tag;

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
                 "TODO: allocates GIArgument(array)[array][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_gi_out_argument_init_array_array_interface(argument,
                                                      arg_info,
                                                      array_type_info,
                                                      element_type_info);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[array][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_out_argument_init_array(GIArgument *argument, GIArgInfo *arg_info,
                              GITypeInfo *array_type_info)
{
    GIArrayType array_type;
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    array_type = g_type_info_get_array_type(array_type_info);
    element_type_info = g_type_info_get_param_type(array_type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (array_type) {
    case GI_ARRAY_TYPE_C:
        rb_gi_out_argument_init_array_c(argument,
                                        arg_info,
                                        array_type_info,
                                        element_type_info);
        break;
    case GI_ARRAY_TYPE_ARRAY:
        rb_gi_out_argument_init_array_array(argument,
                                            arg_info,
                                            array_type_info,
                                            element_type_info);
        break;
    case GI_ARRAY_TYPE_PTR_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[ptr-array][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_ARRAY_TYPE_BYTE_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(array)[byte-array][%s] for output",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(element_type_info);
}

static void
rb_gi_out_argument_init_interface(GIArgument *argument,
                                  G_GNUC_UNUSED GIArgInfo *arg_info,
                                  GITypeInfo *type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    /* TODO: Can we use the following code? */
    /*
    if (!g_arg_info_is_caller_allocates(arg_info)) {
        argument->v_pointer = ALLOC(gpointer);
        return;
    }
    */

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: allocates GIArgument(interface)[%s] for output",
                 g_info_type_to_string(interface_type));
        break;
    case GI_INFO_TYPE_STRUCT:
    {
        gsize struct_size;

        /* Should we care gtype?
           Related: rb_gi_out_argument_fin_interface() */
        struct_size = g_struct_info_get_size(interface_info);
        argument->v_pointer = xmalloc(struct_size);
        memset(argument->v_pointer, 0, struct_size);
    }
    break;
    case GI_INFO_TYPE_BOXED:
      rb_raise(rb_eNotImpError,
               "TODO: allocates GIArgument(interface)[%s] for output",
               g_info_type_to_string(interface_type));
      break;
    case GI_INFO_TYPE_ENUM:
      {
          gint *pointer = ALLOC(gint);
          *pointer = 0;
          argument->v_pointer = pointer;
      }
      break;
    case GI_INFO_TYPE_FLAGS:
      {
          guint *pointer = ALLOC(guint);
          *pointer = 0;
          argument->v_pointer = pointer;
      }
      break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
      {
          gpointer *pointer = ALLOC(gpointer);
          *pointer = NULL;
          argument->v_pointer = pointer;
      }
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
                 "TODO: allocates GIArgument(interface)[%s] for output",
                 g_info_type_to_string(interface_type));
        break;
    default:
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(interface_info);
}

void
rb_gi_out_argument_init(GIArgument *argument, GIArgInfo *arg_info)
{
    GITypeInfo type_info;
    GITypeTag type_tag;

    memset(argument, 0, sizeof(GIArgument));

    g_arg_info_load_type(arg_info, &type_info);
    type_tag = g_type_info_get_tag(&type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        if (g_type_info_is_pointer(&type_info)) {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_BOOLEAN:
        {
            gboolean *pointer = ALLOC(gboolean);
            *pointer = FALSE;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT8:
        {
            gint8 *pointer = ALLOC(gint8);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT8:
        {
            guint8 *pointer = ALLOC(guint8);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT16:
        {
            gint16 *pointer = ALLOC(gint16);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT16:
        {
            guint16 *pointer = ALLOC(guint16);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT32:
        {
            gint32 *pointer = ALLOC(gint32);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT32:
        {
            guint32 *pointer = ALLOC(guint32);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_INT64:
        {
            gint64 *pointer = ALLOC(gint64);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UINT64:
        {
            guint64 *pointer = ALLOC(guint64);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_FLOAT:
        {
            gfloat *pointer = ALLOC(gfloat);
            *pointer = 0.0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_DOUBLE:
        {
            gdouble *pointer = ALLOC(gdouble);
            *pointer = 0.0;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_GTYPE:
        {
            GType *pointer = ALLOC(GType);
            *pointer = G_TYPE_INVALID;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        {
            gchar **pointer = ALLOC(gchar *);
            *pointer = NULL;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_out_argument_init_array(argument, arg_info, &type_info);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_out_argument_init_interface(argument, arg_info, &type_info);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        {
            gpointer *pointer = ALLOC(gpointer);
            *pointer = NULL;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_ERROR:
        {
            GError **pointer = ALLOC(GError *);
            *pointer = NULL;
            argument->v_pointer = pointer;
        }
        break;
      case GI_TYPE_TAG_UNICHAR:
        {
            gunichar *pointer = ALLOC(gunichar);
            *pointer = 0;
            argument->v_pointer = pointer;
        }
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

VALUE
rb_gi_out_argument_to_ruby(GIArgument *argument,
                           GIArgInfo *arg_info,
                           GArray *in_args,
                           GArray *out_args,
                           GPtrArray *args_metadata)
{
    GIArgument normalized_argument;
    GITypeInfo type_info;
    GITypeTag type_tag;
    gboolean duplicate = FALSE;

    memset(&normalized_argument, 0, sizeof(GIArgument));
    g_arg_info_load_type(arg_info, &type_info);
    type_tag = g_type_info_get_tag(&type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        if (g_type_info_is_pointer(&type_info)) {
            normalized_argument.v_pointer = *((gpointer *)(argument->v_pointer));
        }
        break;
      case GI_TYPE_TAG_BOOLEAN:
        normalized_argument.v_boolean = *((gboolean *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT8:
        normalized_argument.v_int8 = *((gint8 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT8:
        normalized_argument.v_uint8 = *((guint8 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT16:
        normalized_argument.v_int16 = *((gint16 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT16:
        normalized_argument.v_uint16 = *((guint16 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT32:
        normalized_argument.v_int32 = *((gint32 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT32:
        normalized_argument.v_uint32 = *((guint32 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_INT64:
        normalized_argument.v_int64 = *((gint64 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UINT64:
        normalized_argument.v_uint64 = *((guint64 *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_FLOAT:
        normalized_argument.v_float = *((gfloat *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_DOUBLE:
        normalized_argument.v_double = *((gdouble *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_GTYPE:
        normalized_argument.v_size = *((GType *)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        normalized_argument.v_string = *((gchar **)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        if (g_arg_info_is_caller_allocates(arg_info)) {
            duplicate = TRUE;
            normalized_argument.v_pointer = argument->v_pointer;
        } else {
            normalized_argument.v_pointer = *((gpointer *)(argument->v_pointer));
        }
        break;
      case GI_TYPE_TAG_ERROR:
        normalized_argument.v_pointer = *((GError **)(argument->v_pointer));
        break;
      case GI_TYPE_TAG_UNICHAR:
        normalized_argument.v_uint32 = *((guint32 *)(argument->v_pointer));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    return rb_gi_argument_to_ruby(&normalized_argument,
                                  duplicate,
                                  &type_info,
                                  in_args,
                                  out_args,
                                  args_metadata);
}


static void
rb_gi_out_argument_fin_array_c(GIArgument *argument,
                               G_GNUC_UNUSED GIArgInfo *arg_info,
                               G_GNUC_UNUSED GITypeInfo *array_type_info,
                               GITypeInfo *element_type_info,
                               GITransfer transfer)
{
    GITypeTag element_type_tag;

    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (element_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
      case GI_TYPE_TAG_INT8:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[c][%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UINT8:
        if (transfer == GI_TRANSFER_EVERYTHING) {
            g_free(*((guint8 **)(argument->v_pointer)));
        }
        xfree(argument->v_pointer);
        break;
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
                 "TODO: free out GIArgument(array)[c][%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        if (transfer != GI_TRANSFER_NOTHING) {
            g_free(*((gchar **)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_FILENAME:
      case GI_TYPE_TAG_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[c][%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_INTERFACE:
        if (transfer == GI_TRANSFER_EVERYTHING) {
            g_base_info_unref(element_type_info);
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(array)[c][%s][%s]",
                     g_type_tag_to_string(element_type_tag),
                     rb_gi_transfer_to_string(transfer));
        }
        if (transfer != GI_TRANSFER_NOTHING) {
            g_free(*((gpointer *)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[c][%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
      default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_out_argument_fin_array(GIArgument *argument,
                             GIArgInfo *arg_info,
                             GITypeInfo *array_type_info,
                             GITransfer transfer)
{
    GIArrayType array_type;
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;

    array_type = g_type_info_get_array_type(array_type_info);
    element_type_info = g_type_info_get_param_type(array_type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);

    switch (array_type) {
    case GI_ARRAY_TYPE_C:
        rb_gi_out_argument_fin_array_c(argument,
                                       arg_info,
                                       array_type_info,
                                       element_type_info,
                                       transfer);
        break;
    case GI_ARRAY_TYPE_ARRAY:
        if (transfer != GI_TRANSFER_NOTHING) {
            g_base_info_unref(element_type_info);
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(array)[ptr-array][%s][%s]",
                     g_type_tag_to_string(element_type_tag),
                     rb_gi_transfer_to_string(transfer));
        }
        g_array_free(argument->v_pointer, TRUE);
        break;
    case GI_ARRAY_TYPE_PTR_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[ptr-array][%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_ARRAY_TYPE_BYTE_ARRAY:
        g_base_info_unref(element_type_info);
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(array)[byte-array][%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_base_info_unref(element_type_info);
        g_assert_not_reached();
        break;
    }

    g_base_info_unref(element_type_info);
}

static void
rb_gi_out_argument_fin_interface(GIArgument *argument,
                                 G_GNUC_UNUSED GIArgInfo *arg_info,
                                 GITypeInfo *interface_type_info,
                                 GITransfer transfer)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    interface_info = g_type_info_get_interface(interface_type_info);
    interface_type = g_base_info_get_type(interface_info);
    g_base_info_unref(interface_info);

    switch (interface_type) {
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: free out transfer GIArgument(interface)[%s][%s]",
                 g_info_type_to_string(interface_type),
                 rb_gi_transfer_to_string(transfer));
        break;
      case GI_INFO_TYPE_STRUCT:
        /* Should we care gtype?
           Related: rb_gi_out_argument_init_interface() */
        xfree(argument->v_pointer);
        break;
      case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: free out transfer GIArgument(interface)[%s][%s]",
                 g_info_type_to_string(interface_type),
                 rb_gi_transfer_to_string(transfer));
        break;
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
        break;
      case GI_INFO_TYPE_OBJECT:
        switch (transfer) {
          case GI_TRANSFER_NOTHING:
            break;
          case GI_TRANSFER_CONTAINER:
          case GI_TRANSFER_EVERYTHING:
            {
                GObject *object = *((GObject **)argument->v_pointer);
                if (object) {
                    g_object_unref(object);
                }
            }
            break;
          default:
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(interface)[%s][%s]",
                     g_info_type_to_string(interface_type),
                     rb_gi_transfer_to_string(transfer));
            break;
        }
        xfree(argument->v_pointer);
        break;
      case GI_INFO_TYPE_INTERFACE:
        if (transfer != GI_TRANSFER_NOTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(interface)[%s][%s]",
                     g_info_type_to_string(interface_type),
                     rb_gi_transfer_to_string(transfer));
        }
        xfree(argument->v_pointer);
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
                 "TODO: free out transfer GIArgument(interface)[%s][%s]",
                 g_info_type_to_string(interface_type),
                 rb_gi_transfer_to_string(transfer));
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

void
rb_gi_out_argument_fin(GIArgument *argument, GIArgInfo *arg_info)
{
    GITypeInfo type_info;
    GITypeTag type_tag;
    GITransfer transfer;

    if (g_arg_info_get_direction(arg_info) != GI_DIRECTION_OUT) {
        return;
    }

    g_arg_info_load_type(arg_info, &type_info);
    type_tag = g_type_info_get_tag(&type_info);
    transfer = g_arg_info_get_ownership_transfer(arg_info);

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
        xfree(argument->v_pointer);
        break;
    case GI_TYPE_TAG_UTF8:
        if (transfer != GI_TRANSFER_NOTHING) {
            g_free(*((gchar **)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
    case GI_TYPE_TAG_FILENAME:
        rb_raise(rb_eNotImpError,
                 "TODO: free out GIArgument(%s)",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_gi_out_argument_fin_array(argument, arg_info, &type_info, transfer);
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_gi_out_argument_fin_interface(argument,
                                         arg_info,
                                         &type_info,
                                         transfer);
        break;
    case GI_TYPE_TAG_GLIST:
        if (transfer == GI_TRANSFER_EVERYTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(%s)[%s]",
                     g_type_tag_to_string(type_tag),
                     rb_gi_transfer_to_string(transfer));
        }
        if (transfer != GI_TRANSFER_NOTHING) {
            g_list_free(*((GList **)argument->v_pointer));
        }
        xfree(argument->v_pointer);
        break;
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
        if (transfer != GI_TRANSFER_NOTHING) {
            rb_raise(rb_eNotImpError,
                     "TODO: free out transfer GIArgument(%s)[%s]",
                     g_type_tag_to_string(type_tag),
                     rb_gi_transfer_to_string(transfer));
        }
        xfree(argument->v_pointer);
        break;
    case GI_TYPE_TAG_UNICHAR:
        xfree(argument->v_pointer);
        break;
    default:
        g_assert_not_reached();
        break;
    }
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

static void
rb_gi_value_argument_from_ruby_interface(GIArgument *argument,
                                         GITypeInfo *type_info,
                                         VALUE rb_argument,
                                         VALUE self)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(interface)[%s]: <%s>",
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_STRUCT:
        gtype = g_registered_type_info_get_g_type(interface_info);
        if (gtype == G_TYPE_NONE) {
            argument->v_pointer = rb_gi_struct_get_raw(rb_argument, gtype);
        } else if (gtype == G_TYPE_VALUE) {
            GValue *gvalue;
            if (RVAL2CBOOL(rb_obj_is_kind_of(rb_argument, rb_cGLibValue))) {
                gvalue = RVAL2BOXED(rb_argument, G_TYPE_VALUE);
            } else {
                gvalue = ALLOC(GValue);
                memset(gvalue, 0, sizeof(GValue));
                rbgobj_initialize_gvalue(gvalue, rb_argument);
            }
            argument->v_pointer = gvalue;
        } else if (gtype == G_TYPE_BYTES) {
            if (RVAL2CBOOL(rb_obj_is_kind_of(rb_argument, rb_cGLibBytes))) {
                argument->v_pointer = RVAL2BOXED(rb_argument, G_TYPE_BYTES);
                g_bytes_ref(argument->v_pointer);
            } else {
                VALUE rb_string;
                GBytes *gbytes;

                rb_string = StringValue(rb_argument);
                gbytes = g_bytes_new(RSTRING_PTR(rb_string),
                                     RSTRING_LEN(rb_string));
                argument->v_pointer = gbytes;
            }
        } else if (gtype == G_TYPE_CLOSURE) {
            GClosure *rclosure = NULL;

            rclosure = g_rclosure_new(rb_argument, Qnil, NULL);
            g_rclosure_attach(rclosure, self);
            argument->v_pointer = rclosure;
        } else if (gtype == G_TYPE_VARIANT) {
            argument->v_pointer = rbg_variant_from_ruby(rb_argument);
        } else {
            argument->v_pointer = RVAL2BOXED(rb_argument, gtype);
        }
        break;
    case GI_INFO_TYPE_BOXED:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(interface)[%s]: <%s>",
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_ENUM:
        gtype = g_registered_type_info_get_g_type(interface_info);
        if (gtype == G_TYPE_NONE) {
            argument->v_int32 = NUM2INT(rb_argument);
        } else {
            argument->v_int32 = RVAL2GENUM(rb_argument, gtype);
        }
        break;
    case GI_INFO_TYPE_FLAGS:
        gtype = g_registered_type_info_get_g_type(interface_info);
        if (gtype == G_TYPE_NONE) {
            argument->v_int32 = NUM2INT(rb_argument);
        } else {
            argument->v_int32 = RVAL2GFLAGS(rb_argument, gtype);
        }
        break;
    case GI_INFO_TYPE_OBJECT:
    case GI_INFO_TYPE_INTERFACE:
        gtype = g_registered_type_info_get_g_type(interface_info);
        argument->v_pointer = RVAL2GOBJ(rb_argument);
        if (argument->v_pointer &&
            gtype != G_TYPE_NONE &&
            !G_TYPE_CHECK_INSTANCE_TYPE(argument->v_pointer, gtype)) {
            rb_raise(rb_eArgError,
                     "must be <%s> object: <%" PRIsVALUE ">",
                     g_type_name(gtype),
                     rb_argument);
        }
        break;
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(interface)[%s]: <%s>",
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
    case GI_INFO_TYPE_UNION:
        gtype = g_registered_type_info_get_g_type(interface_info);
        if (gtype == G_TYPE_NONE) {
            rb_raise(rb_eNotImpError,
                     "TODO: Ruby -> GIArgument(interface)[%s]: <%s>"
                     "(G_TYPE_NONE)",
                     g_info_type_to_string(interface_type),
                     g_base_info_get_name(interface_info));
        } else {
            argument->v_pointer = RVAL2BOXED(rb_argument, gtype);
        }
        break;
    case GI_INFO_TYPE_VALUE:
    case GI_INFO_TYPE_SIGNAL:
    case GI_INFO_TYPE_VFUNC:
    case GI_INFO_TYPE_PROPERTY:
    case GI_INFO_TYPE_FIELD:
    case GI_INFO_TYPE_ARG:
    case GI_INFO_TYPE_TYPE:
    case GI_INFO_TYPE_UNRESOLVED:
    default:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(interface)[%s]: <%s>",
                 g_info_type_to_string(interface_type),
                 g_base_info_get_name(interface_info));
        break;
    }

    g_base_info_unref(interface_info);
}

static void
rb_gi_value_argument_from_ruby_glist(GIArgument *argument,
                                     GITypeInfo *type_info,
                                     VALUE rb_argument,
                                     G_GNUC_UNUSED VALUE self)
{
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;
    GType gtype = G_TYPE_NONE;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);
    if (element_type_tag == GI_TYPE_TAG_INTERFACE) {
        GIBaseInfo *interface_info;
        interface_info = g_type_info_get_interface(element_type_info);
        gtype = g_registered_type_info_get_g_type(interface_info);
        g_base_info_unref(interface_info);
    }
    g_base_info_unref(element_type_info);

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
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GList)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        argument->v_pointer = RVAL2GOBJGLIST(rb_argument);
        if (gtype != G_TYPE_NONE) {
            GList *node;
            for (node = argument->v_pointer; node; node = g_list_next(node)) {
                GObject *object = node->data;
                if (!G_TYPE_CHECK_INSTANCE_TYPE(object, gtype)) {
                    g_list_free(argument->v_pointer);
                    rb_raise(rb_eArgError,
                             "must be <%s> objects: <%" PRIsVALUE ">",
                             g_type_name(gtype),
                             rb_argument);
                }
            }
        }
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GList)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_value_argument_from_ruby_gslist(GIArgument *argument,
                                      GITypeInfo *type_info,
                                      VALUE rb_argument,
                                      G_GNUC_UNUSED VALUE self)
{
    GITypeInfo *element_type_info;
    GITypeTag element_type_tag;
    GType gtype = G_TYPE_NONE;

    element_type_info = g_type_info_get_param_type(type_info, 0);
    element_type_tag = g_type_info_get_tag(element_type_info);
    if (element_type_tag == GI_TYPE_TAG_INTERFACE) {
        GIBaseInfo *interface_info;
        interface_info = g_type_info_get_interface(element_type_info);
        gtype = g_registered_type_info_get_g_type(interface_info);
        g_base_info_unref(interface_info);
    }
    g_base_info_unref(element_type_info);

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
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GSList)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        argument->v_pointer = RVAL2GOBJGSLIST(rb_argument);
        if (gtype != G_TYPE_NONE) {
            GSList *node;
            for (node = argument->v_pointer; node; node = g_slist_next(node)) {
                GObject *object = node->data;
                if (!G_TYPE_CHECK_INSTANCE_TYPE(object, gtype)) {
                    g_slist_free(argument->v_pointer);
                    rb_raise(rb_eArgError,
                             "must be <%s> objects: <%" PRIsVALUE ">",
                             g_type_name(gtype),
                             rb_argument);
                }
            }
        }
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(GSList)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_value_argument_from_ruby_void(GIArgument *argument, GITypeInfo *type_info,
                                    VALUE rb_argument)
{
    if (!g_type_info_is_pointer(type_info)) {
        return;
    }

    if (RB_TYPE_P(rb_argument, RUBY_T_STRING)) {
        argument->v_pointer = RSTRING_PTR(rb_argument);
    } else {
        argument->v_pointer = GUINT_TO_POINTER(NUM2ULONG(rb_argument));
    }
}

static GType
rb_gi_value_argument_from_ruby_gtype(VALUE rb_argument)
{
    ID id_gtype;
    VALUE rb_gtype;

    if (RB_TYPE_P(rb_argument, RUBY_T_STRING)) {
        GType gtype;
        gtype = g_type_from_name(RVAL2CSTR(rb_argument));
        if (gtype == 0) {
            rb_raise(rb_eArgError,
                     "unknown GType name: <%s>",
                     RVAL2CSTR(rb_argument));
        }
        return gtype;
    }

    CONST_ID(id_gtype, "gtype");

    if (rb_respond_to(rb_argument, id_gtype)) {
        rb_gtype = rb_funcall(rb_argument, id_gtype, 0);
    } else {
        rb_gtype = rb_argument;
    }

    return NUM2ULONG(rb_gtype);
}

GIArgument *
rb_gi_value_argument_from_ruby(GIArgument *argument, GITypeInfo *type_info,
                               VALUE rb_argument, VALUE self)
{
    GITypeTag type_tag;

    memset(argument, 0, sizeof(GIArgument));

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
        rb_gi_value_argument_from_ruby_void(argument, type_info, rb_argument);
        break;
    case GI_TYPE_TAG_BOOLEAN:
        argument->v_boolean = RVAL2CBOOL(rb_argument);
        break;
    case GI_TYPE_TAG_INT8:
        argument->v_int8 = NUM2CHR(rb_argument);
        break;
    case GI_TYPE_TAG_UINT8:
        argument->v_uint8 = (guint8)NUM2CHR(rb_argument);
        break;
    case GI_TYPE_TAG_INT16:
        argument->v_int16 = NUM2SHORT(rb_argument);
        break;
    case GI_TYPE_TAG_UINT16:
        argument->v_uint16 = NUM2USHORT(rb_argument);
        break;
    case GI_TYPE_TAG_INT32:
        argument->v_int32 = NUM2INT(rb_argument);
        break;
    case GI_TYPE_TAG_UINT32:
        argument->v_uint32 = NUM2UINT(rb_argument);
        break;
    case GI_TYPE_TAG_INT64:
        argument->v_int64 = NUM2LL(rb_argument);
        break;
    case GI_TYPE_TAG_UINT64:
        argument->v_uint64 = NUM2ULL(rb_argument);
        break;
    case GI_TYPE_TAG_FLOAT:
        argument->v_float = NUM2DBL(rb_argument);
        break;
    case GI_TYPE_TAG_DOUBLE:
        argument->v_double = NUM2DBL(rb_argument);
        break;
    case GI_TYPE_TAG_GTYPE:
        argument->v_size = rb_gi_value_argument_from_ruby_gtype(rb_argument);
        break;
    case GI_TYPE_TAG_UTF8:
        /* TODO: support UTF-8 convert like rb_argument.encode("UTF-8"). */
        argument->v_string = (gchar *)RVAL2CSTR_ACCEPT_SYMBOL(rb_argument);
        break;
    case GI_TYPE_TAG_FILENAME:
        argument->v_string = (gchar *)RVAL2CSTR(rb_argument);
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "should not be reached: Ruby -> GIArgument(%s)",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_gi_value_argument_from_ruby_interface(argument, type_info,
                                                 rb_argument, self);
        break;
    case GI_TYPE_TAG_GLIST:
        rb_gi_value_argument_from_ruby_glist(argument, type_info,
                                             rb_argument, self);
        break;
    case GI_TYPE_TAG_GSLIST:
        rb_gi_value_argument_from_ruby_gslist(argument, type_info,
                                              rb_argument, self);
        break;
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(%s)",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_UNICHAR:
      if (NIL_P(rb_argument)) {
          argument->v_uint32 = 0;
      } else {
          argument->v_uint32 = NUM2UINT(rb_argument);
      }
      break;
    default:
        g_assert_not_reached();
        break;
    }

    return argument;
}

static void
rb_gi_inout_argument_from_ruby(GIArgument *argument,
                               G_GNUC_UNUSED GIArgInfo *arg_info,
                               GITypeInfo *type_info,
                               VALUE rb_argument,
                               VALUE self)
{
    GIArgument in_argument;
    GITypeTag type_tag;

    rb_gi_value_argument_from_ruby(&in_argument, type_info, rb_argument, self);

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
        break;
    case GI_TYPE_TAG_BOOLEAN:
        argument->v_pointer = ALLOC(gboolean);
        *((gboolean *)argument->v_pointer) = in_argument.v_boolean;
        break;
    case GI_TYPE_TAG_INT8:
        argument->v_pointer = ALLOC(gint8);
        *((gint8 *)argument->v_pointer) = in_argument.v_int8;
        break;
    case GI_TYPE_TAG_UINT8:
        argument->v_pointer = ALLOC(guint8);
        *((guint8 *)argument->v_pointer) = in_argument.v_uint8;
        break;
    case GI_TYPE_TAG_INT16:
        argument->v_pointer = ALLOC(gint16);
        *((gint16 *)argument->v_pointer) = in_argument.v_int16;
        break;
    case GI_TYPE_TAG_UINT16:
        argument->v_pointer = ALLOC(guint16);
        *((guint16 *)argument->v_pointer) = in_argument.v_uint16;
        break;
    case GI_TYPE_TAG_INT32:
        argument->v_pointer = ALLOC(gint32);
        *((gint32 *)argument->v_pointer) = in_argument.v_int32;
        break;
    case GI_TYPE_TAG_UINT32:
        argument->v_pointer = ALLOC(guint32);
        *((guint32 *)argument->v_pointer) = in_argument.v_uint32;
        break;
    case GI_TYPE_TAG_INT64:
        argument->v_pointer = ALLOC(gint64);
        *((gint64 *)argument->v_pointer) = in_argument.v_int64;
        break;
    case GI_TYPE_TAG_UINT64:
        argument->v_pointer = ALLOC(guint64);
        *((guint64 *)argument->v_pointer) = in_argument.v_uint64;
        break;
    case GI_TYPE_TAG_FLOAT:
        argument->v_pointer = ALLOC(gfloat);
        *((gfloat *)argument->v_pointer) = in_argument.v_float;
        break;
    case GI_TYPE_TAG_DOUBLE:
        argument->v_pointer = ALLOC(gdouble);
        *((gdouble *)argument->v_pointer) = in_argument.v_double;
        break;
    case GI_TYPE_TAG_GTYPE:
        argument->v_pointer = ALLOC(gsize);
        *((gsize *)argument->v_pointer) = in_argument.v_size;
        break;
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
        argument->v_pointer = ALLOC(gchar *);
        *((gchar **)argument->v_pointer) = in_argument.v_string;
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "should not be reached: Ruby -> GIArgument(%s)",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
        argument->v_pointer = ALLOC(gpointer);
        *((gpointer *)argument->v_pointer) = in_argument.v_pointer;
        break;
    case GI_TYPE_TAG_ERROR:
        argument->v_pointer = ALLOC(GError *);
        *((GError **)argument->v_pointer) = in_argument.v_pointer;
        break;
    case GI_TYPE_TAG_UNICHAR:
        argument->v_pointer = ALLOC(gunichar);
        *((gunichar *)argument->v_pointer) = in_argument.v_uint32;
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_in_argument_transfer_interface(GIArgument *argument,
                                     G_GNUC_UNUSED GITransfer transfer,
                                     GITypeInfo *type_info,
                                     VALUE rb_argument)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    G_GNUC_UNUSED GType gtype;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);
    gtype = g_registered_type_info_get_g_type(interface_info);
    g_base_info_unref(interface_info);

    switch (interface_type) {
    case GI_INFO_TYPE_INVALID:
    case GI_INFO_TYPE_FUNCTION:
    case GI_INFO_TYPE_CALLBACK:
        rb_raise(rb_eNotImpError,
                 "TODO: in transfer (interface) [%s]",
                 g_info_type_to_string(interface_type));
        break;
    case GI_INFO_TYPE_STRUCT:
        rbgobj_boxed_unown(rb_argument);
        break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
        rb_raise(rb_eNotImpError,
                 "TODO: in transfer (interface) [%s]",
                 g_info_type_to_string(interface_type));
        break;
    case GI_INFO_TYPE_OBJECT:
        g_object_ref(argument->v_pointer);
        break;
    case GI_INFO_TYPE_INTERFACE:
    case GI_INFO_TYPE_CONSTANT:
        rb_raise(rb_eNotImpError,
                 "TODO: in transfer (interface) [%s]",
                 g_info_type_to_string(interface_type));
        break;
    case GI_INFO_TYPE_INVALID_0:
        g_assert_not_reached();
        break;
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
                 "TODO: in transfer (interface) [%s]",
                 g_info_type_to_string(interface_type));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_in_argument_transfer(GIArgument *argument, GITransfer transfer,
                           GITypeInfo *type_info, VALUE rb_argument)
{
    GITypeTag type_tag;

    if (transfer == GI_TRANSFER_NOTHING) {
        return;
    }

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
        break;
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
    case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: in transfer (%s)",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_gi_in_argument_transfer_interface(argument, transfer,
                                             type_info, rb_argument);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: in transfer (%s)",
                 g_type_tag_to_string(type_tag));
    default:
        g_assert_not_reached();
        break;
    }
}

GIArgument *
rb_gi_in_argument_from_ruby(GICallableInfo *callable_info,
                            GIArgument *argument,
                            GIArgInfo *arg_info,
                            guint nth_rb_argument,
                            VALUE rb_argument,
                            VALUE self)
{
    GITypeInfo type_info;

    if (NIL_P(rb_argument)) {
        if (rb_gi_arg_info_may_be_null(arg_info)) {
            memset(argument, 0, sizeof(GIArgument));
            return argument;
        } else {
            const char *module_name;
            GIBaseInfo *klass;
            const char *suffix;

            module_name = g_base_info_get_namespace(callable_info);
            klass = g_base_info_get_container(callable_info);
            if (nth_rb_argument == 1) {
                suffix = "st";
            } else if (nth_rb_argument == 2) {
                suffix = "nd";
            } else if (nth_rb_argument == 3) {
                suffix = "rd";
            } else {
                suffix = "th";
            }
            rb_raise(rb_eArgError,
                     "<%s%s%s%s%s>: the %u%s argument must not nil: <%s>",
                     module_name,
                     klass ? "::" : "",
                     klass ? g_base_info_get_name(klass) : "",
                     klass ? "#" : ".",
                     g_base_info_get_name(callable_info),
                     nth_rb_argument,
                     suffix,
                     g_base_info_get_name(arg_info));
        }
    }

    g_arg_info_load_type(arg_info, &type_info);
    if (g_arg_info_get_direction(arg_info) == GI_DIRECTION_INOUT) {
        rb_gi_inout_argument_from_ruby(argument, arg_info, &type_info,
                                       rb_argument, self);
    } else {
        rb_gi_value_argument_from_ruby(argument, &type_info, rb_argument, self);
        rb_gi_in_argument_transfer(argument,
                                   g_arg_info_get_ownership_transfer(arg_info),
                                   &type_info,
                                   rb_argument);
    }

    return argument;
}

static void
set_in_array_length_argument(GIArgument *argument,
                             GITypeInfo *type_info,
                             gint64 length)
{
    GITypeTag type_tag;

    if (!argument) {
        return;
    }

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid argument?: length[%s]",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_INT8:
        argument->v_int8 = length;
        break;
    case GI_TYPE_TAG_UINT8:
        argument->v_uint8 = length;
        break;
    case GI_TYPE_TAG_INT16:
        argument->v_int16 = length;
        break;
    case GI_TYPE_TAG_UINT16:
        argument->v_uint16 = length;
        break;
    case GI_TYPE_TAG_INT32:
        argument->v_int32 = (gint32)length;
        break;
    case GI_TYPE_TAG_UINT32:
        argument->v_uint32 = (guint32)length;
        break;
    case GI_TYPE_TAG_INT64:
        argument->v_int64 = length;
        break;
    case GI_TYPE_TAG_UINT64:
        argument->v_uint64 = length;
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
                 "TODO: invalid argument?: length[%s]",
                 g_type_tag_to_string(type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
set_in_array_boolean_arguments_from_ruby(GIArgument *array_argument,
                                         VALUE rb_boolean_array)
{
    gboolean *booleans;
    long i, n_args;

    n_args = RARRAY_LEN(rb_boolean_array);
    booleans = ALLOC_N(gboolean, n_args);
    for (i = 0; i < n_args; i++) {
        booleans[i] = RVAL2CBOOL(RARRAY_PTR(rb_boolean_array)[i]);
    }

    array_argument->v_pointer = booleans;
}

static void
set_in_array_int8_arguments_from_ruby(GIArgument *array_argument,
                                      VALUE rb_number_array)
{
    gint8 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(gint8, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2CHR(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_uint8_arguments_from_ruby(GIArgument *array_argument,
                                       VALUE rb_number_array)
{
    guint8 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(guint8, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = (guint8)NUM2CHR(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_int16_arguments_from_ruby(GIArgument *array_argument,
                                       VALUE rb_number_array)
{
    gint16 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(gint16, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2SHORT(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_uint16_arguments_from_ruby(GIArgument *array_argument,
                                        VALUE rb_number_array)
{
    guint16 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(guint16, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2USHORT(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_int32_arguments_from_ruby(GIArgument *array_argument,
                                       VALUE rb_number_array)
{
    gint32 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(gint32, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2INT(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_uint32_arguments_from_ruby(GIArgument *array_argument,
                                        VALUE rb_number_array)
{
    guint32 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(guint32, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2UINT(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_int64_arguments_from_ruby(GIArgument *array_argument,
                                       VALUE rb_number_array)
{
    gint64 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(gint64, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2LL(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_uint64_arguments_from_ruby(GIArgument *array_argument,
                                        VALUE rb_number_array)
{
    guint64 *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(guint64, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2ULL(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_float_arguments_from_ruby(GIArgument *array_argument,
                                       VALUE rb_number_array)
{
    gfloat *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(gfloat, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2DBL(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_double_arguments_from_ruby(GIArgument *array_argument,
                                        VALUE rb_number_array)
{
    gdouble *numbers;
    long i, n_args;

    n_args = RARRAY_LEN(rb_number_array);
    numbers = ALLOC_N(gdouble, n_args);
    for (i = 0; i < n_args; i++) {
        numbers[i] = NUM2DBL(RARRAY_PTR(rb_number_array)[i]);
    }

    array_argument->v_pointer = numbers;
}

static void
set_in_array_gtype_arguments_from_ruby(GIArgument *array_argument,
                                       VALUE rb_class_array)
{
    GType *types;
    long i, n_args;

    n_args = RARRAY_LEN(rb_class_array);
    types = ALLOC_N(GType, n_args);
    for (i = 0; i < n_args; i++) {
        types[i] = CLASS2GTYPE(RARRAY_PTR(rb_class_array)[i]);
    }

    array_argument->v_pointer = types;
}

typedef struct {
    GIStructInfo *struct_info;
    VALUE rb_argument;
    long n_args;
    gchar *values;
} ArrayInterfaceStructFromRubyData;

static VALUE
set_in_array_interface_struct_arguments_from_ruby_body(VALUE value)
{
    ArrayInterfaceStructFromRubyData *data;
    gsize size;
    gint i;

    data = (ArrayInterfaceStructFromRubyData *)value;
    size = g_struct_info_get_size(data->struct_info);
    data->values = xmalloc(size * data->n_args);

    for (i = 0; i < data->n_args; i++) {
        VALUE rb_element;
        gpointer element;

        rb_element = RARRAY_PTR(data->rb_argument)[i];
        element = rb_gi_struct_info_from_ruby(data->struct_info, rb_element);
        memcpy(data->values + (size * i),
               element,
               size);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
set_in_array_interface_struct_arguments_from_ruby_rescue(VALUE value)
{
    ArrayInterfaceStructFromRubyData *data;

    data = (ArrayInterfaceStructFromRubyData *)value;
    g_base_info_unref((GIBaseInfo *)(data->struct_info));
    xfree(data->values);

    rb_exc_raise(rb_errinfo());
}

static void
set_in_array_interface_struct_arguments_from_ruby(GIArgument *array_argument,
                                                  GIStructInfo *struct_info,
                                                  VALUE rb_argument)
{
    ArrayInterfaceStructFromRubyData data;

    data.struct_info = struct_info;
    data.rb_argument = rb_argument;
    data.n_args = RARRAY_LEN(rb_argument);
    data.values = NULL;
    rb_rescue(set_in_array_interface_struct_arguments_from_ruby_body,
              (VALUE)&data,
              set_in_array_interface_struct_arguments_from_ruby_rescue,
              (VALUE)&data);
    array_argument->v_pointer = data.values;
}

typedef struct {
    VALUE rb_argument;
    long n_args;
    gpointer *values;
} ArrayInterfaceObjectFromRubyData;

static VALUE
set_in_array_interface_object_arguments_from_ruby_body(VALUE value)
{
    ArrayInterfaceObjectFromRubyData *data;
    gint i;

    data = (ArrayInterfaceObjectFromRubyData *)value;
    data->values = xmalloc(sizeof(gpointer) * data->n_args);

    for (i = 0; i < data->n_args; i++) {
        VALUE rb_element;

        rb_element = RARRAY_AREF(data->rb_argument, i);
        data->values[i] = RVAL2GOBJ(rb_element);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
set_in_array_interface_object_arguments_from_ruby_rescue(VALUE value)
{
    ArrayInterfaceObjectFromRubyData *data;

    data = (ArrayInterfaceObjectFromRubyData *)value;
    xfree(data->values);

    rb_exc_raise(rb_errinfo());
}

static void
set_in_array_interface_object_arguments_from_ruby(GIArgument *array_argument,
                                                  VALUE rb_argument)
{
    ArrayInterfaceObjectFromRubyData data;

    data.rb_argument = rb_argument;
    data.n_args = RARRAY_LEN(rb_argument);
    data.values = NULL;
    rb_rescue(set_in_array_interface_object_arguments_from_ruby_body,
              (VALUE)&data,
              set_in_array_interface_object_arguments_from_ruby_rescue,
              (VALUE)&data);
    array_argument->v_pointer = data.values;
}

static void
set_in_array_interface_arguments_from_ruby(GIArgument *array_argument,
                                           GITypeInfo *element_type_info,
                                           VALUE rb_argument)
{
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
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[interface(%s)](%s)",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_STRUCT:
        {
            GIStructInfo *struct_info = (GIStructInfo *)interface_info;
            set_in_array_interface_struct_arguments_from_ruby(array_argument,
                                                              struct_info,
                                                              rb_argument);
        }
        break;
    case GI_INFO_TYPE_BOXED:
    case GI_INFO_TYPE_ENUM:
    case GI_INFO_TYPE_FLAGS:
        interface_name = g_info_type_to_string(interface_type);
        g_base_info_unref(interface_info);
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[interface(%s)](%s)",
                 interface_name,
                 g_type_name(gtype));
        break;
    case GI_INFO_TYPE_OBJECT:
        set_in_array_interface_object_arguments_from_ruby(array_argument,
                                                          rb_argument);
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
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[interface(%s)](%s)",
                 interface_name,
                 g_type_name(gtype));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
in_array_c_argument_from_ruby(GIArgument *array_argument,
                              GIArgument *length_argument,
                              G_GNUC_UNUSED GITypeInfo *array_type_info,
                              GITypeInfo *length_type_info,
                              GITypeInfo *element_type_info,
                              VALUE rb_argument)
{
    GITypeTag element_type_tag;

    element_type_tag = g_type_info_get_tag(element_type_info);
    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_BOOLEAN:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_boolean_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_INT8:
        if (RB_TYPE_P(rb_argument, RUBY_T_STRING)) {
            array_argument->v_pointer = RSTRING_PTR(rb_argument);
            set_in_array_length_argument(length_argument, length_type_info,
                                         RSTRING_LEN(rb_argument));
        } else {
            rb_argument = rbg_to_array(rb_argument);
            set_in_array_int8_arguments_from_ruby(array_argument, rb_argument);
            set_in_array_length_argument(length_argument, length_type_info,
                                         RARRAY_LEN(rb_argument));
        }
        break;
    case GI_TYPE_TAG_UINT8:
        if (RB_TYPE_P(rb_argument, RUBY_T_STRING)) {
            array_argument->v_pointer = RSTRING_PTR(rb_argument);
            set_in_array_length_argument(length_argument, length_type_info,
                                         RSTRING_LEN(rb_argument));
        } else {
            rb_argument = rbg_to_array(rb_argument);
            set_in_array_uint8_arguments_from_ruby(array_argument, rb_argument);
            set_in_array_length_argument(length_argument, length_type_info,
                                         RARRAY_LEN(rb_argument));
        }
        break;
    case GI_TYPE_TAG_INT16:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_int16_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_UINT16:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_uint16_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_INT32:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_int32_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_UINT32:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_uint32_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_INT64:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_int64_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_UINT64:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_uint64_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_FLOAT:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_float_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_DOUBLE:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_double_arguments_from_ruby(array_argument, rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_GTYPE:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_gtype_arguments_from_ruby(array_argument,
                                               rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
        rb_argument = rbg_to_array(rb_argument);
        array_argument->v_pointer = RVAL2STRV(rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_argument = rbg_to_array(rb_argument);
        set_in_array_interface_arguments_from_ruby(array_argument,
                                                   element_type_info,
                                                   rb_argument);
        set_in_array_length_argument(length_argument, length_type_info,
                                     RARRAY_LEN(rb_argument));
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
in_array_argument_from_ruby(GIArgument *array_argument,
                            GIArgument *length_argument,
                            GITypeInfo *array_type_info,
                            GITypeInfo *length_type_info,
                            VALUE rb_argument)
{
    GIArrayType array_type;
    GITypeInfo *element_type_info;

    array_type = g_type_info_get_array_type(array_type_info);
    element_type_info = g_type_info_get_param_type(array_type_info, 0);
    switch (array_type) {
    case GI_ARRAY_TYPE_C:
        in_array_c_argument_from_ruby(array_argument,
                                      length_argument,
                                      array_type_info,
                                      length_type_info,
                                      element_type_info,
                                      rb_argument);
        break;
    case GI_ARRAY_TYPE_ARRAY:
    case GI_ARRAY_TYPE_PTR_ARRAY:
    case GI_ARRAY_TYPE_BYTE_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[%s]",
                 g_type_tag_to_string(g_type_info_get_tag(element_type_info)));
        break;
    default:
        g_assert_not_reached();
        break;
    }
    g_base_info_unref(element_type_info);
}

static void
set_inout_array_length_argument(GIArgument *argument,
                                GITypeInfo *type_info,
                                GIArgument *length)
{
    GITypeTag type_tag;

    if (!argument) {
        return;
    }

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
    case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: invalid argument?: length[%s]",
                 g_type_tag_to_string(type_tag));
        break;
    case GI_TYPE_TAG_INT8:
        argument->v_pointer = ALLOC(gint8);
        *((gint8 *)argument->v_pointer) = length->v_int8;
        break;
    case GI_TYPE_TAG_UINT8:
        argument->v_pointer = ALLOC(guint8);
        *((guint8 *)argument->v_pointer) = length->v_uint8;
        break;
    case GI_TYPE_TAG_INT16:
        argument->v_pointer = ALLOC(gint16);
        *((gint16 *)argument->v_pointer) = length->v_int16;
        break;
    case GI_TYPE_TAG_UINT16:
        argument->v_pointer = ALLOC(guint16);
        *((guint16 *)argument->v_pointer) = length->v_uint16;
        break;
    case GI_TYPE_TAG_INT32:
        argument->v_pointer = ALLOC(gint32);
        *((gint32 *)argument->v_pointer) = length->v_int32;
        break;
    case GI_TYPE_TAG_UINT32:
        argument->v_pointer = ALLOC(guint32);
        *((guint32 *)argument->v_pointer) = length->v_uint32;
        break;
    case GI_TYPE_TAG_INT64:
        argument->v_pointer = ALLOC(gint64);
        *((gint64 *)argument->v_pointer) = length->v_int64;
        break;
    case GI_TYPE_TAG_UINT64:
        argument->v_pointer = ALLOC(guint64);
        *((guint64 *)argument->v_pointer) = length->v_uint64;
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
                 "TODO: invalid argument?: length[%s]",
                 g_type_tag_to_string(type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
inout_array_argument_from_ruby(GIArgument *array_argument,
                               GIArgument *length_argument,
                               GITypeInfo *array_type_info,
                               GITypeInfo *length_type_info,
                               VALUE rb_argument)
{
    GIArgument in_array_argument;
    GIArgument in_length_argument;

    in_array_argument_from_ruby(&in_array_argument,
                                &in_length_argument,
                                array_type_info,
                                length_type_info,
                                rb_argument);

    array_argument->v_pointer = ALLOC(gpointer);
    *((gpointer *)array_argument->v_pointer) = in_array_argument.v_pointer;
    set_inout_array_length_argument(length_argument,
                                    length_type_info,
                                    &in_length_argument);
}

GIArgument *
rb_gi_in_array_argument_from_ruby(GIArgument *array_argument,
                                  GIArgument *length_argument,
                                  GIArgInfo *array_arg_info,
                                  GIArgInfo *length_arg_info,
                                  VALUE rb_argument)
{
    GITypeInfo array_type_info;
    GITypeInfo length_type_info;
    GITypeInfo *length_type_info_arg = NULL;

    if (NIL_P(rb_argument) && rb_gi_arg_info_may_be_null(array_arg_info)) {
        memset(array_argument, 0, sizeof(GIArgument));
        if (length_argument) {
            memset(length_argument, 0, sizeof(GIArgument));
        }
        return array_argument;
    }

    g_arg_info_load_type(array_arg_info, &array_type_info);
    if (length_arg_info) {
        g_arg_info_load_type(length_arg_info, &length_type_info);
        length_type_info_arg = &length_type_info;
    }
    if (g_arg_info_get_direction(array_arg_info) == GI_DIRECTION_INOUT) {
        inout_array_argument_from_ruby(array_argument,
                                       length_argument,
                                       &array_type_info,
                                       length_type_info_arg,
                                       rb_argument);
    } else {
        GITransfer transfer;

        in_array_argument_from_ruby(array_argument,
                                    length_argument,
                                    &array_type_info,
                                    length_type_info_arg,
                                    rb_argument);
        transfer = g_arg_info_get_ownership_transfer(array_arg_info);
        rb_gi_in_argument_transfer(array_argument,
                                   transfer,
                                   &array_type_info,
                                   rb_argument);
    }

    return array_argument;
}

static void
rb_gi_value_argument_free_array_c(VALUE rb_argument,
                                  GIArgument *argument,
                                  G_GNUC_UNUSED GITypeInfo *type_info,
                                  GITypeInfo *element_type_info)
{
    GITypeTag element_type_tag;

    element_type_tag = g_type_info_get_tag(element_type_info);
    switch (element_type_tag) {
    case GI_TYPE_TAG_VOID:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_BOOLEAN:
        xfree(argument->v_pointer);
        break;
    case GI_TYPE_TAG_INT8:
    case GI_TYPE_TAG_UINT8:
        if (RB_TYPE_P(rb_argument, RUBY_T_STRING)) {
            /* Do nothing */
        } else {
            xfree(argument->v_pointer);
        }
        break;
    case GI_TYPE_TAG_INT16:
    case GI_TYPE_TAG_UINT16:
    case GI_TYPE_TAG_INT32:
    case GI_TYPE_TAG_UINT32:
    case GI_TYPE_TAG_INT64:
    case GI_TYPE_TAG_UINT64:
    case GI_TYPE_TAG_FLOAT:
    case GI_TYPE_TAG_DOUBLE:
    case GI_TYPE_TAG_GTYPE:
        xfree(argument->v_pointer);
        break;
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
        g_free(argument->v_pointer);
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    case GI_TYPE_TAG_INTERFACE:
        xfree(argument->v_pointer);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_value_argument_free_array(VALUE rb_argument,
                                GIArgument *argument,
                                GITypeInfo *type_info)
{
    GIArrayType array_type;
    GITypeInfo *element_type_info;

    array_type = g_type_info_get_array_type(type_info);
    element_type_info = g_type_info_get_param_type(type_info, 0);
    switch (array_type) {
    case GI_ARRAY_TYPE_C:
        rb_gi_value_argument_free_array_c(rb_argument,
                                          argument,
                                          type_info,
                                          element_type_info);
        break;
    case GI_ARRAY_TYPE_ARRAY:
    case GI_ARRAY_TYPE_PTR_ARRAY:
    case GI_ARRAY_TYPE_BYTE_ARRAY:
        break;
    default:
        g_assert_not_reached();
        break;
    }
    g_base_info_unref(element_type_info);
}

static void
rb_gi_value_argument_free_interface(VALUE rb_argument,
                                    GIArgument *argument,
                                    GITypeInfo *type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    if (interface_type == GI_INFO_TYPE_STRUCT) {
        GType gtype;
        gtype = g_registered_type_info_get_g_type(interface_info);

        if (gtype == G_TYPE_VALUE) {
            if (!RVAL2CBOOL(rb_obj_is_kind_of(rb_argument, rb_cGLibValue))) {
                GValue *gvalue = argument->v_pointer;
                g_value_unset(gvalue);
                xfree(argument->v_pointer);
            }
        } else if (gtype == G_TYPE_BYTES) {
            g_bytes_unref(argument->v_pointer);
        }
    }

    g_base_info_unref(interface_info);
}

void
rb_gi_value_argument_free(VALUE rb_argument,
                          GIArgument *argument,
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
        break;
    case GI_TYPE_TAG_ARRAY:
        rb_gi_value_argument_free_array(rb_argument, argument, type_info);
        break;
    case GI_TYPE_TAG_INTERFACE:
        rb_gi_value_argument_free_interface(rb_argument, argument, type_info);
        break;
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
    case GI_TYPE_TAG_ERROR:
    case GI_TYPE_TAG_UNICHAR:
        break;
    default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_inout_argument_free(VALUE rb_argument,
                          GIArgument *argument,
                          GITypeInfo *type_info)
{
    GIArgument in_argument;
    GITypeTag type_tag;

    memset(&in_argument, 0, sizeof(GIArgument));
    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
    case GI_TYPE_TAG_VOID:
        break;
    case GI_TYPE_TAG_BOOLEAN:
        in_argument.v_boolean = *((gboolean *)(argument->v_pointer));
        break;
    case GI_TYPE_TAG_INT8:
        in_argument.v_int8 = *((gint8 *)(argument->v_pointer));
        break;
    case GI_TYPE_TAG_UINT8:
        /* TODO!!! */
        in_argument.v_uint8 = *((guint8 *)(argument->v_pointer));
        argument->v_pointer = ALLOC(guint8);
        *((guint8 *)argument->v_pointer) = in_argument.v_uint8;
        break;
    case GI_TYPE_TAG_INT16:
        in_argument.v_int16 = *((gint16 *)(argument->v_pointer));
        argument->v_pointer = ALLOC(gint16);
        *((gint16 *)argument->v_pointer) = in_argument.v_int16;
        break;
    case GI_TYPE_TAG_UINT16:
        in_argument.v_uint16 = *((guint16 *)(argument->v_pointer));
        argument->v_pointer = ALLOC(guint16);
        *((guint16 *)argument->v_pointer) = in_argument.v_uint16;
        break;
    case GI_TYPE_TAG_INT32:
        in_argument.v_int32 = *((gint32 *)(argument->v_pointer));
        argument->v_pointer = ALLOC(gint32);
        *((gint32 *)argument->v_pointer) = in_argument.v_int32;
        break;
    case GI_TYPE_TAG_UINT32:
        in_argument.v_uint32 = *((guint32 *)(argument->v_pointer));
        argument->v_pointer = ALLOC(guint32);
        *((guint32 *)argument->v_pointer) = in_argument.v_uint32;
        break;
    case GI_TYPE_TAG_INT64:
        in_argument.v_int64 = *((gint64 *)(argument->v_pointer));
        argument->v_pointer = ALLOC(gint64);
        *((gint64 *)argument->v_pointer) = in_argument.v_int64;
        break;
    case GI_TYPE_TAG_UINT64:
        in_argument.v_uint64 = *((guint64 *)(argument->v_pointer));
        argument->v_pointer = ALLOC(guint64);
        *((guint64 *)argument->v_pointer) = in_argument.v_uint64;
        break;
    case GI_TYPE_TAG_FLOAT:
        in_argument.v_float = *((gfloat *)(argument->v_pointer));
        argument->v_pointer = ALLOC(gfloat);
        *((gfloat *)argument->v_pointer) = in_argument.v_float;
        break;
    case GI_TYPE_TAG_DOUBLE:
        in_argument.v_double = *((gdouble *)(argument->v_pointer));
        argument->v_pointer = ALLOC(gdouble);
        *((gdouble *)argument->v_pointer) = in_argument.v_double;
        break;
    case GI_TYPE_TAG_GTYPE:
        in_argument.v_size = *((gsize *)(argument->v_pointer));
        break;
    case GI_TYPE_TAG_UTF8:
    case GI_TYPE_TAG_FILENAME:
        in_argument.v_string = *((gchar **)(argument->v_pointer));
        break;
    case GI_TYPE_TAG_ARRAY:
    case GI_TYPE_TAG_INTERFACE:
    case GI_TYPE_TAG_GLIST:
    case GI_TYPE_TAG_GSLIST:
    case GI_TYPE_TAG_GHASH:
        in_argument.v_pointer = *((gpointer *)(argument->v_pointer));
        break;
    case GI_TYPE_TAG_ERROR:
        in_argument.v_pointer = *((GError **)(argument->v_pointer));
        break;
    case GI_TYPE_TAG_UNICHAR:
        in_argument.v_uint32 = *((gunichar *)(argument->v_pointer));
        break;
    default:
        g_assert_not_reached();
        break;
    }

    rb_gi_value_argument_free(rb_argument, &in_argument, type_info);
    xfree(argument->v_pointer);
}

void
rb_gi_in_argument_free(VALUE rb_argument,
                       GIArgument *argument,
                       GIArgInfo *arg_info)
{
    GITypeInfo type_info;

    g_arg_info_load_type(arg_info, &type_info);
    if (g_arg_info_get_direction(arg_info) == GI_DIRECTION_INOUT) {
        rb_gi_inout_argument_free(rb_argument, argument, &type_info);
    } else {
        rb_gi_value_argument_free(rb_argument, argument, &type_info);
    }
}

void
rb_gi_argument_init(void)
{
    rb_cGLibValue = rb_const_get(mGLib, rb_intern("Value"));
    rb_cGLibBytes = rb_const_get(mGLib, rb_intern("Bytes"));
    rb_cGLibBoxed = rb_const_get(mGLib, rb_intern("Boxed"));
}
