/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_value.c -

  $Author: sakai $
  $Date: 2002/06/17 18:14:24 $

  Copyright (C) 2002  Masahiro Sakai

**********************************************************************/

#include "global.h"

/**********************************************************************/

static VALUE r2g_func_table;
static VALUE g2r_func_table;

void
rbgobj_register_r2g_func(VALUE klass, RValueToGValueFunc func)
{
    VALUE obj = Data_Wrap_Struct(rb_cData, NULL, NULL, func);
    rb_hash_aset(r2g_func_table, klass, obj);
}

void
rbgobj_register_g2r_func(GType gtype, GValueToRValueFunc func)
{
    VALUE obj = Data_Wrap_Struct(rb_cData, NULL, NULL, func);
    rb_hash_aset(g2r_func_table, INT2FIX(gtype), obj);
}

/**********************************************************************/

VALUE
rbgobj_gvalue_to_rvalue(GValue* value)
{
    GType gtype;

    if (!value)
        return Qnil;

    for (gtype = G_VALUE_TYPE(value);
         gtype != G_TYPE_INVALID;
         gtype = g_type_parent(gtype))
    {
        GValueToRValueFunc func;
        VALUE obj = rb_hash_aref(g2r_func_table, INT2NUM(gtype));
        if (NIL_P(obj))
            continue;
        Data_Get_Struct(obj, GValueToRValueFunc, func);
        return func(value);
    }

    if (0)
        ;
    else if (G_VALUE_HOLDS_CHAR(value))
        return CHR2FIX(g_value_get_char(value));
    else if (G_VALUE_HOLDS_UCHAR(value))
        return INT2FIX(g_value_get_uchar(value));
    else if (G_VALUE_HOLDS_BOOLEAN(value))
        return g_value_get_boolean(value) ? Qtrue : Qfalse;
    else if (G_VALUE_HOLDS_INT(value))
        return INT2NUM(g_value_get_int(value));
    else if (G_VALUE_HOLDS_UINT(value))
        return UINT2NUM(g_value_get_uint(value));
    else if (G_VALUE_HOLDS_LONG(value))
        return INT2NUM(g_value_get_long(value));
    else if (G_VALUE_HOLDS_ULONG(value))
        return UINT2NUM(g_value_get_ulong(value));
    else if (G_VALUE_HOLDS_INT64(value))
        return INT2NUM(g_value_get_long(value)); /* FIXME */
    else if (G_VALUE_HOLDS_UINT64(value)) 
        return UINT2NUM(g_value_get_ulong(value)); /* FIXME */
    else if (G_VALUE_HOLDS_FLOAT(value))
        return rb_float_new(g_value_get_float(value));
    else if (G_VALUE_HOLDS_DOUBLE(value))
        return rb_float_new(g_value_get_double(value));
    else if (G_VALUE_HOLDS_STRING(value))
        return rb_str_new2(g_value_get_string(value));

    /* XXX */
    printf("rbgobj_gvalue_to_rvalue: unsupported (gobject) type: %s\n",
           g_type_name(G_VALUE_TYPE(value)));
    return Qnil;
}

void
rbgobj_rvalue_to_gvalue(VALUE val, GValue* result)
{
    VALUE mods;
    VALUE* c;

    mods = rb_mod_ancestors(CLASS_OF(val));
    Check_Type(mods, T_ARRAY);

    for (c = RARRAY(mods)->ptr;
         c != RARRAY(mods)->ptr + RARRAY(mods)->len;
         c++)
    {
        RValueToGValueFunc func;
        VALUE obj = rb_hash_aref(r2g_func_table, *c);
        if (NIL_P(obj))
            continue;
        Data_Get_Struct(obj, RValueToGValueFunc, func);
        func(val, result);
        return;
    }
 
    /* XXX */
    printf("rbgobj_rvalue_to_gvalue: unsupported (ruby) class: %s\n",
           rb_class2name(CLASS_OF(val)));
}

/**********************************************************************/
/* GLib -> Ruby */

/**********************************************************************/
/* Ruby -> GLib */

static void
nil_to_gvalue(VALUE from, GValue* to)
{
    /* ??? */
}

static void
bool_to_gvalue(VALUE from, GValue* to)
{
    g_value_set_boolean(to, RTEST(from));
}

static void
str_to_gvalue(VALUE from, GValue* to)
{
    StringValue(from);
    g_value_set_string(to, StringValuePtr(from));
}

static void
int_to_gvalue(VALUE from, GValue* to)
{
    g_value_set_int(to, NUM2INT(from));
}

static void
float_to_gvalue(VALUE from, GValue* to)
{
    g_value_set_double(to, NUM2DBL(from));
}

/**********************************************************************/

void Init_gobject_gvalue()
{
    r2g_func_table = rb_hash_new();
    g2r_func_table = rb_hash_new();
    rb_global_variable(&r2g_func_table);
    rb_global_variable(&g2r_func_table);

    rbgobj_register_r2g_func(rb_cNilClass, &nil_to_gvalue);
    rbgobj_register_r2g_func(rb_cTrueClass, &bool_to_gvalue);
    rbgobj_register_r2g_func(rb_cFalseClass, &bool_to_gvalue);
    rbgobj_register_r2g_func(rb_cString, &str_to_gvalue);
    rbgobj_register_r2g_func(rb_cFloat, &float_to_gvalue);
}
