/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2013  Ruby-GNOME2 Project Team
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

static void
array_c_to_ruby(const gchar **elements, GITypeInfo *type_info, VALUE rb_array)
{
    gint n_elements;
    gboolean fixed_size_p;
    gboolean zero_terminated_p;

    if (!elements) {
        return;
    }

    n_elements = g_type_info_get_array_length(type_info);
    fixed_size_p = g_type_info_get_array_fixed_size(type_info);
    zero_terminated_p = g_type_info_is_zero_terminated(type_info);
    if (n_elements != -1) {
        gint i;
        for (i = 0; i < n_elements; i++) {
            rb_ary_push(rb_array, CSTR2RVAL(elements[i]));
        }
    } else if (zero_terminated_p) {
        for (; *elements; elements++) {
            rb_ary_push(rb_array, CSTR2RVAL(*elements));
        }
    } else {
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(array)[c] -> Ruby: "
                 "zero-terminated: %s "
                 "fixed-size: %s "
                 "length: %d",
                 zero_terminated_p ? "true" : "false",
                 fixed_size_p ? "true" : "false",
                 n_elements);
    }
}

static VALUE
array_to_ruby(gpointer array, GITypeInfo *type_info)
{
    VALUE rb_array;
    GIArrayType array_type;
    gint n_elements;

    array_type = g_type_info_get_array_type(type_info);
    n_elements = g_type_info_get_array_length(type_info);
    if (n_elements == -1) {
        rb_array = rb_ary_new();
    } else {
        rb_array = rb_ary_new2(n_elements);
    }
    switch (array_type) {
      case GI_ARRAY_TYPE_C:
        array_c_to_ruby(array, type_info, rb_array);
        break;
      case GI_ARRAY_TYPE_ARRAY:
        rb_raise(rb_eNotImpError, "TODO: GIArgument(array)[array] -> Ruby");
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
interface_to_ruby(GIArgument *argument, GITypeInfo *type_info)
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
        rb_interface = BOXED2RVAL(argument->v_pointer, gtype);
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

VALUE
rb_gi_argument_to_ruby(GIArgument *argument, GITypeInfo *type_info)
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
        rb_argument = LONG2NUM(argument->v_int64);
        break;
      case GI_TYPE_TAG_UINT64:
        rb_argument = ULONG2NUM(argument->v_uint64);
        break;
      case GI_TYPE_TAG_FLOAT:
        rb_argument = DBL2NUM(argument->v_float);
        break;
      case GI_TYPE_TAG_DOUBLE:
        rb_argument = DBL2NUM(argument->v_double);
        break;
      case GI_TYPE_TAG_GTYPE:
        rb_argument = rbgobj_gtype_new(argument->v_size);
        break;
      case GI_TYPE_TAG_UTF8:
        rb_argument = CSTR2RVAL(argument->v_string);
        break;
      case GI_TYPE_TAG_FILENAME:
        /* TODO: set encoding */
        rb_argument = CSTR2RVAL(argument->v_string);
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_argument = array_to_ruby(argument->v_pointer, type_info);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_argument = interface_to_ruby(argument, type_info);
        break;
      case GI_TYPE_TAG_GLIST:
        rb_argument = GOBJGLIST2RVAL(argument->v_pointer);
        break;
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(%s) -> Ruby",
                 g_type_tag_to_string(type_tag));
        break;
      case GI_TYPE_TAG_ERROR:
        rb_argument = GERROR2RVAL(argument->v_pointer);
        break;
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: GIArgument(%s) -> Ruby",
                 g_type_tag_to_string(type_tag));
        break;
      default:
        g_assert_not_reached();
        break;
    }

    return rb_argument;
}

static void
rb_gi_out_argument_init_interface(GIArgument *argument, GIArgInfo *arg_info,
                                  GITypeInfo *type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    if (!g_arg_info_is_caller_allocates(arg_info)) {
        argument->v_pointer = ALLOC(gpointer);
        return;
    }

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

            struct_size = g_struct_info_get_size(interface_info);
            argument->v_pointer = xmalloc(struct_size);
            memset(argument->v_pointer, 0, struct_size);
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
        break;
      case GI_TYPE_TAG_BOOLEAN:
        argument->v_pointer = ALLOC(gboolean);
        break;
      case GI_TYPE_TAG_INT8:
        argument->v_pointer = ALLOC(gint8);
        break;
      case GI_TYPE_TAG_UINT8:
        argument->v_pointer = ALLOC(guint8);
        break;
      case GI_TYPE_TAG_INT16:
        argument->v_pointer = ALLOC(gint16);
        break;
      case GI_TYPE_TAG_UINT16:
        argument->v_pointer = ALLOC(guint16);
        break;
      case GI_TYPE_TAG_INT32:
        argument->v_pointer = ALLOC(gint32);
        break;
      case GI_TYPE_TAG_UINT32:
        argument->v_pointer = ALLOC(guint32);
        break;
      case GI_TYPE_TAG_INT64:
        argument->v_pointer = ALLOC(gint64);
        break;
      case GI_TYPE_TAG_UINT64:
        argument->v_pointer = ALLOC(guint64);
        break;
      case GI_TYPE_TAG_FLOAT:
        argument->v_pointer = ALLOC(gfloat);
        break;
      case GI_TYPE_TAG_DOUBLE:
        argument->v_pointer = ALLOC(gdouble);
        break;
      case GI_TYPE_TAG_GTYPE:
        argument->v_pointer = ALLOC(GType);
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        argument->v_pointer = ALLOC(gchar *);
        break;
      case GI_TYPE_TAG_ARRAY:
        argument->v_pointer = ALLOC(gpointer);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_out_argument_init_interface(argument, arg_info, &type_info);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        argument->v_pointer = ALLOC(gpointer);
        break;
      case GI_TYPE_TAG_ERROR:
        argument->v_pointer = ALLOC(GError *);
        memset(argument->v_pointer, 0, sizeof(GError *));
        break;
      case GI_TYPE_TAG_UNICHAR:
        argument->v_pointer = ALLOC(gunichar);
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

VALUE
rb_gi_out_argument_to_ruby(GIArgument *argument, GIArgInfo *arg_info)
{
    GIArgument normalized_argument;
    GITypeInfo type_info;
    GITypeTag type_tag;

    memset(&normalized_argument, 0, sizeof(GIArgument));
    g_arg_info_load_type(arg_info, &type_info);
    type_tag = g_type_info_get_tag(&type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
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

    return rb_gi_argument_to_ruby(&normalized_argument, &type_info);
}

void
rb_gi_out_argument_fin(GIArgument *argument, GIArgInfo *arg_info)
{
    GITypeInfo type_info;

    if (g_arg_info_get_direction(arg_info) != GI_DIRECTION_OUT) {
        return;
    }

    g_arg_info_load_type(arg_info, &type_info);
    xfree(argument->v_pointer);
}

static void
rb_gi_return_argument_free_container(GIArgument *argument,
                                     GITypeInfo *type_info)
{
    GITypeTag type_tag;

    type_tag = g_type_info_get_tag(type_info);

    rb_raise(rb_eNotImpError,
             "TODO: free GIArgument(%s) as container",
             g_type_tag_to_string(type_tag));
}

static void
rb_gi_return_argument_free_everything_array(GIArgument *argument,
                                            GITypeInfo *type_info)
{
    switch (g_type_info_get_array_type(type_info)) {
      case GI_ARRAY_TYPE_C:
        g_strfreev(argument->v_pointer);
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
            g_object_unref(argument->v_pointer);
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
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(interface)[union] everything");
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
rb_gi_return_argument_free_everything(GIArgument *argument,
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
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) everything",
                 g_type_tag_to_string(type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
        g_free(argument->v_string);
        break;
      case GI_TYPE_TAG_FILENAME:
        rb_raise(rb_eNotImpError,
                 "TODO: free GIArgument(%s) everything",
                 g_type_tag_to_string(type_tag));
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_return_argument_free_everything_array(argument, type_info);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_return_argument_free_everything_interface(argument, type_info);
        break;
      case GI_TYPE_TAG_GLIST:
        g_list_foreach(argument->v_pointer, (GFunc)g_object_unref, NULL);
        g_list_free(argument->v_pointer);
        break;
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
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
rb_gi_return_argument_to_ruby(GIArgument *argument,
                              GICallableInfo *callable_info)
{
    VALUE rb_argument;
    gboolean may_return_null;
    GITypeInfo return_value_info;

    may_return_null = g_callable_info_may_return_null(callable_info);
    if (may_return_null && !argument->v_pointer) {
        return Qnil;
    }

    g_callable_info_load_return_type(callable_info, &return_value_info);
    rb_argument = rb_gi_argument_to_ruby(argument, &return_value_info);
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
rb_gi_argument_from_ruby_array_c(GIArgument *argument,
                                 G_GNUC_UNUSED GITypeInfo *type_info,
                                 GITypeInfo *element_type_info,
                                 VALUE rb_argument)
{
    GITypeTag element_type_tag;

    element_type_tag = g_type_info_get_tag(element_type_info);
    switch (element_type_tag) {
      case GI_TYPE_TAG_VOID:
      case GI_TYPE_TAG_BOOLEAN:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_INT8:
      case GI_TYPE_TAG_UINT8:
        argument->v_pointer = RSTRING_PTR(rb_argument);
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
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(array)[%s]",
                 g_type_tag_to_string(element_type_tag));
        break;
      case GI_TYPE_TAG_UTF8:
      case GI_TYPE_TAG_FILENAME:
        argument->v_pointer = RVAL2STRV(rb_argument);
        break;
      case GI_TYPE_TAG_ARRAY:
      case GI_TYPE_TAG_INTERFACE:
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
rb_gi_argument_from_ruby_array(GIArgument *argument, GITypeInfo *type_info,
                               VALUE rb_argument)
{
    GIArrayType array_type;
    GITypeInfo *element_type_info;

    array_type = g_type_info_get_array_type(type_info);
    element_type_info = g_type_info_get_param_type(type_info, 0);
    switch (array_type) {
      case GI_ARRAY_TYPE_C:
        rb_gi_argument_from_ruby_array_c(argument,
                                         type_info, element_type_info,
                                         rb_argument);
        break;
      case GI_ARRAY_TYPE_ARRAY:
      case GI_ARRAY_TYPE_PTR_ARRAY:
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        /* TODO */
        break;
      default:
        g_assert_not_reached();
        break;
    }
    g_base_info_unref(element_type_info);
}

static void
rb_gi_argument_from_ruby_interface(GIArgument *argument, GITypeInfo *type_info,
                                   VALUE rb_argument)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;
    GType gtype;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    gtype = g_registered_type_info_get_g_type(interface_info);
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
        if (gtype == G_TYPE_VALUE) {
            GValue *gvalue;
            gvalue = ALLOC(GValue);
            memset(gvalue, 0, sizeof(GValue));
            if (rb_obj_is_kind_of(rb_argument, rb_cGLibValue)) {
                GValue *source_gvalue;
                source_gvalue = RVAL2BOXED(rb_argument, G_TYPE_VALUE);
                g_value_init(gvalue, source_gvalue->g_type);
                g_value_copy(source_gvalue, gvalue);
            } else {
                rbgobj_initialize_gvalue(gvalue, rb_argument);
            }
            argument->v_pointer = gvalue;
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
        if (gtype == G_TYPE_NONE) {
            argument->v_int32 = NUM2INT(rb_argument);
        } else {
            argument->v_int32 = RVAL2GENUM(rb_argument, gtype);
        }
        break;
      case GI_INFO_TYPE_FLAGS:
        if (gtype == G_TYPE_NONE) {
            argument->v_int32 = NUM2INT(rb_argument);
        } else {
            argument->v_int32 = RVAL2GFLAGS(rb_argument, gtype);
        }
        break;
      case GI_INFO_TYPE_OBJECT:
      case GI_INFO_TYPE_INTERFACE:
        argument->v_pointer = RVAL2GOBJ(rb_argument);
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


GIArgument *
rb_gi_value_argument_from_ruby(GIArgument *argument, GITypeInfo *type_info,
                               VALUE rb_argument)
{
    GITypeTag type_tag;

    memset(argument, 0, sizeof(GIArgument));

    type_tag = g_type_info_get_tag(type_info);
    switch (type_tag) {
      case GI_TYPE_TAG_VOID:
        if (g_type_info_is_pointer(type_info)) {
            argument->v_pointer = GUINT_TO_POINTER(NUM2ULONG(rb_argument));
        }
        break;
      case GI_TYPE_TAG_BOOLEAN:
        argument->v_boolean = RVAL2CBOOL(rb_argument);
        break;
      case GI_TYPE_TAG_INT8:
        argument->v_int8 = NUM2INT(rb_argument);
        break;
      case GI_TYPE_TAG_UINT8:
        argument->v_uint8 = NUM2UINT(rb_argument);
        break;
      case GI_TYPE_TAG_INT16:
        argument->v_int16 = NUM2INT(rb_argument);
        break;
      case GI_TYPE_TAG_UINT16:
        argument->v_uint16 = NUM2UINT(rb_argument);
        break;
      case GI_TYPE_TAG_INT32:
        argument->v_int32 = NUM2INT(rb_argument);
        break;
      case GI_TYPE_TAG_UINT32:
        argument->v_uint32 = NUM2UINT(rb_argument);
        break;
      case GI_TYPE_TAG_INT64:
        argument->v_int64 = NUM2LONG(rb_argument);
        break;
      case GI_TYPE_TAG_UINT64:
        argument->v_uint64 = NUM2ULONG(rb_argument);
        break;
      case GI_TYPE_TAG_FLOAT:
        argument->v_float = NUM2DBL(rb_argument);
        break;
      case GI_TYPE_TAG_DOUBLE:
        argument->v_double = NUM2DBL(rb_argument);
        break;
      case GI_TYPE_TAG_GTYPE:
        /* TODO: support GLib::Type and String as GType name. */
        argument->v_size = NUM2ULONG(rb_argument);
        break;
      case GI_TYPE_TAG_UTF8:
        /* TODO: support UTF-8 convert like rb_argument.encode("UTF-8"). */
        argument->v_string = (gchar *)RVAL2CSTR(rb_argument);
        break;
      case GI_TYPE_TAG_FILENAME:
        argument->v_string = (gchar *)RVAL2CSTR(rb_argument);
        break;
      case GI_TYPE_TAG_ARRAY:
        rb_gi_argument_from_ruby_array(argument, type_info, rb_argument);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_argument_from_ruby_interface(argument, type_info, rb_argument);
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        rb_raise(rb_eNotImpError,
                 "TODO: Ruby -> GIArgument(%s)",
                 g_type_tag_to_string(type_tag));
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
                               GITypeInfo *type_info, VALUE rb_argument)
{
    GIArgument in_argument;
    GITypeTag type_tag;

    rb_gi_value_argument_from_ruby(&in_argument, type_info, rb_argument);

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
                                     G_GNUC_UNUSED VALUE rb_argument)
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
rb_gi_in_argument_from_ruby(GIArgument *argument, GIArgInfo *arg_info,
                            VALUE rb_argument)
{
    GITypeInfo type_info;

    if (g_arg_info_may_be_null(arg_info) && NIL_P(rb_argument)) {
        memset(argument, 0, sizeof(GIArgument));
        return argument;
    }

    g_arg_info_load_type(arg_info, &type_info);
    if (g_arg_info_get_direction(arg_info) == GI_DIRECTION_INOUT) {
        rb_gi_inout_argument_from_ruby(argument, arg_info, &type_info,
                                       rb_argument);
    } else {
        rb_gi_value_argument_from_ruby(argument, &type_info, rb_argument);
        rb_gi_in_argument_transfer(argument,
                                   g_arg_info_get_ownership_transfer(arg_info),
                                   &type_info,
                                   rb_argument);
    }

    return argument;
}

static void
rb_gi_value_argument_free_array(GIArgument *argument, GITypeInfo *type_info)
{
    GIArrayType array_type;

    array_type = g_type_info_get_array_type(type_info);
    switch (array_type) {
      case GI_ARRAY_TYPE_C:
        g_free(argument->v_pointer);
        break;
      case GI_ARRAY_TYPE_ARRAY:
      case GI_ARRAY_TYPE_PTR_ARRAY:
      case GI_ARRAY_TYPE_BYTE_ARRAY:
        break;
      default:
        g_assert_not_reached();
        break;
    }
}

static void
rb_gi_value_argument_free_interface(GIArgument *argument, GITypeInfo *type_info)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    interface_info = g_type_info_get_interface(type_info);
    interface_type = g_base_info_get_type(interface_info);

    if (interface_type == GI_INFO_TYPE_STRUCT) {
        GType gtype;
        gtype = g_registered_type_info_get_g_type(interface_info);

        if (gtype == G_TYPE_VALUE) {
            GValue *gvalue = argument->v_pointer;
            g_value_unset(gvalue);
            xfree(argument->v_pointer);
        }
    }

    g_base_info_unref(interface_info);
}

void
rb_gi_value_argument_free(GIArgument *argument, GITypeInfo *type_info)
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
        rb_gi_value_argument_free_array(argument, type_info);
        break;
      case GI_TYPE_TAG_INTERFACE:
        rb_gi_value_argument_free_interface(argument, type_info);
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
rb_gi_inout_argument_free(GIArgument *argument, GITypeInfo *type_info)
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

    rb_gi_value_argument_free(&in_argument, type_info);
    xfree(argument->v_pointer);
}

void
rb_gi_in_argument_free(GIArgument *argument, GIArgInfo *arg_info)
{
    GITypeInfo type_info;

    g_arg_info_load_type(arg_info, &type_info);
    if (g_arg_info_get_direction(arg_info) == GI_DIRECTION_INOUT) {
        rb_gi_inout_argument_free(argument, &type_info);
    } else {
        rb_gi_value_argument_free(argument, &type_info);
    }
}

void
rb_gi_argument_init(void)
{
    rb_cGLibValue = rb_const_get(mGLib, rb_intern("Value"));
}
