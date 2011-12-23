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

#define RG_TARGET_NAMESPACE mDrive
#define _SELF(value) G_DRIVE(RVAL2GOBJ(value))

static VALUE
rg_name(VALUE self)
{
        return CSTR2RVAL_FREE(g_drive_get_name(_SELF(self)));
}

static VALUE
rg_icon(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_drive_get_icon(_SELF(self)));
}

static VALUE
rg_has_volumes_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_has_volumes(_SELF(self)));
}

static VALUE
rg_volumes(VALUE self)
{
        return GLIST2ARY_FREE(g_drive_get_volumes(_SELF(self)));
}

static VALUE
rg_can_eject_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_eject(_SELF(self)));
}

static VALUE
rg_start_stop_type(VALUE self)
{
        return GDRIVESTARTSTOPTYPE2RVAL(g_drive_get_start_stop_type(_SELF(self)));
}

static VALUE
rg_can_start_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_start(_SELF(self)));
}

static VALUE
rg_can_start_degraded_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_start_degraded(_SELF(self)));
}

static VALUE
rg_can_stop_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_stop(_SELF(self)));
}

static VALUE
rg_can_poll_for_media_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_can_poll_for_media(_SELF(self)));
}

static VALUE
rg_poll_for_media(int argc, VALUE *argv, VALUE self)
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
rg_poll_for_media_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_poll_for_media_finish, self, result);
}

static VALUE
rg_has_media_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_has_media(_SELF(self)));
}

static VALUE
rg_media_check_automatic_p(VALUE self)
{
        return CBOOL2RVAL(g_drive_is_media_check_automatic(_SELF(self)));
}

static VALUE
rg_media_removable_p(VALUE self)
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
rg_eject_with_operation(int argc, VALUE *argv, VALUE self)
{
        return eject_stop_method(g_drive_eject_with_operation, argc, argv, self);
}

static VALUE
rg_eject_with_operation_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_eject_with_operation_finish, self, result);
}

static VALUE
rg_start(int argc, VALUE *argv, VALUE self)
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
rg_start_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_start_finish, self, result);
}

static VALUE
rg_stop(int argc, VALUE *argv, VALUE self)
{
        return eject_stop_method(g_drive_stop, argc, argv, self);
}

static VALUE
rg_stop_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_drive_stop_finish, self, result);
}

static VALUE
rg_enumerate_identifiers(VALUE self)
{
        return STRV2RVAL_FREE(g_drive_enumerate_identifiers(_SELF(self)));
}

static VALUE
rg_get_identifier(VALUE self, VALUE kind)
{
        return CSTR2RVAL_FREE(g_drive_get_identifier(_SELF(self), RVAL2CSTR(kind)));
}

void
Init_gdrive(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_DRIVE, "Drive", mGio);

        G_DEF_CLASS(G_TYPE_DRIVE_START_FLAGS, "StartFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_DRIVE_START_FLAGS, "G_DRIVE_");

        G_DEF_CLASS(G_TYPE_DRIVE_START_STOP_TYPE, "StartStopType", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_DRIVE_START_STOP_TYPE, "G_DRIVE_");

        RG_DEF_METHOD(name, 0);
        RG_DEF_METHOD(icon, 0);
        RG_DEF_METHOD_P(has_volumes, 0);
        RG_DEF_METHOD(volumes, 0);
        RG_DEF_METHOD_P(can_eject, 0);
        RG_DEF_METHOD(start_stop_type, 0);
        RG_DEF_METHOD_P(can_start, 0);
        RG_DEF_METHOD_P(can_start_degraded, 0);
        RG_DEF_METHOD_P(can_stop, 0);
        RG_DEF_METHOD_P(can_poll_for_media, 0);
        RG_DEF_METHOD(poll_for_media, -1);
        RG_DEF_METHOD(poll_for_media_finish, 1);
        RG_DEF_METHOD_P(has_media, 0);
        RG_DEF_METHOD_P(media_check_automatic, 0);
        RG_DEF_METHOD_P(media_removable, 0);
        RG_DEF_METHOD(eject_with_operation, -1);
        RG_DEF_METHOD(eject_with_operation_finish, 1);
        RG_DEF_METHOD(start, -1);
        RG_DEF_METHOD(start_finish, 1);
        RG_DEF_METHOD(stop, -1);
        RG_DEF_METHOD(stop_finish, 1);
        RG_DEF_METHOD(enumerate_identifiers, -1);
        RG_DEF_ALIAS("identifiers", "enumerate_identifiers");
        RG_DEF_METHOD(get_identifier, 1);
}
