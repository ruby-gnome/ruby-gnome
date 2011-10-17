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
strv_to_ruby(const GValue *from)
{
    return STRV2RVAL((const gchar **)g_value_get_boxed(from));
}

static void
strv_from_ruby(VALUE from, GValue *to)
{
    const gchar **strings = RVAL2STRV(from);
    g_value_set_boxed(to, strings);
    g_free(strings);
}

void
Init_gobject_gstrv(void)
{
    /* GStrv is treated as Array */
    rbgobj_register_g2r_func(G_TYPE_STRV, strv_to_ruby);
    rbgobj_register_r2g_func(G_TYPE_STRV, strv_from_ruby);
}

#endif
