/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012-2014  Ruby-GNOME2 Project Team
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

#ifndef RB_GI_CONVERSIONS_H
#define RB_GI_CONVERSIONS_H

#define RVAL2GI_REPOSITORY(rb_object) (G_IREPOSITORY(RVAL2GOBJ(rb_object)))
#define RVAL2GI_REPOSITORY_LOAD_FLAGS(rb_flags) \
    (RVAL2GFLAGS(rb_flags, G_TYPE_I_REPOSITORY_LOAD_FLAGS))

#define GI_BASE_INFO2RVAL(info)				\
    (rb_gi_base_info_to_ruby((GIBaseInfo *)(info)))
#define GI_BASE_INFO2RVAL_WITH_UNREF(info)		\
    (rb_gi_base_info_to_ruby_with_unref((GIBaseInfo *)(info)))
#define RVAL2GI_BASE_INFO(rb_object) (rb_gi_base_info_from_ruby(rb_object))

#define GI_ARGUMENT2RVAL(argument, duplicate, type_info,                \
                         in_args, out_args, args_metadata)              \
    (rb_gi_argument_to_ruby((argument), (duplicate), (type_info),       \
                            (in_args), (out_args), (args_metadata)))
#define GI_RETURN_ARGUMENT2RVAL(callable_info, argument,                \
                                in_args, out_args, args_metadata)       \
    (rb_gi_return_argument_to_ruby((callable_info), (argument),         \
                                   (in_args), (out_args), (args_metadata)))


#define RVAL2GI_REGISTERED_TYPE_INFO(rb_object)			\
    ((GIRegisteredTypeInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_FUNCTION_INFO(rb_object)			\
    ((GIFunctionInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_STRUCT_INFO(rb_object)			\
    ((GIStructInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_ENUM_INFO(rb_object)			\
    ((GIEnumInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_OBJECT_INFO(rb_object)			\
    ((GIObjectInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_INTERFACE_INFO(rb_object)		\
    ((GIInterfaceInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_CONSTANT_INFO(rb_object)		\
    ((GIConstantInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_UNION_INFO(rb_object)			\
    ((GIUnionInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_VALUE_INFO(rb_object)			\
    ((GIValueInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_ARG_INFO(rb_object)		\
    ((GIArgInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_FIELD_INFO(rb_object)                   \
    ((GIFieldInfo *)RVAL2GI_BASE_INFO(rb_object))
#define RVAL2GI_TYPE_INFO(rb_object)			\
    ((GITypeInfo *)RVAL2GI_BASE_INFO(rb_object))

#define GI_INFO_TYPE2RVAL(type)      (GENUM2RVAL(type, G_TYPE_I_INFO_TYPE))
#define GI_TRANSFER2RVAL(transfer)   (GENUM2RVAL(transfer, G_TYPE_I_TRANSFER))
#define GI_DIRECTION2RVAL(direction) (GENUM2RVAL(direction, G_TYPE_I_DIRECTION))
#define GI_SCOPE_TYPE2RVAL(scope)    (GENUM2RVAL(scope, G_TYPE_I_SCOPE_TYPE))
#define RVAL2GI_TYPE_TAG(rb_tag)     (RVAL2GENUM(rb_tag, G_TYPE_I_TYPE_TAG))
#define GI_TYPE_TAG2RVAL(tag)        (GENUM2RVAL(tag, G_TYPE_I_TYPE_TAG))
#define GI_ARRAY_TYPE2RVAL(type)     (rb_gi_array_type_to_ruby(type))

#define GI_FUNCTION_INFO_FLAGS2RVAL(tag) \
    (GFLAGS2RVAL(tag, G_TYPE_I_FUNCTION_INFO_FLAGS))
#define GI_SIGNAL_FLAGS2RVAL(tag)          \
    (GFLAGS2RVAL(tag, G_TYPE_SIGNAL_FLAGS))
#define GI_VFUNC_INFO_FLAGS2RVAL(tag) \
    (GFLAGS2RVAL(tag, G_TYPE_IV_FUNC_INFO_FLAGS))
#define GI_FIELD_INFO_FLAGS2RVAL(tag)                   \
    (GFLAGS2RVAL(tag, G_TYPE_I_FIELD_INFO_FLAGS))

VALUE       rb_gi_base_info_to_ruby           (GIBaseInfo *info);
VALUE       rb_gi_base_info_to_ruby_with_unref(GIBaseInfo *info);
GIBaseInfo *rb_gi_base_info_from_ruby         (VALUE rb_info);

VALUE       rb_gi_argument_to_ruby            (GIArgument     *argument,
                                               gboolean        duplicate,
                                               GITypeInfo     *type_info,
                                               GArray         *in_args,
                                               GArray         *out_args,
                                               GPtrArray      *args_metadata);
VALUE       rb_gi_return_argument_to_ruby     (GICallableInfo *callable_info,
                                               GIArgument     *argument,
                                               GArray         *in_args,
                                               GArray         *out_args,
                                               GPtrArray      *args_metadata);

VALUE       rb_gi_array_type_to_ruby          (GIArrayType type);

#endif

