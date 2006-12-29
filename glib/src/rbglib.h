/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.h -

  $Author: mutoh $
  $Date: 2006/12/29 08:43:24 $

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
#define RBGLIB_MINOR_VERSION 16
#define RBGLIB_MICRO_VERSION 0

#ifndef StringValue
#define StringValue(s) ((s) = (TYPE(s) == T_STRING) ? (s) : rb_str_to_str(s))
#endif
#ifndef SafeStringValue
#define SafeStringValue(v) rb_check_safe_str(StringValue(v))
#endif
#ifndef StringValuePtr
#define StringValuePtr(v) rbg_string_value_ptr(&(v))
#endif

#ifndef LONG2NUM
#define LONG2NUM INT2NUM
#endif

#ifndef ULONG2NUM
#define ULONG2NUM UINT2NUM
#endif

#if ! GLIB_CHECK_VERSION(2,4,0)
typedef int GPid;
#endif

#define RVAL2CSTR(v) (StringValuePtr(v))
#define RVAL2CSTR2(v) (NIL_P(v) ? NULL : StringValuePtr(v))
#define CSTR2RVAL(s) (rbg_cstr2rval(s))
#define CSTR2RVAL2(s) (rbg_cstr2rval_with_free(s))

#define CBOOL2RVAL(b)   ((b) ? Qtrue : Qfalse)
#define RVAL2CBOOL(b)   (RTEST(b))
#define RAISE_GERROR(error) rb_exc_raise(rbgerr_gerror2exception(error))
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
extern gchar* rbg_string_value_ptr(volatile VALUE* ptr);
extern VALUE rbg_cstr2rval(const char* str);
extern VALUE rbg_cstr2rval_with_free(gchar* str);

/* rbgerror.h */
extern VALUE rbgerr_gerror2exception(GError *error);
extern VALUE rbgerr_define_gerror(GQuark domain, gchar* name, VALUE module, VALUE parent, VALUE gtype);

extern VALUE rbglib_int64_to_num(guint64 val);
extern VALUE rbglib_uint64_to_num(guint64 val);
extern gint64 rbglib_num_to_int64(VALUE val);
extern guint64 rbglib_num_to_uint64(VALUE val);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
