/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib.h -

  $Author: sakai $
  $Date: 2002/10/02 04:56:42 $

  Copyright (C) 2002  Masahiro Sakai

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
#define StringValuePtr(s) STR2CSTR(s)
#endif

#ifndef LONG2NUM
#define LONG2NUM INT2NUM
#endif

#ifndef ULONG2NUM
#define ULONG2NUM UINT2NUM
#endif


#define RVAL2CSTR(v) (rbg_val2cstr(v))
#define CSTR2RVAL(s) (rb_str_new2(s))

extern VALUE mGLib;
extern gchar* rbg_val2cstr(VALUE str);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RBGLIB_H__ */
