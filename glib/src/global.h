/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

   global.h -

   $Author: sakai $
   $Date: 2003/04/07 11:26:29 $

   Copyright (C) 2002,2003  Masahiro Sakai
************************************************/

#include "rbgobject.h"

extern VALUE rbglib_int64_to_num(guint64 val);
extern VALUE rbglib_uint64_to_num(guint64 val);
extern gint64 rbglib_num_to_int64(VALUE val);
extern guint64 rbglib_num_to_uint64(VALUE val);


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
#ifdef RBGLIB_ENABLE_EXPERIMENTAL
extern void rbgobj_define_action_methods(VALUE klass);
#endif

extern void rbgobj_param_spec_initialize(VALUE self, GParamSpec* pspec);
extern void rbgobj_boxed_initialize(VALUE obj, gpointer boxed);
extern void rbgobj_gobject_initialize(VALUE obj, gpointer cobj);

extern GParamSpec* rbgobj_param_spec_get_struct(VALUE obj);
extern GObject* rbgobj_get_gobject(VALUE obj);

extern VALUE rbgobj_get_value_from_gobject(GObject* gobj);
extern VALUE rbgobj_get_value_from_param_spec(GParamSpec* pspec);
