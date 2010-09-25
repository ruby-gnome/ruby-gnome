/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.h -

  $Author: sakai $
  $Date: 2007/07/04 13:13:20 $

  Copyright (C) 2002-2005  Ruby-GNOME2 Project
  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include "ruby.h"

#ifndef __RBGLIB_H__
#define __RBGLIB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RBGLIB_MAJOR_VERSION 0
#define RBGLIB_MINOR_VERSION 90
#define RBGLIB_MICRO_VERSION 2

#ifndef RSTRING_PTR
#  define RSTRING_PTR(s) (RSTRING(s)->ptr)
#  define RSTRING_LEN(s) (RSTRING(s)->len)
#endif

#ifndef RARRAY_PTR
#  define RARRAY_PTR(s) (RARRAY(s)->ptr)
#  define RARRAY_LEN(s) (RARRAY(s)->len)
#endif

#ifndef DBL2NUM
#  define DBL2NUM(v)      (rb_float_new(v))
#endif

#if ! GLIB_CHECK_VERSION(2,4,0)
typedef int GPid;
#endif

#define RBG_INSPECT(object) (rbg_rval_inspect(object))

#define RVAL2CSTR(v) (StringValuePtr(v))
#define RVAL2CSTR_ACCEPT_NIL(v) (rbg_rval2cstr_accept_nil(v))
#define RVAL2CSTR2(v) (RVAL2CSTR_ACCEPT_NIL(v))
#define CSTR2RVAL(s) (rbg_cstr2rval(s))
#define CSTR2RVAL_FREE(s) (rbg_cstr2rval_with_free(s))
#define CSTR2RVAL2(s) (CSTR2RVAL_FREE(s))

#define CBOOL2RVAL(b)   ((b) ? Qtrue : Qfalse)
#define RVAL2CBOOL(b)   (RTEST(b))
#define GERROR2RVAL(error) (rbgerr_gerror2exception(error))
#define RAISE_GERROR(error) rb_exc_raise(GERROR2RVAL(error))
#define G_DEF_ERROR(domain, name, module, parent, gtype)         \
    rbgerr_define_gerror(domain, name, module, parent, gtype)
#define G_DEF_ERROR2(domain, name, module, parent) \
    rbgerr_define_gerror(domain, name, module, parent, Qnil)

#if defined(G_PLATFORM_WIN32) && !defined(RUBY_GLIB2_STATIC_COMPILATION)
#  ifdef RUBY_GLIB2_COMPILATION
#    define RUBY_GLIB2_VAR __declspec(dllexport)
#  else
#    define RUBY_GLIB2_VAR extern __declspec(dllimport)
#  endif
#else
#  define RUBY_GLIB2_VAR extern
#endif

RUBY_GLIB2_VAR VALUE mGLib;

extern const gchar *rbg_rval_inspect(VALUE object);

extern gchar* rbg_string_value_ptr(volatile VALUE* ptr); /* no longer used */
extern gchar *rbg_rval2cstr_accept_nil(VALUE str);
extern VALUE rbg_cstr2rval(const char* str);
extern VALUE rbg_cstr2rval_with_free(gchar* str);

/* rbgerror.h */
extern VALUE rbgerr_gerror2exception(GError *error);
extern VALUE rbgerr_define_gerror(GQuark domain, const gchar* name, VALUE module, VALUE parent, VALUE gtype);

extern VALUE rbglib_int64_to_num(guint64 val);
extern VALUE rbglib_uint64_to_num(guint64 val);
extern gint64 rbglib_num_to_int64(VALUE val);
extern guint64 rbglib_num_to_uint64(VALUE val);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
