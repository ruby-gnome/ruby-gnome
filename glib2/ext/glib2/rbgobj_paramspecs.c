/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2004-2021  Ruby-GNOME Project Team
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

#include "rbgprivate.h"

#define DEF_NUMERIC_PSPEC_METHODS_FUNC(pspec_type, typename, from_ruby, to_ruby, pspec_cast) \
static VALUE                                                            \
typename##_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,  \
                      VALUE minimum, VALUE maximum, VALUE default_value, \
                      VALUE flags)                                      \
{                                                                       \
    GParamSpec* pspec;                                                  \
    pspec = g_param_spec_##typename(StringValuePtr(name),               \
                                    StringValuePtr(nick),               \
                                    StringValuePtr(blurb),              \
                                    from_ruby(minimum),                 \
                                    from_ruby(maximum),                 \
                                    from_ruby(default_value),           \
                                    NUM2UINT(flags));                   \
    rbgobj_param_spec_initialize(self, pspec);                          \
    return Qnil;                                                        \
}                                                                       \
                                                                        \
static VALUE                                                            \
typename##_minimum(VALUE self)                                          \
{                                                                       \
    return to_ruby(pspec_cast(RVAL2GOBJ(self))->minimum);               \
}                                                                       \
                                                                        \
static VALUE                                                            \
typename##_maximum(VALUE self)                                          \
{                                                                       \
    return to_ruby(pspec_cast(RVAL2GOBJ(self))->maximum);               \
}                                                                       \
                                                                        \
static VALUE                                                            \
typename##_range(VALUE self)                                            \
{                                                                       \
    pspec_type* pspec = pspec_cast(RVAL2GOBJ(self));                    \
    return rb_range_new(pspec->minimum, pspec->maximum, 0);             \
}

DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecChar, char, NUM2INT, INT2FIX, G_PARAM_SPEC_CHAR)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecUChar, uchar, NUM2UINT, INT2FIX, G_PARAM_SPEC_UCHAR)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecInt, int, NUM2INT, INT2NUM, G_PARAM_SPEC_INT)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecUInt, uint, NUM2UINT, UINT2NUM, G_PARAM_SPEC_UINT)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecLong, long, NUM2LONG, INT2NUM, G_PARAM_SPEC_LONG)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecULong, ulong, NUM2ULONG, UINT2NUM, G_PARAM_SPEC_ULONG)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecInt64, int64, rbglib_num_to_int64, rbglib_int64_to_num, G_PARAM_SPEC_INT64)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecUInt64, uint64, rbglib_num_to_uint64, rbglib_uint64_to_num, G_PARAM_SPEC_UINT64)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecFloat, float, NUM2DBL, rb_float_new, G_PARAM_SPEC_FLOAT)
DEF_NUMERIC_PSPEC_METHODS_FUNC(GParamSpecDouble, double, NUM2DBL, rb_float_new, G_PARAM_SPEC_DOUBLE)

static VALUE
float_epsilon(VALUE self)
{
    return rb_float_new(G_PARAM_SPEC_FLOAT(RVAL2GOBJ(self))->epsilon);
}

static VALUE
double_epsilon(VALUE self)
{
    return rb_float_new(G_PARAM_SPEC_DOUBLE(RVAL2GOBJ(self))->epsilon);
}


static VALUE
boolean_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                   VALUE default_value, VALUE flags)
{
    GParamSpec* pspec;
    pspec = g_param_spec_boolean(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 RVAL2CBOOL(default_value),
                                 NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
unichar_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                   VALUE default_value, VALUE flags)
{
    GParamSpec* pspec;
    pspec = g_param_spec_unichar(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 NUM2UINT(default_value),
                                 NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
enum_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                VALUE enum_type, VALUE default_value, VALUE flags)
{
    GParamSpec* pspec;
    GType gtype = rbgobj_gtype_from_ruby(enum_type);

    pspec = g_param_spec_enum(StringValuePtr(name),
                              StringValuePtr(nick),
                              StringValuePtr(blurb),
                              gtype,
                              RVAL2GENUM(default_value, gtype),
                              NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
flags_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                 VALUE flags_type, VALUE default_value, VALUE flags)
{
    GParamSpec* pspec;
    GType gtype = rbgobj_gtype_from_ruby(flags_type);

    pspec = g_param_spec_flags(StringValuePtr(name),
                              StringValuePtr(nick),
                              StringValuePtr(blurb),
                              gtype,
                              RVAL2GFLAGS(default_value, gtype),
                              NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
string_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                  VALUE default_value, VALUE flags)
{
    GParamSpec* pspec;
    pspec = g_param_spec_string(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                NIL_P(default_value) ? NULL : StringValuePtr(default_value),
                                NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
param_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                 VALUE param_type, VALUE flags)
{
    GParamSpec* pspec;
    pspec = g_param_spec_param(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               rbgobj_gtype_from_ruby(param_type),
                               NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
boxed_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                 VALUE boxed_type, VALUE flags)
{
    GParamSpec* pspec;
    pspec = g_param_spec_boxed(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               rbgobj_gtype_from_ruby(boxed_type),
                               NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
pointer_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb, VALUE flags)
{
    GParamSpec* pspec;
    pspec = g_param_spec_pointer(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
object_initialize(VALUE self, VALUE name, VALUE nick, VALUE blurb,
                  VALUE object_type, VALUE flags)
{
    GParamSpec* pspec;
    pspec = g_param_spec_object(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                rbgobj_gtype_from_ruby(object_type),
                                NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

void
Init_gobject_gparamspecs(void)
{
    VALUE cParamSpec = GTYPE2CLASS(G_TYPE_PARAM);
    VALUE c;

#define DEF_NUMERIC_PSPEC_METHODS(c, typename) \
  G_STMT_START {\
    rbg_define_method(c, "initialize", typename##_initialize, 7); \
    rbg_define_method(c, "minimum", typename##_minimum, 0); \
    rbg_define_method(c, "maximum", typename##_maximum, 0); \
    rbg_define_method(c, "range", typename##_range, 0); \
  } G_STMT_END

#if 0
    rbg_define_method(c, "default_value", typename##_default_value, 0); \
    rb_define_alias(c, "default", "default_value"); \

#endif

    c = G_DEF_CLASS(G_TYPE_PARAM_CHAR, "Char", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, char);

    c = G_DEF_CLASS(G_TYPE_PARAM_UCHAR, "UChar", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, uchar);

    c = G_DEF_CLASS(G_TYPE_PARAM_INT, "Int", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, int);

    c = G_DEF_CLASS(G_TYPE_PARAM_UINT, "UInt", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, uint);

    c = G_DEF_CLASS(G_TYPE_PARAM_LONG, "Long", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, long);

    c = G_DEF_CLASS(G_TYPE_PARAM_ULONG, "ULong", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, ulong);

    c = G_DEF_CLASS(G_TYPE_PARAM_INT64, "Int64", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, int64);

    c = G_DEF_CLASS(G_TYPE_PARAM_UINT64, "UInt64", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, uint64);

    c = G_DEF_CLASS(G_TYPE_PARAM_FLOAT, "Float", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, float);
    rbg_define_method(c, "epsilon", float_epsilon, 0);

    c = G_DEF_CLASS(G_TYPE_PARAM_DOUBLE, "Double", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, double);
    rbg_define_method(c, "epsilon", double_epsilon, 0);

    c = G_DEF_CLASS(G_TYPE_PARAM_BOOLEAN, "Boolean", cParamSpec);
    rbg_define_method(c, "initialize", boolean_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_UNICHAR, "UniChar", cParamSpec);
    rbg_define_method(c, "initialize", unichar_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_ENUM, "Enum", cParamSpec);
    rbg_define_method(c, "initialize", enum_initialize, 6);

    c = G_DEF_CLASS(G_TYPE_PARAM_FLAGS, "Flags", cParamSpec);
    rbg_define_method(c, "initialize", flags_initialize, 6);

    c = G_DEF_CLASS(G_TYPE_PARAM_STRING, "String", cParamSpec);
    rbg_define_method(c, "initialize", string_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_PARAM, "Param", cParamSpec);
    rbg_define_method(c, "initialize", param_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_BOXED, "Boxed", cParamSpec);
    rbg_define_method(c, "initialize", boxed_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_POINTER, "Pointer", cParamSpec);
    rbg_define_method(c, "initialize", pointer_initialize, 4);

    c = G_DEF_CLASS(G_TYPE_PARAM_OBJECT, "Object", cParamSpec);
    rbg_define_method(c, "initialize", object_initialize, 5);
}
