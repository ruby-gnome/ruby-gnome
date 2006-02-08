/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_i18n.c -

  $Author: ktou $
  $Date: 2006/02/08 14:54:51 $

  Copyright (C) 2006 Kouhei Sutou

**********************************************************************/
#include "global.h"
#include "rbglib.h"
#undef _
#include <glib/gi18n.h>

#if GLIB_CHECK_VERSION(2,6,0)
static VALUE
rbglib_m_language_names(self)
    VALUE self;
{
    const gchar * const *languages;
    const gchar *language;
    VALUE rb_languages = rb_ary_new();

    languages = g_get_language_names();
    for (language = *languages; *languages; language = *(++languages)) {
        rb_ary_push(rb_languages, CSTR2RVAL(language));
    }

    return rb_languages;
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
