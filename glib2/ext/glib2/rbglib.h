/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2002-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2002,2003  Masahiro Sakai
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

#include <glib-object.h>
#include "glib-enum-types.h"

#include "ruby.h"

#include "rbglibdeprecated.h"
#include "rbglib2conversions.h"

#pragma once

G_BEGIN_DECLS

#define RBGLIB_MAJOR_VERSION 3
#define RBGLIB_MINOR_VERSION 4
#define RBGLIB_MICRO_VERSION 9

#ifndef RB_ZALLOC
#  ifdef ZALLOC
#    define RB_ZALLOC(type) ZALLOC(type)
#  else
#    define RB_ZALLOC(type) rbg_memzero(ALLOC(type), sizeof(type))
#  endif
#endif

#ifndef RB_ALLOC
#  define RB_ALLOC(type) ALLOC(type)
#endif

#ifndef RB_ALLOC_N
#  define RB_ALLOC_N(type, n) ALLOC_N(type, n)
#endif

/* For Ruby < 2.3 */
#ifndef RB_OBJ_FROZEN
#  define RB_OBJ_FROZEN(obj) OBJ_FROZEN(obj)
#endif

#ifndef RSTRING_PTR
#  define RSTRING_PTR(s) (RSTRING(s)->ptr)
#  define RSTRING_LEN(s) (RSTRING(s)->len)
#endif

#ifndef RARRAY_PTR
#  define RARRAY_PTR(a) (RARRAY(a)->ptr)
#  define RARRAY_LEN(a) (RARRAY(a)->len)
#endif

#ifndef RARRAY_CONST_PTR
#  define RARRAY_CONST_PTR(a) RARRAY_PTR(a)
#endif

#ifndef DBL2NUM
#  define DBL2NUM(v)      (rb_float_new(v))
#endif

#ifndef RBASIC_CLASS
#  define RBASIC_CLASS(obj) (RBASIC(obj)->klass)
#endif

#ifndef G_SOURCE_REMOVE
#  define G_SOURCE_REMOVE FALSE
#endif

#ifndef G_SOURCE_CONTINUE
#  define G_SOURCE_CONTINUE TRUE
#endif

#define RBG_INSPECT(object) (rbg_rval_inspect(object))

#define RVAL2CSTR(v) (rbg_rval2cstr(&(v)))
#define RVAL2CSTR_RAW(v) (rbg_rval2cstr_raw(&(v)))
#define RVAL2CSTR_PTR(v) (rbg_rval2cstr_ptr(&(v)))
#define RVAL2CSTR_ACCEPT_NIL(v) (rbg_rval2cstr_accept_nil(&(v)))
#define RVAL2CSTR_RAW_ACCEPT_NIL(v) (rbg_rval2cstr_raw_accept_nil(&(v)))
#define RVAL2CSTR_PTR_ACCEPT_NIL(v) (rbg_rval2cstr_ptr_accept_nil(&(v)))
#define RVAL2CSTR2(v) (RVAL2CSTR_ACCEPT_NIL(v))
#define RVAL2CSTR_ACCEPT_SYMBOL(v) (rbg_rval2cstr_accept_symbol(&(v)))
#define RVAL2CSTR_ACCEPT_SYMBOL_ACCEPT_NIL(v) (rbg_rval2cstr_accept_symbol_accept_nil(&(v)))
#define RVAL2GLIBID(v, buf) (rbg_rval2glibid(&(v), &(buf), FALSE))
#define RVAL2GLIBID_ACCEPT_NIL(v, buf) (rbg_rval2glibid(&(v), &(buf), TRUE))
#define CSTR2RVAL(s) (rbg_cstr2rval(s))
#define CSTR2RVAL_LEN(s, l) (rbg_cstr2rval_len(s, l))
#define CSTR2RVAL_LEN_FREE(s, l) (rbg_cstr2rval_len_free(s, l))
#define CSTR2RVAL_ENC(s, e) (rbg_cstr2rval_with_encoding(s, e))
#define CSTR2RVAL_LEN_ENC(s, l, e) (rbg_cstr2rval_len_with_encoding(s, l, e))
#define CSTR2RVAL_FREE(s) (rbg_cstr2rval_free(s))
#define CSTR2RVAL2(s) (CSTR2RVAL_FREE(s))

#if G_BYTE_ORDER == G_LITTLE_ENDIAN
#  define CSTR2RVAL_LEN_UCS4(s, l) (CSTR2RVAL_LEN_ENC(s, l, "UTF-32LE"))
#  define CSTR2RVAL_LEN_UTF16(s, l) (CSTR2RVAL_LEN_ENC(s, l, "UTF-16LE"))
#else
#  define CSTR2RVAL_LEN_UCS4(s, l) (CSTR2RVAL_LEN_ENC(s, l, "UTF-32BE"))
#  define CSTR2RVAL_LEN_UTF16(s, l) (CSTR2RVAL_LEN_ENC(s, l, "UTF-16BE"))
#endif

#define RVAL2CSTRFILENAME(v) (rbg_filename_from_ruby(v))
#define CSTRFILENAME2RVAL(s) (rbg_filename_to_ruby(s))
#define CSTRFILENAME2RVAL_FREE(s) (rbg_filename_to_ruby_free(s))

#define RVAL2STRS(ary, n) rbg_rval2strv(&(ary), &(n))
#define RVAL2STRS_ACCEPT_NIL(ary, n) rbg_rval2strv_accept_nil(&(ary), &(n))
#define RVAL2STRV(ary) rbg_rval2strv(&(ary), NULL)
#define RVAL2STRV_ACCEPT_NIL(ary) rbg_rval2strv_accept_nil(&(ary), NULL)
#define RVAL2STRS_DUP(ary, n) rbg_rval2strv_dup(&(ary), &(n))
#define RVAL2STRS_DUP_ACCEPT_NIL(ary, n) rbg_rval2strv_dup_accept_nil(&(ary), &(n))
#define RVAL2STRV_DUP(ary) rbg_rval2strv_dup(&(ary), NULL)
#define RVAL2STRV_DUP_ACCEPT_NIL(ary) rbg_rval2strv_dup_accept_nil(&(ary), NULL)

#define STRV2RVAL(strings) rbg_strv2rval(strings)
#define STRV2RVAL_FREE(strings) rbg_strv2rval_free(strings)

#define RVAL2GBOOLEANS(ary, n) rbg_rval2gbooleans(&(ary), &(n))
#define RVAL2GINTS(ary, n) rbg_rval2gints(&(ary), &(n))
#define RVAL2GINT8S(ary, n) rbg_rval2gint8s(&(ary), &(n))
#define RVAL2GUINT8S(ary, n) rbg_rval2guint8s(&(ary), &(n))
#define RVAL2GUINT16S(ary, n) rbg_rval2guint16s(&(ary), &(n))
#define RVAL2GUINT32S(ary, n) rbg_rval2guint32s(&(ary), &(n))
#define RVAL2GDOUBLES(ary, n) rbg_rval2gdoubles(&(ary), &(n))

#define RVAL2POINTER(value) ((gpointer)(value))
#define POINTER2RVAL(pointer) ((VALUE)(pointer))

#define GINTS2RVAL(ary, n) rbg_gints2rval(ary, n)
#define GINTS2RVAL_FREE(ary, n) rbg_gints2rval(ary, n)

#define CBOOL2RVAL(b)   ((b) ? Qtrue : Qfalse)
#define RVAL2CBOOL(b)   (RTEST(b))
#define GERROR2RVAL(error) (rbgerr_gerror2exception(error))
#define RG_RAISE_ERROR(error) rb_exc_raise(GERROR2RVAL(error))
#define RAISE_GERROR(error) RG_RAISE_ERROR(error) /* deprecated */
#define G_DEF_ERROR(domain, name, module, parent, gtype)         \
    rbgerr_define_gerror(domain, name, module, parent, gtype)
#define G_DEF_ERROR2(domain, name, module, parent) \
    rbgerr_define_gerror(domain, name, module, parent, G_TYPE_INVALID)

#ifdef G_PLATFORM_WIN32
#  ifdef RUBY_GLIB2_COMPILATION
#    define RUBY_GLIB2_VAR extern __declspec(dllexport)
#  else
#    define RUBY_GLIB2_VAR extern __declspec(dllimport)
#  endif
#else
#  define RUBY_GLIB2_VAR extern
#endif

RUBY_GLIB2_VAR VALUE mGLib;

extern const gchar *rbg_rval_inspect(VALUE object);

extern gchar* rbg_string_value_ptr(volatile VALUE* ptr); /* no longer used */
extern const gchar *rbg_rval2cstr(VALUE *str);
extern const gchar *rbg_rval2cstr_raw(VALUE *str);
extern const gchar *rbg_rval2cstr_ptr(VALUE *str);
extern const gchar *rbg_rval2cstr_accept_nil(VALUE *str);
extern const gchar *rbg_rval2cstr_raw_accept_nil(VALUE *str);
extern const gchar *rbg_rval2cstr_ptr_accept_nil(VALUE *str);
extern const gchar *rbg_rval2cstr_accept_symbol(volatile VALUE *value);
extern const gchar *rbg_rval2cstr_accept_symbol_accept_nil(volatile VALUE *value);
extern const gchar *rbg_rval2glibid(volatile VALUE *value, volatile VALUE *buf, gboolean accept_nil);

extern VALUE rbg_cstr2rval(const gchar* str);
extern VALUE rbg_cstr2rval_len(const gchar* str, gsize len);
extern VALUE rbg_cstr2rval_len_free(gchar *str, gsize len);
extern VALUE rbg_cstr2rval_with_encoding(const gchar* str,
                                         const gchar *encoding);
extern VALUE rbg_cstr2rval_len_with_encoding(const gchar* str, gsize len,
                                             const gchar *encoding);
extern VALUE rbg_cstr2rval_free(gchar *str);
/* just for backward compatibility. */
extern VALUE rbg_cstr2rval_with_free(gchar *str);

VALUE rbg_filename_to_ruby(const gchar *filename);
extern VALUE rbg_filename_to_ruby_free(gchar *filename);
extern gchar *rbg_filename_from_ruby(VALUE filename);

gchar **rbg_rval2strv(volatile VALUE *value, long *n);
gchar **rbg_rval2strv_accept_nil(volatile VALUE *value, long *n);
gchar **rbg_rval2strv_dup(volatile VALUE *value, long *n);
gchar **rbg_rval2strv_dup_accept_nil(volatile VALUE *value, long *n);
gchar **rbg_rval2filenamev(volatile VALUE *value, long *n);
gchar **rbg_rval2filenamev_accept_nil(volatile VALUE *value, long *n);
VALUE rbg_strv2rval(const gchar **strings);
VALUE rbg_strv2rval_free(gchar **strings);

gboolean *rbg_rval2gbooleans(volatile VALUE *value, long *n);
gint *rbg_rval2gints(volatile VALUE *value, long *n);
gint8 *rbg_rval2gint8s(volatile VALUE *value, long *n);
guint8 *rbg_rval2guint8s(volatile VALUE *value, long *n);
guint16 *rbg_rval2guint16s(volatile VALUE *value, long *n);
guint32 *rbg_rval2guint32s(volatile VALUE *value, long *n);
gdouble *rbg_rval2gdoubles(volatile VALUE *value, long *n);

VALUE rbg_gints2rval(const gint *gints, long n);
VALUE rbg_gints2rval_free(gint *gints, long n);

extern VALUE rbg_to_array(VALUE object);
extern VALUE rbg_to_hash(VALUE object);
extern VALUE rbg_check_array_type(VALUE object);
extern VALUE rbg_check_hash_type(VALUE object);
extern void rbg_scan_options(VALUE options, ...);

/* rbgerror.h */
typedef enum {
  RBG_RUBY_ERROR_UNKNOWN,
} RBGRubyError;

#define RBG_RUBY_ERROR rbgerr_ruby_error_quark()
extern GQuark rbgerr_ruby_error_quark(void);

extern VALUE rbgerr_gerror2exception(GError *error);
extern VALUE rbgerr_define_gerror(GQuark domain, const gchar* name, VALUE module, VALUE parent, GType gtype);

extern VALUE rbglib_int64_to_num(guint64 val);
extern VALUE rbglib_uint64_to_num(guint64 val);
extern gint64 rbglib_num_to_int64(VALUE val);
extern guint64 rbglib_num_to_uint64(VALUE val);


extern VALUE rbg_variant_to_ruby(GVariant *variant);
extern GVariant *rbg_variant_from_ruby(VALUE rb_variant);

extern GVariantType *rbg_variant_type_from_ruby(VALUE rb_variant_type);

extern void rbg_gc_guard(gpointer key, VALUE rb_object);
extern void rbg_gc_unguard(gpointer key);


/* predicates */
extern gboolean rbg_is_bytes(VALUE object);
extern gboolean rbg_is_object(VALUE object);
extern gboolean rbg_is_value(VALUE object);

G_END_DECLS
