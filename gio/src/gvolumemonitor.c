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

#define _SELF(value) G_VOLUME_MONITOR(RVAL2GOBJ(value))

static VALUE
volumemonitor_get(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL(g_volume_monitor_get());
}

static VALUE
volumemonitor_get_connected_drives(VALUE self)
{
        return GLIST2ARY_FREE(g_volume_monitor_get_connected_drives(_SELF(self)));
}

static VALUE
volumemonitor_get_volumes(VALUE self)
{
        return GLIST2ARY_FREE(g_volume_monitor_get_volumes(_SELF(self)));
}

static VALUE
volumemonitor_get_mounts(VALUE self)
{
        return GLIST2ARY_FREE(g_volume_monitor_get_mounts(_SELF(self)));
}

static VALUE
volumemonitor_get_mount_for_uuid(VALUE self, VALUE value)
{
        return GOBJ2RVAL(g_volume_monitor_get_mount_for_uuid(_SELF(self), RVAL2CSTR(value)));
}

static VALUE
volumemonitor_get_volume_for_uuid(VALUE self, VALUE value)
{
        return GOBJ2RVAL(g_volume_monitor_get_volume_for_uuid(_SELF(self), RVAL2CSTR(value)));
}

void
Init_gvolumemonitor(VALUE glib)
{
        VALUE volumemonitor = G_DEF_CLASS(G_TYPE_VOLUME_MONITOR, "VolumeMonitor", glib);

        rb_define_const(volumemonitor, "EXTENSION_POINT_NAME", CSTR2RVAL(G_VOLUME_MONITOR_EXTENSION_POINT_NAME));

        rb_undef_alloc_func(volumemonitor);

        rb_define_singleton_method(volumemonitor, "get", volumemonitor_get, 0);

        rb_define_method(volumemonitor, "connected_drives", volumemonitor_get_connected_drives, 0);
        rb_define_method(volumemonitor, "volumes", volumemonitor_get_volumes, 0);
        rb_define_method(volumemonitor, "mounts", volumemonitor_get_mounts, 0);
        rb_define_method(volumemonitor, "get_mount_for_uuid", volumemonitor_get_mount_for_uuid, 0);
        rb_define_method(volumemonitor, "get_volume_for_uuid", volumemonitor_get_volume_for_uuid, 0);
}
