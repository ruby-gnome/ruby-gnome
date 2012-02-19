/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004-2006  Ruby-GNOME2 Project Team
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
#include <ctype.h>

#define RG_TARGET_NAMESPACE rbgobj_cEnum

VALUE RG_TARGET_NAMESPACE;

static ID id_new;
static ID id_to_s;

/**********************************************************************/

static gchar *
nick_to_const_name(const gchar *nick)
{
    gchar *const_name;
    gchar *p;

    if (!nick)
        return NULL;

    const_name = g_strdup(nick);
    for (p = const_name; *p; p++) {
        if (*p == '-' || *p == ' ')
            *p = '_';
        else
            *p = g_ascii_toupper(*p);
    }
    return const_name;
}

VALUE
rg_enum_resolve_value(VALUE klass, VALUE nick)
{
    VALUE value = Qnil;
    gchar *const_nick;
    ID const_nick_id;

    if (RVAL2CBOOL(rb_obj_is_kind_of(nick, klass)))
        return nick;

    nick = rb_funcall(nick, id_to_s, 0);
    const_nick = nick_to_const_name(RVAL2CSTR(nick));
    const_nick_id = rb_intern(const_nick);
    if (rb_const_defined(klass, const_nick_id)) {
        value = rb_const_get(klass, const_nick_id);
    }
    g_free(const_nick);

    return value;
}

void
rg_enum_add_constants(VALUE mod, GType enum_type, const gchar *strip_prefix)
{
    GEnumClass *gclass;
    guint i;
    int prefix_len = strlen(strip_prefix);

    gclass = G_ENUM_CLASS(g_type_class_ref(enum_type));

    for (i = 0; i < gclass->n_values; i++) {
        const GEnumValue* value = &gclass->values[i];

        if (strncmp(value->value_name, strip_prefix, prefix_len)) {
            g_warning("\"%s\" doesn't have prefix \"%s\"",
                      value->value_name, strip_prefix);
        } else {
            const char* name = value->value_name + prefix_len;
            rbgobj_define_const(mod, name,
                                rbgobj_make_enum(value->value, enum_type));
        }
    }

    g_type_class_unref(gclass);
}

/**********************************************************************/

typedef struct {
    GEnumClass* gclass;
    gint value;
    GEnumValue* info;
} enum_holder;

static void
enum_free(enum_holder* p)
{
    g_type_class_unref(p->gclass);
    free(p);
}

static enum_holder*
enum_get_holder(VALUE obj)
{
    enum_holder* p;
    Data_Get_Struct(obj, enum_holder, p);
    return p;
}

static VALUE
make_enum(gint n, VALUE klass)
{
    return rb_funcall(klass, id_new, 1, INT2NUM(n));
}

VALUE
rbgobj_make_enum(gint n, GType gtype)
{
    return make_enum(n, GTYPE2CLASS(gtype));
}

gint
rbgobj_get_enum(VALUE obj, GType gtype)
{
    VALUE klass;

    if (!g_type_is_a(gtype, G_TYPE_ENUM))
        rb_raise(rb_eTypeError, "%s is not a %s: %s",
                 g_type_name(gtype), g_type_name(G_TYPE_ENUM),
                 RBG_INSPECT(obj));

    /* for compatibility */
    if (rb_obj_is_kind_of(obj, rb_cInteger))
        obj = rbgobj_make_enum(NUM2INT(obj), gtype);

    klass = GTYPE2CLASS(gtype);

    if (!rb_obj_is_kind_of(obj, klass)) {
        VALUE enum_value;

        enum_value = rg_enum_resolve_value(klass, obj);
        if (!NIL_P(enum_value))
            obj = enum_value;
    }

    if (rb_obj_is_kind_of(obj, klass))
        return enum_get_holder(obj)->value;
    else
        rb_raise(rb_eTypeError, "not a %s: %s",
                 rb_class2name(klass), RBG_INSPECT(obj));
}

/**********************************************************************/

void
rbgobj_init_enum_class(VALUE klass)
{
    GEnumClass* gclass = g_type_class_ref(CLASS2GTYPE(klass));
    guint i;

    for (i = 0; i < gclass->n_values; i++) {
        GEnumValue* entry = &(gclass->values[i]);
        gchar *const_nick_name;

        const_nick_name = nick_to_const_name(entry->value_nick);

#if 0
        {
            ID id = rb_intern(const_nick_name);
            if (rb_is_const_id(id)) {
                VALUE value;

                value = make_enum(entry->value, klass);
                rb_define_const(klass, const_nick_name, value);
            }
        }
#else
        {
            if (const_nick_name) {
                VALUE value;

                value = make_enum(entry->value, klass);
                rbgobj_define_const(klass, const_nick_name, value);
            }
        }
#endif

        g_free(const_nick_name);
    }

    g_type_class_unref(gclass);
}

static VALUE
rg_s_range(VALUE self)
{
    GEnumClass* gclass = g_type_class_ref(CLASS2GTYPE(self));
    VALUE result = rb_range_new(INT2NUM(gclass->minimum),
                                INT2NUM(gclass->maximum),
                                FALSE);
    g_type_class_unref(gclass);
    return result;
}

struct enum_s_values_body_args {
    GEnumClass *gclass;
    VALUE self;
};

static VALUE
enum_s_values_body(VALUE value)
{
    struct enum_s_values_body_args *args = (struct enum_s_values_body_args *)value;
    VALUE result = rb_ary_new();
    guint i;

    for (i = 0; i < args->gclass->n_values; i++)
        rb_ary_push(result, make_enum(args->gclass->values[i].value, args->self));

    return result;
}

static VALUE
enum_s_values_ensure(VALUE gclass)
{
    g_type_class_unref((GEnumClass *)gclass);

    return Qnil;
}

static VALUE
rg_s_values(VALUE self)
{
    struct enum_s_values_body_args args = {
        g_type_class_ref(CLASS2GTYPE(self)),
        self
    };

    return rb_ensure(enum_s_values_body, (VALUE)&args,
                     enum_s_values_ensure, (VALUE)args.gclass);
}

static VALUE
enum_s_allocate(VALUE self)
{
    GType gtype = CLASS2GTYPE(self);

    if (G_TYPE_IS_ABSTRACT(gtype)) {
        rb_raise(rb_eTypeError, "abstract class");
    } else {
        enum_holder* p;
        VALUE result = Data_Make_Struct(self, enum_holder, NULL, enum_free, p);
        p->gclass = g_type_class_ref(gtype);
        p->info   = NULL;
        return result;
    }
}

static VALUE
rg_initialize(VALUE self, VALUE arg)
{
    enum_holder* p = enum_get_holder(self);

    if (rb_respond_to(arg, rb_intern("to_str"))) {
        const char* str = StringValuePtr(arg);
        p->info = g_enum_get_value_by_name(p->gclass, str);
        if (! p->info)
            p->info = g_enum_get_value_by_nick(p->gclass, str);
        if (! p->info)
            rb_raise(rb_eArgError, "invalid argument");
    } else {
        p->value = NUM2INT(arg);
        p->info  = g_enum_get_value(p->gclass, p->value);
    }

    return Qnil;
}

static VALUE
rg_to_i(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return INT2NUM(p->value);
}

static VALUE
rg_name(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_name) : Qnil;
}

static VALUE
rg_nick(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_nick) : Qnil;
}

static VALUE
rg_inspect(VALUE self)
{
    const char* cname = rb_class2name(CLASS_OF(self));
    enum_holder* p = enum_get_holder(self);
    gchar* str;
    VALUE result;

    if (p->info)
        str = g_strdup_printf("#<%s %s>",
                              cname, p->info->value_nick);
    else
        str = g_strdup_printf("#<%s %d>",
                              cname, p->value);
    result = rb_str_new2(str);
    g_free(str);

    return result;
}

static VALUE
rg_operator_enum_eqv(VALUE self, VALUE rhs)
{
    enum_holder* p = enum_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    VALUE klass = GTYPE2CLASS(gtype);

    if (!rb_obj_is_kind_of(rhs, rb_cInteger)) {
        rhs = rg_enum_resolve_value(klass, rhs);
        if (CLASS_OF(rhs) != CLASS_OF(self))
            return Qnil;
    }
    return CBOOL2RVAL(rbgobj_get_enum(self, gtype) == rbgobj_get_enum(rhs, gtype));
}

static VALUE
rg_hash(VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    return UINT2NUM(p->value ^ G_TYPE_FROM_CLASS(p->gclass));
}

static VALUE
rg_coerce(VALUE self, VALUE other)
{
    enum_holder *holder;
    GType gtype;

    if (!rb_obj_is_kind_of(other, rb_cInteger))
        rb_raise(rb_eTypeError, "can't coerce");

    holder = enum_get_holder(self);
    gtype = G_TYPE_FROM_CLASS(holder->gclass);
    other = rbgobj_make_enum(NUM2INT(other), gtype);
    return rb_ary_new3(2, other, self);
}

/**********************************************************************/

void
Init_gobject_genums(void)
{
    id_new = rb_intern("new");
    id_to_s = rb_intern("to_s");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_ENUM, "Enum", mGLib);

    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "gtype", generic_s_gtype, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "gtype", generic_gtype, 0);

    RG_DEF_SMETHOD(range, 0);
    RG_DEF_SMETHOD(values, 0);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, enum_s_allocate);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(to_i, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(nick, 0);

    RG_DEF_METHOD(inspect, 0);
    RG_DEF_METHOD_OPERATOR("==", enum_eqv, 1);
    RG_DEF_METHOD(hash, 0);
    RG_DEF_ALIAS("eql?", "==");

    /* for compatibility */
    RG_DEF_METHOD(coerce, 1);
    RG_DEF_ALIAS("to_int", "to_i");
}
