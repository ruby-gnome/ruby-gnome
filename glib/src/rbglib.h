/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.h -

  $Author: sakai $
  $Date: 2003/02/03 08:52:24 $

  Copyright (C) 2002,2003  Masahiro Sakai

**********************************************************************/

#include <glib.h>
#include "ruby.h"

#ifndef __RBGLIB_H__
#define __RBGLIB_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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

#define RVAL2CSTR(v) (StringValuePtr(v))
#define CSTR2RVAL(s) (rbg_cstr2rval(s))

extern VALUE mGLib;
extern gchar* rbg_string_value_ptr(volatile VALUE* ptr);
extern VALUE rbg_cstr2rval(const char* str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
