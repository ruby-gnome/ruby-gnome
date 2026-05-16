/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2024  Ruby-GNOME Project Team
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

/*
 * Compatibility layer for girepository-2.0 (GLib >= 2.80).
 *
 * This header maps the old gobject-introspection-1.0 API (g_* prefix) to
 * the new girepository-2.0 API (gi_* prefix) so the rest of the C code can
 * be compiled against either version without changes.
 *
 * Include this AFTER all system headers.
 */

#pragma once

#ifdef HAVE_GIREPOSITORY_2_0

/* ===== GIInfoType compatibility ===== */

/*
 * GIInfoType was removed in girepository-2.0.  Define our own enum with the
 * same values as v1 so switch statements elsewhere compile unchanged.
 */
typedef enum {
    GI_INFO_TYPE_INVALID   = 0,
    GI_INFO_TYPE_FUNCTION  = 1,
    GI_INFO_TYPE_CALLBACK  = 2,
    GI_INFO_TYPE_STRUCT    = 3,
    GI_INFO_TYPE_BOXED     = 4,
    GI_INFO_TYPE_ENUM      = 5,
    GI_INFO_TYPE_FLAGS     = 6,
    GI_INFO_TYPE_OBJECT    = 7,
    GI_INFO_TYPE_INTERFACE = 8,
    GI_INFO_TYPE_CONSTANT  = 9,
    GI_INFO_TYPE_INVALID_0 = 10,
    GI_INFO_TYPE_UNION     = 11,
    GI_INFO_TYPE_VALUE     = 12,
    GI_INFO_TYPE_SIGNAL    = 13,
    GI_INFO_TYPE_VFUNC     = 14,
    GI_INFO_TYPE_PROPERTY  = 15,
    GI_INFO_TYPE_FIELD     = 16,
    GI_INFO_TYPE_ARG       = 17,
    GI_INFO_TYPE_TYPE      = 18,
    GI_INFO_TYPE_UNRESOLVED = 19,
} GIInfoType;

/* Register our compat GIInfoType as a GType (called from rb_gi_base_info_init). */
GType rb_gi_info_type_get_gtype(void);
#define G_TYPE_I_INFO_TYPE (rb_gi_info_type_get_gtype())

/*
 * Emulate g_base_info_get_type() using the GI_IS_*_INFO() macros that v2
 * provides.  We call the library's native gi_*_info_get_type() functions
 * directly to avoid conflicts with our overridden GI_TYPE_*_INFO macros.
 */
static inline GIInfoType
g_base_info_get_type(GIBaseInfo *info)
{
    /* Most-derived first to avoid false positives on base-class matches. */
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_function_info_get_type()))
        return GI_INFO_TYPE_FUNCTION;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_callback_info_get_type()))
        return GI_INFO_TYPE_CALLBACK;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_struct_info_get_type())) {
        /* In v2, GIBoxedInfo was merged into GIStructInfo.
         * Always return STRUCT so StructInfo Ruby methods work on boxed types. */
        return GI_INFO_TYPE_STRUCT;
    }
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_flags_info_get_type()))
        return GI_INFO_TYPE_FLAGS;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_enum_info_get_type()))
        return GI_INFO_TYPE_ENUM;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_object_info_get_type()))
        return GI_INFO_TYPE_OBJECT;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_interface_info_get_type()))
        return GI_INFO_TYPE_INTERFACE;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_constant_info_get_type()))
        return GI_INFO_TYPE_CONSTANT;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_union_info_get_type()))
        return GI_INFO_TYPE_UNION;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_value_info_get_type()))
        return GI_INFO_TYPE_VALUE;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_signal_info_get_type()))
        return GI_INFO_TYPE_SIGNAL;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_vfunc_info_get_type()))
        return GI_INFO_TYPE_VFUNC;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_property_info_get_type()))
        return GI_INFO_TYPE_PROPERTY;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_field_info_get_type()))
        return GI_INFO_TYPE_FIELD;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_arg_info_get_type()))
        return GI_INFO_TYPE_ARG;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_type_info_get_type()))
        return GI_INFO_TYPE_TYPE;
    if (G_TYPE_CHECK_INSTANCE_TYPE(info, gi_unresolved_info_get_type()))
        return GI_INFO_TYPE_UNRESOLVED;
    return GI_INFO_TYPE_INVALID;
}

static inline const gchar *
g_info_type_to_string(GIInfoType type)
{
    switch (type) {
    case GI_INFO_TYPE_INVALID:   return "invalid";
    case GI_INFO_TYPE_FUNCTION:  return "function";
    case GI_INFO_TYPE_CALLBACK:  return "callback";
    case GI_INFO_TYPE_STRUCT:    return "struct";
    case GI_INFO_TYPE_BOXED:     return "boxed";
    case GI_INFO_TYPE_ENUM:      return "enum";
    case GI_INFO_TYPE_FLAGS:     return "flags";
    case GI_INFO_TYPE_OBJECT:    return "object";
    case GI_INFO_TYPE_INTERFACE: return "interface";
    case GI_INFO_TYPE_CONSTANT:  return "constant";
    case GI_INFO_TYPE_INVALID_0: return "invalid";
    case GI_INFO_TYPE_UNION:     return "union";
    case GI_INFO_TYPE_VALUE:     return "value";
    case GI_INFO_TYPE_SIGNAL:    return "signal";
    case GI_INFO_TYPE_VFUNC:     return "vfunc";
    case GI_INFO_TYPE_PROPERTY:  return "property";
    case GI_INFO_TYPE_FIELD:     return "field";
    case GI_INFO_TYPE_ARG:       return "arg";
    case GI_INFO_TYPE_TYPE:      return "type";
    case GI_INFO_TYPE_UNRESOLVED: return "unresolved";
    default:                     return "unknown";
    }
}

/* ===== GI_TYPE_*_INFO macro overrides =====
 *
 * In v2, GI_TYPE_*_INFO macros point to GTypeInstance-based types that cannot
 * be used with BOXED2RVAL/RVAL2BOXED.  Override them to use our own GBoxed
 * wrappers (rb_gi_*_get_type) declared in rb-gi-types.h.
 */
#undef GI_TYPE_BASE_INFO
#define GI_TYPE_BASE_INFO            (rb_gi_base_info_get_type())
#undef GI_TYPE_CALLABLE_INFO
#define GI_TYPE_CALLABLE_INFO        (rb_gi_callable_info_get_type())
#undef GI_TYPE_FUNCTION_INFO
#define GI_TYPE_FUNCTION_INFO        (rb_gi_function_info_get_type())
#undef GI_TYPE_CALLBACK_INFO
#define GI_TYPE_CALLBACK_INFO        (rb_gi_callback_info_get_type())
#undef GI_TYPE_REGISTERED_TYPE_INFO
#define GI_TYPE_REGISTERED_TYPE_INFO (rb_gi_registered_type_info_get_type())
#undef GI_TYPE_STRUCT_INFO
#define GI_TYPE_STRUCT_INFO          (rb_gi_struct_info_get_type())
#undef GI_TYPE_ENUM_INFO
#define GI_TYPE_ENUM_INFO            (rb_gi_enum_info_get_type())
#undef GI_TYPE_FLAGS_INFO
#define GI_TYPE_FLAGS_INFO           (rb_gi_flags_info_get_type())
#undef GI_TYPE_OBJECT_INFO
#define GI_TYPE_OBJECT_INFO          (rb_gi_object_info_get_type())
#undef GI_TYPE_INTERFACE_INFO
#define GI_TYPE_INTERFACE_INFO       (rb_gi_interface_info_get_type())
#undef GI_TYPE_CONSTANT_INFO
#define GI_TYPE_CONSTANT_INFO        (rb_gi_constant_info_get_type())
#undef GI_TYPE_UNION_INFO
#define GI_TYPE_UNION_INFO           (rb_gi_union_info_get_type())
#undef GI_TYPE_VALUE_INFO
#define GI_TYPE_VALUE_INFO           (rb_gi_value_info_get_type())
#undef GI_TYPE_SIGNAL_INFO
#define GI_TYPE_SIGNAL_INFO          (rb_gi_signal_info_get_type())
#undef GI_TYPE_VFUNC_INFO
#define GI_TYPE_VFUNC_INFO           (rb_gi_vfunc_info_get_type())
#undef GI_TYPE_PROPERTY_INFO
#define GI_TYPE_PROPERTY_INFO        (rb_gi_property_info_get_type())
#undef GI_TYPE_FIELD_INFO
#define GI_TYPE_FIELD_INFO           (rb_gi_field_info_get_type())
#undef GI_TYPE_ARG_INFO
#define GI_TYPE_ARG_INFO             (rb_gi_arg_info_get_type())
#undef GI_TYPE_TYPE_INFO
#define GI_TYPE_TYPE_INFO            (rb_gi_type_info_get_type())
#undef GI_TYPE_UNRESOLVED_INFO
#define GI_TYPE_UNRESOLVED_INFO      (rb_gi_unresolved_info_get_type())

/* GI_TYPE_BOXED_INFO has no v2 equivalent; keep our own GBoxed type. */
#define GI_TYPE_BOXED_INFO           (rb_gi_boxed_info_get_type())

/* "Method" and "Constructor" are Ruby-binding-only subtypes of FunctionInfo. */
#define GI_TYPE_METHOD_INFO          (rb_gi_method_info_get_type())
#define GI_TYPE_CONSTRUCTOR_INFO     (rb_gi_constructor_info_get_type())

/* ===== GIRepository compatibility ===== */

/* In v2, there is no singleton repository; use a cached instance instead. */
GIRepository *rb_gi_get_repository(void);

#define G_TYPE_IREPOSITORY           GI_TYPE_REPOSITORY
#define G_IREPOSITORY(obj)           GI_REPOSITORY(obj)
#define G_IREPOSITORY_ERROR          GI_REPOSITORY_ERROR

/* g_irepository_* → gi_repository_* wrappers */
static inline void
g_irepository_prepend_search_path(const gchar *path)
{
    gi_repository_prepend_search_path(rb_gi_get_repository(), path);
}

static inline GSList *
g_irepository_get_search_path(void)
{
    /* v2 returns const char * const *; convert to GSList for callers. */
    gsize n = 0;
    const char * const *paths = gi_repository_get_search_path(rb_gi_get_repository(), &n);
    GSList *list = NULL;
    for (gsize i = n; i > 0; i--)
        list = g_slist_prepend(list, (gpointer)paths[i - 1]);
    return list;
}

static inline GITypelib *
g_irepository_require(GIRepository *repo,
                       const gchar *namespace_,
                       const gchar *version,
                       GIRepositoryLoadFlags flags,
                       GError **error)
{
    return gi_repository_require(repo, namespace_, version, flags, error);
}

static inline gchar **
g_irepository_get_dependencies(GIRepository *repo, const gchar *namespace_)
{
    gsize n = 0;
    return gi_repository_get_dependencies(repo, namespace_, &n);
}

static inline gchar **
g_irepository_get_loaded_namespaces(GIRepository *repo)
{
    gsize n = 0;
    return gi_repository_get_loaded_namespaces(repo, &n);
}

static inline gint
g_irepository_get_n_infos(GIRepository *repo, const gchar *namespace_)
{
    return (gint)gi_repository_get_n_infos(repo, namespace_);
}

#define g_irepository_get_info       gi_repository_get_info
#define g_irepository_find_by_gtype  gi_repository_find_by_gtype
#define g_irepository_find_by_name   gi_repository_find_by_name
#define g_irepository_get_version    gi_repository_get_version

/* In v2, g_irepository_get_default() returns the cached instance. */
static inline GIRepository *
g_irepository_get_default(void)
{
    return rb_gi_get_repository();
}

/* ===== Base info renames ===== */
#define g_base_info_ref                    gi_base_info_ref
#define g_base_info_unref                  gi_base_info_unref
#define g_base_info_get_name               gi_base_info_get_name
#define g_base_info_get_namespace          gi_base_info_get_namespace
#define g_base_info_get_attribute          gi_base_info_get_attribute
#define g_base_info_iterate_attributes     gi_base_info_iterate_attributes
#define g_base_info_get_container          gi_base_info_get_container
#define g_base_info_is_deprecated          gi_base_info_is_deprecated
#define g_base_info_equal                  gi_base_info_equal

/* ===== Callable info renames ===== */
#define g_callable_info_get_return_type                   gi_callable_info_get_return_type
#define g_callable_info_get_caller_owns                   gi_callable_info_get_caller_owns
#define g_callable_info_may_return_null                   gi_callable_info_may_return_null
#define g_callable_info_skip_return                       gi_callable_info_skip_return
#define g_callable_info_get_n_args                        gi_callable_info_get_n_args
#define g_callable_info_get_arg                           gi_callable_info_get_arg
#define g_callable_info_load_arg                          gi_callable_info_load_arg
#define g_callable_info_load_return_type                  gi_callable_info_load_return_type
#define g_callable_info_is_method                         gi_callable_info_is_method
#define g_callable_info_can_throw_gerror                  gi_callable_info_can_throw_gerror
#define g_callable_info_get_return_attribute              gi_callable_info_get_return_attribute
#define g_callable_info_iterate_return_attributes         gi_callable_info_iterate_return_attributes
#define g_callable_info_get_instance_ownership_transfer   gi_callable_info_get_instance_ownership_transfer
#define g_callable_info_invoke                            gi_callable_info_invoke
#define g_callable_info_create_closure                    gi_callable_info_create_closure
#define g_callable_info_get_closure_native_address        gi_callable_info_get_closure_native_address
#define g_callable_info_destroy_closure                   gi_callable_info_destroy_closure

/* ===== Function info renames ===== */
#define g_function_info_get_symbol   gi_function_info_get_symbol
#define g_function_info_get_flags    gi_function_info_get_flags
#define g_function_info_get_property gi_function_info_get_property
#define g_function_info_get_vfunc    gi_function_info_get_vfunc
#define g_function_info_invoke       gi_function_info_invoke

/* ===== Signal info renames ===== */
#define g_signal_info_get_flags         gi_signal_info_get_flags
#define g_signal_info_get_class_closure gi_signal_info_get_class_closure
#define g_signal_info_true_stops_emit   gi_signal_info_true_stops_emit

/* ===== VFunc info renames ===== */
#define g_vfunc_info_get_flags          gi_vfunc_info_get_flags
#define g_vfunc_info_get_offset         gi_vfunc_info_get_offset
#define g_vfunc_info_get_signal         gi_vfunc_info_get_signal
#define g_vfunc_info_get_invoker        gi_vfunc_info_get_invoker
#define g_vfunc_info_invoke             gi_vfunc_info_invoke

/* ===== Property info renames ===== */
#define g_property_info_get_flags       gi_property_info_get_flags
/* Renamed: g_property_info_get_type(info) → gi_property_info_get_type_info(info) */
#define g_property_info_get_type        gi_property_info_get_type_info
#define g_property_info_get_ownership_transfer gi_property_info_get_ownership_transfer
#define g_property_info_get_getter      gi_property_info_get_getter
#define g_property_info_get_setter      gi_property_info_get_setter

/* ===== Field info renames ===== */
#define g_field_info_get_flags          gi_field_info_get_flags
#define g_field_info_get_offset         gi_field_info_get_offset
#define g_field_info_get_size           gi_field_info_get_size
/* Renamed: g_field_info_get_type(info) → gi_field_info_get_type_info(info) */
#define g_field_info_get_type           gi_field_info_get_type_info
#define g_field_info_get_field          gi_field_info_get_field
#define g_field_info_set_field          gi_field_info_set_field

/* ===== Arg info renames + signature changes ===== */
#define g_arg_info_get_direction           gi_arg_info_get_direction
#define g_arg_info_is_return_value         gi_arg_info_is_return_value
#define g_arg_info_is_optional             gi_arg_info_is_optional
#define g_arg_info_is_caller_allocates     gi_arg_info_is_caller_allocates
#define g_arg_info_may_be_null             gi_arg_info_may_be_null
#define g_arg_info_is_skip                 gi_arg_info_is_skip
#define g_arg_info_get_scope               gi_arg_info_get_scope
/* v1 uses g_arg_info_get_ownership_transfer (since v1.80) */
#define g_arg_info_get_ownership_transfer  gi_arg_info_get_ownership_transfer
/* Renamed: g_arg_info_get_type(info) → gi_arg_info_get_type_info(info) */
#define g_arg_info_get_type                gi_arg_info_get_type_info
/* Renamed: g_arg_info_load_type → gi_arg_info_load_type_info */
#define g_arg_info_load_type               gi_arg_info_load_type_info

/* g_arg_info_get_closure/destroy: v2 changed to (gboolean, *out_idx) signature.
 * Provide wrappers that return gint (-1 when not applicable) like v1 did. */
static inline gint
g_arg_info_get_closure(GIArgInfo *info)
{
    guint idx = 0;
    if (gi_arg_info_get_closure_index(info, &idx))
        return (gint)idx;
    return -1;
}

static inline gint
g_arg_info_get_destroy(GIArgInfo *info)
{
    guint idx = 0;
    if (gi_arg_info_get_destroy_index(info, &idx))
        return (gint)idx;
    return -1;
}

/* ===== Type tag renames ===== */
#define g_type_tag_to_string            gi_type_tag_to_string

/* ===== Type info renames + signature changes ===== */
#define g_type_info_get_tag             gi_type_info_get_tag
#define g_type_info_is_pointer          gi_type_info_is_pointer
#define g_type_info_get_interface       gi_type_info_get_interface
#define g_type_info_get_array_type      gi_type_info_get_array_type
#define g_type_info_is_zero_terminated  gi_type_info_is_zero_terminated
#define g_type_info_get_param_type      gi_type_info_get_param_type
#define g_type_info_get_storage_type    gi_type_info_get_storage_type

/* g_type_info_get_array_length: v2 changed to (gboolean, *out_idx) signature. */
static inline gint
g_type_info_get_array_length(GITypeInfo *info)
{
    guint idx = 0;
    if (gi_type_info_get_array_length_index(info, &idx))
        return (gint)idx;
    return -1;
}

/* g_type_info_get_array_fixed_size: v2 changed to (gboolean, *out_sz) signature. */
static inline gint
g_type_info_get_array_fixed_size(GITypeInfo *info)
{
    size_t sz = 0;
    if (gi_type_info_get_array_fixed_size(info, &sz))
        return (gint)sz;
    return -1;
}

/* ===== Enum info renames ===== */
#define g_enum_info_get_n_values        gi_enum_info_get_n_values
#define g_enum_info_get_value           gi_enum_info_get_value
#define g_enum_info_get_n_methods       gi_enum_info_get_n_methods
#define g_enum_info_get_method          gi_enum_info_get_method
#define g_enum_info_get_storage_type    gi_enum_info_get_storage_type
#define g_enum_info_get_error_domain    gi_enum_info_get_error_domain
#define g_value_info_get_value          gi_value_info_get_value

/* ===== Struct info renames ===== */
#define g_struct_info_get_n_fields      gi_struct_info_get_n_fields
#define g_struct_info_get_field         gi_struct_info_get_field
#define g_struct_info_find_field        gi_struct_info_find_field
#define g_struct_info_get_n_methods     gi_struct_info_get_n_methods
#define g_struct_info_get_method        gi_struct_info_get_method
#define g_struct_info_find_method       gi_struct_info_find_method
#define g_struct_info_get_size          gi_struct_info_get_size
#define g_struct_info_get_alignment     gi_struct_info_get_alignment
#define g_struct_info_is_gtype_struct   gi_struct_info_is_gtype_struct
#define g_struct_info_is_foreign        gi_struct_info_is_foreign
/* Renamed: get_copy_function → get_copy_function_name */
#define g_struct_info_get_copy_function gi_struct_info_get_copy_function_name

/* ===== Union info renames ===== */
#define g_union_info_get_n_fields       gi_union_info_get_n_fields
#define g_union_info_get_field          gi_union_info_get_field
#define g_union_info_get_n_methods      gi_union_info_get_n_methods
#define g_union_info_get_method         gi_union_info_get_method
#define g_union_info_find_method        gi_union_info_find_method
#define g_union_info_is_discriminated   gi_union_info_is_discriminated
#define g_union_info_get_discriminator_type gi_union_info_get_discriminator_type
#define g_union_info_get_discriminator  gi_union_info_get_discriminator
#define g_union_info_get_size           gi_union_info_get_size
#define g_union_info_get_alignment      gi_union_info_get_alignment

/* g_union_info_get_discriminator_offset: v2 changed to (gboolean, *out) signature. */
static inline gint
g_union_info_get_discriminator_offset(GIUnionInfo *info)
{
    size_t offset = 0;
    if (gi_union_info_get_discriminator_offset(info, &offset))
        return (gint)offset;
    return -1;
}

/* ===== Object info renames ===== */
#define g_object_info_get_type_name        gi_object_info_get_type_name
/* Renamed: get_type_init → get_type_init_function_name */
#define g_object_info_get_type_init        gi_object_info_get_type_init_function_name
#define g_object_info_get_abstract         gi_object_info_get_abstract
#define g_object_info_get_fundamental      gi_object_info_get_fundamental
#define g_object_info_get_parent           gi_object_info_get_parent
#define g_object_info_get_n_interfaces     gi_object_info_get_n_interfaces
#define g_object_info_get_interface        gi_object_info_get_interface
#define g_object_info_get_n_fields         gi_object_info_get_n_fields
#define g_object_info_get_field            gi_object_info_get_field
#define g_object_info_get_n_properties     gi_object_info_get_n_properties
#define g_object_info_get_property         gi_object_info_get_property
#define g_object_info_get_n_methods        gi_object_info_get_n_methods
#define g_object_info_get_method           gi_object_info_get_method
#define g_object_info_find_method          gi_object_info_find_method
#define g_object_info_find_method_using_interfaces gi_object_info_find_method_using_interfaces
#define g_object_info_get_n_signals        gi_object_info_get_n_signals
#define g_object_info_get_signal           gi_object_info_get_signal
#define g_object_info_find_signal          gi_object_info_find_signal
#define g_object_info_get_n_vfuncs         gi_object_info_get_n_vfuncs
#define g_object_info_get_vfunc            gi_object_info_get_vfunc
#define g_object_info_find_vfunc           gi_object_info_find_vfunc
#define g_object_info_find_vfunc_using_interfaces gi_object_info_find_vfunc_using_interfaces
#define g_object_info_get_n_constants      gi_object_info_get_n_constants
#define g_object_info_get_constant         gi_object_info_get_constant
#define g_object_info_get_class_struct     gi_object_info_get_class_struct
#define g_object_info_get_ref_function     gi_object_info_get_ref_function_name
#define g_object_info_get_unref_function   gi_object_info_get_unref_function_name
#define g_object_info_get_ref_function_pointer  gi_object_info_get_ref_function_pointer
#define g_object_info_get_unref_function_pointer gi_object_info_get_unref_function_pointer
#define g_object_info_get_set_value_function gi_object_info_get_set_value_function_name
#define g_object_info_get_get_value_function gi_object_info_get_get_value_function_name

/* ===== Interface info renames ===== */
#define g_interface_info_get_n_prerequisites gi_interface_info_get_n_prerequisites
#define g_interface_info_get_prerequisite    gi_interface_info_get_prerequisite
#define g_interface_info_get_n_properties    gi_interface_info_get_n_properties
#define g_interface_info_get_property        gi_interface_info_get_property
#define g_interface_info_get_n_methods       gi_interface_info_get_n_methods
#define g_interface_info_get_method          gi_interface_info_get_method
#define g_interface_info_find_method         gi_interface_info_find_method
#define g_interface_info_get_n_signals       gi_interface_info_get_n_signals
#define g_interface_info_get_signal          gi_interface_info_get_signal
#define g_interface_info_find_signal         gi_interface_info_find_signal
#define g_interface_info_get_n_vfuncs        gi_interface_info_get_n_vfuncs
#define g_interface_info_get_vfunc           gi_interface_info_get_vfunc
#define g_interface_info_find_vfunc          gi_interface_info_find_vfunc
#define g_interface_info_get_n_constants     gi_interface_info_get_n_constants
#define g_interface_info_get_constant        gi_interface_info_get_constant
#define g_interface_info_get_iface_struct    gi_interface_info_get_iface_struct

/* ===== Registered type info renames ===== */
#define g_registered_type_info_get_type_name gi_registered_type_info_get_type_name
/* Renamed: get_type_init → get_type_init_function_name */
#define g_registered_type_info_get_type_init gi_registered_type_info_get_type_init_function_name
#define g_registered_type_info_get_g_type    gi_registered_type_info_get_g_type

/* ===== Constant info renames ===== */
/* Renamed: g_constant_info_get_type(info) → gi_constant_info_get_type_info(info) */
#define g_constant_info_get_type            gi_constant_info_get_type_info
#define g_constant_info_free_value          gi_constant_info_free_value
#define g_constant_info_get_value           gi_constant_info_get_value

#endif /* HAVE_GIREPOSITORY_2_0 */
