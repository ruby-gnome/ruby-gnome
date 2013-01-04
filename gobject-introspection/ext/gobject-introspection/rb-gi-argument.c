/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rb-gobject-introspection.h"

VALUE
rb_gi_argument_to_ruby(GIArgument *argument, GITypeInfo *type_info)
{
    VALUE rb_argument = Qnil;

    switch (g_type_info_get_tag(type_info)) {
      case GI_TYPE_TAG_VOID:
        rb_argument = Qnil;
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
        /* TODO */
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            GIBaseInfo *interface_info;
            GIInfoType interface_type;
            GType gtype;

            interface_info = g_type_info_get_interface(type_info);
            interface_type = g_base_info_get_type(interface_info);

            gtype = g_registered_type_info_get_g_type(interface_info);
            switch (interface_type) {
              case GI_INFO_TYPE_ENUM:
                rb_argument = GENUM2RVAL(argument->v_int32, gtype);
                break;
              case GI_INFO_TYPE_FLAGS:
                rb_argument = GFLAGS2RVAL(argument->v_int32, gtype);
                break;
              case GI_INFO_TYPE_OBJECT:
                rb_argument = GOBJ2RVAL(argument->v_pointer);
                break;
              case GI_INFO_TYPE_STRUCT:
                rb_argument = BOXED2RVAL(argument->v_pointer, gtype);
                break;
            }

            g_base_info_unref(interface_info);
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
        /* TODO */
        break;
      case GI_TYPE_TAG_ERROR:
        RG_RAISE_ERROR(argument->v_pointer);
        break;
      case GI_TYPE_TAG_UNICHAR:
        /* TODO */
        break;
      default:
        g_assert_not_reached();
        break;
    }

    return rb_argument;
}

VALUE
rb_gi_return_argument_to_ruby(GIArgument *argument,
                              GICallableInfo *callable_info)
{
    gboolean may_return_null;
    GITypeInfo return_value_info;

    may_return_null = g_callable_info_may_return_null(callable_info);
    if (may_return_null && !argument->v_pointer) {
        return Qnil;
    }

    g_callable_info_load_return_type(callable_info, &return_value_info);
    return rb_gi_argument_to_ruby(argument, &return_value_info);
}

GIArgument *
rb_gi_argument_from_ruby(GIArgument *argument, GITypeInfo *type_info,
                         VALUE rb_argument)
{
    memset(argument, 0, sizeof(GIArgument));

    switch (g_type_info_get_tag(type_info)) {
      case GI_TYPE_TAG_VOID:
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
        {
            GIArrayType array_type;
            gboolean zero_terminated_p;

            array_type = g_type_info_get_array_type(type_info);
            zero_terminated_p = g_type_info_is_zero_terminated(type_info);
            switch (array_type) {
              case GI_ARRAY_TYPE_C:
                if (zero_terminated_p) {
                    argument->v_pointer = (gchar *)RVAL2CSTR(rb_argument);
                } else {
                    argument->v_pointer = RSTRING_PTR(rb_argument);
                }
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
        }
        break;
      case GI_TYPE_TAG_INTERFACE:
        {
            GIBaseInfo *interface_info;
            GIInfoType interface_type;
            GType gtype;

            interface_info = g_type_info_get_interface(type_info);
            interface_type = g_base_info_get_type(interface_info);

            gtype = g_registered_type_info_get_g_type(interface_info);
            switch (interface_type) {
              case GI_INFO_TYPE_ENUM:
                argument->v_int32 = RVAL2GENUM(rb_argument, gtype);
                break;
              case GI_INFO_TYPE_FLAGS:
                argument->v_int32 = RVAL2GFLAGS(rb_argument, gtype);
                break;
              case GI_INFO_TYPE_OBJECT:
              case GI_INFO_TYPE_INTERFACE:
                argument->v_pointer = RVAL2GOBJ(rb_argument);
                break;
              case GI_INFO_TYPE_STRUCT:
                argument->v_pointer = RVAL2BOXED(rb_argument, gtype);
                break;
              default:
                /* TODO */
                rb_raise(rb_eNotImpError,
                         "not implemented Ruby -> GIArgument conversion: "
                         "<%s>: <%s>",
                         g_base_info_get_name(interface_info),
                         g_info_type_to_string(interface_type));
                break;
            }

            g_base_info_unref(interface_info);
        }
        break;
      case GI_TYPE_TAG_GLIST:
      case GI_TYPE_TAG_GSLIST:
      case GI_TYPE_TAG_GHASH:
      case GI_TYPE_TAG_ERROR:
      case GI_TYPE_TAG_UNICHAR:
        /* TODO */
        break;
      default:
        g_assert_not_reached();
        break;
    }

    return argument;
}

GIArgument *
rb_gi_call_argument_from_ruby(GIArgument *argument, GIArgInfo *arg_info,
                              VALUE rb_argument)
{
    if (g_arg_info_may_be_null(arg_info) && NIL_P(rb_argument)) {
        memset(argument, 0, sizeof(GIArgument));
    } else {
        GITypeInfo type_info;

        g_arg_info_load_type(arg_info, &type_info);
        rb_gi_argument_from_ruby(argument, &type_info, rb_argument);
    }

    return argument;
}
