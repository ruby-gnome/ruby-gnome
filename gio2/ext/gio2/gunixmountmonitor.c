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

#define RG_TARGET_NAMESPACE cUnixMountMonitor
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

#endif

void
Init_gunixmountmonitor(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_MOUNT_MONITOR, "UnixMountMonitor", glib);

        rb_define_method(RG_TARGET_NAMESPACE, "initialize", unixmountmonitor_initialize, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "set_rate_limit", unixmountmonitor_set_rate_limit, 1);
        G_DEF_SETTER(RG_TARGET_NAMESPACE, "rate_limit");
#endif
}
