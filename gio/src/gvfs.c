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

#define _SELF(value) G_VFS(RVAL2GOBJ(value))

static VALUE
vfs_get_file_for_path(VALUE self, VALUE path)
{
        return GOBJ2RVAL_UNREF(g_vfs_get_file_for_path(_SELF(self), RVAL2CSTR(path)));
}

static VALUE
vfs_get_file_for_uri(VALUE self, VALUE uri)
{
        return GOBJ2RVAL_UNREF(g_vfs_get_file_for_uri(_SELF(self), RVAL2CSTR(uri)));
}

static VALUE
vfs_parse_name(VALUE self, VALUE parse_name)
{
        return GOBJ2RVAL_UNREF(g_vfs_parse_name(_SELF(self), RVAL2CSTR(parse_name)));
}

static VALUE
vfs_get_default(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL(g_vfs_get_default());
}

static VALUE
vfs_get_local(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL(g_vfs_get_local());
}

static VALUE
vfs_is_active(VALUE self)
{
        return CBOOL2RVAL(g_vfs_is_active(_SELF(self)));
}

static VALUE
vfs_get_supported_uri_schemes(VALUE self)
{
        return STRVECTOR2ARY(g_vfs_get_supported_uri_schemes(_SELF(self)));
}

void
Init_gvfs(VALUE glib)
{
        VALUE vfs = G_DEF_CLASS(G_TYPE_VFS, "Vfs", glib);

        rb_define_const(vfs, "EXTENSION_POINT_NAME", CSTR2RVAL(G_VFS_EXTENSION_POINT_NAME));

        rb_define_singleton_method(vfs, "default", vfs_get_default, 0);
        rb_define_singleton_method(vfs, "local", vfs_get_local, 0);

        rb_undef_alloc_func(vfs);

        rb_define_method(vfs, "file_for_path", vfs_get_file_for_path, 1);
        rb_define_method(vfs, "file_for_uri", vfs_get_file_for_uri, 1);
        rb_define_method(vfs, "parse_name", vfs_parse_name, 1);
        rb_define_method(vfs, "active?", vfs_is_active, 0);
        rb_define_method(vfs, "supported_uri_schemes", vfs_get_supported_uri_schemes, 0);
}
