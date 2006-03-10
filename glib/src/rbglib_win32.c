/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_win32.c -

  $Author: mutoh $
  $Date: 2006/03/10 18:58:51 $

  Copyright (C) 2006 Kouhei Sutou

**********************************************************************/
#include "global.h"
#include "rbglib.h"
#include <glib/gwin32.h>

#ifdef G_OS_WIN32
static VALUE
rbglib_m_win32_locale(self)
    VALUE self;
{
    gchar *locale;
    VALUE rb_locale;

    locale = g_win32_getlocale();
    rb_locale = CSTR2RVAL(locale);
    g_free(locale);

    return rb_locale;
}

#  if GLIB_CHECK_VERSION(2,8,0)
static VALUE
rbglib_m_win32_locale_filename_from_utf8(self, utf8_filename)
    VALUE self, utf8_filename;
{
    gchar *filename;
    VALUE rb_filename;

    filename = g_win32_locale_filename_from_utf8(RVAL2CSTR(utf8_filename));
    rb_filename = CSTR2RVAL(filename);
    g_free(filename);

    return rb_filename;
}
#  endif
#endif

void
Init_glib_win32()
{
#ifdef G_OS_WIN32
    /* glib/gwin32.h */
    rb_define_module_function(mGLib, "win32_locale", rbglib_m_win32_locale, 0);
#  if GLIB_CHECK_VERSION(2,8,0)
    rb_define_module_function(mGLib, "win32_locale_filename_from_utf8",
                              rbglib_m_win32_locale_filename_from_utf8, 1);
#  endif
#endif
}
