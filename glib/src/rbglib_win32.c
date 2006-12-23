/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/**********************************************************************

  rbglib_win32.c -

  $Author: mutoh $
  $Date: 2006/12/23 17:43:03 $

  Copyright (C) 2006 Kouhei Sutou

**********************************************************************/
#include "global.h"
#include "rbglib.h"
#include <glib/gwin32.h>

#ifdef G_OS_WIN32
static VALUE
rbglib_m_win32_error_message(self, error)
    VALUE self, error;
{
    return CSTR2RVAL2(g_win32_error_message(NUM2INT(error)));
}

static VALUE
rbglib_m_win32_locale(self)
    VALUE self;
{
    return CSTR2RVAL2(g_win32_get_locale());
}

static VALUE
rbglib_m_win32_locale_deprecated(self)
  VALUE self;
{
	rb_warn("GLib.win32_locale() is deprecated. Use GLib::Win32.locale instead");
	return rbglib_m_win32_locale(self);
}

static VALUE
rbglib_m_win32_get_package_installation_directory(self, package, dll_name)
  VALUE self, package, dll_name;
{
	return CSTR2RVAL2(g_win32_get_package_installation_directory(RVAL2CSTR(package), 
	                                                            RVAL2CSTR(dll_name)));
}

static VALUE
rbglib_m_get_package_installation_subdirectory(self, package, dll_name, subdir)
  VALUE self, package, dll_name, subdir;
{
	return CSTR2RVAL2(g_win32_get_package_installation_subdirectory(RVAL2CSTR(package), 
	                                                                RVAL2CSTR(dll_name)
	                                                                RVAL2CSTR(subdir)));
}

#if GLIB_CHECK_VERSION(2,6,0)
static VALUE
rbglib_m_get_windows_version(self)
  VALUE self;
{
	return UINT2NUM(g_win32_get_windows_version());
}
#endif

#if GLIB_CHECK_VERSION(2,8,0)
static VALUE
rbglib_m_win32_locale_filename_from_utf8(self, utf8_filename)
    VALUE self, utf8_filename;
{
    return CSTR2RVAL2(g_win32_locale_filename_from_utf8(RVAL2CSTR(utf8_filename)));
}

static VALUE
rbglib_m_win32_locale_filename_from_utf8_deprecated(self)
  VALUE self;
{
	rb_warn("GLib.win32_locale_filename_from_utf8() is deprecated. Use GLib::Win32.locale_filename_from_utf8 instead");
	return rbglib_m_win32_locale_filename_from_utf8(self);
}

#endif
#endif

void
Init_glib_win32()
{
#ifdef G_OS_WIN32
    /* glib/gwin32.h */
    VALUE mWin32 = rb_define_module_under(mGLib, "Win32");

    rb_define_module_function(mWin32, "error_message", rbglib_m_win32_error_message, 1);
    rb_define_module_function(mWin32, "locale", rbglib_m_win32_locale, 0);
    rb_define_module_function(mWin32, "get_package_installation_directory", rbglib_m_win32_get_package_installation_directory, 2);
    rb_define_module_function(mWin32, "get_package_installation_subdirectory", rbglib_m_get_package_installation_subdirectory, 3);
    rb_define_module_function(mWin32, "version", rbglib_m_get_windows_version, 0);
    /* Deprecated */
    rb_define_module_function(mGLib, "win32_locale", rbglib_m_win32_locale_deprecated, 0);

#  if GLIB_CHECK_VERSION(2,8,0)
    rb_define_module_function(mWin32, "locale_filename_from_utf8",
                              rbglib_m_win32_locale_filename_from_utf8, 1);
    /* Deprecated */
    rb_define_module_function(mGLib, "win32_locale_filename_from_utf8",
                              rbglib_m_win32_locale_filename_from_utf8_deprecated, 1);
#  endif
#endif
}
