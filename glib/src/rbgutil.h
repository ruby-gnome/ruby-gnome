/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgutil.h -

  $Author: sakai $
  $Date: 2007/07/08 03:17:21 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#ifndef __RBGUTIL_H__
#define __RBGUTIL_H__

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
#define G_PROTECT_CALLBACK(func, data) (rbgutil_invoke_callback((VALUE(*)(VALUE))func, (VALUE)data))

#define GLIST2ARY(list)           (rbgutil_glist2ary(list))
#define GLIST2ARYF(list)          (rbgutil_glist2ary_and_free(list))
#define GLIST2ARY2(list, gtype)   (rbgutil_glist2ary_boxed(list, gtype))
#define GLIST2ARY2F(list, gtype)  (rbgutil_glist2ary_boxed_and_free(list, gtype))
#define GSLIST2ARY(list)          (rbgutil_gslist2ary(list))
#define GSLIST2ARYF(list)         (rbgutil_gslist2ary_and_free(list))
#define GSLIST2ARY2(list, gtype)  (rbgutil_gslist2ary_boxed(list, gtype))
#define GSLIST2ARY2F(list, gtype) (rbgutil_gslist2ary_boxed_and_free(list, gtype))

extern VALUE rbgutil_def_setters(VALUE klass);
extern void rbgutil_set_properties(VALUE self, VALUE hash);
extern VALUE rbgutil_glist2ary(GList* list);
extern VALUE rbgutil_glist2ary_boxed(GList* list, GType gtype);
extern VALUE rbgutil_glist2ary_and_free(GList* list);
extern VALUE rbgutil_glist2ary_boxed_and_free(GList* list, GType gtype);
extern VALUE rbgutil_gslist2ary(GSList* list);
extern VALUE rbgutil_gslist2ary_and_free(GSList* list);
extern VALUE rbgutil_gslist2ary_boxed(GSList* list, GType gtype);
extern VALUE rbgutil_gslist2ary_boxed_and_free(GSList* list, GType gtype);
extern VALUE rbgutil_protect(VALUE (*proc) (VALUE), VALUE data);
extern VALUE rbgutil_invoke_callback(VALUE (*func)(VALUE), VALUE arg);
extern void rbgutil_start_callback_handle_thread(void);
extern void rbgutil_stop_callback_handle_thread(void);

/*< protected >*/
RUBY_GLIB2_VAR ID rbgutil_id_module_eval;
extern VALUE rbgutil_sym_g2r_func(const GValue *from);

/* deprecated */
#define G_BLOCK_PROC rb_block_proc

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
