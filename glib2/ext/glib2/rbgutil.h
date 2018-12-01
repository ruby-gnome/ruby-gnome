/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2015  Ruby-GNOME2 Project Team
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

#pragma once

#include <glib-object.h>
#include <ruby.h>
#include <ruby/encoding.h>
#include "rbglib.h"
#include "rbgutil_list.h"
#include "rbgutildeprecated.h"

G_BEGIN_DECLS

#define RG_DEF_MODFUNC(method, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, #method, rg_m_ ## method, argc)
#define RG_DEF_MODFUNC_P(method, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, #method"?", rg_m_ ## method ## _p, argc)
#define RG_DEF_MODFUNC_OPERATOR(ope, func, argc) \
        rb_define_module_function(RG_TARGET_NAMESPACE, ope, rg_m_operator_ ## func, argc)
#define RG_DEF_SMETHOD(method, argc) \
        rbg_define_singleton_method(RG_TARGET_NAMESPACE, #method, rg_s_ ## method, argc)
#define RG_DEF_SMETHOD_P(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method"?", rg_s_ ## method ## _p, argc)
#define RG_DEF_SMETHOD_BANG(method, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, #method"!", rg_s_ ## method ## _bang, argc)
#define RG_DEF_SMETHOD_OPERATOR(ope, func, argc) \
        rb_define_singleton_method(RG_TARGET_NAMESPACE, ope, rg_s_operator_ ## func, argc)
#define RG_DEF_METHOD(method, argc) \
        rbg_define_method(RG_TARGET_NAMESPACE, #method, rg_ ## method, argc)
#define RG_DEF_METHOD_P(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method"?", rg_ ## method ## _p, argc)
#define RG_DEF_METHOD_BANG(method, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, #method"!", rg_ ## method ## _bang, argc)
#define RG_DEF_METHOD_OPERATOR(ope, func, argc) \
        rb_define_method(RG_TARGET_NAMESPACE, ope, rg_operator_ ## func, argc)
#define RG_DEF_ATTR(attr, read, write, ex) \
        rb_attr(RG_TARGET_NAMESPACE, rb_intern(attr), read, write, ex)
#define RG_DEF_ALIAS(new, old) rb_define_alias(RG_TARGET_NAMESPACE, new, old)

#define RG_REG_GLIBID_SETTER(name) \
        rbgobj_register_property_setter(CLASS2GTYPE(RG_TARGET_NAMESPACE), name, rbgutil_glibid_r2g_func)
#define RG_REG_SYMBOL_GETTER(name) \
        rbgobj_register_property_getter(CLASS2GTYPE(RG_TARGET_NAMESPACE), name, rbgutil_sym_g2r_func)

#define RG_REPLACE_SET_PROPERTY(name, args)   \
    G_REPLACE_SET_PROPERTY(RG_TARGET_NAMESPACE, #name, rg_set_ ## name, args)
#define RG_REPLACE_GET_PROPERTY(name, args)   \
    G_REPLACE_GET_PROPERTY(RG_TARGET_NAMESPACE, #name, rg_get_ ## name, args)
#define RG_REPLACE_ACTION(name, args)                               \
    G_REPLACE_ACTION(RG_TARGET_NAMESPACE, #name, rg_ ## name, args)

#define G_REPLACE_SET_PROPERTY(klass, name, function, args) \
    rb_undef_method(klass, "set_" name); \
    rb_undef_method(klass, name "="); \
    rbg_define_method(klass, "set_" name, function, args)

#define G_REPLACE_GET_PROPERTY(klass, name, function, args) \
    rb_undef_method(klass, name); \
    rb_define_method(klass, name, function, args)

#define G_REPLACE_ACTION(klass, name, function, args) \
    rb_undef_method(klass, name); \
    rb_define_method(klass, name, function, args)

#define G_SET_PROPERTIES(self, hash) (rbgutil_set_properties(self, hash))
#define G_PROTECT_CALLBACK(func, data) (rbgutil_invoke_callback((VALUE(*)(VALUE))func, (VALUE)data))

#define RBG_STRING_SET_UTF8_ENCODING(string) \
    (rbgutil_string_set_utf8_encoding(string))

extern void rbg_define_method(VALUE klass, const char *name, VALUE (*func)(ANYARGS), int argc);
extern void rbg_define_singleton_method(VALUE obj, const char *name, VALUE (*func)(ANYARGS), int argc);
extern VALUE rbgutil_def_setters(VALUE klass);
extern void rbgutil_set_properties(VALUE self, VALUE hash);
extern VALUE rbgutil_protect(VALUE (*proc) (VALUE), VALUE data);
extern VALUE rbgutil_invoke_callback(VALUE (*func)(VALUE), VALUE arg);
extern void rbgutil_start_callback_dispatch_thread(void);
extern void rbgutil_stop_callback_dispatch_thread(void);

extern VALUE rbgutil_string_set_utf8_encoding(VALUE string);
extern gboolean rbgutil_key_equal(VALUE rb_string, const char *key);

extern const gchar *rbg_inspect(VALUE object);
extern GSource *rbg_interrupt_source_new(void);

extern gchar *rbg_name_to_nick(const gchar *name);

extern void *rbg_memzero(void *poitner, size_t size);

/*< protected >*/
RUBY_GLIB2_VAR ID rbgutil_id_module_eval;
extern void rbgutil_glibid_r2g_func(VALUE from, GValue* to);
extern VALUE rbgutil_sym_g2r_func(const GValue *from);

G_END_DECLS
