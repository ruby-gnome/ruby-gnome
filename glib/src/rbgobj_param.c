/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_param.c -

  $Author: sakai $
  $Date: 2002/08/09 12:44:01 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

static VALUE cParamSpec;
static GQuark qparamspec;

static VALUE pspec_s_allocate(VALUE klass);

typedef struct {
    GParamSpec* instance;
    const RGObjClassInfo* cinfo;
} pspec_holder;

static void
pspec_mark(holder)
    pspec_holder* holder;
{
    if (holder->instance && holder->cinfo && holder->cinfo->mark)
        holder->cinfo->mark(holder->instance);
}

static void
pspec_free(holder)
    pspec_holder* holder;
{
    if (holder->instance){
        if (holder->cinfo && holder->cinfo->free)
            holder->cinfo->free(holder->instance);
        g_param_spec_set_qdata(holder->instance, qparamspec, NULL);
        g_param_spec_unref(holder->instance);
    }
    free(holder);
}

GParamSpec*
rbgobj_param_spec_get_struct(VALUE obj)
{
    pspec_holder* holder;
    Data_Get_Struct(obj, pspec_holder, holder);
    return G_PARAM_SPEC(holder->instance);
}

void
rbgobj_param_spec_initialize(self, pspec)
    VALUE self;
    GParamSpec* pspec;
{
    pspec_holder* holder;
    Data_Get_Struct(self, pspec_holder, holder);

    pspec = g_param_spec_ref(pspec);
    g_param_spec_sink(pspec);

    holder->instance = pspec;
    holder->cinfo    = rbgobj_lookup_class_by_gtype(G_PARAM_SPEC_TYPE(pspec));
    g_param_spec_set_qdata(pspec, qparamspec, (gpointer)self);
}

VALUE
rbgobj_get_value_from_paramspec(GParamSpec* pspec)
{
    gpointer data = g_param_spec_get_qdata(pspec, qparamspec);
    if (!data)
        return (VALUE)data;
    else {
        VALUE result = pspec_s_allocate(GTYPE2CLASS(G_PARAM_SPEC_TYPE(pspec)));
        rbgobj_param_spec_initialize(result, pspec);
        return result;
    }
}

/**********************************************************************/

static VALUE
pspec_s_allocate(VALUE klass)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(klass);
    if (G_TYPE_IS_ABSTRACT(cinfo->gtype))
        rb_raise(rb_eTypeError, "abstract class");

    {
        pspec_holder* holder;
        VALUE result;

        result = Data_Make_Struct(klass, pspec_holder, pspec_mark, pspec_free,
                                  holder);
        holder->instance = NULL;
        holder->cinfo    = NULL;

        return result;
    }
}

#ifdef HAVE_OBJECT_ALLOCATE
#define pspec_s_new rb_class_new_instance
#else
static VALUE 
pspec_s_new(int argc, VALUE* argv, VALUE klass)
{
    VALUE obj = pspec_s_allocate(klass);
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif

// XXX: ripped from rbgobj_object.c
static VALUE
gobj_s_get_gtype(klass)
    VALUE klass;
{
    return rbgobj_gtype_new(rbgobj_lookup_class(klass)->gtype);
}

// XXX: ripped from rbgobj_object.c
static VALUE
gobj_get_gtype(self)
    VALUE self;
{
    return rbgobj_gtype_new(G_TYPE_FROM_INSTANCE(RVAL2GOBJ(self)));
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
    return rbgobj_gtype_new(G_PARAM_SPEC_VALUE_TYPE(rbgobj_param_spec_get_struct(self)));
}

static VALUE
get_owner_type(VALUE self)
{
    return rbgobj_gtype_new(rbgobj_param_spec_get_struct(self)->owner_type);
}

static VALUE
value_set_default(VALUE self, VALUE val)
{
    GValue tmp = {0,};
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

static VALUE
value_validate(self, value)
    VALUE self, value;
{
    GValue tmp = {0,};
    gboolean result;

    g_value_init(&tmp,
                 G_PARAM_SPEC_VALUE_TYPE(rbgobj_param_spec_get_struct(self)));
    rbgobj_rvalue_to_gvalue(value, &tmp);
    result = g_param_value_validate(rbgobj_param_spec_get_struct(self), &tmp);
    g_value_unset(&tmp);

    return result ? Qtrue : Qfalse;
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
get_ref_count(self)
    VALUE self;
{
    return INT2NUM(G_PARAM_SPEC(rbgobj_param_spec_get_struct(self))->ref_count);
}

/**********************************************************************/

static void
Init_gobject_gparam_spec()
{    
    qparamspec = g_quark_from_static_string("__ruby_gobject_param_spec__");
    cParamSpec = G_DEF_CLASS(G_TYPE_PARAM, "ParamSpec", mGLib);

    rb_define_singleton_method(cParamSpec, "allocate", pspec_s_allocate, 0);
#ifndef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(cParamSpec, "new", pspec_s_new, -1);
#endif

    rb_define_singleton_method(cParamSpec, "gtype", gobj_s_get_gtype, 0);
    rb_define_method(cParamSpec, "gtype", gobj_get_gtype, 0);

    rb_define_method(cParamSpec, "name", get_name, 0);
    rb_define_method(cParamSpec, "nick", get_nick, 0);
    rb_define_method(cParamSpec, "blurb", get_blurb, 0);

    rb_define_method(cParamSpec, "flags", get_flags, 0);
    rb_define_method(cParamSpec, "value_type", get_value_type, 0);
    rb_define_method(cParamSpec, "owner_type", get_owner_type, 0);

    rb_define_method(cParamSpec, "default=", value_set_default, 1);
    rb_define_method(cParamSpec, "defaults", value_defaults, 1);

    /* for debugging */
    rb_define_method(cParamSpec, "ref_count", get_ref_count, 0);
}

/**********************************************************************/

void Init_gobject_gparam()
{
    Init_gobject_gparam_spec();
}
