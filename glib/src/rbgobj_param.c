/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_param.c -

  $Author: sakai $
  $Date: 2002/06/17 18:14:24 $
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

static VALUE
rbgobj_param_spec_wrap(GParamSpec* pspec)
{
    pspec = g_param_spec_ref(pspec);
    return Data_Wrap_Struct(rbgobj_cParamSpec, NULL,
                            g_param_spec_unref, pspec);
}

static VALUE
get_name(VALUE self)
{
    return rb_str_new2(rbgobj_param_spec_get_struct(self)->name);
}

static VALUE
get_flags(VALUE self)
{
    return INT2NUM(rbgobj_param_spec_get_struct(self)->flags);
}

static VALUE
get_value_type(VALUE self)
{
    return INT2NUM(rbgobj_param_spec_get_struct(self)->value_type);
}

static VALUE
get_owner_type(VALUE self)
{
    return INT2NUM(rbgobj_param_spec_get_struct(self)->owner_type);
}

static void
Init_gobject_gparam_spec()
{
    rbgobj_cParamSpec = rb_define_class_under(mGLib, "ParamSpec", rb_cData);
    rb_define_method(rbgobj_cParamSpec, "name", get_name, 0);
    rb_define_method(rbgobj_cParamSpec, "flags", get_flags, 0);
    rb_define_method(rbgobj_cParamSpec, "value_type", get_value_type, 0);
    rb_define_method(rbgobj_cParamSpec, "owner_type", get_owner_type, 0);
}

/**********************************************************************/

static
void Init_gobject_gparameter()
{
    
}

/**********************************************************************/

void Init_gobject_gparam()
{
    Init_gobject_gparam_spec();
    Init_gobject_gparameter();
}
