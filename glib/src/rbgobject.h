/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobject.h -

  $Author: sakai $
  $Date: 2002/06/10 18:51:43 $

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

typedef struct _rbgobj_class_info rbgobj_class_info;
struct _rbgobj_class_info {
    VALUE klass;
    GType gtype;
    void (*mark)(GObject *);
    void (*free)(GObject *);
};

extern void rbgobj_register_class(const rbgobj_class_info* cinfo);
extern const rbgobj_class_info* rbgobj_lookup_class(VALUE klass);
extern const rbgobj_class_info* rbgobj_lookup_class_by_gtype(GType gtype);
extern VALUE rbgobj_lookup_rbclass(const GObject* gobj);

extern ID id_class_info;
extern ID id_gobject_data;
extern ID id_relatives;
extern ID id_relative_callbacks;
extern ID id_delete;

extern GObject* rbgobj_get_gobject(VALUE obj);
extern GObject* rbgobj_force_get_gobject(VALUE obj);
extern void rbgobj_set_gobject(VALUE obj, GObject* gobj);
extern VALUE rbgobj_get_value_from_gobject(GObject* gobj);
extern VALUE rbgobj_make_gobject(VALUE klass, GObject* gobj);
extern VALUE rbgobj_make_gobject_auto_type(GObject* gobj);

extern void rbgobj_add_relative(VALUE obj, VALUE relative);
extern void rbgobj_add_relative_removable(VALUE obj, VALUE relative,
                                             ID obj_ivar_id, VALUE hash_key);
extern void rbgobj_remove_relative(VALUE obj, ID obj_ivar_id, VALUE hash_key);

extern GTypeClass* rbgobj_type_class_get_struct(VALUE obj);
extern GTypeInstance* rbgobj_type_instance_get_struct(VALUE obj);

extern VALUE rbgobj_cGObject;

/* rbgobj_closure.c */
extern GClosure* g_rclosure_new(VALUE callback_proc);

/* rbgobj_value.c */
typedef void (*RValueToGValueFunc)(VALUE from, GValue* to);
typedef VALUE (*GValueToRValueFunc)(GValue* from);
extern void rbgobj_register_r2g_func(VALUE klass, RValueToGValueFunc func);
extern void rbgobj_register_g2r_func(GType gtype, GValueToRValueFunc func);
extern VALUE rbgobj_gvalue_to_rvalue(GValue* value);
extern void rbgobj_rvalue_to_gvalue(VALUE val, GValue* result);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGOBJECT_H__ */
