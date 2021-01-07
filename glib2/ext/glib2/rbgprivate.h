/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2007-2021  Ruby-GNOME Project Team
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

/*
 * CentOS 7: GLib 2.50
 * CentOS 8: GLib 2.56
 * Ubuntu 16.04: GLib 2.48
 * Ubuntu 18.04: GLib 2.56
 * Ubuntu 20.04: GLib 2.64
 */
#define GLIB_VERSION_MIN_REQUIRED GLIB_VERSION_2_48

#include "rbgobject.h"

#ifndef HAVE_RB_ERRINFO
#  define rb_errinfo() (ruby_errinfo)
#endif

#ifndef HAVE_RB_EXC_NEW_STR
#  define rb_exc_new_str(klass, message) rb_exc_new3(klass, message)
#endif

#ifndef G_VALUE_INIT
#  define G_VALUE_INIT { 0, { { 0 } } }
#endif

G_BEGIN_DECLS

typedef struct {
    VALUE self;
    GObject* gobj;
    const RGObjClassInfo* cinfo;
    gboolean destroyed;
    GHashTable *rb_relatives;
} gobj_holder;

typedef struct {
    gpointer boxed;
    gboolean own;
    GType type;
} boxed_holder;

G_GNUC_INTERNAL extern GPrivate rg_polling_key;
G_GNUC_INTERNAL extern rb_encoding *rbg_filename_encoding;

extern VALUE rbgobj_cEnum;
extern VALUE rbgobj_cFlags;
extern VALUE rbgobj_cBoxed;
extern VALUE rbgobj_cParam;
extern VALUE rbgobj_mInterface;
extern VALUE rbgobj_cObject;

extern VALUE rbgobj_cInstantiatable;
extern VALUE rbgobj_mMetaInterface;
#define cInstantiatable   rbgobj_cInstantiatable
#define mMetaInterface rbgobj_mMetaInterface

extern void rbgobj_define_property_accessors(VALUE klass);
extern void rbgobj_define_action_methods(VALUE klass);

extern void rbgobj_param_spec_initialize(VALUE self, GParamSpec* pspec);
extern void rbgobj_boxed_initialize(VALUE obj, gpointer boxed);

extern GParamSpec* rbgobj_get_param_spec(VALUE obj);
extern GObject* rbgobj_get_gobject(VALUE obj);

extern VALUE rbgobj_get_ruby_object_from_param_spec(GParamSpec* pspec, gboolean alloc);

extern void rbgobj_init_object_class(VALUE klass);
extern void rbgobj_init_flags_class(VALUE klass);
extern void rbgobj_init_enum_class(VALUE klass);
extern void rbgobj_init_interface(VALUE interf);

/* FIXME: should have better name */
extern void rbgobj_instance_call_cinfo_mark(gpointer instance);
extern void rbgobj_instance_call_cinfo_free(gpointer instance);

VALUE rbgutil_generic_s_gtype(VALUE klass);
VALUE rbgutil_generic_gtype(VALUE self);
#define generic_s_gtype rbgutil_generic_s_gtype
#define generic_gtype rbgutil_generic_gtype

extern gboolean rbgobj_convert_has_type(GType type);
extern RGConvertTable *rbgobj_convert_lookup(GType type);

extern gboolean rbgobj_convert_get_superclass(GType type, VALUE *result);
extern gboolean rbgobj_convert_type_init_hook(GType type, VALUE klass);
extern gboolean rbgobj_convert_gvalue2rvalue(GType type, const GValue *value,
                                             VALUE *result);
extern gboolean rbgobj_convert_rvalue2gvalue(GType type, VALUE val,
                                             GValue *result);
extern GType rbgobj_convert_rvalue2gtype(VALUE val);
extern gboolean rbgobj_convert_initialize(GType type, VALUE obj, gpointer cobj);
extern gboolean rbgobj_convert_robj2instance(GType type, VALUE obj,
                                             gpointer *result);
extern gboolean rbgobj_convert_instance2robj(GType type, gpointer instance,
                                             VALUE *result);
extern gboolean rbgobj_convert_unref(GType type, gpointer instance);

#define RubyGObjectHookModule "RubyGObjectHook__"

G_GNUC_INTERNAL VALUE rg_enum_resolve_value(VALUE klass, VALUE nick);
G_GNUC_INTERNAL void rg_enum_add_constants(VALUE mod, GType enum_type, const gchar *strip_prefix);
G_GNUC_INTERNAL void rg_flags_add_constants(VALUE mod, GType flags_type, const gchar *strip_prefix);
G_GNUC_INTERNAL char *rg_obj_constant_lookup(const char *name);

G_GNUC_INTERNAL VALUE rbg_gc_marker_new(void);
G_GNUC_INTERNAL void rbg_gc_marker_guard(VALUE rb_gc_marker,
                                         VALUE rb_object);
G_GNUC_INTERNAL void rbg_gc_marker_unguard(VALUE rb_gc_marker,
                                           VALUE rb_object);
G_GNUC_INTERNAL void rbg_gc_marker_unguard_all(VALUE rb_gc_marker);

G_GNUC_INTERNAL void Init_glib_gc(void);

G_GNUC_INTERNAL void Init_gutil(void);
G_GNUC_INTERNAL void Init_gutil_callback(void);
G_GNUC_INTERNAL void Init_glib_gettext(void);
G_GNUC_INTERNAL void Init_glib_int64(void);
G_GNUC_INTERNAL void Init_glib_error(void);
G_GNUC_INTERNAL void Init_glib_error_conversions(void);
G_GNUC_INTERNAL void Init_glib_threads(void);
G_GNUC_INTERNAL void Init_glib_convert(void);
G_GNUC_INTERNAL void Init_glib_messages(void);
G_GNUC_INTERNAL void Init_glib_spawn(void);
G_GNUC_INTERNAL void Init_glib_spawnerror(void);
G_GNUC_INTERNAL void Init_glib_fileutils(void);
G_GNUC_INTERNAL void Init_glib_utils(void);
G_GNUC_INTERNAL void Init_glib_i18n(void);
G_GNUC_INTERNAL void Init_glib_win32(void);
G_GNUC_INTERNAL void Init_gobject(void);
G_GNUC_INTERNAL void Init_glib_main_loop(void);
G_GNUC_INTERNAL void Init_glib_main_context(void);
G_GNUC_INTERNAL void Init_glib_source(void);
G_GNUC_INTERNAL void Init_glib_poll_fd(void);
G_GNUC_INTERNAL void Init_glib_io_constants(void);
G_GNUC_INTERNAL void Init_glib_io_channel(void);
G_GNUC_INTERNAL void Init_glib_io_channelerror(void);
G_GNUC_INTERNAL void Init_glib_io_channel_win32_socket(void);
G_GNUC_INTERNAL void Init_glib_shell(void);
G_GNUC_INTERNAL void Init_glib_shellerror(void);
G_GNUC_INTERNAL void Init_glib_timer(void);
G_GNUC_INTERNAL void Init_glib_unicode(void);
G_GNUC_INTERNAL void Init_glib_utf8(void);
G_GNUC_INTERNAL void Init_glib_utf16(void);
G_GNUC_INTERNAL void Init_glib_ucs4(void);
G_GNUC_INTERNAL void Init_glib_unichar(void);
G_GNUC_INTERNAL void Init_glib_keyfile(void);
G_GNUC_INTERNAL void Init_glib_bookmark_file(void);
G_GNUC_INTERNAL void Init_glib_variant_type(void);
G_GNUC_INTERNAL void Init_glib_variant(void);
G_GNUC_INTERNAL void Init_glib_regex(void);
G_GNUC_INTERNAL void Init_glib_matchinfo(void);
G_GNUC_INTERNAL void Init_glib_date_time(void);
G_GNUC_INTERNAL void Init_glib_time_zone(void);
G_GNUC_INTERNAL void Init_glib_bytes(void);

G_GNUC_INTERNAL void Init_gobject_convert(void);
G_GNUC_INTERNAL void Init_gobject_gtype(void);
G_GNUC_INTERNAL void Init_gobject_typeinterface(void);
G_GNUC_INTERNAL void Init_gobject_typeinstance(void);
G_GNUC_INTERNAL void Init_gobject_gvalue(void);
G_GNUC_INTERNAL void Init_gobject_gvaluetypes(void);
G_GNUC_INTERNAL void Init_gobject_gboxed(void);
G_GNUC_INTERNAL void Init_gobject_gstrv(void);
G_GNUC_INTERNAL void Init_gobject_genumflags(void);
G_GNUC_INTERNAL void Init_gobject_genums(void);
G_GNUC_INTERNAL void Init_gobject_gflags(void);
G_GNUC_INTERNAL void Init_gobject_gparam(void);
G_GNUC_INTERNAL void Init_gobject_gparamspecs(void);
G_GNUC_INTERNAL void Init_gobject_gclosure(void);
G_GNUC_INTERNAL void Init_gobject_gobject(void);
G_GNUC_INTERNAL void Init_gobject_gsignal(void);
G_GNUC_INTERNAL void Init_gobject_gtypeplugin(void);
G_GNUC_INTERNAL void Init_gobject_gtypemodule(void);
G_GNUC_INTERNAL void Init_gobject_gbinding(void);

G_END_DECLS
