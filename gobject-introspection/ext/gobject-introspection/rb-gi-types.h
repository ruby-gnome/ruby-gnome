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

#ifndef RB_GI_TYPES_H
#define RB_GI_TYPES_H

#ifdef HAVE_GIREPOSITORY_2_0

/*
 * In girepository-2.0, the gi_*_get_type() functions are provided by the
 * library and return GTypeInstance-based (not GBoxed) types.  We cannot use
 * those with BOXED2RVAL/RVAL2BOXED, so we register our own GBoxed wrappers
 * with "RbGI" prefixed type names.  rb-gi-compat.h then overrides the
 * GI_TYPE_*_INFO macros to point to these wrappers.
 */

GType rb_gi_base_info_get_type          (void);
GType rb_gi_callable_info_get_type      (void);
GType rb_gi_function_info_get_type      (void);
GType rb_gi_method_info_get_type        (void);
GType rb_gi_constructor_info_get_type   (void);
GType rb_gi_callback_info_get_type      (void);
GType rb_gi_registered_type_info_get_type (void);
GType rb_gi_struct_info_get_type        (void);
GType rb_gi_boxed_info_get_type         (void);
GType rb_gi_enum_info_get_type          (void);
GType rb_gi_flags_info_get_type         (void);
GType rb_gi_object_info_get_type        (void);
GType rb_gi_interface_info_get_type     (void);
GType rb_gi_constant_info_get_type      (void);
GType rb_gi_union_info_get_type         (void);
GType rb_gi_value_info_get_type         (void);
GType rb_gi_signal_info_get_type        (void);
GType rb_gi_vfunc_info_get_type         (void);
GType rb_gi_property_info_get_type      (void);
GType rb_gi_field_info_get_type         (void);
GType rb_gi_arg_info_get_type           (void);
GType rb_gi_type_info_get_type          (void);
GType rb_gi_unresolved_info_get_type    (void);

#else /* !HAVE_GIREPOSITORY_2_0 */

/*
 * In gobject-introspection-1.0, these types are not provided by the library;
 * we register our own GBoxed wrappers.  The function names match the macro
 * names used in GI_TYPE_*_INFO, which in v1 also come from the system headers
 * (gibaseinfo.h etc.) so we only declare what isn't already defined there.
 */

#define GI_TYPE_CALLABLE_INFO        (gi_callable_info_get_type())
#define GI_TYPE_FUNCTION_INFO        (gi_function_info_get_type())
#define GI_TYPE_METHOD_INFO          (gi_method_info_get_type())
#define GI_TYPE_CONSTRUCTOR_INFO     (gi_constructor_info_get_type())
#define GI_TYPE_CALLBACK_INFO        (gi_callback_info_get_type())
#define GI_TYPE_REGISTERED_TYPE_INFO (gi_registered_type_info_get_type())
#define GI_TYPE_STRUCT_INFO          (gi_struct_info_get_type())
#define GI_TYPE_BOXED_INFO           (gi_boxed_info_get_type())
#define GI_TYPE_ENUM_INFO            (gi_enum_info_get_type())
#define GI_TYPE_FLAGS_INFO           (gi_flags_info_get_type())
#define GI_TYPE_OBJECT_INFO          (gi_object_info_get_type())
#define GI_TYPE_INTERFACE_INFO       (gi_interface_info_get_type())
#define GI_TYPE_CONSTANT_INFO        (gi_constant_info_get_type())
#define GI_TYPE_UNION_INFO           (gi_union_info_get_type())
#define GI_TYPE_VALUE_INFO           (gi_value_info_get_type())
#define GI_TYPE_SIGNAL_INFO          (gi_signal_info_get_type())
#define GI_TYPE_VFUNC_INFO           (gi_vfunc_info_get_type())
#define GI_TYPE_PROPERTY_INFO        (gi_property_info_get_type())
#define GI_TYPE_FIELD_INFO           (gi_field_info_get_type())
#define GI_TYPE_ARG_INFO             (gi_arg_info_get_type())
#define GI_TYPE_TYPE_INFO            (gi_type_info_get_type())
#define GI_TYPE_UNRESOLVED_INFO      (gi_unresolved_info_get_type())

GType gi_callable_info_get_type        (void);
GType gi_function_info_get_type        (void);
GType gi_method_info_get_type          (void);
GType gi_constructor_info_get_type     (void);
GType gi_callback_info_get_type        (void);
GType gi_registered_type_info_get_type (void);
GType gi_struct_info_get_type          (void);
GType gi_boxed_info_get_type           (void);
GType gi_enum_info_get_type            (void);
GType gi_flags_info_get_type           (void);
GType gi_object_info_get_type          (void);
GType gi_interface_info_get_type       (void);
GType gi_constant_info_get_type        (void);
GType gi_union_info_get_type           (void);
GType gi_value_info_get_type           (void);
GType gi_signal_info_get_type          (void);
GType gi_vfunc_info_get_type           (void);
GType gi_property_info_get_type        (void);
GType gi_field_info_get_type           (void);
GType gi_arg_info_get_type             (void);
GType gi_type_info_get_type            (void);
GType gi_unresolved_info_get_type      (void);

#endif /* HAVE_GIREPOSITORY_2_0 */

#endif /* RB_GI_TYPES_H */
