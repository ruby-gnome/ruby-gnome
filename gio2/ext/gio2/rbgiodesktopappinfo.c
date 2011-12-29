/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008-2009  Ruby-GNOME2 Project Team
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

#include "rbgio2private.h"

#ifdef HAVE_GIO_UNIX
#include <gio/gdesktopappinfo.h>

#define RG_TARGET_NAMESPACE cDesktopAppInfo
#define _SELF(value) RVAL2GDESKTOPAPPINFO(value)

static VALUE
rg_s_new_from_filename(G_GNUC_UNUSED VALUE self, VALUE filename)
{
        return GOBJ2RVAL_UNREF(g_desktop_app_info_new_from_filename(RVAL2CSTR(filename)));
}

static VALUE
rg_s_new_from_keyfile(G_GNUC_UNUSED VALUE self, VALUE keyfile)
{
        return GOBJ2RVAL_UNREF(g_desktop_app_info_new_from_keyfile(RVAL2GKEYFILE(keyfile)));
}

static VALUE
rg_initialize(VALUE self, VALUE desktop_id)
{
        G_INITIALIZE(self, g_desktop_app_info_new(RVAL2CSTR(desktop_id)));

        return Qnil;
}

static VALUE
rg_filename(VALUE self)
{
        return CSTR2RVAL(g_desktop_app_info_get_filename(_SELF(self)));
}

static VALUE
rg_hidden_p(VALUE self)
{
        return CBOOL2RVAL(g_desktop_app_info_get_is_hidden(_SELF(self)));
}

static VALUE
rg_s_set_desktop_env(G_GNUC_UNUSED VALUE self, VALUE desktop_env)
{
        g_desktop_app_info_set_desktop_env(RVAL2CSTR(desktop_env));

        return self;
}

/* TODO: g_desktop_app_info_lookup_get_default_for_uri_scheme ()? */
#endif

void
Init_gdesktopappinfo(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_DESKTOP_APP_INFO, "DesktopAppInfo", mGio);

        RG_DEF_SMETHOD(new_from_filename, 1);
        RG_DEF_SMETHOD(new_from_keyfile, 1);
        RG_DEF_SMETHOD(set_desktop_env, 1);

        RG_DEF_METHOD(initialize, 1);
        RG_DEF_METHOD(filename, 0);
        RG_DEF_METHOD_P(hidden, 0);
#endif
}
