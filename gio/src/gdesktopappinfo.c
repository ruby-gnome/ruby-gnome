/* -*- c-file-style: "ruby" -*- */
/*
 * Ruby/GIO: a Ruby binding of gio-2.0.x.
 * Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "gio2.h"

#ifdef HAVE_GIO_UNIX
#include <gio/gdesktopappinfo.h>

#define _SELF(value) G_DESKTOP_APP_INFO(RVAL2GOBJ(value))

#define RVAL2GKEYFILE(value) \
        ((GKeyFile *)(RVAL2BOXED((value), G_TYPE_KEY_FILE)))

static VALUE
desktopappinfo_new_from_filename(G_GNUC_UNUSED VALUE self, VALUE filename)
{
        return GOBJ2RVAL(g_desktop_app_info_new_from_filename(RVAL2CSTR(filename)));
}

static VALUE
desktopappinfo_new_from_keyfile(G_GNUC_UNUSED VALUE self, VALUE keyfile)
{
        return GOBJ2RVAL(g_desktop_app_info_new_from_keyfile(RVAL2GKEYFILE(keyfile)));
}

static VALUE
desktopappinfo_initialize(VALUE self, VALUE desktop_id)
{
        G_INITIALIZE(self, g_desktop_app_info_new(RVAL2CSTR(desktop_id)));

        return Qnil;
}

static VALUE
desktopappinfo_get_filename(VALUE self)
{
        return CSTR2RVAL(g_desktop_app_info_get_filename(_SELF(self)));
}

static VALUE
desktopappinfo_get_is_hidden(VALUE self)
{
        return CBOOL2RVAL(g_desktop_app_info_get_is_hidden(_SELF(self)));
}

static VALUE
desktopappinfo_set_desktop_env(G_GNUC_UNUSED VALUE self, VALUE desktop_env)
{
        g_desktop_app_info_set_desktop_env(RVAL2CSTR(desktop_env));

        return self;
}

/* TODO: g_desktop_app_info_lookup_get_default_for_uri_scheme ()? */
#endif

void
Init_gdesktopappinfo(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE desktopappinfo = G_DEF_CLASS(G_TYPE_DESKTOP_APP_INFO, "DesktopAppInfo", glib);

        rb_define_const(desktopappinfo, "LOOKUP_EXTENSION_POINT_NAME", CSTR2RVAL(G_DESKTOP_APP_INFO_LOOKUP_EXTENSION_POINT_NAME));

        rb_define_singleton_method(desktopappinfo, "new_from_filename", desktopappinfo_new_from_filename, 1);
        rb_define_singleton_method(desktopappinfo, "new_from_keyfile", desktopappinfo_new_from_keyfile, 1);
        rb_define_singleton_method(desktopappinfo, "set_desktop_env", desktopappinfo_set_desktop_env, 1);

        rb_define_method(desktopappinfo, "initialize", desktopappinfo_initialize, 1);
        rb_define_method(desktopappinfo, "filename", desktopappinfo_get_filename, 0);
        rb_define_method(desktopappinfo, "hidden?", desktopappinfo_get_is_hidden, 0);
#endif
}
