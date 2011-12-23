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

#define RG_TARGET_NAMESPACE cVfs
#define _SELF(value) G_VFS(RVAL2GOBJ(value))

static VALUE
rg_file_for_path(VALUE self, VALUE path)
{
        return GOBJ2RVAL_UNREF(g_vfs_get_file_for_path(_SELF(self), RVAL2CSTR(path)));
}

static VALUE
rg_file_for_uri(VALUE self, VALUE uri)
{
        return GOBJ2RVAL_UNREF(g_vfs_get_file_for_uri(_SELF(self), RVAL2CSTR(uri)));
}

static VALUE
rg_parse_name(VALUE self, VALUE parse_name)
{
        return GOBJ2RVAL_UNREF(g_vfs_parse_name(_SELF(self), RVAL2CSTR(parse_name)));
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL(g_vfs_get_default());
}

static VALUE
rg_s_local(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL(g_vfs_get_local());
}

static VALUE
rg_active_p(VALUE self)
{
        return CBOOL2RVAL(g_vfs_is_active(_SELF(self)));
}

static VALUE
rg_supported_uri_schemes(VALUE self)
{
        return STRV2RVAL((const gchar **)g_vfs_get_supported_uri_schemes(_SELF(self)));
}

void
Init_gvfs(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_VFS, "Vfs", mGio);

        rb_define_const(RG_TARGET_NAMESPACE, "EXTENSION_POINT_NAME", CSTR2RVAL(G_VFS_EXTENSION_POINT_NAME));

        RG_DEF_SMETHOD(default, 0);
        RG_DEF_SMETHOD(local, 0);

        RG_DEF_METHOD(file_for_path, 1);
        RG_DEF_METHOD(file_for_uri, 1);
        RG_DEF_METHOD(parse_name, 1);
        RG_DEF_METHOD_P(active, 0);
        RG_DEF_METHOD(supported_uri_schemes, 0);
}
