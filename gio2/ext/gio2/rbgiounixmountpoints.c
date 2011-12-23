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
#include <gio/gunixmounts.h>

#define RG_TARGET_NAMESPACE mUnixMountPoints

static VALUE
rg_m_get(G_GNUC_UNUSED VALUE self)
{
        guint64 time_read;
        GList *mount_points;

        mount_points = g_unix_mount_points_get(&time_read);

        return rb_assoc_new(GLIST2ARY_STR_FREE(mount_points),
                            GUINT642RVAL(time_read));
}

static VALUE
rg_m_changed_since_p(G_GNUC_UNUSED VALUE self, VALUE time_read)
{
        return CBOOL2RVAL(g_unix_mount_points_changed_since(RVAL2GUINT64(time_read)));
}

#endif

void
Init_gunixmountpoints(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGio, "UnixMountPoints");

        RG_DEF_MODFUNC(get, 0);
        RG_DEF_MODFUNC_P(changed_since, 1);
#endif
}
