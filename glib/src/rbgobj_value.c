/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbgobj_value.c -

  $Author: sakai $
  $Date: 2002/08/10 16:07:09 $

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
rbgobj_gvalue_to_rvalue(const GValue* value)
{
    if (!value)
        return Qnil;

    switch (G_TYPE_FUNDAMENTAL(G_VALUE_TYPE(value))) {
      case G_TYPE_NONE:
        return Qnil;
      case G_TYPE_CHAR:
        return CHR2FIX(g_value_get_char(value));
      case G_TYPE_UCHAR:
        return INT2FIX(g_value_get_uchar(value));
      case G_TYPE_BOOLEAN:
        return g_value_get_boolean(value) ? Qtrue : Qfalse;
      case G_TYPE_INT:
        return INT2NUM(g_value_get_int(value));
      case G_TYPE_UINT:
        return UINT2NUM(g_value_get_uint(value));
      case G_TYPE_LONG:
        return INT2NUM(g_value_get_long(value));
      case G_TYPE_ULONG:
        return UINT2NUM(g_value_get_ulong(value));
      case G_TYPE_INT64:
        return rbgobj_int64_to_num(g_value_get_int64(value));
      case G_TYPE_UINT64:
        return rbgobj_uint64_to_num(g_value_get_uint64(value));
      case G_TYPE_FLOAT:
        return rb_float_new(g_value_get_float(value));
      case G_TYPE_DOUBLE:
        return rb_float_new(g_value_get_double(value));
      case G_TYPE_STRING:
        return rb_str_new2(g_value_get_string(value));
      case G_TYPE_ENUM:
        return UINT2NUM(g_value_get_enum(value));
      case G_TYPE_FLAGS:
        return UINT2NUM(g_value_get_flags(value));        
      case G_TYPE_OBJECT:
        {
            GObject* gobj = g_value_get_object(value);
            return gobj ? GOBJ2RVAL(gobj) : Qnil;
        }
      case G_TYPE_PARAM:
        {
            GParamSpec* pspec = g_value_get_param(value);
            return pspec ? rbgobj_get_value_from_param_spec(pspec) : Qnil;
        }
      case G_TYPE_POINTER:
        {
            gpointer ptr = g_value_get_pointer(value);
            if (!ptr)
                return Qnil;
            else
                return Data_Wrap_Struct(GTYPE2CLASS(G_VALUE_TYPE(value)),
                                        NULL, NULL, ptr);
        }
#if 0
      case G_TYPE_INTERFACE:
      case G_TYPE_BOXED:
#endif
    }

    {
    GType gtype;
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
    }

    /* XXX */
    printf("rbgobj_gvalue_to_rvalue: unsupported gobject type: %s\n",
           g_type_name(G_VALUE_TYPE(value)));
    return Qnil;
}

void
rbgobj_rvalue_to_gvalue(VALUE val, GValue* result)
{
    switch (G_TYPE_FUNDAMENTAL(G_VALUE_TYPE(result))) {
      case G_TYPE_NONE:
        return;
      case G_TYPE_CHAR:
        g_value_set_char(result, NUM2INT(val));
        return;
      case G_TYPE_UCHAR:
        g_value_set_uchar(result, NUM2UINT(val));
        return;
      case G_TYPE_BOOLEAN:
        g_value_set_boolean(result, RTEST(val));
        return;
      case G_TYPE_INT:
        g_value_set_int(result, NUM2INT(val));
        return;
      case G_TYPE_UINT:
        g_value_set_uint(result, NUM2UINT(val));
        return;
      case G_TYPE_LONG:
        g_value_set_long(result, NUM2LONG(val));
        return;
      case G_TYPE_ULONG:
        g_value_set_ulong(result, NUM2ULONG(val));
        return;
      case G_TYPE_INT64:
        g_value_set_int64(result, rbgobj_num_to_int64(val));
        return;
      case G_TYPE_UINT64:
        g_value_set_uint64(result, rbgobj_num_to_uint64(val));
        return;
      case G_TYPE_ENUM:
        g_value_set_enum(result, NUM2UINT(val));
        return;
      case G_TYPE_FLAGS:
        g_value_set_flags(result, NUM2UINT(val));
        return;
      case G_TYPE_FLOAT:
        g_value_set_float(result, NUM2DBL(val));
        return;
      case G_TYPE_DOUBLE:
        g_value_set_double(result, NUM2DBL(val));
        return;
      case G_TYPE_STRING:
        StringValue(val);
        g_value_set_string(result, StringValuePtr(val));
        return;
      case G_TYPE_OBJECT:
        g_value_set_object(result, NIL_P(val) ? NULL : RVAL2GOBJ(val));
        return;
      case G_TYPE_PARAM:
        g_value_set_param(result, NIL_P(val) ? NULL : rbgobj_param_spec_get_struct(val));
        return;
      case G_TYPE_POINTER:
        if (NIL_P(val))
            g_value_set_pointer(result, NULL);
        else {
            gpointer ptr;
            Data_Get_Struct(val, void, ptr);
            g_value_set_pointer(result, ptr);
        }
#if 0
      case G_TYPE_INTERFACE:
      case G_TYPE_BOXED:
#endif
    }

    {
    VALUE mods = rb_mod_ancestors(CLASS_OF(val));
    VALUE* c;

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
    }
 
    /* XXX */
    printf("rbgobj_rvalue_to_gvalue: unsupported (ruby) class: %s\n",
           rb_class2name(CLASS_OF(val)));
}

/**********************************************************************/

void Init_gobject_gvalue()
{
    r2g_func_table = rb_hash_new();
    g2r_func_table = rb_hash_new();
    rb_global_variable(&r2g_func_table);
    rb_global_variable(&g2r_func_table);
}
