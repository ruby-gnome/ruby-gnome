/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobject.h -

  $Author: sakai $
  $Date: 2002/09/01 13:19:22 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#ifndef __RBGOBJECT_H__
#define __RBGOBJECT_H__

#include <glib-object.h>
#include "ruby.h"
#include "rbglib.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* macros */

#define RBGOBJ_INITIALIZE(obj, cobj)\
 (rbgobj_initialize_object(obj, (gpointer)cobj))

#define G_DEF_CLASS(gtype, name, module)\
 (rbgobj_define_class(gtype, name, module, 0, 0))
#define G_DEF_CLASS2(gtype, name, module, mark, free)\
 (rbgobj_define_class(gtype, name, module, mark, free))

#define G_DEF_INTERFACE(gtype, name, module)\
 (rbgobj_define_class(gtype, name, module, 0, 0))
#define G_DEF_INTERFACE2(gtype, name, module, mark, free)\
 (rbgobj_define_class(gtype, name, module, mark, free))

#define CLASS2CINFO(klass) (rbgobj_lookup_class(klass))
#define GTYPE2CINFO(gtype) (rbgobj_lookup_class_by_gtype(gtype))
#define RVAL2CINFO(obj)    (rbgobj_lookup_class(CLASS_OF(obj)))
#define GTYPE2CLASS(gtype) (GTYPE2CINFO(gtype)->klass)
#define CLASS2GTYPE(klass) (rbgobj_lookup_class(klass)->gtype)
#define RVAL2GTYPE(obj)    (CLASS2GTYPE(CLASS_OF(obj)))

#define RVAL2GOBJ(obj)  (rbgobj_get_gobject(obj))
#define GOBJ2RVAL(gobj) (rbgobj_get_value_from_gobject(G_OBJECT(gobj)))
#define GVAL2RVAL(v)    (rbgobj_gvalue_to_rvalue((GValue*)v))

#define RVAL2BOXED(obj)         (rbgobj_boxed_get(obj))
#define BOXED2RVAL(cobj, gtype) (rbgobj_make_boxed(cobj, gtype))

typedef struct {
    VALUE klass;
    GType gtype;
    void (*mark)(gpointer);
    void (*free)(gpointer);
} RGObjClassInfo;

typedef struct {
    VALUE self;
    GObject* gobj;
    const RGObjClassInfo* cinfo;
    gboolean destroyed;
} gobj_holder;

typedef struct {
    gpointer boxed;
    GType type;
} boxed_holder;

/* rbgobject.c */
extern ID id_relatives;

extern GObject* rbgobj_get_gobject(VALUE obj);
extern void rbgobj_initialize_object(VALUE obj, gpointer cobj);
extern VALUE rbgobj_get_value_from_gobject(GObject* gobj);

extern void rbgobj_add_relative(VALUE obj, VALUE relative);
extern void rbgobj_add_relative_removable(VALUE obj, VALUE relative,
                                          ID obj_ivar_id, VALUE hash_key);
extern void rbgobj_remove_relative(VALUE obj, ID obj_ivar_id, VALUE hash_key);
extern GObject* rbgobj_gobject_new(GType type, VALUE params_hash);
extern void rbgobj_define_property_accessors(VALUE klass);
extern void rbgobj_define_signal_constants(VALUE klass);
extern VALUE rbgobj_create_object(VALUE klass);

/* rbgobj_type.c */
extern const RGObjClassInfo* rbgobj_lookup_class(VALUE klass);
extern const RGObjClassInfo* rbgobj_lookup_class_by_gtype(GType gtype);
extern VALUE rbgobj_define_class(GType gtype, gchar* name, VALUE module,
                                 void* mark, void* free); 
extern VALUE rbgobj_cType;
extern VALUE rbgobj_gtype_new(GType gtype);
extern GType rbgobj_gtype_get(VALUE obj);


/* rbgobj_closure.c */
extern GClosure* g_rclosure_new(VALUE callback_proc, VALUE extra_args);

/* rbgobj_value.c */
extern VALUE rbgobj_gvalue_to_rvalue(const GValue* value);
extern void rbgobj_rvalue_to_gvalue(VALUE val, GValue* result);

typedef void (*RValueToGValueFunc)(VALUE from, GValue* to);
typedef VALUE (*GValueToRValueFunc)(const GValue* from);
extern void rbgobj_register_r2g_func(GType gtype, RValueToGValueFunc func);
extern void rbgobj_register_g2r_func(GType gtype, GValueToRValueFunc func);

/* rbgobj_valuetypes.c */
extern VALUE rbgobj_ptr_new(GType type, gpointer ptr);
extern gpointer rbgobj_ptr2cptr(VALUE ptr);

#define RBGOBJ_TYPE_RUBY_VALUE (rbgobj_ruby_value_get_type())
extern GType rbgobj_ruby_value_get_type();
extern VALUE g_value_get_ruby_value(const GValue* value);
extern void g_value_set_ruby_value(GValue* value, VALUE ruby);

/* rbgobj_param.c */
extern GParamSpec* rbgobj_param_spec_get_struct(VALUE obj);
extern void rbgobj_param_spec_initialize(VALUE self, GParamSpec* pspec);
extern VALUE rbgobj_get_value_from_param_spec(GParamSpec* pspec);

/* rbgobj_boxed.c */
extern VALUE rbgobj_boxed_create(VALUE klass);
extern void rbgobj_boxed_initialize(VALUE obj, gpointer boxed);
extern gpointer rbgobj_boxed_get(VALUE obj);
extern VALUE rbgobj_make_boxed(gpointer data, GType gtype);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGOBJECT_H__ */
