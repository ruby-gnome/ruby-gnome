/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************
  rbgprivate.h -

  Copyright (C) 2007  Ruby-GNOME2 Project Team
**********************************************************************/

#ifndef __RBGPRIVATE_H__
#define __RBGPRIVATE_H__

#include "rbgobject.h"
#include "glib-enum-types.h"

G_BEGIN_DECLS

typedef struct {
    VALUE self;
    GObject* gobj;
    const RGObjClassInfo* cinfo;
    gboolean destroyed;
} gobj_holder;

typedef struct {
    gpointer boxed;
    gboolean own;
    GType type;
} boxed_holder;

extern VALUE rbgobj_cInstantiatable;
extern VALUE rbgobj_mMetaInterface;
#define cInstantiatable   rbgobj_cInstantiatable
#define mMetaInterface rbgobj_mMetaInterface

extern void rbgobj_define_property_accessors(VALUE klass);
extern void rbgobj_define_action_methods(VALUE klass);

extern void rbgobj_param_spec_initialize(VALUE self, GParamSpec* pspec);
extern void rbgobj_boxed_initialize(VALUE obj, gpointer boxed);
extern void rbgobj_gobject_initialize(VALUE obj, gpointer cobj);

extern GParamSpec* rbgobj_get_param_spec(VALUE obj);
extern GObject* rbgobj_get_gobject(VALUE obj);

extern VALUE rbgobj_get_value_from_gobject(GObject* gobj, gboolean alloc);
extern VALUE rbgobj_get_value_from_param_spec(GParamSpec* pspec, gboolean alloc);

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
extern gboolean rbgobj_convert_initialize(GType type, VALUE obj, gpointer cobj);
extern gboolean rbgobj_convert_robj2instance(GType type, VALUE obj,
                                             gpointer *result);
extern gboolean rbgobj_convert_instance2robj(GType type, gpointer instance,
                                             VALUE *result);
extern gboolean rbgobj_convert_unref(GType type, gpointer instance);

#define RubyGObjectHookModule "RubyGObjectHook__"

G_END_DECLS

#endif
