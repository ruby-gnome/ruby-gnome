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
extern void Init_gobject_gtypeplugin();
extern void Init_gobject_gtypemodule();
extern void Init_gobject_gboxed();
extern void Init_gobject_genums();

extern void Init_utils_int64();

#ifndef StringValue
#define StringValue(s) ((s) = (TYPE(s) == T_STRING) ? (s) : rb_str_to_str(s))
#endif
#ifndef SafeStringValue
#define SafeStringValue(v) rb_check_safe_str(StringValue(v))
#endif
#ifndef StringValuePtr
#define StringValuePtr(s) STR2CSTR(s)
#endif

extern VALUE rbgobj_int64_to_num(guint64 val);
extern VALUE rbgobj_uint64_to_num(guint64 val);
extern gint64 rbgobj_num_to_int64(VALUE val);
extern guint64 rbgobj_num_to_uint64(VALUE val);
