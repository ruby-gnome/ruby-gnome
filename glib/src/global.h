/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */

#include "rbgobject.h"

#ifndef StringValue
#define StringValue(s) ((s) = (TYPE(s) == T_STRING) ? (s) : rb_str_to_str(s))
#endif
#ifndef SafeStringValue
#define SafeStringValue(v) rb_check_safe_str(StringValue(v))
#endif
#ifndef StringValuePtr
#define StringValuePtr(s) STR2CSTR(s)
#endif

extern VALUE rbglib_int64_to_num(guint64 val);
extern VALUE rbglib_uint64_to_num(guint64 val);
extern gint64 rbglib_num_to_int64(VALUE val);
extern guint64 rbglib_num_to_uint64(VALUE val);
