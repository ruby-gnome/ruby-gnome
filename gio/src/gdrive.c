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

#define _SELF(value) G_DRIVE(RVAL2GOBJ(value))

#define GDRIVESTARTSTOPTYPE2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_DRIVE_START_STOP_TYPE)

static VALUE
drive_get_name(VALUE self)
{
        return CSTR2RVAL_FREE(g_drive_get_name(_SELF(self)));
}

static VALUE
drive_get_icon(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_drive_get_icon(_SELF(self)));
}

static VALUE
drive_has_volumes(VALUE self)
{
        return CBOOL2RVAL(g_drive_has_volumes(_SELF(self)));
}

static VALUE
drive_get_volumes(VALUE self)
{
        return GLIST2ARY_FREE(g_drive_get_volumes(_SELF(self)));
}

static VALUE
drive_can_eject(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_eject(_SELF(self)));
}

static VALUE
drive_get_start_stop_type(VALUE self)
{
        return GDRIVESTARTSTOPTYPE2RVAL(g_drive_get_start_stop_type(_SELF(self)));
}

static VALUE
drive_can_start(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_start(_SELF(self)));
}

static VALUE
drive_can_start_degraded(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_start_degraded(_SELF(self)));
}

static VALUE
drive_can_stop(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_stop(_SELF(self)));
}

static VALUE
drive_can_poll_for_media(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_poll_for_media(_SELF(self)));
}

static VALUE
drive_poll_for_media(int argc, VALUE *argv, VALUE self)
{
        VALUE rbcancellable, block;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "01&", &rbcancellable, &block);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        g_drive_poll_for_media(_SELF(self),
                               cancellable,
                               rbgio_async_ready_callback,
                               (gpointer)block);

        return self;
}

typedef gboolean (*BooleanFinishMethod)(GDrive *, GAsyncResult *, GError **);

static VALUE
boolean_finish_method(BooleanFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;

        if (!method(_SELF(self), RVAL2GASYNCRESULT(result), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
drive_poll_for_media_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_poll_for_media_finish, self, result);
}

static VALUE
drive_has_media(VALUE self)
{
        return CBOOL2RVAL(g_drive_has_media(_SELF(self)));
}

static VALUE
drive_is_media_check_automatic(VALUE self)
{
        return CBOOL2RVAL(g_drive_is_media_check_automatic(_SELF(self)));
}

static VALUE
drive_is_media_removable(VALUE self)
{
        return CBOOL2RVAL(g_drive_is_media_removable(_SELF(self)));
}

typedef void (*EjectStopMethod)(GDrive *,
                                GMountUnmountFlags,
                                GMountOperation *,
                                GCancellable *,
                                GAsyncReadyCallback,
                                gpointer);

static VALUE
eject_stop_method(EjectStopMethod method, int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbmount_operation, rbcancellable, block;
        GMountUnmountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbflags, &rbmount_operation, &rbcancellable, &block);
        flags = RVAL2GMOUNTUNMOUNTFLAGSDEFAULT(rbflags);
        mount_operation = RVAL2GMOUNTOPERATION(rbmount_operation);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        method(_SELF(self),
               flags,
               mount_operation,
               cancellable,
               rbgio_async_ready_callback,
               (gpointer)block);

        return self;
}

static VALUE
drive_eject_with_operation(int argc, VALUE *argv, VALUE self)
{
        return eject_stop_method(g_drive_eject_with_operation, argc, argv, self);
}

static VALUE
drive_eject_with_operation_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_eject_with_operation_finish, self, result);
}

static VALUE
drive_start(int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbmount_operation, rbcancellable, block;
        GDriveStartFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03", &rbflags, &rbmount_operation, &rbcancellable, &block);
        flags = RVAL2GDRIVESTARTFLAGSDEFAULT(rbflags);
        mount_operation = RVAL2GMOUNTOPERATION(rbmount_operation);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_drive_start(_SELF(self), flags, mount_operation, cancellable,
                      rbgio_async_ready_callback, (gpointer)block);

        return self;
}

static VALUE
drive_start_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_start_finish, self, result);
}

static VALUE
drive_stop(int argc, VALUE *argv, VALUE self)
{
        return eject_stop_method(g_drive_stop, argc, argv, self);
}

static VALUE
drive_stop_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_stop_finish, self, result);
}

static VALUE
drive_enumerate_identifiers(VALUE self)
{
        return STRVECTOR2ARY_FREE(g_drive_enumerate_identifiers(_SELF(self)));
}

static VALUE
drive_get_identifier(VALUE self, VALUE kind)
{
        return CSTR2RVAL_FREE(g_drive_get_identifier(_SELF(self), RVAL2CSTR(kind)));
}

void
Init_gdrive(VALUE glib)
{
        VALUE drive = G_DEF_INTERFACE(G_TYPE_DRIVE, "Drive", glib);

        G_DEF_CLASS(G_TYPE_DRIVE_START_FLAGS, "StartFlags", drive);
        G_DEF_CONSTANTS(drive, G_TYPE_DRIVE_START_FLAGS, "G_DRIVE_");

        G_DEF_CLASS(G_TYPE_DRIVE_START_STOP_TYPE, "StartStopType", drive);
        G_DEF_CONSTANTS(drive, G_TYPE_DRIVE_START_STOP_TYPE, "G_DRIVE_");

        rb_define_method(drive, "name", drive_get_name, 0);
        rb_define_method(drive, "icon", drive_get_icon, 0);
        rb_define_method(drive, "has_volumes?", drive_has_volumes, 0);
        rb_define_method(drive, "volumes", drive_get_volumes, 0);
        rb_define_method(drive, "can_eject?", drive_can_eject, 0);
        rb_define_method(drive, "start_stop_type", drive_get_start_stop_type, 0);
        rb_define_method(drive, "can_start?", drive_can_start, 0);
        rb_define_method(drive, "can_start_degraded?", drive_can_start_degraded, 0);
        rb_define_method(drive, "can_stop?", drive_can_stop, 0);
        rb_define_method(drive, "can_poll_for_media?", drive_can_poll_for_media, 0);
        rb_define_method(drive, "poll_for_media", drive_poll_for_media, -1);
        rb_define_method(drive, "poll_for_media_finish", drive_poll_for_media_finish, 1);
        rb_define_method(drive, "has_media?", drive_has_media, 0);
        rb_define_method(drive, "media_check_automatic?", drive_is_media_check_automatic, 0);
        rb_define_method(drive, "media_removable?", drive_is_media_removable, 0);
        rb_define_method(drive, "eject_with_operation", drive_eject_with_operation, -1);
        rb_define_method(drive, "eject_with_operation_finish", drive_eject_with_operation_finish, 1);
        rb_define_method(drive, "start", drive_start, -1);
        rb_define_method(drive, "start_finish", drive_start_finish, 1);
        rb_define_method(drive, "stop", drive_stop, -1);
        rb_define_method(drive, "stop_finish", drive_stop_finish, 1);
        rb_define_method(drive, "enumerate_identifiers", drive_enumerate_identifiers, -1);
        rb_define_alias(drive, "identifiers", "enumerate_identifiers");
        rb_define_method(drive, "get_identifier", drive_get_identifier, 1);
}
