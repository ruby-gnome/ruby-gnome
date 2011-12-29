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

#define RG_TARGET_NAMESPACE cVolumeMonitor
#define _SELF(value) RVAL2GVOLUMEMONITOR(value)

static VALUE
rg_s_get(G_GNUC_UNUSED VALUE self)
{
        return GOBJ2RVAL_UNREF(g_volume_monitor_get());
}

static VALUE
rg_connected_drives(VALUE self)
{
        return GLIST2ARY_FREE(g_volume_monitor_get_connected_drives(_SELF(self)));
}

static VALUE
rg_volumes(VALUE self)
{
        return GLIST2ARY_FREE(g_volume_monitor_get_volumes(_SELF(self)));
}

static VALUE
rg_mounts(VALUE self)
{
        return GLIST2ARY_FREE(g_volume_monitor_get_mounts(_SELF(self)));
}

static VALUE
rg_get_mount_for_uuid(VALUE self, VALUE value)
{
        return GOBJ2RVAL_UNREF(g_volume_monitor_get_mount_for_uuid(_SELF(self), RVAL2CSTR(value)));
}

static VALUE
rg_get_volume_for_uuid(VALUE self, VALUE value)
{
        return GOBJ2RVAL_UNREF(g_volume_monitor_get_volume_for_uuid(_SELF(self), RVAL2CSTR(value)));
}

void
Init_gvolumemonitor(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_VOLUME_MONITOR, "VolumeMonitor", mGio);

        rb_define_const(RG_TARGET_NAMESPACE, "EXTENSION_POINT_NAME", CSTR2RVAL(G_VOLUME_MONITOR_EXTENSION_POINT_NAME));

        RG_DEF_SMETHOD(get, 0);

        RG_DEF_METHOD(connected_drives, 0);
        RG_DEF_METHOD(volumes, 0);
        RG_DEF_METHOD(mounts, 0);
        RG_DEF_METHOD(get_mount_for_uuid, 0);
        RG_DEF_METHOD(get_volume_for_uuid, 0);
}
