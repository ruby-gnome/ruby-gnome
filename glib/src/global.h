/* -*- c-file-style: "ruby" -*- */

#include "rbgobject.h"

extern void Init_gobject();
extern void Init_gobject_gtype();
extern void Init_gobject_gobject();
extern void Init_gobject_gclosure();
extern void Init_gobject_gvalue();
extern void Init_gobject_gsignal();
extern void Init_gobject_gparam();
extern void Init_gobject_gparamspecs();
extern void Init_gobject_gtypemodule();
extern void Init_gobject_gboxed();
extern void Init_gobject_genums();

#ifndef StringValue
#define StringValue(s) ((s) = (TYPE(s) == T_STRING) ? (s) : rb_str_to_str(s))
#endif
#ifndef SafeStringValue
#define SafeStringValue(v) rb_check_safe_str(StringValue(v))
#endif
#ifndef StringValuePtr
#define StringValuePtr(s) STR2CSTR(s)
#endif
