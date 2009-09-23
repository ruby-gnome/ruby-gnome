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

#define _SELF(value) \
        G_VOLUME(RVAL2GOBJ(value))

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_volume_get_##name(_SELF(self))); }

DEFGETMETHOD(name, CSTR2RVAL_FREE)
DEFGETMETHOD(uuid, CSTR2RVAL_FREE)
DEFGETMETHOD(icon, GOBJ2RVAL)
DEFGETMETHOD(drive, GOBJ2RVAL)

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_volume_##name(_SELF(self))); }

DEFSIMPLEMETHOD(get_mount, GOBJ2RVAL)
DEFSIMPLEMETHOD(can_mount, CBOOL2RVAL)
DEFSIMPLEMETHOD(should_automount, CBOOL2RVAL)
DEFGETMETHOD(activation_root, GOBJ2RVAL)

DECLARE_VARG(mount)
{
        GMountMountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;
        VALUE block;

        SCAN_3_ARGS_WITH_BLOCK("03",
                               flags, RVAL2GMOUNTMOUNTFLAGSDEFAULT,
                               mount_operation, RVAL2GMOUNTOPERATION,
                               cancellable, RVAL2GCANCELLABLE);

        g_volume_mount(_SELF(self),
                       flags,
                       mount_operation,
                       cancellable,
                       rbgio_async_ready_callback,
                       (gpointer)block);

        return self;
}

DECLARE(mount_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_volume_mount_finish(_SELF(self),
                                   RVAL2GASYNCRESULT(result),
                                   &error))
                rbgio_raise_io_error(error);

        return self;
}

DEFSIMPLEMETHOD(can_eject, CBOOL2RVAL)

DECLARE_VARG(eject)
{
        GMountUnmountFlags flags;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               flags, RVAL2GMOUNTUNMOUNTFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_volume_eject(_SELF(self),
                       flags,
                       cancellable,
                       rbgio_async_ready_callback,
                       (gpointer)block);

        return self;
}

DECLARE(eject_finish)(VALUE self, VALUE result)
{
        GError *error = NULL;
        if (!g_volume_eject_finish(_SELF(self),
                                   RVAL2GASYNCRESULT(result),
                                   &error))
                rbgio_raise_io_error(error);

        return self;
}

DEFSIMPLEMETHOD(enumerate_identifiers, str_vector_to_ary)

DECLARE(get_identifier)(VALUE self, VALUE kind)
{
        return CSTR2RVAL_FREE(g_volume_get_identifier(_SELF(self),
                                                      RVAL2CSTR(kind)));
}

void
Init_gvolume(VALUE glib)
{
        VALUE volume = G_DEF_INTERFACE(G_TYPE_VOLUME, "Volume", glib);

        rb_define_const(volume, "IDENTIFIER_KIND_HAL_UDI",
                        CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_HAL_UDI));
        rb_define_const(volume, "IDENTIFIER_KIND_LABEL",
                        CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_LABEL));
        rb_define_const(volume, "IDENTIFIER_KIND_NFS_MOUNT",
                        CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_NFS_MOUNT));
        rb_define_const(volume, "IDENTIFIER_KIND_UNIX_DEVICE",
                        CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE));
        rb_define_const(volume, "IDENTIFIER_KIND_UUID",
                        CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_UUID));

        DEF_METHOD(volume, name, 0);
        DEF_METHOD(volume, uuid, 0);
        DEF_METHOD(volume, icon, 0);
        DEF_METHOD(volume, drive, 0);
        DEF_METHOD(volume, get_mount, 0);
        DEF_BOOL_METHOD(volume, can_mount, 0);
        DEF_BOOL_METHOD(volume, should_automount, 0);
        DEF_METHOD(volume, activation_root, 0);
        DEF_METHOD(volume, mount, -1);
        DEF_METHOD(volume, mount_finish, 1);
        DEF_BOOL_METHOD(volume, can_eject, 0);
        DEF_METHOD(volume, eject, -1);
        DEF_METHOD(volume, eject_finish, 1);
        DEF_METHOD(volume, enumerate_identifiers, -1);
        /* TODO: Does this make sense? */
        rb_define_alias(volume, "identifiers", "enumerate_identifiers");
        DEF_METHOD(volume, get_identifier, 1);
}
