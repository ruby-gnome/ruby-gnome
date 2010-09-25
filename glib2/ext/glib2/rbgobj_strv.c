/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbgobj_gstrv.c -

  $Author: sakai $
  $Date: 2007/07/07 08:46:50 $

  Copyright (C) 2005  Masao Mutoh
**********************************************************************/

#include "rbgprivate.h"

#if GLIB_CHECK_VERSION(2,6,0)

static VALUE
strv_to_ruby(const GValue* from)
{
    VALUE ary;
    const gchar** boxed = (const gchar**)g_value_get_boxed(from);
    if (!boxed)
        return Qnil;
  
    ary = rb_ary_new();
    while (*boxed){
        rb_ary_push(ary, CSTR2RVAL(*boxed));
        boxed++;
    }
    return ary;
}

static void
strv_from_ruby(VALUE from, GValue* to)
{
    int i;
    gchar** gstrv;

    if (NIL_P(from)) {
        g_value_set_boxed(to, NULL);
        return;
    }

    Check_Type(from, T_ARRAY);
    gstrv = g_new(gchar*, RARRAY_LEN(from) + 1);

    for (i = 0; i < RARRAY_LEN(from); i++) {
        VALUE v = RARRAY_PTR(from)[i];
        gstrv[i] = g_strdup(StringValuePtr(v));
    }
    gstrv[RARRAY_LEN(from)] = NULL;

    g_value_set_boxed(to, gstrv);
}

void Init_gobject_gstrv()
{
    /* GStrv is treated as Array */
    rbgobj_register_g2r_func(G_TYPE_STRV, strv_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_STRV, strv_from_ruby);
}
#endif
