/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_param.c -

  $Author: sakai $
  $Date: 2002/07/26 14:31:33 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

VALUE rbgobj_cParamSpec;

GParamSpec*
rbgobj_param_spec_get_struct(VALUE obj)
{
    GParamSpec* result;
    Data_Get_Struct(obj, GParamSpec, result);
    return G_PARAM_SPEC(result);
}

VALUE
rbgobj_param_spec_wrap(GParamSpec* pspec)
{
    pspec = g_param_spec_ref(pspec);
    g_param_spec_sink(pspec);
    return Data_Wrap_Struct(rbgobj_cParamSpec, NULL,
                            g_param_spec_unref, pspec);
}

static VALUE
get_name(VALUE self)
{
    return rb_str_new2(g_param_spec_get_name(rbgobj_param_spec_get_struct(self)));
}

static VALUE
get_nick(VALUE self)
{
    return rb_str_new2(g_param_spec_get_nick(rbgobj_param_spec_get_struct(self)));
}

static VALUE
get_blurb(VALUE self)
{
    return rb_str_new2(g_param_spec_get_blurb(rbgobj_param_spec_get_struct(self)));
}

static VALUE
get_flags(VALUE self)
{
    return INT2NUM(rbgobj_param_spec_get_struct(self)->flags);
}

static VALUE
get_value_type(VALUE self)
{
    return INT2NUM(G_PARAM_SPEC_VALUE_TYPE(rbgobj_param_spec_get_struct(self)));
}

static VALUE
get_owner_type(VALUE self)
{
    return INT2NUM(rbgobj_param_spec_get_struct(self)->owner_type);
}

static VALUE
value_set_default(VALUE self, VALUE val)
{
    GValue tmp;
    g_value_init(&tmp,
                 G_PARAM_SPEC_VALUE_TYPE(rbgobj_param_spec_get_struct(self)));
    rbgobj_rvalue_to_gvalue(val, &tmp);
    g_param_value_set_default(rbgobj_param_spec_get_struct(self), &tmp);
    g_value_unset(&tmp);
    return val;
}

static VALUE
value_defaults(VALUE self, VALUE val)
{
    GValue tmp;
    gboolean result;

    g_value_init(&tmp,
                 G_PARAM_SPEC_VALUE_TYPE(rbgobj_param_spec_get_struct(self)));
    rbgobj_rvalue_to_gvalue(val, &tmp);
    result = g_param_value_defaults(rbgobj_param_spec_get_struct(self), &tmp);
    g_value_unset(&tmp);

    return result ? Qtrue : Qfalse;
}


#if 0
gboolean	g_param_value_validate		(GParamSpec    *pspec,
						 GValue	       *value);
#endif
static VALUE
value_validate(VALUE self)
{
}

#if 0
gboolean	g_param_value_convert		(GParamSpec    *pspec,
						 const GValue  *src_value,
						 GValue	       *dest_value,
						 gboolean	strict_validation);
gint		g_param_values_cmp		(GParamSpec    *pspec,
						 const GValue  *value1,
						 const GValue  *value2);
#endif

static VALUE
_pspec_to_ruby(const GValue* from)
{
    return rbgobj_param_spec_wrap(g_value_get_param(from));
}

static void
_pspec_from_ruby(VALUE from, GValue* to)
{
    g_value_set_param(to, rbgobj_param_spec_get_struct(from));
}

static void
Init_gobject_gparam_spec()
{
    rbgobj_cParamSpec = rb_define_class_under(mGLib, "ParamSpec", rb_cData);

    rbgobj_register_r2g_func(rbgobj_cParamSpec, _pspec_from_ruby);
    rbgobj_register_g2r_func(G_TYPE_PARAM, _pspec_to_ruby);

    rb_define_method(rbgobj_cParamSpec, "name", get_name, 0);
    rb_define_method(rbgobj_cParamSpec, "nick", get_nick, 0);
    rb_define_method(rbgobj_cParamSpec, "blurb", get_blurb, 0);

    rb_define_method(rbgobj_cParamSpec, "flags", get_flags, 0);
    rb_define_method(rbgobj_cParamSpec, "value_type", get_value_type, 0);
    rb_define_method(rbgobj_cParamSpec, "owner_type", get_owner_type, 0);

    rb_define_method(rbgobj_cParamSpec, "default=", value_set_default, 1);
    rb_define_method(rbgobj_cParamSpec, "defaults", value_defaults, 1);
}

/**********************************************************************/

void Init_gobject_gparam()
{
    Init_gobject_gparam_spec();
    Init_gobject_gparamspecs();
}
