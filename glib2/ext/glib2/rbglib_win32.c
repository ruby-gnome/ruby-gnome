/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2021  Ruby-GNOME Project Team
 *  Copyright (C) 2006  Kouhei Sutou
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
#include "rbglib.h"

#ifdef G_OS_WIN32

#define RG_TARGET_NAMESPACE mWin32

static VALUE
rg_s_error_message(VALUE self, VALUE error)
{
    return CSTR2RVAL_FREE(g_win32_error_message(NUM2INT(error)));
}

static VALUE
rg_s_locale(VALUE self)
{
    return CSTR2RVAL_FREE(g_win32_getlocale());
}

static VALUE
rbglib_m_win32_locale_deprecated(VALUE self)
{
    rb_warn("GLib.win32_locale() is deprecated. Use GLib::Win32.locale instead");
    return rg_s_locale(self);
}

static VALUE
rg_s_version(VALUE self)
{
    return UINT2NUM(g_win32_get_windows_version());
}

static VALUE
rg_s_locale_filename_from_utf8(VALUE self, VALUE utf8_filename)
{
    return CSTR2RVAL_FREE(g_win32_locale_filename_from_utf8(RVAL2CSTR(utf8_filename)));
}

static VALUE
rbglib_m_win32_locale_filename_from_utf8_deprecated(VALUE self,
                                                    VALUE utf8_filename)
{
    rb_warn("GLib.win32_locale_filename_from_utf8() is deprecated. Use GLib::Win32.locale_filename_from_utf8 instead");
    return rg_s_locale_filename_from_utf8(self, utf8_filename);
}

static VALUE
rg_s_get_package_installation_directory_of_module(int argc,
                                                  VALUE *argv,
                                                  VALUE self)
{
    VALUE rb_module;
    gchar *directory;
    gpointer hmodule;

    rb_scan_args(argc, argv, "01", &rb_module);
    if (NIL_P(rb_module))
        hmodule = NULL;
    else
        hmodule = GINT_TO_POINTER(NUM2INT(rb_module));

    directory = g_win32_get_package_installation_directory_of_module(hmodule);
    return CSTR2RVAL_FREE(directory);
}
#endif

void
Init_glib_win32(void)
{
#ifdef G_OS_WIN32
    /* glib/gwin32.h */
    VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "Win32");

    RG_DEF_SMETHOD(error_message, 1);
    RG_DEF_SMETHOD(locale, 0);
    RG_DEF_SMETHOD(version, 0);
    /* Deprecated */
    rbg_define_singleton_method(mGLib, "win32_locale", rbglib_m_win32_locale_deprecated, 0);

    RG_DEF_SMETHOD(locale_filename_from_utf8, 1);
    /* Deprecated */
    rbg_define_singleton_method(mGLib, "win32_locale_filename_from_utf8",
                              rbglib_m_win32_locale_filename_from_utf8_deprecated, 1);

    RG_DEF_SMETHOD(get_package_installation_directory_of_module, -1);
#endif
}
