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

#pragma once

#include <ruby.h>
#include <rbgobject.h>
#include <glib-enum-types.h>

#include <girffi.h>
#include "gobject-introspection-enum-types.h"

#ifndef GI_CHECK_VERSION
#  include "rbgiversion.h"
#endif

#include "rb-gi-types.h"
#include "rb-gi-conversions.h"

#include "rb-gobject-introspection.h"

#include "rb-gi-private-arg-info.h"
#include "rb-gi-private-arguments.h"
#include "rb-gi-private-arguments-in.h"
#include "rb-gi-private-arguments-out.h"
#include "rb-gi-private-array-type.h"
#include "rb-gi-private-callback.h"
#include "rb-gi-private-direction.h"
#include "rb-gi-private-transfer.h"

#ifndef RB_TYPE_P
#  define RB_TYPE_P(object, type) (TYPE(object) == type)
#endif

extern void Init_gobject_introspection(void);

gboolean rb_gi_is_debug_mode(void);

void rb_gi_argument_init             (void);

void rb_gi_type_tag_init             (VALUE rb_mGI);
void rb_gi_base_info_init            (VALUE rb_mGI);
void rb_gi_callable_info_init        (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_function_info_init        (VALUE rb_mGI,
                                      VALUE rb_cGICallableInfo);
void rb_gi_method_info_init          (VALUE rb_mGI,
                                      VALUE rb_cGIFunctionInfo);
void rb_gi_constructor_info_init     (VALUE rb_mGI,
                                      VALUE rb_cGIFunctionInfo);
void rb_gi_callback_info_init        (VALUE rb_mGI,
                                      VALUE rb_cGICallableInfo);
void rb_gi_registered_type_info_init (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_struct_info_init          (VALUE rb_mGI,
                                      VALUE rb_cGIRegisteredTypeInfo);
void rb_gi_boxed_info_init           (VALUE rb_mGI,
                                      VALUE rb_cGIRegisteredTypeInfo);
void rb_gi_enum_info_init            (VALUE rb_mGI,
                                      VALUE rb_cGIRegisteredTypeInfo);
void rb_gi_flags_info_init           (VALUE rb_mGI,
                                      VALUE rb_cGIEnumInfo);
void rb_gi_object_info_init          (VALUE rb_mGI,
                                      VALUE rb_cGIRegisteredTypeInfo);
void rb_gi_interface_info_init       (VALUE rb_mGI,
                                      VALUE rb_cGIRegisteredTypeInfo);
void rb_gi_constant_info_init        (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_union_info_init           (VALUE rb_mGI,
                                      VALUE rb_cGIRegisteredTypeInfo);
void rb_gi_value_info_init           (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_signal_info_init          (VALUE rb_mGI,
                                      VALUE rb_cGICallableInfo);
void rb_gi_vfunc_info_init           (VALUE rb_mGI,
                                      VALUE rb_cGICallableInfo);
void rb_gi_property_info_init        (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_field_info_init           (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_arg_info_init             (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_type_info_init            (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_unresolved_info_init      (VALUE rb_mGI,
                                      VALUE rb_cGIBaseInfo);
void rb_gi_repository_init           (VALUE rb_mGI);
void rb_gi_loader_init               (VALUE rb_mGI);

VALUE rb_gi_function_info_invoke_raw (GIFunctionInfo *info,
                                      VALUE rb_info,
                                      VALUE rb_receiver,
                                      VALUE rb_arguments,
                                      GIArgument *return_value,
                                      VALUE *rb_return_value);

VALUE rb_gi_field_info_get_field_raw (GIFieldInfo *info,
                                      gpointer     memory);
void  rb_gi_field_info_set_field_raw (GIFieldInfo *info,
                                      gpointer     memory,
                                      VALUE        rb_field_value);

VALUE rb_gi_struct_info_to_ruby(GIStructInfo *info,
                                gpointer object,
                                gboolean is_pointer);
gpointer rb_gi_struct_info_from_ruby(GIStructInfo *info,
                                     VALUE rb_object);
