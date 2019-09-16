/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2019  Ruby-GNOME Project Team
 *  Copyright (C) 2004-2006  Ruby-GNOME Project Team
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

static ID id_find;
static ID id_new;
static ID id_to_i;
static ID id_to_s;
static ID id_values;

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
    if (RVAL2CBOOL(rb_obj_is_kind_of(nick, klass)))
        return nick;
    return rb_funcall(klass, id_find, 1, nick);
}

void
rg_enum_add_constants(VALUE mod, GType enum_type, const gchar *strip_prefix)
{
    VALUE klass;
    GEnumClass *gclass;
    guint i;
    size_t prefix_len = strlen(strip_prefix);

    klass = GTYPE2CLASS(enum_type);
    gclass = G_ENUM_CLASS(g_type_class_ref(enum_type));

    for (i = 0; i < gclass->n_values; i++) {
        const GEnumValue* value = &gclass->values[i];

        if (strncmp(value->value_name, strip_prefix, prefix_len)) {
            g_warning("\"%s\" doesn't have prefix \"%s\"",
                      value->value_name, strip_prefix);
        } else {
            const char* name = value->value_name + prefix_len;
            rbgobj_define_const(mod,
                                name,
                                rb_funcall(klass, id_find, 1, INT2NUM(value->value)));
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
enum_free(void *data)
{
    enum_holder *holder = data;
    g_type_class_unref(holder->gclass);
    xfree(holder);
}

static const rb_data_type_t rg_glib_enum_type = {
    "GLib::Enum",
    {
        NULL,
        enum_free,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

static enum_holder *
enum_get_holder(VALUE rb_enum)
{
    enum_holder *holder;
    TypedData_Get_Struct(rb_enum, enum_holder, &rg_glib_enum_type, holder);
    return holder;
}

VALUE
rbgobj_make_enum(gint n, GType gtype)
{
    VALUE klass = GTYPE2CLASS(gtype);
    VALUE enum_value;

    enum_value = rb_funcall(klass, id_find, 1, INT2NUM(n));
    if (NIL_P(enum_value)) {
        enum_value = rb_funcall(klass,
                                id_new,
                                1,
                                INT2NUM(n));
    }
    return enum_value;
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
    VALUE values;
    guint i;

    values = rb_hash_new();
    rb_iv_set(klass, "values", values);
    for (i = 0; i < gclass->n_values; i++) {
        GEnumValue* entry = &(gclass->values[i]);
        VALUE rb_raw_enum_value;
        VALUE value;
        gchar *const_nick_name;

        rb_raw_enum_value = INT2NUM(entry->value);
        value = rb_funcall(klass, id_new, 1, rb_raw_enum_value);
        rb_hash_aset(values, rb_raw_enum_value, value);
        const_nick_name = nick_to_const_name(entry->value_nick);
        if (const_nick_name) {
            rbgobj_define_const(klass, const_nick_name, value);
            g_free(const_nick_name);
        }
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

static VALUE
rg_s_values(VALUE self)
{
    VALUE values = rb_iv_get(self, "values");
    return rb_funcall(values, id_values, 0);
}

static VALUE
rg_s_find(VALUE self, VALUE key)
{
    if (RB_TYPE_P(key, RUBY_T_FIXNUM)) {
        VALUE values = rb_iv_get(self, "values");
        return rb_hash_aref(values, key);
    } else if (RB_TYPE_P(key, RUBY_T_STRING) || RB_TYPE_P(key, RUBY_T_SYMBOL)) {
        VALUE nick = rb_funcall(key, id_to_s, 0);
        gchar *const_nick = nick_to_const_name(RVAL2CSTR(nick));
        ID const_nick_id = rb_intern(const_nick);
        g_free(const_nick);
        if (rb_const_defined(self, const_nick_id)) {
            return rb_const_get(self, const_nick_id);
        } else {
            return Qnil;
        }
    } else {
        return Qnil;
    }
}

VALUE
rbgobj_enum_alloc_func(VALUE self)
{
    GType gtype = CLASS2GTYPE(self);

    if (G_TYPE_IS_ABSTRACT(gtype)) {
        rb_raise(rb_eTypeError, "abstract class");
    } else {
        enum_holder *holder;
        VALUE rb_enum;

        rb_enum = TypedData_Make_Struct(self,
                                        enum_holder,
                                        &rg_glib_enum_type,
                                        holder);
        holder->gclass = g_type_class_ref(gtype);
        holder->info   = NULL;
        return rb_enum;
    }
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    enum_holder* p = enum_get_holder(self);
    VALUE rb_value;
    VALUE klass;

    rb_scan_args(argc, argv, "01", &rb_value);

    klass = CLASS_OF(self);

    switch (TYPE(rb_value)) {
    case RUBY_T_NIL:
        p->value = 0;
        break;
    case RUBY_T_FIXNUM:
        p->value = NUM2UINT(rb_value);
        break;
    case RUBY_T_STRING:
    case RUBY_T_SYMBOL:
    {
        const gchar *name;

        name = RVAL2CSTR_ACCEPT_SYMBOL(rb_value);
        p->info = g_enum_get_value_by_name(p->gclass, name);
        if (!p->info) {
            gchar *nick;
            nick = rbg_name_to_nick(name);
            p->info = g_enum_get_value_by_nick(p->gclass, nick);
            g_free(nick);
        }
        if (!p->info) {
            rb_raise(rb_eArgError, "unknown enum name: <%s>(%s)",
                     name,
                     g_type_name(G_TYPE_FROM_CLASS(p->gclass)));
        }
        p->value = p->info->value;
        break;
    }
    default:
        if (RVAL2CBOOL(rb_obj_is_kind_of(rb_value, klass))) {
            p->value = NUM2INT(rb_funcall(rb_value, id_to_i, 0));
        } else {
            rb_raise(rb_eArgError,
                     "enum value must be one of "
                     "nil, Fixnum, String, Symbol o %s: "
                     "<%s>(%s)",
                     RBG_INSPECT(klass),
                     RBG_INSPECT(rb_value),
                     g_type_name(G_TYPE_FROM_CLASS(p->gclass)));
        }
        break;
    }

    if (!p->info) {
        p->info = g_enum_get_value(p->gclass, p->value);
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
    id_find = rb_intern("find");
    id_new = rb_intern("new");
    id_to_i = rb_intern("to_i");
    id_to_s = rb_intern("to_s");
    id_values = rb_intern("values");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_ENUM, "Enum", mGLib);

    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "gtype", generic_s_gtype, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "gtype", generic_gtype, 0);

    RG_DEF_SMETHOD(range, 0);
    RG_DEF_SMETHOD(values, 0);
    RG_DEF_SMETHOD(find, 1);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rbgobj_enum_alloc_func);

    RG_DEF_METHOD(initialize, -1);
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
