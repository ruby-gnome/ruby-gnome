/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************
  rbgprivate.h -

  Copyright (C) 2007  Ruby-GNOME2 Project Team
**********************************************************************/

#ifndef __RBGPRIVATE_H__
#define __RBGPRIVATE_H__

G_BEGIN_DECLS

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

G_END_DECLS

#endif
