/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_paramspecs.c -

  $Author: sakai $
  $Date: 2002/08/08 15:17:32 $
  created at: Sun Jul 26 14:31:33 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE
char_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
uchar_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
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
                                 NUM2INT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
int_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
uint_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
long_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
ulong_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

// FIXME
static VALUE
int64_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

// FIXME
static VALUE
uint64_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
unichar_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_unichar(StringValuePtr(name),
                                 StringValuePtr(nick),
                                 StringValuePtr(blurb),
                                 NUM2INT(default_value),
                                 NUM2INT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
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
float_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
double_initialize(self, name, nick, blurb, minimum, maximum, default_value, flags)
   VALUE self, name, nick, blurb, minimum, maximum, default_value, flags;
{
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
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
}

static VALUE
string_initialize(self, name, nick, blurb, default_value, flags)
   VALUE self, name, nick, blurb, default_value, flags;
{
    GParamSpec* pspec;
    StringValue(name);
    StringValue(nick);
    StringValue(blurb);
    pspec = g_param_spec_string(StringValuePtr(name),
                                StringValuePtr(nick),
                                StringValuePtr(blurb),
                                StringValuePtr(default_value),
                                NUM2INT(flags));
    rbgobj_param_spec_initialize(self, pspec);
    return Qnil;
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
    VALUE cParamSpec = GTYPE2CLASS(G_TYPE_PARAM);
    VALUE c;

    c = G_DEF_CLASS(G_TYPE_PARAM_CHAR, "Char", cParamSpec);
    rb_define_method(c, "initialize", char_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_UCHAR, "UChar", cParamSpec);
    rb_define_method(c, "initialize", uchar_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_BOOLEAN, "Boolean", cParamSpec);
    rb_define_method(c, "initialize", boolean_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_INT, "Int", cParamSpec);
    rb_define_method(c, "initialize", int_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_UINT, "UInt", cParamSpec);
    rb_define_method(c, "initialize", uint_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_LONG, "Long", cParamSpec);
    rb_define_method(c, "initialize", long_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_ULONG, "ULong", cParamSpec);
    rb_define_method(c, "initialize", ulong_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_INT64, "Int64", cParamSpec);
    rb_define_method(c, "initialize", int64_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_UINT64, "UInt64", cParamSpec);
    rb_define_method(c, "initialize", uint64_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_UNICHAR, "UniChar", cParamSpec);
    rb_define_method(c, "initialize", unichar_initialize, 5);

    c = G_DEF_CLASS(G_TYPE_PARAM_FLOAT, "Float", cParamSpec);
    rb_define_method(c, "initialize", float_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_DOUBLE, "Double", cParamSpec);
    rb_define_method(c, "initialize", double_initialize, 7);

    c = G_DEF_CLASS(G_TYPE_PARAM_STRING, "String", cParamSpec);
    rb_define_method(c, "initialize", string_initialize, 5);
}
