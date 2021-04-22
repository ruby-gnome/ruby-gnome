/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE rbgobj_cParam

VALUE RG_TARGET_NAMESPACE;
static GQuark qparamspec;

static VALUE pspec_s_allocate(VALUE klass);

typedef struct {
    GParamSpec* instance;
    const RGObjClassInfo* cinfo;
} pspec_holder;

static void
pspec_mark(void *data)
{
    pspec_holder *holder = data;
    if (holder->instance)
        rbgobj_instance_call_cinfo_mark(holder->instance);
}

static void
pspec_free(void *data)
{
    pspec_holder *holder = data;
    if (holder->instance){
        rbgobj_instance_call_cinfo_free(holder->instance);
        g_param_spec_set_qdata(holder->instance, qparamspec, NULL);
        g_param_spec_unref(holder->instance);
    }
    xfree(holder);
}

static const rb_data_type_t rg_glib_param_type = {
    "GLib::Param",
    {
        pspec_mark,
        pspec_free,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

static pspec_holder *
rbgobj_param_get_raw(VALUE rb_param)
{
    pspec_holder *holder;
    TypedData_Get_Struct(rb_param, pspec_holder, &rg_glib_param_type, holder);
    return holder;
}

GParamSpec*
rbgobj_get_param_spec(VALUE obj)
{
    return G_PARAM_SPEC(rbgobj_param_get_raw(obj)->instance);
}

void
rbgobj_param_spec_initialize(VALUE self, GParamSpec *pspec)
{
    pspec_holder *holder;

    holder = rbgobj_param_get_raw(self);

    pspec = g_param_spec_ref(pspec);
    g_param_spec_sink(pspec);

    holder->instance = pspec;
    holder->cinfo    = GTYPE2CINFO(G_PARAM_SPEC_TYPE(pspec));
    g_param_spec_set_qdata(pspec, qparamspec, (gpointer)self);
}

VALUE
rbgobj_get_ruby_object_from_param_spec(GParamSpec* pspec, gboolean alloc)
{
    gpointer data = g_param_spec_get_qdata(pspec, qparamspec);
    if (data)
        return (VALUE)data;
    else if (alloc) {
        VALUE result = pspec_s_allocate(GTYPE2CLASS(G_PARAM_SPEC_TYPE(pspec)));
        rbgobj_param_spec_initialize(result, pspec);
        return result;
    } else
        return Qnil;
}

/**********************************************************************/

static VALUE
pspec_s_allocate(VALUE klass)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(klass);
    if (G_TYPE_IS_ABSTRACT(cinfo->gtype))
        rb_raise(rb_eTypeError, "abstract class");

    {
        pspec_holder *holder;
        VALUE rb_param;

        rb_param = TypedData_Make_Struct(klass,
                                         pspec_holder,
                                         &rg_glib_param_type,
                                         holder);
        holder->instance = NULL;
        holder->cinfo    = NULL;

        return rb_param;
    }
}

static VALUE
rg_inspect(VALUE self)
{
    GParamSpec *pspec = rbgobj_get_param_spec(self);

    return rb_sprintf("#<%" PRIsVALUE "%" PRIsVALUE "#%s",
                      CLASS_OF(self),
                      GTYPE2CLASS(pspec->owner_type),
                      g_param_spec_get_name(pspec));
}

static VALUE
rg_name(VALUE self)
{
    return rb_str_new2(g_param_spec_get_name(rbgobj_get_param_spec(self)));
}

static VALUE
rg_nick(VALUE self)
{
    const gchar* str = g_param_spec_get_nick(rbgobj_get_param_spec(self));
    return str ? rb_str_new2(str) : Qnil;
}

static VALUE
rg_blurb(VALUE self)
{
    const gchar* str = g_param_spec_get_blurb(rbgobj_get_param_spec(self));
    return str ? rb_str_new2(str) : Qnil;
}

static VALUE
rg_flags(VALUE self)
{
    return INT2NUM(rbgobj_get_param_spec(self)->flags);
}

static VALUE
rg_value_type(VALUE self)
{
    return rbgobj_gtype_new(G_PARAM_SPEC_VALUE_TYPE(rbgobj_get_param_spec(self)));
}

static VALUE
rg_owner_type(VALUE self)
{
    return rbgobj_gtype_new(rbgobj_get_param_spec(self)->owner_type);
}

static VALUE
rg_owner(VALUE self)
{
    return GTYPE2CLASS(rbgobj_get_param_spec(self)->owner_type);
}

static VALUE
rg_value_default(VALUE self)
{
    GValue tmp = G_VALUE_INIT;
    VALUE result;

    g_value_init(&tmp,
                 G_PARAM_SPEC_VALUE_TYPE(rbgobj_get_param_spec(self)));
    g_param_value_set_default(rbgobj_get_param_spec(self), &tmp);
    result = rbgobj_gvalue_to_rvalue(&tmp);
    g_value_unset(&tmp);

    return result;
}

#if 0
static VALUE
rg_value_defaults_p(VALUE self, VALUE val)
{
    GValue tmp = {0,};
    gboolean result;

    /* FIXME: use rb_ensure to ensure following g_value_unset() call*/
    g_value_init(&tmp,
                 G_PARAM_SPEC_VALUE_TYPE(rbgobj_get_param_spec(self)));
    rbgobj_rvalue_to_gvalue(val, &tmp);
    result = g_param_value_defaults(rbgobj_get_param_spec(self), &tmp);
    g_value_unset(&tmp);

    return CBOOL2RVAL(result);
}
#endif

struct validate_arg{
    GParamSpec* pspec;
    GValue* value;
    VALUE obj;
};

static VALUE
value_validate_body(VALUE rb_arg)
{
    struct validate_arg *arg = (struct validate_arg *)rb_arg;
    VALUE ret;
    gboolean b;

    rbgobj_rvalue_to_gvalue(arg->obj, arg->value);
    b = g_param_value_validate(arg->pspec, arg->value);
    ret = rbgobj_gvalue_to_rvalue(arg->value);
    return rb_ary_new3(2, CBOOL2RVAL(b), ret);
}

static VALUE
value_validate_ensure(VALUE rb_arg)
{
    struct validate_arg *arg = (struct validate_arg *)rb_arg;
    g_value_unset(arg->value);
    return Qnil;
}

static VALUE
rg_value_validate(VALUE self, VALUE obj)
{
    struct validate_arg arg;
    GValue value = G_VALUE_INIT;

    arg.pspec = rbgobj_get_param_spec(self);
    arg.value = &value;
    arg.obj = obj;

    g_value_init(&value, G_PARAM_SPEC_VALUE_TYPE(arg.pspec));

    return rb_ensure(value_validate_body, (VALUE)&arg,
                     value_validate_ensure, (VALUE)&arg);
}

static VALUE
rg_value_convert(int argc, VALUE* argv, VALUE self)
{
    GParamSpec* pspec = rbgobj_get_param_spec(self);
    VALUE src, strict_validation;
    VALUE src_type;
    VALUE result = Qnil;
    GValue src_value = G_VALUE_INIT;
    GValue dest_value = G_VALUE_INIT;
    gboolean b;

    rb_scan_args(argc, argv, "21", &src, &src_type, &strict_validation);

    /* FIXME: use rb_ensure to ensure following g_value_unset() call*/
    g_value_init(&src_value, rbgobj_gtype_from_ruby(src_type));
    g_value_init(&dest_value, G_PARAM_SPEC_VALUE_TYPE(pspec));

    rbgobj_rvalue_to_gvalue(src, &src_value);

    b = g_param_value_convert(rbgobj_get_param_spec(self),
                              &src_value, &dest_value,
                              RVAL2CBOOL(strict_validation));

    if (b)
        result = rbgobj_gvalue_to_rvalue(&dest_value);

    g_value_unset(&src_value);
    g_value_unset(&dest_value);

    if (b)
        return result;
    else
        rb_raise(rb_eTypeError, "can't convert");
}

static VALUE
rg_value_compare(VALUE self, VALUE a, VALUE b)
{
    GParamSpec* pspec = rbgobj_get_param_spec(self);
    GType type = G_PARAM_SPEC_VALUE_TYPE(pspec);
    GValue v1 = G_VALUE_INIT;
    GValue v2 = G_VALUE_INIT;
    gint result;

    g_value_init(&v1, type);
    g_value_init(&v2, type);

    /* FIXME: use rb_ensure to ensure following g_value_unset() call*/
    rbgobj_rvalue_to_gvalue(a, &v1);
    rbgobj_rvalue_to_gvalue(b, &v2);

    result = g_param_values_cmp(pspec, &v1, &v2);

    g_value_unset(&v1);
    g_value_unset(&v2);

    return INT2NUM(result);
}

static VALUE
rg_ref_count(VALUE self)
{
    return INT2NUM(G_PARAM_SPEC(rbgobj_get_param_spec(self))->ref_count);
}

#define param_is_flag(flag) \
    static VALUE \
    param_is_##flag(VALUE self) \
    { \
        GParamSpec* pspec = G_PARAM_SPEC(rbgobj_get_param_spec(self)); \
        return CBOOL2RVAL((pspec->flags & flag) == flag); \
    }

param_is_flag(G_PARAM_READABLE)
param_is_flag(G_PARAM_WRITABLE)
param_is_flag(G_PARAM_READWRITE)
param_is_flag(G_PARAM_CONSTRUCT)
param_is_flag(G_PARAM_CONSTRUCT_ONLY)
param_is_flag(G_PARAM_LAX_VALIDATION)
param_is_flag(G_PARAM_STATIC_NAME)
param_is_flag(G_PARAM_STATIC_NICK)
param_is_flag(G_PARAM_STATIC_BLURB)
param_is_flag(G_PARAM_EXPLICIT_NOTIFY)
param_is_flag(G_PARAM_DEPRECATED)

/**********************************************************************/

void
Init_gobject_gparam(void)
{
    qparamspec = g_quark_from_static_string("__ruby_gobject_param_spec__");
    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_PARAM, "Param", mGLib);

    /* GParamFlags */
    rb_define_const(RG_TARGET_NAMESPACE, "READABLE",
                    INT2FIX(G_PARAM_READABLE));
    rb_define_const(RG_TARGET_NAMESPACE, "WRITABLE",
                    INT2FIX(G_PARAM_WRITABLE));
    rb_define_const(RG_TARGET_NAMESPACE, "READWRITE",
                    INT2FIX(G_PARAM_READWRITE));
    rb_define_const(RG_TARGET_NAMESPACE, "CONSTRUCT",
                    INT2FIX(G_PARAM_CONSTRUCT));
    rb_define_const(RG_TARGET_NAMESPACE, "CONSTRUCT_ONLY",
                    INT2FIX(G_PARAM_CONSTRUCT_ONLY));
    rb_define_const(RG_TARGET_NAMESPACE, "LAX_VALIDATION",
                    INT2FIX(G_PARAM_LAX_VALIDATION));
    rb_define_const(RG_TARGET_NAMESPACE, "STATIC_NAME",
                    INT2FIX(G_PARAM_STATIC_NAME));
    rb_define_const(RG_TARGET_NAMESPACE, "STATIC_NICK",
                    INT2FIX(G_PARAM_STATIC_NICK));
    rb_define_const(RG_TARGET_NAMESPACE, "STATIC_BLURB",
                    INT2FIX(G_PARAM_STATIC_BLURB));
    rb_define_const(RG_TARGET_NAMESPACE, "EXPLICIT_NOTIFY",
                    INT2FIX(G_PARAM_EXPLICIT_NOTIFY));
    rb_define_const(RG_TARGET_NAMESPACE, "DEPRECATED",
                    INT2FIX(G_PARAM_DEPRECATED));
    rb_define_const(RG_TARGET_NAMESPACE, "MASK",
                    INT2FIX(G_PARAM_MASK));
    rb_define_const(RG_TARGET_NAMESPACE, "USER_SHIFT",
                    INT2FIX(G_PARAM_USER_SHIFT));

    rb_define_alloc_func(RG_TARGET_NAMESPACE, pspec_s_allocate);

    RG_DEF_METHOD(inspect, 0);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(nick, 0);
    RG_DEF_METHOD(blurb, 0);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(value_type, 0);
    RG_DEF_METHOD(owner_type, 0);
    RG_DEF_METHOD(owner, 0);

    RG_DEF_METHOD(value_default, 0);
    RG_DEF_ALIAS("default", "value_default");

    // FIXME: better name
#if 0
    RG_DEF_METHOD_P(value_defaults, 1);
#endif
    RG_DEF_METHOD(value_validate, 1);
    RG_DEF_METHOD(value_convert, -1);
    RG_DEF_METHOD(value_compare, 2);

    /* for debugging */
    RG_DEF_METHOD(ref_count, 0);

    rbg_define_method(RG_TARGET_NAMESPACE, "readable?",
                      param_is_G_PARAM_READABLE, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "writable?",
                      param_is_G_PARAM_WRITABLE, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "readwrite?",
                      param_is_G_PARAM_READWRITE, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "construct?",
                      param_is_G_PARAM_CONSTRUCT, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "construct_only?",
                      param_is_G_PARAM_CONSTRUCT_ONLY, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "lax_validation?",
                      param_is_G_PARAM_LAX_VALIDATION, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "static_name?",
                      param_is_G_PARAM_STATIC_NAME, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "static_nick?",
                      param_is_G_PARAM_STATIC_NICK, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "static_blurb?",
                      param_is_G_PARAM_STATIC_BLURB, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "explicit_notify?",
                      param_is_G_PARAM_EXPLICIT_NOTIFY, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "deprecated?",
                      param_is_G_PARAM_DEPRECATED, 0);
}
