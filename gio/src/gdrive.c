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
        G_DRIVE(RVAL2GOBJ(value))

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_drive_get_##name(_SELF(self))); }

#define DEFSIMPLEMETHOD(name, type) \
        DECLARE(name)(VALUE self){ return type(g_drive_##name(_SELF(self))); }

DEFGETMETHOD(name, CSTR2RVAL2)
DEFGETMETHOD(icon, GOBJ2RVAL)
DEFSIMPLEMETHOD(has_volumes, CBOOL2RVAL)
DEFGETMETHOD(volumes, GLIST2ARY_FREE)
DEFSIMPLEMETHOD(can_eject, CBOOL2RVAL)
DEFSIMPLEMETHOD(can_poll_for_media, CBOOL2RVAL)

DECLARE_VARG(poll_for_media)
{
        GCancellable *cancellable;
        VALUE block;

        SCAN_1_ARG_WITH_BLOCK("01", cancellable, RVAL2GCANCELLABLE);

        g_drive_poll_for_media(_SELF(self),
                               cancellable,
                               rbgio_async_ready_callback,
                               (gpointer)block);

        return self;
}

#define DEFFINISHMETHOD(name) \
        DECLARE(name)(VALUE self, VALUE result) \
        { \
                GError *error = NULL; \
                if (!g_drive_##name(_SELF(self), \
                                     RVAL2GASYNCRESULT(result), \
                                     &error)) \
                        rbgio_raise_io_error(error); \
                return self; \
        }

DEFFINISHMETHOD(poll_for_media_finish)

DEFSIMPLEMETHOD(has_media, CBOOL2RVAL)
DEFSIMPLEMETHOD(is_media_check_automatic, CBOOL2RVAL)
DEFSIMPLEMETHOD(is_media_removable, CBOOL2RVAL)

DECLARE_VARG(eject)
{
        GMountUnmountFlags flags;
        GCancellable *cancellable;
        VALUE block;

        SCAN_2_ARGS_WITH_BLOCK("02",
                               flags, RVAL2GMOUNTUNMOUNTFLAGSDEFAULT,
                               cancellable, RVAL2GCANCELLABLE);

        g_drive_eject(_SELF(self),
                      flags,
                      cancellable,
                      rbgio_async_ready_callback,
                      (gpointer)block);

        return self;
}

DEFFINISHMETHOD(eject_finish)

DEFSIMPLEMETHOD(enumerate_identifiers, str_vector_to_ary)

DECLARE(get_identifier)(VALUE self, VALUE kind)
{
        return CSTR2RVAL_FREE(g_drive_get_identifier(_SELF(self),
                                                     RVAL2CSTR(kind)));
}

void
Init_gdrive(VALUE glib)
{
        VALUE drive = G_DEF_INTERFACE(G_TYPE_DRIVE, "Drive", glib);

        DEF_METHOD(drive, name, 0);
        DEF_METHOD(drive, icon, 0);
        DEF_BOOL_METHOD(drive, has_volumes, 0);
        DEF_METHOD(drive, volumes, 0);
        DEF_BOOL_METHOD(drive, can_eject, 0);
        DEF_BOOL_METHOD(drive, can_poll_for_media, 0);
        DEF_METHOD(drive, poll_for_media, -1);
        DEF_METHOD(drive, poll_for_media_finish, 1);
        DEF_BOOL_METHOD(drive, has_media, 0);
        DEF_IS_METHOD(drive, media_check_automatic, 0);
        DEF_IS_METHOD(drive, media_removable, 0);
        DEF_METHOD(drive, eject, -1);
        DEF_METHOD(drive, eject_finish, 1);
        DEF_METHOD(drive, enumerate_identifiers, -1);
        /* TODO: Should we actually rename enumerate_identifiers to identifiers
         * and only have that method? */
        rb_define_alias(drive, "identifiers", "enumerate_identifiers");
        DEF_METHOD(drive, get_identifier, 1);
}
