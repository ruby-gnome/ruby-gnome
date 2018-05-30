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

#include "rb-gi-private.h"

#define RG_TARGET_NAMESPACE rb_cGIConstructorInfo
#define SELF(self) RVAL2GI_FUNCTION_INFO(self)

GType
gi_constructor_info_get_type(void)
{
    static GType type = 0;
    if (type == 0) {
        type = g_boxed_type_register_static("GIConstructorInfo",
                                            (GBoxedCopyFunc)g_base_info_ref,
                                            (GBoxedFreeFunc)g_base_info_unref);
    }
    return type;
}

static void
initialize_receiver(VALUE receiver,
                    GICallableInfo *callable_info,
                    GIArgument *value)
{
    GITypeInfo return_value_info;
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    g_callable_info_load_return_type(callable_info, &return_value_info);

    if (g_type_info_get_tag(&return_value_info) != GI_TYPE_TAG_INTERFACE) {
        rb_raise(rb_eRuntimeError, "TODO: returned value isn't interface");
    }

    interface_info = g_type_info_get_interface(&return_value_info);
    interface_type = g_base_info_get_type(interface_info);
    g_base_info_unref(interface_info);
    switch (interface_type) {
      case GI_INFO_TYPE_OBJECT:
        G_INITIALIZE(receiver, value->v_pointer);
        {
            gboolean was_floating;
            was_floating = g_object_is_floating(value->v_pointer);
            g_object_ref_sink(value->v_pointer);
            switch (g_callable_info_get_caller_owns(callable_info)) {
              case GI_TRANSFER_NOTHING:
                break;
              case GI_TRANSFER_CONTAINER:
                break;
              case GI_TRANSFER_EVERYTHING:
                if (!was_floating) {
                    g_object_unref(value->v_pointer);
                }
                break;
              default:
                g_assert_not_reached();
                break;
            }
        }
        break;
      case GI_INFO_TYPE_STRUCT:
        G_INITIALIZE(receiver, value->v_pointer);
        break;
      case GI_INFO_TYPE_INVALID:
      case GI_INFO_TYPE_FUNCTION:
      case GI_INFO_TYPE_CALLBACK:
      case GI_INFO_TYPE_BOXED:
      case GI_INFO_TYPE_ENUM:
      case GI_INFO_TYPE_FLAGS:
      case GI_INFO_TYPE_INTERFACE:
      case GI_INFO_TYPE_CONSTANT:
      case GI_INFO_TYPE_INVALID_0:
        rb_raise(rb_eRuntimeError,
                 "TODO: returned value isn't object, struct or union");
        break;
      case GI_INFO_TYPE_UNION:
        G_INITIALIZE(receiver, value->v_pointer);
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
        rb_raise(rb_eRuntimeError,
                 "TODO: returned value isn't object, struct or union");
        break;
    }
}

static VALUE
rg_invoke(VALUE self, VALUE rb_receiver, VALUE rb_arguments)
{
    GIFunctionInfo *info;
    GICallableInfo *callable_info;
    GIArgument return_value;

    info = SELF(self);
    callable_info = (GICallableInfo *)info;

    if (NIL_P(rb_receiver)) {
        rb_raise(rb_eArgError, "receiver is missing");
    }
    /* TODO: use rb_protect */
    rb_gi_function_info_invoke_raw(info,
                                   self,
                                   Qnil,
                                   rb_arguments,
                                   &return_value,
                                   NULL);

    initialize_receiver(rb_receiver, callable_info, &return_value);

    return rb_receiver;
}

void
rb_gi_constructor_info_init(VALUE rb_mGI, VALUE rb_cGIFunctionInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS_WITH_PARENT(GI_TYPE_CONSTRUCTOR_INFO,
                                "ConstructorInfo", rb_mGI,
                                rb_cGIFunctionInfo);

    RG_DEF_METHOD(invoke, 2);
}
