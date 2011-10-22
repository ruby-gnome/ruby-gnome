/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003,2006  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

#ifndef __RBGOBJECT_H__
#define __RBGOBJECT_H__

#include <glib-object.h>
#include "ruby.h"
#include "rbglib.h"
#include "rbgutil.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* macros */

#define G_INITIALIZE(obj, cobj)\
 (rbgobj_initialize_object(obj, (gpointer)cobj))

#define G_DEF_CLASS(gtype, name, module)\
    (rbgobj_define_class(gtype, name, module, 0, 0, Qnil))
#define G_DEF_CLASS_WITH_GC_FUNC(gtype, name, module, mark, free)	\
    (rbgobj_define_class(gtype, name, module, mark, free, Qnil))
#define G_DEF_CLASS2(gtype, name, module, mark, free)			\
    G_DEF_CLASS_WITH_GC_FUNC(gtype, name, module, mark, free)
#define G_DEF_CLASS3(gtype_name, name, module)\
    (rbgobj_define_class_dynamic(gtype_name, name, module, 0, 0))
#define G_DEF_CLASS4(gtype, name, module, parent) \
    (rbgobj_define_class(gtype, name, module, 0, 0, parent))

#define G_DEF_INTERFACE(gtype, name, module)\
    (rbgobj_define_class(gtype, name, module, 0, 0, Qnil))
#define G_DEF_INTERFACE2(gtype, name, module, mark, free)\
    (rbgobj_define_class(gtype, name, module, mark, free, Qnil))

#define RG_DEF_CONVERSION(table) (rbgobj_convert_define(table))

#define G_RELATIVE(obj, rel) (rbgobj_add_relative(obj, rel))

/* G_RELATIVE2 is useless now. Try G_CHILD_ADD/REMOVE first. */
#define G_RELATIVE2(obj, rel, id, hash_key)\
 (rbgobj_add_relative_removable(obj, rel, id, hash_key))
#define G_GET_RELATIVE(obj, id, hash_key)\
 (rbgobj_get_relative_removable(obj, id, hash_key))
#define G_REMOVE_RELATIVE(obj, id, hash_key)\
 (rbgobj_remove_relative(obj, id, hash_key))

RUBY_GLIB2_VAR  ID rbgobj_id_children;
#define G_CHILD_SET(self, id, child)  (rb_ivar_set(self, id, child))
#define G_CHILD_UNSET(self, id)  (rb_ivar_set(self, id, Qnil))

/* G_CHILD_ADD is same as G_RELATIVE, but the macro name is more obviously
   to use than G_RELATIVE, and also support "remove" operation with the key
   which is the object itself.
*/
#define G_CHILD_ADD(self, child) \
    (rbgobj_add_relative_removable(self, Qnil, rbgobj_id_children, child))
#define G_CHILD_REMOVE(self, child) \
    (rbgobj_remove_relative(self, rbgobj_id_children, child))
#define G_CHILD_REMOVE_ALL(self) \
    (rbgobj_remove_relative_all(self, rbgobj_id_children))

#define G_DEF_SIGNAL_FUNC(klass, sig_name, func)\
 (rbgobj_set_signal_func(klass, sig_name, func))

#define CLASS2CINFO(klass) (rbgobj_lookup_class(klass))
#define GTYPE2CINFO(gtype) (rbgobj_lookup_class_by_gtype(gtype, Qnil))
#define GTYPE2CINFO_NO_CREATE(gtype) (rbgobj_lookup_class_by_gtype_full(gtype, Qnil, FALSE))
#define RVAL2CINFO(obj)    (rbgobj_lookup_class(CLASS_OF(obj)))
#define GTYPE2CLASS(gtype) (rbgobj_gtype_to_ruby_class(gtype))
#define CLASS2GTYPE(klass) (rbgobj_lookup_class(klass)->gtype)
#define RVAL2GTYPE(obj)    (CLASS2GTYPE(CLASS_OF(obj)))

#define RVAL2GOBJ(obj)  (rbgobj_instance_from_ruby_object(obj))
#define GOBJ2RVAL(gobj) (rbgobj_ruby_object_from_instance(gobj))
#define GOBJ2RVAL_UNREF(gobj) (rbgobj_ruby_object_from_instance_with_unref(gobj))
#define GOBJ2RVALU(gobj) GOBJ2RVAL_UNREF(gobj)
#define GVAL2RVAL(v)    (rbgobj_gvalue_to_rvalue(v))
#define GVAL2RVAL_UNSET(v) (rbgobj_gvalue_to_rvalue(v))

#define RVAL2BOXED(obj, gtype)  (rbgobj_boxed_get(obj, gtype))
#define BOXED2RVAL(cobj, gtype) (rbgobj_make_boxed(cobj, gtype))

#define RVAL2GENUM(obj, gtype)  (rbgobj_get_enum(obj, gtype))
#define RVAL2GFLAGS(obj, gtype) (rbgobj_get_flags(obj, gtype))
#define GENUM2RVAL(n, gtype)    (rbgobj_make_enum(n, gtype))
#define GFLAGS2RVAL(n, gtype)   (rbgobj_make_flags(n, gtype))

#define RVAL2GPTR(object)       (rbgobj_ptr2cptr(object))
#define GPTR2RVAL(ptr, gtype)   (rbgobj_ptr_new(gtype, ptr))

#define G_DEF_CONSTANTS(mod, type, strip_prefix) \
	rbgobj_add_constants(mod, type, strip_prefix)
#define G_RENAME_CONSTANT(orig, alt) \
	rbgobj_constant_remap(orig, alt)
#define G_RENAME_NICK(orig, alt) \
	rbgobj_constant_remap(orig, alt)

typedef enum
{
    RBGOBJ_ABSTRACT_BUT_CREATABLE = 1 << 0, /* deprecated */
    RBGOBJ_BOXED_NOT_COPY         = 1 << 1,
    RBGOBJ_DEFINED_BY_RUBY        = 1 << 2,
} RGObjClassFlag;
    
typedef struct {
    VALUE klass;
    GType gtype;
    void (*mark)(gpointer);
    void (*free)(gpointer);
    int flags; /* RGObjClassFlag */
} RGObjClassInfo;

/* rbgobject.c */

extern void rbgobj_initialize_object(VALUE obj, gpointer cobj);
extern gpointer rbgobj_instance_from_ruby_object(VALUE obj);
extern VALUE rbgobj_ruby_object_from_instance(gpointer instance);
extern VALUE rbgobj_ruby_object_from_instance2(gpointer instance, gboolean alloc);
extern VALUE rbgobj_ruby_object_from_instance_with_unref(gpointer instance);

extern void rbgobj_add_relative(VALUE obj, VALUE relative);
extern void rbgobj_invalidate_relatives(VALUE obj);
extern void rbgobj_add_relative_removable(VALUE obj, VALUE relative,
                                          ID obj_ivar_id, VALUE hash_key);
extern VALUE rbgobj_get_relative_removable(VALUE obj, ID obj_ivar_id,
                                           VALUE hash_key);
extern void rbgobj_remove_relative(VALUE obj, ID obj_ivar_id, VALUE hash_key);
extern void rbgobj_remove_relative_all(VALUE obj, ID obj_ivar_id);

extern GObject* rbgobj_gobject_new(GType type, VALUE params_hash);
extern VALUE rbgobj_create_object(VALUE klass); /* deprecated */

extern VALUE rbgobj_get_ruby_object_from_gobject(GObject* gobj, gboolean alloc);

/* For Ruby/Gstreamer */
extern void rbgobj_gobject_initialize(VALUE obj, gpointer cobj);

/* deprecated */
extern void rbgobj_add_abstract_but_create_instance_class(GType gtype);

/* rbgobj_typeinstance.c */
extern void rbgobj_gc_mark_instance(gpointer instance);


/* rbgobj_type.c */
extern const RGObjClassInfo *rbgobj_lookup_class(VALUE klass);
extern const RGObjClassInfo *rbgobj_lookup_class_by_gtype(GType gtype, VALUE parent);
extern const RGObjClassInfo *rbgobj_lookup_class_by_gtype_full(GType gtype,
							       VALUE parent,
							       gboolean create_object);
extern VALUE rbgobj_gtype_to_ruby_class(GType gtype);
extern VALUE rbgobj_define_class(GType gtype, const gchar* name, VALUE module,
                                 void* mark, void* free, VALUE parent); 
extern VALUE rbgobj_define_class_dynamic(const gchar* gtype_name, 
                                         const gchar* name, VALUE module, 
                                         void* mark, void* free); 
extern void rbgobj_register_class(VALUE klass,
                                  GType gtype,
                                  gboolean klass2gtype,
                                  gboolean gtype2klass);
extern VALUE rbgobj_cType;
extern VALUE rbgobj_gtype_new(GType gtype);
extern GType rbgobj_gtype_get(VALUE obj);

/* rbgobj_signal.c */
typedef VALUE (*GValToRValSignalFunc)(guint num,const GValue* values);
extern void rbgobj_set_signal_func(VALUE klass, const gchar *sig_name, GValToRValSignalFunc func);
extern GValToRValSignalFunc rbgobj_get_signal_func(guint signal_id);
extern VALUE rbgobj_signal_wrap(guint sig_id);


/* rbgobj_closure.c */
extern GClosure* g_rclosure_new(VALUE callback_proc, VALUE extra_args,
                                GValToRValSignalFunc func);
extern void g_rclosure_attach(GClosure *closure, VALUE object);
extern void g_rclosure_set_tag(GClosure *closure, const gchar *tag);

/* rbgobj_value.c */
extern VALUE rbgobj_gvalue_to_rvalue(const GValue* value);
extern VALUE rbgobj_gvalue_to_rvalue_unset(GValue *value);
extern void rbgobj_rvalue_to_gvalue(VALUE val, GValue* result);
extern void rbgobj_initialize_gvalue(GValue *result, VALUE value);

typedef void (*RValueToGValueFunc)(VALUE from, GValue* to);
typedef VALUE (*GValueToRValueFunc)(const GValue* from);
extern void rbgobj_register_r2g_func(GType gtype, RValueToGValueFunc func);
extern void rbgobj_register_g2r_func(GType gtype, GValueToRValueFunc func);

extern void rbgobj_gc_mark_gvalue(GValue* value);

/* rbgobj_valuetypes.c */
extern VALUE rbgobj_ptr_new(GType type, gpointer ptr);
extern gpointer rbgobj_ptr2cptr(VALUE ptr);

#define RBGOBJ_TYPE_RUBY_VALUE (rbgobj_ruby_value_get_type())
extern GType rbgobj_ruby_value_get_type(void);
extern VALUE g_value_get_ruby_value(const GValue* value);
extern void g_value_set_ruby_value(GValue* value, VALUE ruby);

/* rbgobj_object.c */
extern void rbgobj_register_property_setter(GType gtype, const char* prop_name, RValueToGValueFunc func);
extern void rbgobj_register_property_getter(GType gtype, const char* prop_name, GValueToRValueFunc func);

/* rbgobj_boxed.c */
extern VALUE rbgobj_boxed_create(VALUE klass); /* deprecated */
extern gpointer rbgobj_boxed_get(VALUE obj, GType gtype);
extern VALUE rbgobj_make_boxed(gpointer data, GType gtype);
extern void rbgobj_boxed_not_copy_obj(GType gtype);

/* rbgobj_enums.c */
extern void rbgobj_constant_remap(const char *original, const char *replacement);
extern void rbgobj_add_constants(VALUE mod, GType type, const gchar *strip_prefix);
extern VALUE rbgobj_make_enum(gint n, GType gtype);
extern gint rbgobj_get_enum(VALUE obj, GType gtype);
extern VALUE rbgobj_make_flags(guint n, GType gtype);
extern guint rbgobj_get_flags(VALUE obj, GType gtype);
extern void rbgobj_define_const(VALUE mod, const char *name, VALUE value);


/* rbglib_mainloop.c */
#define G_TYPE_MAIN_LOOP (g_main_loop_get_type())
extern GType g_main_loop_get_type(void);

/* rbglib_maincontext.c */
#define G_TYPE_MAIN_CONTEXT (g_main_context_get_type())
#define G_TYPE_SOURCE (g_source_get_type())
#define G_TYPE_POLL_FD (g_poll_fd_get_type())
extern GType g_main_context_get_type(void);
extern GType g_source_get_type(void);
extern GType g_poll_fd_get_type(void);

/* rbglib_keyfile.c */
#define G_TYPE_KEY_FILE (g_key_file_get_type())
extern GType g_key_file_get_type(void);

/* rbgobj_convert.c */
typedef struct {
    GType type;
    VALUE klass;
    VALUE (*get_superclass)(void);
    void (*type_init_hook)(VALUE);
    void (*rvalue2gvalue)(VALUE val, GValue *result);
    VALUE (*gvalue2rvalue)(const GValue *);
    void (*initialize)(VALUE, gpointer);
    gpointer (*robj2instance)(VALUE);
    VALUE (*instance2robj)(gpointer);
    void (*unref)(gpointer instance);
} RGConvertTable;

extern void rbgobj_convert_define(RGConvertTable *table);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGOBJECT_H__ */

#include "rbgcompat.h"
