/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_i18n.c -

  $Author: ktou $
  $Date: 2006/02/08 14:54:51 $

  Copyright (C) 2006 Kouhei Sutou

**********************************************************************/

#include "rbgprivate.h"
#include "rbglib.h"
#undef _
#include <glib/gi18n.h>

#if GLIB_CHECK_VERSION(2,6,0)
static VALUE
rbglib_m_language_names(VALUE self)
{
    return STRV2RVAL((const gchar **)g_get_language_names());
}
#endif

void
Init_glib_i18n()
{
    /* glib/gi18n.h */
#if GLIB_CHECK_VERSION(2,6,0)
    rb_define_module_function(mGLib, "language_names",
                              rbglib_m_language_names, 0);
#endif
}
