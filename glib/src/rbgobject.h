/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobject.h -

  $Author: sakai $
  $Date: 2002/07/26 14:31:34 $

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

/* rbgobj_type.c */

typedef struct _RGObjClassInfo RGObjClassInfo;
struct _RGObjClassInfo {
    VALUE klass;
    GType gtype;
    void (*mark)(GObject *);
    void (*free)(GObject *);
};

extern void rbgobj_register_class(const RGObjClassInfo* cinfo);
extern const RGObjClassInfo* rbgobj_lookup_class(VALUE klass);
extern const RGObjClassInfo* rbgobj_lookup_class_by_gtype(GType gtype);
extern VALUE rbgobj_lookup_rbclass(const GObject* gobj);

extern ID id_class_info;
extern ID id_gobject_data;
extern ID id_relatives;
extern ID id_relative_callbacks;
extern ID id_delete;

#define RVAL2GOBJ(obj) (rbgobj_get_gobject(obj))
#define GOBJ2RVAL(gobj) (rbgobj_get_value_from_gobject(G_OBJECT(gobj)))
#define RBGOBJ_INITIALIZE(obj, gobj) (rbgobj_initialize_gobject(obj, G_OBJECT(gobj)))

extern GObject* rbgobj_get_gobject(VALUE obj);
extern GObject* rbgobj_force_get_gobject(VALUE obj);
extern void rbgobj_initialize_gobject(VALUE obj, GObject* gobj);
extern VALUE rbgobj_get_value_from_gobject(GObject* gobj);

extern void rbgobj_add_relative(VALUE obj, VALUE relative);
extern void rbgobj_add_relative_removable(VALUE obj, VALUE relative,
                                             ID obj_ivar_id, VALUE hash_key);
extern void rbgobj_remove_relative(VALUE obj, ID obj_ivar_id, VALUE hash_key);

extern GTypeClass* rbgobj_type_class_get_struct(VALUE obj);
extern GTypeInstance* rbgobj_type_instance_get_struct(VALUE obj);

extern VALUE rbgobj_cGObject;
extern VALUE rbgobj_cParamSpec;

/* rbgobj_closure.c */
extern GClosure* g_rclosure_new(VALUE callback_proc);

/* rbgobj_value.c */
typedef void (*RValueToGValueFunc)(VALUE from, GValue* to);
typedef VALUE (*GValueToRValueFunc)(const GValue* from);
extern void rbgobj_register_r2g_func(VALUE klass, RValueToGValueFunc func);
extern void rbgobj_register_g2r_func(GType gtype, GValueToRValueFunc func);
extern VALUE rbgobj_gvalue_to_rvalue(const GValue* value);
extern void rbgobj_rvalue_to_gvalue(VALUE val, GValue* result);

/* rbobj_param.c */
extern GParamSpec* rbgobj_param_spec_get_struct(VALUE obj);
extern VALUE rbgobj_param_spec_wrap(GParamSpec* pspec);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGOBJECT_H__ */
