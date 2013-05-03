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
initialize_receiver(VALUE receiver, GITypeInfo *info, GIArgument *value)
{
    GIBaseInfo *interface_info;
    GIInfoType interface_type;

    if (g_type_info_get_tag(info) != GI_TYPE_TAG_INTERFACE) {
        rb_raise(rb_eRuntimeError, "TODO: returned value isn't interface");
    }

    interface_info = g_type_info_get_interface(info);
    interface_type = g_base_info_get_type(interface_info);
    g_base_info_unref(interface_info);
    switch (interface_type) {
      case GI_INFO_TYPE_OBJECT:
        g_object_ref_sink(value->v_pointer);
        G_INITIALIZE(receiver, value->v_pointer);
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
        rb_raise(rb_eRuntimeError,
                 "TODO: returned value isn't object or struct");
        break;
    }
}

static VALUE
rg_invoke(VALUE self, VALUE rb_options)
{
    GIFunctionInfo *info;
    GICallableInfo *callable_info;
    VALUE receiver;
    GIArgument return_value;
    GITypeInfo return_value_info;

    info = SELF(self);
    callable_info = (GICallableInfo *)info;

    rb_options = rbg_to_hash(rb_options);
    receiver = rb_hash_delete(rb_options, ID2SYM(rb_intern("receiver")));
    if (NIL_P(receiver)) {
        receiver = rb_hash_delete(rb_options, rb_str_new_cstr("receiver"));
    }
    if (NIL_P(receiver)) {
        rb_raise(rb_eArgError,
                 "receiver is missing: %s",
                 RBG_INSPECT(rb_options));
    }
    /* TODO: use rb_protect */
    rb_gi_function_info_invoke_raw(info,
                                   rb_options,
                                   &return_value);

    g_callable_info_load_return_type(callable_info, &return_value_info);
    initialize_receiver(receiver, &return_value_info, &return_value);

    return receiver;
}

void
rb_gi_constructor_info_init(VALUE rb_mGI, VALUE rb_cGIFunctionInfo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
	G_DEF_CLASS_WITH_PARENT(GI_TYPE_CONSTRUCTOR_INFO,
                                "ConstructorInfo", rb_mGI,
				rb_cGIFunctionInfo);

    RG_DEF_METHOD(invoke, 1);
}
