/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgutil.h -

  $Author: mutoh $
  $Date: 2003/02/01 16:03:10 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#ifndef __RBGUTIL_H__
#define __RBGUTIL_H__

#include <glib.h>
#include <glib-object.h>
#include "ruby.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define G_DEF_SETTER(klass, name) \
    rb_funcall(klass, id_module_eval, 1, rb_str_new2( \
    "def " name "=(val); set_" name "(val); val; end\n"))
#define G_DEF_SETTERS(klass) \
    rb_funcall(mGLib, id_add_one_arg_setter, 1, klass)

#define G_SET_PROPERTIES(self, hash) (rbgutil_set_properties(self, hash))
#define G_SET_SYMBOL_PROPERTY(gtype, name) \
     rbgobj_register_property_getter(gtype, name, sym_g2r_func)

#define GLIST2ARY(list)              (rbgutil_glist2ary(list))
#define GLIST2ARY2(list, gtype)      (rbgutil_glist2ary_boxed(list, gtype))
#define GSLIST2ARY(list)             (rbgutil_gslist2ary(list))
#define GSLIST2ARY2(list, gtype)     (rbgutil_gslist2ary_boxed(list, gtype))

#define RAISE_GERROR(error)    rb_exc_raise(rbgutil_gerror2exception(error))

    extern ID id_module_eval;
    extern ID id_add_one_arg_setter;

    extern VALUE sym_g2r_func(const GValue *from);
    extern VALUE rbgutil_gerror2exception(GError *error);
    extern void rbgutil_set_properties(VALUE self, VALUE hash);
    extern VALUE rbgutil_glist2ary(GList* list);
    extern VALUE rbgutil_glist2ary_boxed(GList* list, GType gtype);
    extern VALUE rbgutil_gslist2ary(GSList* list);
    extern VALUE rbgutil_gslist2ary_boxed(GSList* list, GType gtype);
    extern void Init_gutil();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
