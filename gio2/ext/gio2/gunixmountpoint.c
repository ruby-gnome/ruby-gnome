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

#include "gio2.h"

#ifdef HAVE_GIO_UNIX
#include <gio/gunixmounts.h>

#define G_TYPE_UNIX_MOUNT_POINT (g_unix_mount_point_get_type())

#define RG_TARGET_NAMESPACE cUnixMountPoint
#define _SELF(value) \
        ((GUnixMountPoint *)(RVAL2BOXED((value), G_TYPE_UNIX_MOUNT_POINT)))

static GUnixMountPoint *
unixmountpoint_copy(const GUnixMountPoint *mount_point)
{
        return (GUnixMountPoint *)mount_point;
}

static GType
g_unix_mount_point_get_type(void)
{
        static GType our_type = 0;

        if (our_type == 0)
                our_type = g_boxed_type_register_static("GUnixMountPoint",
                                                        (GBoxedCopyFunc)unixmountpoint_copy,
                                                        (GBoxedFreeFunc)g_unix_mount_point_free);

        return our_type;
}

static VALUE
unixmountpoint_compare(VALUE self, VALUE other)
{
        return INT2FIX(g_unix_mount_point_compare(_SELF(self), _SELF(other)));
}

static VALUE
unixmountpoint_get_mount_path(VALUE self)
{
        return CSTR2RVAL(g_unix_mount_point_get_mount_path(_SELF(self)));
}

static VALUE
unixmountpoint_get_device_path(VALUE self)
{
        return CSTR2RVAL(g_unix_mount_point_get_device_path(_SELF(self)));
}

static VALUE
unixmountpoint_get_fs_type(VALUE self)
{
        return CSTR2RVAL(g_unix_mount_point_get_fs_type(_SELF(self)));
}

static VALUE
unixmountpoint_is_readonly(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_point_is_readonly(_SELF(self)));
}

static VALUE
unixmountpoint_is_user_mountable(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_point_is_user_mountable(_SELF(self)));
}

static VALUE
unixmountpoint_is_loopback(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_point_is_loopback(_SELF(self)));
}

static VALUE
unixmountpoint_guess_icon(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_unix_mount_point_guess_icon(_SELF(self)));
}

static VALUE
unixmountpoint_guess_name(VALUE self)
{
        return CSTR2RVAL_FREE(g_unix_mount_point_guess_name(_SELF(self)));
}

static VALUE
unixmountpoint_guess_can_eject(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_point_guess_can_eject(_SELF(self)));
}

#endif

void
Init_gunixmountpoint(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_MOUNT_POINT, "UnixMountPoint", glib);

        rb_include_module(RG_TARGET_NAMESPACE, rb_mComparable);

        rbgobj_boxed_not_copy_obj(G_TYPE_UNIX_MOUNT_POINT);

        rb_define_method(RG_TARGET_NAMESPACE, "compare", unixmountpoint_compare, 1);
        rb_define_alias(RG_TARGET_NAMESPACE, "<=>", "compare");
        rb_define_method(RG_TARGET_NAMESPACE, "mount_path", unixmountpoint_get_mount_path, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "device_path", unixmountpoint_get_device_path, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "fs_type", unixmountpoint_get_fs_type, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "readonly?", unixmountpoint_is_readonly, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "user_mountable?", unixmountpoint_is_user_mountable, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "loopback?", unixmountpoint_is_loopback, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "guess_icon", unixmountpoint_guess_icon, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "guess_name", unixmountpoint_guess_name, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "guess_can_eject?", unixmountpoint_guess_can_eject, 0);
#endif
}
