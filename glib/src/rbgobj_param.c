/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_param.c -

  $Author: sakai $
  $Date: 2003/07/13 16:26:45 $
  created at: Sun Jun  9 20:31:47 JST 2002

  Copyright (C) 2002,2003  Masahiro Sakai

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
rbgobj_get_value_from_param_spec(GParamSpec* pspec)
{
    gpointer data = g_param_spec_get_qdata(pspec, qparamspec);
    if (data)
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

static VALUE
inspect(VALUE self)
{
    GParamSpec* pspec = RVAL2GOBJ(self);
    VALUE v = rb_inspect(GTYPE2CLASS(pspec->owner_type));
    gchar* str = g_strdup_printf("#<%s: %s#%s>",
                                 rb_class2name(CLASS_OF(self)),
                                 StringValuePtr(v),
                                 g_param_spec_get_name(pspec));
    VALUE result = rb_str_new2(str);
    g_free(str);
    return result;
}

static VALUE
get_name(VALUE self)
{
    return rb_str_new2(g_param_spec_get_name(rbgobj_param_spec_get_struct(self)));
}

static VALUE
get_nick(VALUE self)
{
    const gchar* str = g_param_spec_get_nick(rbgobj_param_spec_get_struct(self));
    return str ? rb_str_new2(str) : Qnil;
}

static VALUE
get_blurb(VALUE self)
{
    const gchar* str = g_param_spec_get_blurb(rbgobj_param_spec_get_struct(self));
    return str ? rb_str_new2(str) : Qnil;
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
get_owner(VALUE self)
{
    return GTYPE2CLASS(rbgobj_param_spec_get_struct(self)->owner_type);
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

/* fixme: return modified value */
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
#endif

static VALUE
values_compare(self, a, b)
    VALUE self, a, b;
{
    GValue v1 = {0,};
    GValue v2 = {0,};
    GType type = G_PARAM_SPEC_VALUE_TYPE(rbgobj_param_spec_get_struct(self));
    gint result;

    g_value_init(&v1, type);
    g_value_init(&v2, type);

    /* FIXME: use rb_ensure to ensure following g_value_unset() call*/
    rbgobj_rvalue_to_gvalue(a, &v1);
    rbgobj_rvalue_to_gvalue(b, &v2);

    result = g_param_values_cmp(rbgobj_param_spec_get_struct(self), &v1, &v2);

    g_value_unset(&v1);
    g_value_unset(&v2);

    return INT2NUM(result);
}


static VALUE
get_ref_count(self)
    VALUE self;
{
    return INT2NUM(G_PARAM_SPEC(rbgobj_param_spec_get_struct(self))->ref_count);
}


#define param_is_flag(flag) \
    static VALUE \
    param_is_##flag(self) \
        VALUE self; \
    { \
        GParamSpec* pspec = G_PARAM_SPEC(rbgobj_param_spec_get_struct(self)); \
        return (pspec->flags & flag) ? Qtrue : Qfalse; \
    }

param_is_flag(G_PARAM_READABLE)
param_is_flag(G_PARAM_WRITABLE)
param_is_flag(G_PARAM_CONSTRUCT)
param_is_flag(G_PARAM_CONSTRUCT_ONLY)
param_is_flag(G_PARAM_LAX_VALIDATION)
param_is_flag(G_PARAM_PRIVATE)
param_is_flag(G_PARAM_READWRITE)

/**********************************************************************/

static void
Init_gobject_gparam_spec()
{    
    qparamspec = g_quark_from_static_string("__ruby_gobject_param_spec__");
    cParamSpec = G_DEF_CLASS(G_TYPE_PARAM, "Param", mGLib);

    /* GParamFlags */
    rb_define_const(cParamSpec, "READABLE",       INT2FIX(G_PARAM_READABLE));
    rb_define_const(cParamSpec, "WRITABLE",       INT2FIX(G_PARAM_WRITABLE));
    rb_define_const(cParamSpec, "CONSTRUCT",      INT2FIX(G_PARAM_CONSTRUCT));
    rb_define_const(cParamSpec, "CONSTRUCT_ONLY", INT2FIX(G_PARAM_CONSTRUCT_ONLY));
    rb_define_const(cParamSpec, "LAX_VALIDATION", INT2FIX(G_PARAM_LAX_VALIDATION));
    rb_define_const(cParamSpec, "PRIVATE",        INT2FIX(G_PARAM_PRIVATE));
    rb_define_const(cParamSpec, "READWRITE",      INT2FIX(G_PARAM_READWRITE));
    rb_define_const(cParamSpec, "MASK",           INT2FIX(G_PARAM_MASK));
    rb_define_const(cParamSpec, "USER_SHIFT",     INT2FIX(G_PARAM_USER_SHIFT));


#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    rb_define_singleton_method(cParamSpec, "allocate", pspec_s_allocate, 0);
#else
    rb_define_alloc_func(cParamSpec, pspec_s_allocate);
#endif

    rb_define_method(cParamSpec, "inspect", inspect, 0);

    rb_define_method(cParamSpec, "name", get_name, 0);
    rb_define_method(cParamSpec, "nick", get_nick, 0);
    rb_define_method(cParamSpec, "blurb", get_blurb, 0);

    rb_define_method(cParamSpec, "flags", get_flags, 0);
    rb_define_method(cParamSpec, "value_type", get_value_type, 0);
    //rb_define_alias(cParamSpec, "type", "value_type");
    rb_define_method(cParamSpec, "owner_type", get_owner_type, 0);
    rb_define_method(cParamSpec, "owner", get_owner, 0);

    rb_define_method(cParamSpec, "default=", value_set_default, 1);
    rb_define_method(cParamSpec, "defaults", value_defaults, 1);

    rb_define_method(cParamSpec, "validate", value_validate, 0);
    rb_define_method(cParamSpec, "compare", values_compare, 2);

    /* for debugging */
    rb_define_method(cParamSpec, "ref_count", get_ref_count, 0);

    rb_define_method(cParamSpec, "readable?",       param_is_G_PARAM_READABLE, 0);
    rb_define_method(cParamSpec, "writable?",       param_is_G_PARAM_WRITABLE, 0);
    rb_define_method(cParamSpec, "construct?",      param_is_G_PARAM_CONSTRUCT, 0);
    rb_define_method(cParamSpec, "construct_only?", param_is_G_PARAM_CONSTRUCT_ONLY, 0);
    rb_define_method(cParamSpec, "lax_validation?", param_is_G_PARAM_LAX_VALIDATION, 0);
    rb_define_method(cParamSpec, "private?",        param_is_G_PARAM_PRIVATE, 0);
    rb_define_method(cParamSpec, "readwrite?",      param_is_G_PARAM_READWRITE, 0);
}

/**********************************************************************/

void Init_gobject_gparam()
{
    Init_gobject_gparam_spec();
}
