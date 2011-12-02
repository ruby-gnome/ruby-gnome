/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#ifndef __RBGUTIL_H__
#define __RBGUTIL_H__

#include <glib-object.h>
#include "ruby.h"
#ifdef HAVE_RUBY_ENCODING_H
#  include <ruby/encoding.h>
#endif
#include "rbglib.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RG_DEF_MODFUNC(method, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, #method, rg_m_ ## method, argc)
#define RG_DEF_MODFUNC_P(method, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, #method"?", rg_m_ ## method ## _p, argc)
#define RG_DEF_MODFUNC_OPERATOR(ope, func, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, ope, rg_m_operator_ ## func, argc)
#define RG_DEF_SMETHOD(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)
#define RG_DEF_SMETHOD_P(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method"?", rg_s_ ## method ## _p, argc)
#define RG_DEF_SMETHOD_BANG(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method"!", rg_s_ ## method ## _bang, argc)
#define RG_DEF_SMETHOD_OPERATOR(ope, func, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, ope, rg_s_operator_ ## func, argc)
#define RG_DEF_METHOD(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#define RG_DEF_METHOD_P(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method"?", rg_ ## method ## _p, argc)
#define RG_DEF_METHOD_BANG(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method"!", rg_ ## method ## _bang, argc)
#define RG_DEF_METHOD_OPERATOR(ope, func, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, ope, rg_operator_ ## func, argc)
#define RG_DEF_ATTR(attr, read, write, ex) \
        rb_attr(RG_TARGET_NAMESPACE, rb_intern(attr), read, write, ex)
#define RG_DEF_ALIAS(new, old) rb_define_alias(RG_TARGET_NAMESPACE, new, old)

#define G_DEF_SETTER(klass, name) \
    rb_funcall(klass, rbgutil_id_module_eval, 1, rb_str_new2( \
    "def " name "=(val); set_" name "(val); val; end\n"))
#define G_DEF_SETTERS(klass) rbgutil_def_setters(klass)

#define G_REPLACE_SET_PROPERTY(klass, name, function, args) \
    rb_undef_method(klass, "set_" name); \
    rb_define_method(klass, "set_" name, function, args); \
    rb_undef_method(klass, name "="); \
    G_DEF_SETTER(klass, name)

#define G_REPLACE_GET_PROPERTY(klass, name, function, args) \
    rb_undef_method(klass, name); \
    rb_define_method(klass, name, function, args)

#define G_REPLACE_ACTION(klass, name, function, args) \
    rb_undef_method(klass, name); \
    rb_define_method(klass, name, function, args)

#define G_SET_PROPERTIES(self, hash) (rbgutil_set_properties(self, hash))
#define G_SET_SYMBOL_PROPERTY(gtype, name) \
     rbgobj_register_property_getter(gtype, name, rbgutil_sym_g2r_func)
#define G_PROTECT_CALLBACK(func, data) (rbgutil_invoke_callback((VALUE(*)(VALUE))func, (VALUE)data))

#define GLIST2ARY(list)           (rbgutil_glist2ary(list))
#define GLIST2ARY_FREE(list)      (rbgutil_glist2ary_and_free(list))
#define GLIST2ARYF(list)          (GLIST2ARY_FREE(list))
#define GLIST2ARY2(list, gtype)   (rbgutil_glist2ary_boxed(list, gtype))
#define GLIST2ARY2F(list, gtype)  (rbgutil_glist2ary_boxed_and_free(list, gtype))
#define GLIST2ARY_STR(list)       (rbgutil_glist2ary_string(list))
#define GLIST2ARY_STR_FREE(list)  (rbgutil_glist2ary_string_and_free(list))
#define GSLIST2ARY(list)          (rbgutil_gslist2ary(list))
#define GSLIST2ARY_FREE(list)     (rbgutil_gslist2ary_and_free(list))
#define GSLIST2ARYF(list)         (GSLIST2ARY_FREE(list))
#define GSLIST2ARY2(list, gtype)  (rbgutil_gslist2ary_boxed(list, gtype))
#define GSLIST2ARY2F(list, gtype) (rbgutil_gslist2ary_boxed_and_free(list, gtype))
#define GSLIST2ARY_STR(list)      (rbgutil_gslist2ary_string(list))
#define GSLIST2ARY_STR_FREE(list) (rbgutil_gslist2ary_string_and_free(list))
#define GSLIST2ARY_STR_FREE_LIST(list) (rbgutil_gslist2ary_string_and_free_list(list))

#define RBG_STRING_SET_UTF8_ENCODING(string) \
    (rbgutil_string_set_utf8_encoding(string))

extern VALUE rbgutil_def_setters(VALUE klass);
extern void rbgutil_set_properties(VALUE self, VALUE hash);
extern VALUE rbgutil_glist2ary(const GList *list);
extern VALUE rbgutil_glist2ary_and_free(GList* list);
extern VALUE rbgutil_glist2ary_boxed(const GList *list, GType gtype);
extern VALUE rbgutil_glist2ary_boxed_and_free(GList *list, GType gtype);
extern VALUE rbgutil_glist2ary_string(const GList *list);
extern VALUE rbgutil_glist2ary_string_and_free(GList *list);
extern VALUE rbgutil_gslist2ary(const GSList *list);
extern VALUE rbgutil_gslist2ary_and_free(GSList *list);
extern VALUE rbgutil_gslist2ary_boxed(const GSList *list, GType gtype);
extern VALUE rbgutil_gslist2ary_boxed_and_free(GSList *list, GType gtype);
extern VALUE rbgutil_gslist2ary_string(const GSList *list);
extern VALUE rbgutil_gslist2ary_string_and_free(GSList *list);
extern VALUE rbgutil_gslist2ary_string_and_free_list(GSList *list);
extern VALUE rbgutil_protect(VALUE (*proc) (VALUE), VALUE data);
extern VALUE rbgutil_invoke_callback(VALUE (*func)(VALUE), VALUE arg);
extern void rbgutil_start_callback_dispatch_thread(void);
extern void rbgutil_stop_callback_dispatch_thread(void);

extern VALUE rbgutil_string_set_utf8_encoding(VALUE string);

/*< protected >*/
RUBY_GLIB2_VAR ID rbgutil_id_module_eval;
extern VALUE rbgutil_sym_g2r_func(const GValue *from);

/* deprecated */
#define G_BLOCK_PROC rb_block_proc

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
