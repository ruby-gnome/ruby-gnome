/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2004-2019  Ruby-GNOME Project Team
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

#define RG_TARGET_NAMESPACE rbgobj_cFlags

VALUE RG_TARGET_NAMESPACE;

static ID id_new;
static ID id_module_eval;
static ID id_or;
static ID id_to_i;

/**********************************************************************/

void
rg_flags_add_constants(VALUE mod, GType flags_type, const gchar *strip_prefix)
{
    GFlagsClass *gclass;
    guint i;
    size_t prefix_len = strlen(strip_prefix);

    gclass = G_FLAGS_CLASS(g_type_class_ref(flags_type));

    for (i = 0; i < gclass->n_values; i++) {
        const GFlagsValue* value = &gclass->values[i];

        if (strncmp(value->value_name, strip_prefix, prefix_len)) {
            g_warning("\"%s\" doesn't have prefix \"%s\"",
                      value->value_name, strip_prefix);
        } else {
            const char* name = value->value_name + prefix_len;
            rbgobj_define_const(mod, name,
                                rbgobj_make_flags(value->value, flags_type));

        }
    }

    g_type_class_unref(gclass);
}

/**********************************************************************/

typedef struct {
    GFlagsClass* gclass;
    guint value;
    GFlagsValue* info;
} flags_holder;

static void
flags_free(void *data)
{
    flags_holder *holder = data;
    g_type_class_unref(holder->gclass);
    xfree(holder);
}

static const rb_data_type_t rg_glib_flags_type = {
    "GLib::Flags",
    {
        NULL,
        flags_free,
    },
    NULL,
    NULL,
    RUBY_TYPED_FREE_IMMEDIATELY,
};

static flags_holder*
flags_get_holder(VALUE rb_flags)
{
    flags_holder *holder;
    TypedData_Get_Struct(rb_flags, flags_holder, &rg_glib_flags_type, holder);
    return holder;
}

static VALUE
make_flags(guint n, VALUE klass)
{
    return rb_funcall(klass, id_new, 1, UINT2NUM(n));
}

VALUE
rbgobj_make_flags(guint n, GType gtype)
{
    return make_flags(n, GTYPE2CLASS(gtype));
}

guint
rbgobj_get_flags(VALUE obj, GType gtype)
{
    VALUE klass;

    if (!g_type_is_a(gtype, G_TYPE_FLAGS))
        rb_raise(rb_eTypeError, "%s is not a %s",
                 g_type_name(gtype), g_type_name(G_TYPE_FLAGS));

    klass = GTYPE2CLASS(gtype);

    if (!RVAL2CBOOL(rb_obj_is_kind_of(obj, klass))) {
        obj = rb_funcall(klass, id_new, 1, obj);
    }

    return flags_get_holder(obj)->value;
}

/**********************************************************************/

void
rbgobj_init_flags_class(VALUE klass)
{
    GFlagsClass* gclass = g_type_class_ref(CLASS2GTYPE(klass));
    GString* source = g_string_new(NULL);
    guint i;

    for (i = 0; i < gclass->n_values; i++) {
        GFlagsValue* entry = &(gclass->values[i]);
        gchar* nick;
        gchar* p;
        gchar* replace_nick;

        replace_nick = rg_obj_constant_lookup(entry->value_nick);
        if (replace_nick){
            nick = g_strdup(replace_nick);
        } else {
            nick = g_strdup(entry->value_nick);
        }

        for (p = nick; *p; p++)
            if (*p == '-' || *p == ' ')
                *p = '_';
            else
                *p = tolower(*p);

        g_string_append_printf(
            source,
            "def %s%s?; self >= self.class.new(%d); end\n",
            g_ascii_isdigit(nick[0]) ? "_" : "",
            nick, entry->value);

        for (p = nick; *p; p++)
            *p = g_ascii_toupper(*p);

#if 0
        {
            ID id = rb_intern(nick);
            if (rb_is_const_id(id)) {
                rb_define_const(klass, nick, make_flags(entry->value, klass));
            }
        }
#else
        {
            rbgobj_define_const(klass, nick, make_flags(entry->value, klass));
        }
#endif

        g_free(nick);
    }

    rb_funcall(klass, id_module_eval, 3,
               rb_str_new2(source->str),
               rb_str_new2(__FILE__),
               INT2NUM(__LINE__));
    g_string_free(source, TRUE);

    g_type_class_unref(gclass);
}

static VALUE
rg_s_mask(VALUE klass)
{
    GFlagsClass* gclass = g_type_class_ref(CLASS2GTYPE(klass));
    VALUE result = UINT2NUM(gclass->mask);
    g_type_class_unref(gclass);
    return result;
}

static VALUE
rg_s_values(VALUE klass)
{
    GFlagsClass *gclass;
    VALUE result;
    guint i;

    gclass = g_type_class_ref(CLASS2GTYPE(klass));
    result = rb_ary_new();
    for (i = 0; i < gclass->n_values; i++) {
        GFlagsValue *p = &(gclass->values[i]);
        rb_ary_push(result, make_flags(p->value, klass));
    }
    g_type_class_unref(gclass);

    return result;
}

VALUE
rbgobj_flags_alloc_func(VALUE self)
{
    GType gtype = CLASS2GTYPE(self);

    if (G_TYPE_IS_ABSTRACT(gtype)) {
        rb_raise(rb_eTypeError, "abstract class");
    } else {
        flags_holder *holder;
        VALUE rb_flags;
        rb_flags = TypedData_Make_Struct(self,
                                         flags_holder,
                                         &rg_glib_flags_type,
                                         holder);
        holder->gclass = g_type_class_ref(gtype);
        holder->value  = 0;
        holder->info   = NULL;
        return rb_flags;
    }
}

static guint
resolve_flags_value(VALUE klass, GFlagsClass *gclass, VALUE flag_or_flags)
{
    guint value = 0;

    switch (TYPE(flag_or_flags)) {
    case RUBY_T_NIL:
        value = 0;
        break;
    case RUBY_T_FIXNUM:
    case RUBY_T_BIGNUM:
        value = NUM2UINT(flag_or_flags);
        break;
    case RUBY_T_STRING:
    case RUBY_T_SYMBOL:
    {
        const gchar *name;
        GFlagsValue *info;

        name = RVAL2CSTR_ACCEPT_SYMBOL(flag_or_flags);
        info = g_flags_get_value_by_name(gclass, name);
        if (!info) {
            gchar *nick;
            nick = rbg_name_to_nick(name);
            info = g_flags_get_value_by_nick(gclass, nick);
            g_free(nick);
        }
        if (!info) {
            rb_raise(rb_eArgError,
                     "unknown flag name: <%s>(%s)",
                     name,
                     g_type_name(G_TYPE_FROM_CLASS(gclass)));
        }
        value = info->value;
        break;
    }
    case RUBY_T_ARRAY:
    {
        long i, n;
        n = RARRAY_LEN(flag_or_flags);
        for (i = 0; i < n; i++) {
            value |= resolve_flags_value(klass,
                                         gclass,
                                         RARRAY_PTR(flag_or_flags)[i]);
        }
        break;
    }
    default:
        if (RVAL2CBOOL(rb_obj_is_kind_of(flag_or_flags, klass))) {
            value = NUM2UINT(rb_funcall(flag_or_flags, id_to_i, 0));
        } else {
            rb_raise(rb_eArgError,
                     "flag value must be one of "
                     "nil, Fixnum, String, Symbol, %s or Array of them: "
                     "<%s>(%s)",
                     RBG_INSPECT(klass),
                     RBG_INSPECT(flag_or_flags),
                     g_type_name(G_TYPE_FROM_CLASS(gclass)));
        }
        break;
    }

    return value;
}

static VALUE
rg_initialize(int argc, VALUE* argv, VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    VALUE flag_or_flags;

    rb_scan_args(argc, argv, "01", &flag_or_flags);

    p->value = resolve_flags_value(CLASS_OF(self), p->gclass, flag_or_flags);

    if (!p->info) {
        guint i;
        for (i = 0; i < p->gclass->n_values; i++){
            GFlagsValue* val = &(p->gclass->values[i]);
            if (val->value == p->value){
                p->info = val;
                break;
            }
        }
    }

    return Qnil;
}

static VALUE
rg_to_i(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return UINT2NUM(p->value);
}

static VALUE
rg_name(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_name) : Qnil;
}

static VALUE
rg_nick(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return p->info ? rb_str_new2(p->info->value_nick) : Qnil;
}

#define FLAGS_COMP_EQUAL        0
#define FLAGS_COMP_GREATER      1
#define FLAGS_COMP_LESS         -1
#define FLAGS_COMP_ELSE         -2
#define FLAGS_COMP_INCOMPARABLE -3

typedef struct {
    GType gtype;
    VALUE rb_value;
    guint value;
    gboolean compatible;
} compare_data;

static VALUE
flags_compare_get_flags_body(VALUE user_data)
{
    compare_data *data = (compare_data *)user_data;

    data->value = rbgobj_get_flags(data->rb_value, data->gtype);
    data->compatible = TRUE;

    return Qnil;
}

static VALUE
flags_compare_get_flags_rescue(VALUE user_data,
                               G_GNUC_UNUSED VALUE error)
{
    compare_data *data = (compare_data *)user_data;

    data->compatible = FALSE;

    return Qnil;
}

static gint
flags_compare(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    compare_data data;

    data.gtype = G_TYPE_FROM_CLASS(p->gclass);
    data.rb_value = rhs;
    data.value = 0;
    data.compatible = TRUE;

    rb_rescue(flags_compare_get_flags_body, (VALUE)&data,
              flags_compare_get_flags_rescue, (VALUE)&data);
    if (!data.compatible) {
        return FLAGS_COMP_INCOMPARABLE;
    }

    if (p->value == data.value)
        return FLAGS_COMP_EQUAL;
    else if ((p->value & data.value) == data.value)
        return FLAGS_COMP_GREATER;
    else if ((p->value & data.value) == p->value)
        return FLAGS_COMP_LESS;
    else
        return FLAGS_COMP_ELSE;
}

static VALUE
rg_operator_flags_compare(VALUE self, VALUE rhs)
{
    gint ret = flags_compare(self, rhs);
    switch (ret) {
    case FLAGS_COMP_EQUAL:
    case FLAGS_COMP_GREATER:
    case FLAGS_COMP_LESS:
        return INT2FIX(ret);
    default:
        return Qnil;
    }
}

static VALUE
rg_operator_flags_eqv(VALUE self, VALUE rhs)
{
    gint ret = flags_compare(self, rhs);
    if (ret == FLAGS_COMP_INCOMPARABLE)
        return Qnil;
    return CBOOL2RVAL(ret == FLAGS_COMP_EQUAL);
}

static VALUE
rg_operator_flags_gt_eq(VALUE self, VALUE rhs)
{
    gint ret = flags_compare(self, rhs);
    if (ret == FLAGS_COMP_INCOMPARABLE)
        return Qnil;
    return CBOOL2RVAL(ret == FLAGS_COMP_GREATER || ret == FLAGS_COMP_EQUAL);
}

static VALUE
rg_operator_flags_lt_eq(VALUE self, VALUE rhs)
{
    gint ret = flags_compare(self, rhs);
    if (ret == FLAGS_COMP_INCOMPARABLE)
        return Qnil;
    return CBOOL2RVAL(ret == FLAGS_COMP_LESS || ret == FLAGS_COMP_EQUAL);
}

static VALUE
rg_operator_flags_gt(VALUE self, VALUE rhs)
{
    gint ret = flags_compare(self, rhs);
    if (ret == FLAGS_COMP_INCOMPARABLE)
        return Qnil;
    return CBOOL2RVAL(ret == FLAGS_COMP_GREATER);
}

static VALUE
rg_operator_flags_lt(VALUE self, VALUE rhs)
{
    gint ret = flags_compare(self, rhs);
    if (ret == FLAGS_COMP_INCOMPARABLE)
        return Qnil;
    return CBOOL2RVAL(ret == FLAGS_COMP_LESS);
}

static VALUE
rg_operator_flags_not(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return rbgobj_make_flags((~ p->value) & p->gclass->mask,
                             G_TYPE_FROM_CLASS(p->gclass));
}

#define LIFT_BINARY_OP(funcname, op) \
    static VALUE \
    funcname(VALUE self, VALUE rhs) \
    { \
        flags_holder* p = flags_get_holder(self); \
        GType gtype = G_TYPE_FROM_CLASS(p->gclass); \
        return rbgobj_make_flags(p->value op rbgobj_get_flags(rhs, gtype), \
                                 gtype); \
    }

LIFT_BINARY_OP(flags_and, &)
LIFT_BINARY_OP(flags_or, |)
LIFT_BINARY_OP(flags_xor, ^)

static VALUE
rg_operator_flags_minus(VALUE self, VALUE rhs)
{
    flags_holder* p = flags_get_holder(self);
    GType gtype = G_TYPE_FROM_CLASS(p->gclass);
    return rbgobj_make_flags(p->value & ~rbgobj_get_flags(rhs, gtype),
                             gtype);
}

static VALUE
rg_empty_p(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return CBOOL2RVAL(p->value == 0);
}

static VALUE
rg_hash(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return UINT2NUM(p->value ^ G_TYPE_FROM_CLASS(p->gclass));
}

static VALUE
rg_coerce(VALUE self, VALUE other)
{
    flags_holder *holder;
    GType gtype;

    if (rb_obj_is_kind_of(other, rb_cInteger))
        rb_raise(rb_eTypeError, "can't coerce");

    holder = flags_get_holder(self);
    gtype = G_TYPE_FROM_CLASS(holder->gclass);
    other = rbgobj_make_flags(NUM2UINT(other), gtype);
    return rb_ary_new3(2, other, self);
}

static VALUE
rg_nonzero_p(VALUE self)
{
    flags_holder* p = flags_get_holder(self);
    return CBOOL2RVAL(p->value != 0);
}

/**********************************************************************/

void
Init_gobject_gflags(void)
{
    id_module_eval = rb_intern("module_eval");
    id_new = rb_intern("new");
    id_or = rb_intern("|");
    id_to_i = rb_intern("to_i");

    RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_FLAGS, "Flags", mGLib);

    rbg_define_singleton_method(RG_TARGET_NAMESPACE, "gtype", generic_s_gtype, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "gtype", generic_gtype, 0);

    RG_DEF_SMETHOD(mask, 0);
    RG_DEF_SMETHOD(values, 0);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rbgobj_flags_alloc_func);

    RG_DEF_METHOD(initialize, -1);

    RG_DEF_METHOD(to_i, 0);
    RG_DEF_ALIAS("to_int", "to_i");
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(nick, 0);

    /*
    rbg_define_method(RG_TARGET_NAMESPACE, "inspect", flags_inspect, 0);
    */

    RG_DEF_METHOD_OPERATOR("<=>", flags_compare, 1);
    RG_DEF_METHOD_OPERATOR("==", flags_eqv, 1);
    RG_DEF_METHOD_OPERATOR(">=", flags_gt_eq, 1);
    RG_DEF_METHOD_OPERATOR("<=", flags_lt_eq, 1);
    RG_DEF_METHOD_OPERATOR(">", flags_gt, 1);
    RG_DEF_METHOD_OPERATOR("<", flags_lt, 1);
    RG_DEF_METHOD_OPERATOR("~", flags_not, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "&", flags_and, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "|", flags_or, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "^", flags_xor, 1);
    RG_DEF_METHOD_OPERATOR("-", flags_minus, 1);

    RG_DEF_METHOD_P(empty, 0);

    RG_DEF_METHOD(hash, 0);
    RG_DEF_ALIAS("eql?", "==");

    /* for compatibility */
    RG_DEF_METHOD(coerce, 1);
    RG_DEF_ALIAS("zero?", "empty?");
    RG_DEF_METHOD_P(nonzero, 0);
}
