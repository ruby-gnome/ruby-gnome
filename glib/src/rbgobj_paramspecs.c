/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_paramspecs.c -

  $Author: sakai $
  $Date: 2002/07/27 14:46:35 $
  created at: Sun Jul 26 14:31:33 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE
char_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_char(StringValuePtr(name),
                              StringValuePtr(nick),
                              StringValuePtr(blurb),
                              NUM2INT(minimum),
                              NUM2INT(maximum),
                              NUM2INT(default_value),
                              NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
uchar_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_uchar(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               NUM2INT(minimum),
                               NUM2INT(maximum),
                               NUM2INT(default_value),
                               NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
boolean_new(klass, name, nick, blurb, default_value, flags)
   VALUE klass, name, nick, blurb, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_boolean(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 RTEST(default_value),
                                 NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
int_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_int(StringValuePtr(name),
                             StringValuePtr(nick),
                             StringValuePtr(blurb),
                             NUM2INT(minimum),
                             NUM2INT(maximum),
                             NUM2INT(default_value),
                             NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
uint_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_uint(StringValuePtr(name),
                              StringValuePtr(nick),
                              StringValuePtr(blurb),
                              NUM2INT(minimum),
                              NUM2INT(maximum),
                              NUM2INT(default_value),
                              NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
long_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_long(StringValuePtr(name),
                              StringValuePtr(nick),
                              StringValuePtr(blurb),
                              NUM2INT(minimum),
                              NUM2INT(maximum),
                              NUM2INT(default_value),
                              NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
ulong_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_ulong(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               NUM2INT(minimum),
                               NUM2INT(maximum),
                               NUM2INT(default_value),
                               NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

// FIXME
static VALUE
int64_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_int64(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               NUM2INT(minimum),
                               NUM2INT(maximum),
                               NUM2INT(default_value),
                               NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

// FIXME
static VALUE
uint64_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_uint64(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                NUM2INT(minimum),
                                NUM2INT(maximum),
                                NUM2INT(default_value),
                                NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
unichar_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_unichar(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 NUM2INT(default_value),
                                 NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

#if 0
GParamSpec*	g_param_spec_enum	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  enum_type,
					  gint		  default_value,
					  GParamFlags	  flags);
#endif

#if 0
GParamSpec*	g_param_spec_flags	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  flags_type,
					  guint		  default_value,
					  GParamFlags	  flags);
#endif

static VALUE
float_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_float(StringValuePtr(name),
                               StringValuePtr(nick),
                               StringValuePtr(blurb),
                               NUM2DBL(minimum),
                               NUM2DBL(maximum),
                               NUM2DBL(default_value),
                               NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
double_new(klass, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE klass, name, nick, blurb, minimum, maximum, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_double(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                NUM2DBL(minimum),
                                NUM2DBL(maximum),
                                NUM2DBL(default_value),
                                NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

static VALUE
string_new(klass, name, nick, blurb, default_value, flags)
   VALUE klass, name, nick, blurb, default_value, flags;
{
    VALUE result;
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_string(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                StringValuePtr(default_value),
                                NUM2INT(flags));
    result = rbgobj_param_spec_wrap(pspec);
    g_param_spec_unref(pspec);
    return result;
}

#if 0
GParamSpec*	g_param_spec_param	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  param_type,
					  GParamFlags	  flags);
GParamSpec*	g_param_spec_boxed	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  boxed_type,
					  GParamFlags	  flags);
GParamSpec*	g_param_spec_pointer	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GParamFlags	  flags);
GParamSpec*	g_param_spec_value_array (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GParamSpec	 *element_spec,
					  GParamFlags	  flags);
GParamSpec*	g_param_spec_object	 (const gchar	 *name,
					  const gchar	 *nick,
					  const gchar	 *blurb,
					  GType		  object_type,
					  GParamFlags	  flags);
#endif

void
Init_gobject_gparamspecs()
{
    VALUE c;

    c = rb_define_class_under(rbgobj_cParamSpec, "Char", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", char_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "UChar", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", uchar_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "Boolean", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", boolean_new, 5);

    c = rb_define_class_under(rbgobj_cParamSpec, "Int", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", int_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "UInt", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", uint_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "Long", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", long_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "ULong", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", ulong_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "Int64", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", int64_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "UInt64", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", uint64_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "UniChar", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", unichar_new, 5);

    c = rb_define_class_under(rbgobj_cParamSpec, "Float", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", float_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "Double", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", double_new, 7);

    c = rb_define_class_under(rbgobj_cParamSpec, "String", rbgobj_cParamSpec);
    rb_define_singleton_method(c, "new", string_new, 5);
}
