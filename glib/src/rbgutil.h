/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgutil.h -

  $Author: mutoh $
  $Date: 2004/08/18 16:42:36 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#ifndef __RBGUTIL_H__
#define __RBGUTIL_H__

#include <glib.h>
#include <glib-object.h>
#include "ruby.h"
#include "rbglib.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define G_DEF_SETTER(klass, name) \
    rb_funcall(klass, rbgutil_id_module_eval, 1, rb_str_new2( \
    "def " name "=(val); set_" name "(val); val; end\n"))
#define G_DEF_SETTERS(klass) rbgutil_def_setters(klass)

#define G_SET_PROPERTIES(self, hash) (rbgutil_set_properties(self, hash))
#define G_SET_SYMBOL_PROPERTY(gtype, name) \
     rbgobj_register_property_getter(gtype, name, rbgutil_sym_g2r_func)

#define GLIST2ARY(list)              (rbgutil_glist2ary(list))
#define GLIST2ARY2(list, gtype)      (rbgutil_glist2ary_boxed(list, gtype))
#define GSLIST2ARY(list)             (rbgutil_gslist2ary(list))
#define GSLIST2ARY2(list, gtype)     (rbgutil_gslist2ary_boxed(list, gtype))

extern VALUE rbgutil_def_setters(VALUE klass);
extern void rbgutil_set_properties(VALUE self, VALUE hash);
extern VALUE rbgutil_glist2ary(GList* list);
extern VALUE rbgutil_glist2ary_boxed(GList* list, GType gtype);
extern VALUE rbgutil_gslist2ary(GSList* list);
extern VALUE rbgutil_gslist2ary_boxed(GSList* list, GType gtype);

/*< protected >*/
RUBY_GLIB2_VAR ID rbgutil_id_module_eval;
extern VALUE rbgutil_sym_g2r_func(const GValue *from);

#if HAVE_RB_BLOCK_PROC
#define G_BLOCK_PROC rb_block_proc
#else
#define G_BLOCK_PROC rb_f_lambda
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
