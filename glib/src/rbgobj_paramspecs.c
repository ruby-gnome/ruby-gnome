/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_paramspecs.c -

  $Author: sakai $
  $Date: 2002/09/01 13:19:21 $
  created at: Sun Jul 26 14:31:33 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

#define DEF_DEFAULT_VALUE_FUNC(typename, to_ruby, pspec_cast) \
static VALUE \
typename##_default_value(self) \
{ \
    return to_ruby(pspec_cast(rbgobj_param_spec_get_struct(self))->default_value); \
} \

#define DEF_NUMERIC_PSPEC_METHODS_FUNC(typename, from_ruby, to_ruby, pspec_cast) \
static VALUE \
typename##_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags) \
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags; \
{ \
    GParamSpec* pspec; \
    StringValue(name); \
    StringValue(nick); \
    StringValue(blurb); \
    pspec = g_param_spec_##typename(StringValuePtr(name), \
                                    StringValuePtr(nick), \
                                    StringValuePtr(blurb), \
                                    from_ruby(minimum), \
                                    from_ruby(maximum), \
                                    from_ruby(default_value), \
                                    NUM2UINT(flags)); \
    rbgobj_param_spec_initialize(self, pspec); \
    return Qnil; \
} \
\
static \
VALUE typename##_minimum(self) \
{ \
    return to_ruby(pspec_cast(rbgobj_param_spec_get_struct(self))->minimum); \
} \
\
static \
VALUE typename##_maximum(self) \
{ \
    return to_ruby(pspec_cast(rbgobj_param_spec_get_struct(self))->minimum); \
} \
\
DEF_DEFAULT_VALUE_FUNC(typename, to_ruby, pspec_cast) \


DEF_NUMERIC_PSPEC_METHODS_FUNC(char, NUM2INT, INT2FIX, G_PARAM_SPEC_CHAR)
DEF_NUMERIC_PSPEC_METHODS_FUNC(uchar, NUM2UINT, INT2FIX, G_PARAM_SPEC_UCHAR)
DEF_NUMERIC_PSPEC_METHODS_FUNC(int, NUM2INT, INT2NUM, G_PARAM_SPEC_INT)
DEF_NUMERIC_PSPEC_METHODS_FUNC(uint, NUM2UINT, UINT2NUM, G_PARAM_SPEC_UINT)
DEF_NUMERIC_PSPEC_METHODS_FUNC(long, NUM2LONG, INT2NUM, G_PARAM_SPEC_LONG)
DEF_NUMERIC_PSPEC_METHODS_FUNC(ulong, NUM2ULONG, UINT2NUM, G_PARAM_SPEC_ULONG)
DEF_NUMERIC_PSPEC_METHODS_FUNC(int64, rbglib_num_to_int64, rbglib_int64_to_num, G_PARAM_SPEC_INT64)
DEF_NUMERIC_PSPEC_METHODS_FUNC(uint64, rbglib_num_to_uint64, rbglib_uint64_to_num, G_PARAM_SPEC_UINT64)
DEF_NUMERIC_PSPEC_METHODS_FUNC(float, NUM2DBL, rb_float_new, G_PARAM_SPEC_FLOAT)
DEF_NUMERIC_PSPEC_METHODS_FUNC(double, NUM2DBL, rb_float_new, G_PARAM_SPEC_DOUBLE)

static VALUE
float_epsilon(self)
    VALUE self;
{
    return rb_float_new(G_PARAM_SPEC_FLOAT(rbgobj_param_spec_get_struct(self))->epsilon);
}

static VALUE
double_epsilon(self)
    VALUE self;
{
    return rb_float_new(G_PARAM_SPEC_DOUBLE(rbgobj_param_spec_get_struct(self))->epsilon);
}


static VALUE
boolean_initialize(self, name, nick, blurb, default_value, flags)
   VALUE self, name, nick, blurb, default_value, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_boolean(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 RTEST(default_value),
                                 NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static inline VALUE to_bool(gboolean val){return val ? Qtrue : Qfalse;}
DEF_DEFAULT_VALUE_FUNC(boolean, to_bool, G_PARAM_SPEC_BOOLEAN)

static VALUE
unichar_initialize(self, name, nick, blurb, default_value, flags)
   VALUE self, name, nick, blurb, default_value, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_unichar(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 NUM2UINT(default_value),
                                 NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

DEF_DEFAULT_VALUE_FUNC(unichar, INT2NUM, G_PARAM_SPEC_UNICHAR)

static VALUE
enum_initialize(self, name, nick, blurb, enum_type, default_value, flags)
   VALUE self, name, nick, blurb, enum_type, default_value, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_enum(StringValuePtr(name),
                              StringValuePtr(nick),
                              StringValuePtr(blurb),
                              rbgobj_gtype_get(enum_type),
                              NUM2UINT(default_value),
                              NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

DEF_DEFAULT_VALUE_FUNC(enum, INT2FIX, G_PARAM_SPEC_ENUM)

static VALUE
flags_initialize(self, name, nick, blurb, flags_type, default_value, flags)
   VALUE self, name, nick, blurb, flags_type, default_value, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_enum(StringValuePtr(name),
                              StringValuePtr(nick),
                              StringValuePtr(blurb),
                              rbgobj_gtype_get(flags_type),
                              NUM2UINT(default_value),
                              NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

DEF_DEFAULT_VALUE_FUNC(flags, INT2FIX, G_PARAM_SPEC_FLAGS)

static VALUE
string_initialize(self, name, nick, blurb, default_value, flags)
   VALUE self, name, nick, blurb, default_value, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    StringValue(default_value);
    pspec = g_param_spec_string(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                StringValuePtr(default_value),
                                NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

DEF_DEFAULT_VALUE_FUNC(string, rb_str_new2, G_PARAM_SPEC_STRING)

static VALUE
param_initialize(self, name, nick, blurb, param_type, flags)
   VALUE self, name, nick, blurb, param_type, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_param(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               rbgobj_gtype_get(param_type),
                               NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
boxed_initialize(self, name, nick, blurb, boxed_type, flags)
   VALUE self, name, nick, blurb, boxed_type, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_boxed(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               rbgobj_gtype_get(boxed_type),
                               NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
pointer_initialize(self, name, nick, blurb, flags)
   VALUE self, name, nick, blurb, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_pointer(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
value_array_initialize(self, name, nick, blurb, element_spec, flags)
   VALUE self, name, nick, blurb, element_spec, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_value_array(StringValuePtr(name),
                                     StringValuePtr(nick),
                                     StringValuePtr(blurb),
                                     rbgobj_param_spec_get_struct(element_spec),
                                     NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
object_initialize(self, name, nick, blurb, object_type, flags)
   VALUE self, name, nick, blurb, object_type, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_object(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                rbgobj_gtype_get(object_type),
                                NUM2UINT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

void
Init_gobject_gparamspecs()
{
    VALUE cParamSpec = GTYPE2CLASS(G_TYPE_PARAM);
    VALUE c;

#define DEF_NUMERIC_PSPEC_METHODS(c, typename) \
  G_STMT_START {\
    rb_define_method(c, "initialize", typename##_initialize, 7); \
    rb_define_method(c, "minimum", typename##_minimum, 0); \
    rb_define_method(c, "maximum", typename##_maximum, 0); \
    rb_define_method(c, "default_value", typename##_default_value, 0); \
    rb_define_alias(c, "default", "default_value"); \
  } G_STMT_END

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
    rb_define_method(c, "epsilon", float_epsilon, 0);

    c = G_DEF_CLASS(G_TYPE_PARAM_DOUBLE, "Double", cParamSpec);
    DEF_NUMERIC_PSPEC_METHODS(c, double);
    rb_define_method(c, "epsilon", double_epsilon, 0);


    c = G_DEF_CLASS(G_TYPE_PARAM_BOOLEAN, "Boolean", cParamSpec);
    rb_define_method(c, "initialize", boolean_initialize, 5);
    rb_define_method(c, "default_value", boolean_default_value, 0);
    rb_define_alias(c, "default", "default_value");

    c = G_DEF_CLASS(G_TYPE_PARAM_UNICHAR, "UniChar", cParamSpec);
    rb_define_method(c, "initialize", unichar_initialize, 5);
    rb_define_method(c, "default_value", unichar_default_value, 0);
    rb_define_alias(c, "default", "default_value");

    c = G_DEF_CLASS(G_TYPE_PARAM_STRING, "Enum", cParamSpec);
    rb_define_method(c, "initialize", enum_initialize, 6);
    rb_define_method(c, "default_value", enum_default_value, 0);
    rb_define_alias(c, "default", "default_value");

    c = G_DEF_CLASS(G_TYPE_PARAM_STRING, "Flags", cParamSpec);
    rb_define_method(c, "initialize", flags_initialize, 6);
    rb_define_method(c, "default_value", flags_default_value, 0);
    rb_define_alias(c, "default", "default_value");

    c = G_DEF_CLASS(G_TYPE_PARAM_STRING, "String", cParamSpec);
    rb_define_method(c, "initialize", string_initialize, 5);
    rb_define_method(c, "default_value", string_default_value, 0);
    rb_define_alias(c, "default", "default_value");

    c = G_DEF_CLASS(G_TYPE_PARAM_PARAM, "Param", cParamSpec);
    rb_define_method(c, "initialize", param_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_BOXED, "Boxed", cParamSpec);
    rb_define_method(c, "initialize", boxed_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_POINTER, "Pointer", cParamSpec);
    rb_define_method(c, "initialize", pointer_initialize, 4);

    c = G_DEF_CLASS(G_TYPE_PARAM_VALUE_ARRAY, "ValueArray", cParamSpec);
    rb_define_method(c, "initialize", value_array_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_OBJECT, "Object", cParamSpec);
    rb_define_method(c, "initialize", object_initialize, 5);
}
