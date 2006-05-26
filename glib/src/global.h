/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

   global.h -

   $Author: ssimons $
   $Date: 2006/05/26 18:57:44 $

   Copyright (C) 2002,2003  Masahiro Sakai
   Copyright (C) 2004-2006  Ruby-GNOME2 Project Team
************************************************/

#include "rbgobject.h"

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

extern GParamSpec* rbgobj_param_spec_get_struct(VALUE obj);
extern GObject* rbgobj_get_gobject(VALUE obj);

extern VALUE rbgobj_get_value_from_gobject(GObject* gobj);
extern VALUE rbgobj_get_value_from_param_spec(GParamSpec* pspec);

extern void rbgobj_init_flags_class(VALUE klass);
extern void rbgobj_init_enum_class(VALUE klass);


#ifndef HAVE_OBJECT_ALLOCATE
VALUE rbgutil_generic_s_new(int argc, VALUE* argv, VALUE self);
#define generic_s_new rbgutil_generic_s_new
#endif

VALUE rbgutil_generic_s_gtype(VALUE klass);
VALUE rbgutil_generic_gtype(VALUE self);
#define generic_s_gtype rbgutil_generic_s_gtype
#define generic_gtype rbgutil_generic_gtype

#define RubyGObjectHookModule "RubyGObjectHook__"
