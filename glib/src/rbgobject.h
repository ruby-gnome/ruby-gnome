/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobject.h -

  $Author: sakai $
  $Date: 2002/08/05 17:45:57 $

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
#define RBGOBJ_INITIALIZE(obj, gobj)\
 (rbgobj_initialize_gobject(obj, G_OBJECT(gobj)))
#define G_DEF_CLASS(gtype, name, module)\
 (rbgobj_define_class(gtype, name, module, 0, 0))
#define G_DEF_CLASS2(gtype, name, module, mark, free)\
 (rbgobj_define_class(gtype, name, module, mark, free))

#define G_DEF_INTERFACE(gtype, name, module)\
 (rbgobj_define_class(gtype, name, module, 0, 0))
#define G_DEF_INTERFACE2(gtype, name, module, mark, free)\
 (rbgobj_define_class(gtype, name, module, mark, free))

#define GTYPE2CLASS(gtype) (rbgobj_lookup_class_by_gtype(gtype)->klass)
#define RVAL2GOBJ(obj) (rbgobj_get_gobject(obj))
#define GOBJ2RVAL(gobj) (rbgobj_get_value_from_gobject(G_OBJECT(gobj)))
#define GVAL2RVAL(v) rbgobj_gvalue_to_rvalue((GValue*)v)

typedef struct {
    VALUE klass;
    GType gtype;
    void (*mark)(GObject *);
    void (*free)(GObject *);
} RGObjClassInfo;

typedef struct {
    VALUE self;
    GObject* gobj;
    const RGObjClassInfo* cinfo;
    gboolean destroyed;
} gobj_holder;

/* rbgobject.c */
extern ID id_relatives;
extern ID id_relative_callbacks;

extern GObject* rbgobj_get_gobject(VALUE obj);
extern void rbgobj_initialize_gobject(VALUE obj, GObject* gobj);
extern VALUE rbgobj_get_value_from_gobject(GObject* gobj);

extern void rbgobj_add_relative(VALUE obj, VALUE relative);
extern void rbgobj_add_relative_removable(VALUE obj, VALUE relative,
                                             ID obj_ivar_id, VALUE hash_key);
extern void rbgobj_remove_relative(VALUE obj, ID obj_ivar_id, VALUE hash_key);
extern GObject* rbgobj_gobject_new(VALUE type, VALUE params_hash);
extern void rbgobj_define_property_accessors(VALUE klass);
extern void rbgobj_define_signal_constants(VALUE klass);
extern VALUE rbgobj_create_object(VALUE klass);

/* rbgobj_type.c */
extern const RGObjClassInfo* rbgobj_lookup_class(VALUE klass);
extern const RGObjClassInfo* rbgobj_lookup_class_by_gtype(GType gtype);
extern VALUE rbgobj_define_class(GType gtype, gchar* name, VALUE module,
                                 void* mark, void* free); 

extern VALUE rbgobj_cParamSpec;
extern VALUE rbgobj_cBoxed;


/* rbgobj_closure.c */
extern GClosure* g_rclosure_new(VALUE callback_proc, VALUE extra_args);

/* rbgobj_value.c */
typedef void (*RValueToGValueFunc)(VALUE from, GValue* to);
typedef VALUE (*GValueToRValueFunc)(const GValue* from);
extern void rbgobj_register_r2g_func(VALUE klass, RValueToGValueFunc func);
extern void rbgobj_register_g2r_func(GType gtype, GValueToRValueFunc func);
extern VALUE rbgobj_gvalue_to_rvalue(const GValue* value);
extern void rbgobj_rvalue_to_gvalue(VALUE val, GValue* result);

/* rbgobj_param.c */
extern GParamSpec* rbgobj_param_spec_get_struct(VALUE obj);
extern VALUE rbgobj_param_spec_wrap(GParamSpec* pspec);

/* rbgobj_boxed.c */
extern VALUE rbgobj_boxed_class(GType gtype);
extern GType rbgobj_boxed_class_gtype(VALUE klass);
extern gpointer rbgobj_boxed_get(VALUE obj);
extern VALUE rbgobj_make_boxed(gpointer data, GType gtype);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGOBJECT_H__ */
