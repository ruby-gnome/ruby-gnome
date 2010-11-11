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
#include <gio/gunixmounts.h>

static GUnixMountEntry *
unixmount_copy(const GUnixMountEntry *mount)
{
        return (GUnixMountEntry *)mount;
}

static GType
g_unix_mount_entry_get_type(void)
{
        static GType our_type = 0;

        if (our_type == 0)
                our_type = g_boxed_type_register_static("GUnixMountEntry",
                                                        (GBoxedCopyFunc)unixmount_copy,
                                                        (GBoxedFreeFunc)g_unix_mount_free);

        return our_type;
}

#define G_TYPE_UNIX_MOUNT_ENTRY (g_unix_mount_entry_get_type())

#define _SELF(value) \
        ((GUnixMountEntry *)(RVAL2BOXED((value), G_TYPE_UNIX_MOUNT_ENTRY)))

#define GUNIXMOUNTENTRY2RVAL(value) BOXED2RVAL((value), G_TYPE_UNIX_MOUNT_ENTRY)

static VALUE
unixmount_compare(VALUE self, VALUE other)
{
        return INT2FIX(g_unix_mount_compare(_SELF(self), _SELF(other)));
}

static VALUE
unixmount_get_mount_path(VALUE self)
{
        return CSTR2RVAL(g_unix_mount_get_mount_path(_SELF(self)));
}

static VALUE
unixmount_get_device_path(VALUE self)
{
        return CSTR2RVAL(g_unix_mount_get_device_path(_SELF(self)));
}

static VALUE
unixmount_get_fs_type(VALUE self)
{
        return CSTR2RVAL(g_unix_mount_get_fs_type(_SELF(self)));
}

static VALUE
unixmount_is_readonly(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_is_readonly(_SELF(self)));
}

static VALUE
unixmount_is_system_internal(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_is_system_internal(_SELF(self)));
}

static VALUE
unixmount_guess_icon(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_unix_mount_guess_icon(_SELF(self)));
}

static VALUE
unixmount_guess_name(VALUE self)
{
        return CSTR2RVAL_FREE(g_unix_mount_guess_name(_SELF(self)));
}

static VALUE
unixmount_guess_can_eject(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_guess_can_eject(_SELF(self)));
}

static VALUE
unixmount_guess_should_display(VALUE self)
{
        return CBOOL2RVAL(g_unix_mount_guess_should_display(_SELF(self)));
}

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

#define G_TYPE_UNIX_MOUNT_POINT (g_unix_mount_point_get_type())

#undef _SELF
#define _SELF(value) \
        ((GUnixMountPoint *)(RVAL2BOXED((value), G_TYPE_UNIX_MOUNT_POINT)))

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

static VALUE
unixmountpoints_get(G_GNUC_UNUSED VALUE self)
{
        guint64 time_read;
        GList *mount_points;

        mount_points = g_unix_mount_points_get(&time_read);

        return rb_assoc_new(GLIST2ARY_STR_FREE(mount_points),
                            GUINT642RVAL(time_read));
}

static VALUE
unixmounts_get(G_GNUC_UNUSED VALUE self)
{
        guint64 time_read;
        GList *mounts;

        mounts = g_unix_mounts_get(&time_read);

        return rb_assoc_new(GLIST2ARY_STR_FREE(mounts),
                            GUINT642RVAL(time_read));
}

static VALUE
unixmount_at(G_GNUC_UNUSED VALUE self, VALUE mount_path)
{
        guint64 time_read;
        GUnixMountEntry *mount;

        mount = g_unix_mount_at(RVAL2CSTR(mount_path), &time_read);

        return rb_assoc_new(GUNIXMOUNTENTRY2RVAL(mount),
                            GUINT642RVAL(time_read));
}

static VALUE
unixmounts_changed_since(G_GNUC_UNUSED VALUE self, VALUE time_read)
{
        return CBOOL2RVAL(g_unix_mounts_changed_since(RVAL2GUINT64(time_read)));
}

static VALUE
unixmountpoints_changed_since(G_GNUC_UNUSED VALUE self, VALUE time_read)
{
        return CBOOL2RVAL(g_unix_mount_points_changed_since(RVAL2GUINT64(time_read)));
}

#undef _SELF
#define _SELF(value) G_UNIX_MOUNT_MONITOR(RVAL2GOBJ(value))

static VALUE
unixmountmonitor_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE limit_msec;
        GUnixMountMonitor *monitor;

        rb_scan_args(argc, argv, "01", &limit_msec);
        monitor = g_unix_mount_monitor_new();
        if (!NIL_P(limit_msec))
                g_unix_mount_monitor_set_rate_limit(monitor,
                                                    FIX2INT(limit_msec));
        G_INITIALIZE(self, monitor);

        return Qnil;
}

static VALUE
unixmountmonitor_set_rate_limit(VALUE self, VALUE limit_msec)
{
        g_unix_mount_monitor_set_rate_limit(_SELF(self), FIX2INT(limit_msec));

        return self;
}

static VALUE
unix_is_mount_path_system_internal(G_GNUC_UNUSED VALUE self, VALUE mount_path)
{
        return CBOOL2RVAL(g_unix_is_mount_path_system_internal(RVAL2CSTR(mount_path)));
}

#endif

void
Init_gunixmounts(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE unixmount, unixmountpoint, unixmountpoints, unixmounts, unixmountmonitor;

        unixmount = G_DEF_CLASS(G_TYPE_UNIX_MOUNT_ENTRY, "UnixMount", glib);

        rb_include_module(unixmount, rb_mComparable);

        /* TODO: This doesn’t follow the naming conventions, but it seems
         * overkill to have GLib::Unix just for mount_path_system_internal?. */
        rb_define_singleton_method(unixmount, "mount_path_system_internal?", unix_is_mount_path_system_internal, 1);
        rb_define_singleton_method(unixmount, "at", unixmount_at, 1);

        rb_undef_alloc_func(unixmount);
        rbgobj_boxed_not_copy_obj(G_TYPE_UNIX_MOUNT_ENTRY);

        rb_define_method(unixmount, "compare", unixmount_compare, 1);
        rb_define_alias(unixmount, "<=>", "compare");
        rb_define_method(unixmount, "mount_path", unixmount_get_mount_path, 0);
        rb_define_method(unixmount, "device_path", unixmount_get_device_path, 0);
        rb_define_method(unixmount, "fs_type", unixmount_get_fs_type, 0);
        rb_define_method(unixmount, "readonly?", unixmount_is_readonly, 0);
        rb_define_method(unixmount, "system_internal?", unixmount_is_system_internal, 0);
        rb_define_method(unixmount, "guess_icon", unixmount_guess_icon, 0);
        rb_define_method(unixmount, "guess_name", unixmount_guess_name, 0);
        rb_define_method(unixmount, "guess_can_eject?", unixmount_guess_can_eject, 0);
        rb_define_method(unixmount, "guess_should_display?", unixmount_guess_should_display, 0);

        unixmountpoint = G_DEF_CLASS(G_TYPE_UNIX_MOUNT_POINT, "UnixMountPoint", glib);

        rb_include_module(unixmountpoint, rb_mComparable);

        rb_undef_alloc_func(unixmountpoint);
        rbgobj_boxed_not_copy_obj(G_TYPE_UNIX_MOUNT_POINT);

        rb_define_method(unixmountpoint, "compare", unixmountpoint_compare, 1);
        rb_define_alias(unixmountpoint, "<=>", "compare");
        rb_define_method(unixmountpoint, "mount_path", unixmountpoint_get_mount_path, 0);
        rb_define_method(unixmountpoint, "device_path", unixmountpoint_get_device_path, 0);
        rb_define_method(unixmountpoint, "fs_type", unixmountpoint_get_fs_type, 0);
        rb_define_method(unixmountpoint, "readonly?", unixmountpoint_is_readonly, 0);
        rb_define_method(unixmountpoint, "user_mountable?", unixmountpoint_is_user_mountable, 0);
        rb_define_method(unixmountpoint, "loopback?", unixmountpoint_is_loopback, 0);
        rb_define_method(unixmountpoint, "guess_icon", unixmountpoint_guess_icon, 0);
        rb_define_method(unixmountpoint, "guess_name", unixmountpoint_guess_name, 0);
        rb_define_method(unixmountpoint, "guess_can_eject?", unixmountpoint_guess_can_eject, 0);

        unixmountpoints = rb_define_module_under(glib, "UnixMountPoints");

        rb_define_module_function(unixmountpoints, "get", unixmountpoints_get, 0);
        rb_define_module_function(unixmountpoints, "changed_since?", unixmountpoints_changed_since, 1);

        unixmounts = rb_define_module_under(glib, "UnixMounts");

        rb_define_module_function(unixmounts, "get", unixmounts_get, 0);
        rb_define_module_function(unixmounts, "changed_since?", unixmounts_changed_since, 1);

        unixmountmonitor = G_DEF_CLASS(G_TYPE_UNIX_MOUNT_MONITOR, "UnixMountMonitor", glib);

        rb_define_method(unixmountmonitor, "initialize", unixmountmonitor_initialize, -1);
        rb_define_method(unixmountmonitor, "set_rate_limit", unixmountmonitor_set_rate_limit, 1);
        G_DEF_SETTER(unixmountmonitor, "rate_limit");
#endif
}
