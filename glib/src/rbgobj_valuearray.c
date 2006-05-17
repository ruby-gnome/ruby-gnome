/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_valuearray.c -

  $Author: mutoh $
  $Date: 2006/05/17 14:00:24 $

  Copyright (C) 2006  Sjoerd Simons
**********************************************************************/

#include "global.h"

static VALUE
value_array_to_ruby(const GValue* from)
{
    VALUE ary;
    int i;
    GValueArray *val_array = (GValueArray*)g_value_get_boxed(from);
    if (!val_array)
        return Qnil;
  
    ary = rb_ary_new();
    for (i = 0 ;  i < val_array->n_values ; i++) {
        rb_ary_push(ary, GVAL2RVAL(g_value_array_get_nth(val_array, i)));
    }
    return ary;
}

static void
value_array_from_ruby(VALUE from, GValue* to)
{
    int i;
    GValueArray * array;

    if (NIL_P(from)) {
        g_value_set_boxed(to, NULL);
        return;
    }

    Check_Type(from, T_ARRAY);

    array = g_value_array_new(0);

    for (i = 0; i < RARRAY(from)->len; i++) {
      GValue v = { 0, };
      g_value_init(&v, RVAL2GTYPE(RARRAY(from)->ptr[i]));
      rbgobj_rvalue_to_gvalue(RARRAY(from)->ptr[i], &v);
      g_value_array_append(array, &v);
    }
    g_value_set_boxed(to, array);
}

void Init_gobject_value_array()
{
    /* ValueArray is treated as Array */
    rbgobj_register_g2r_func(G_TYPE_VALUE_ARRAY, value_array_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_VALUE_ARRAY, value_array_from_ruby);
}
