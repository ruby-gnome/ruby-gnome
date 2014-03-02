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

#define RG_TARGET_NAMESPACE mMount
#define _SELF(value) RVAL2GMOUNT(value)

static VALUE
rg_name(VALUE self)
{
        return CSTR2RVAL_FREE(g_mount_get_name(_SELF(self)));
}

static VALUE
rg_uuid(VALUE self)
{
        return CSTR2RVAL_FREE(g_mount_get_uuid(_SELF(self)));
}

static VALUE
rg_icon(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_mount_get_icon(_SELF(self)));
}

static VALUE
rg_drive(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_mount_get_drive(_SELF(self)));
}

static VALUE
rg_root(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_mount_get_root(_SELF(self)));
}

static VALUE
rg_volume(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_mount_get_volume(_SELF(self)));
}

static VALUE
rg_default_location(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_mount_get_default_location(_SELF(self)));
}

static VALUE
rg_can_unmount_p(VALUE self)
{
        return CBOOL2RVAL(g_mount_can_unmount(_SELF(self)));
}

typedef void (*UnmountEjectMethod)(GMount *,
                                   GMountUnmountFlags,
                                   GMountOperation *,
                                   GCancellable *,
                                   GAsyncReadyCallback,
                                   gpointer);

static VALUE
unmount_eject_method(UnmountEjectMethod method, int argc, VALUE *argv, VALUE self)
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
rg_unmount_with_operation(int argc, VALUE *argv, VALUE self)
{
        return unmount_eject_method(g_mount_unmount_with_operation, argc, argv, self);
}

typedef gboolean (*BooleanFinishMethod)(GMount *,
                                        GAsyncResult *,
                                        GError **);

static VALUE
boolean_finish_method(BooleanFinishMethod method, VALUE self, VALUE result)
{
        GError *error = NULL;

        if (!method(_SELF(self), RVAL2GASYNCRESULT(result), &error))
                rbgio_raise_error(error);

        return self;
}

static VALUE
rg_unmount_with_operation_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_mount_unmount_with_operation_finish, self, result);
}

static VALUE
rg_remount(int argc, VALUE *argv, VALUE self)
{
        VALUE rbflags, rbmount_operation, rbcancellable, block;
        GMountMountFlags flags;
        GMountOperation *mount_operation;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "03&", &rbflags, &rbmount_operation, &rbcancellable, &block);
        flags = RVAL2GMOUNTMOUNTFLAGSDEFAULT(rbflags);
        mount_operation = RVAL2GMOUNTOPERATION(rbmount_operation);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_mount_remount(_SELF(self),
                        flags,
                        mount_operation,
                        cancellable,
                        rbgio_async_ready_callback,
                        (gpointer)block);

        return self;
}

static VALUE
rg_remount_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_mount_remount_finish, self, result);
}

static VALUE
rg_can_eject_p(VALUE self)
{
        return CBOOL2RVAL(g_mount_can_eject(_SELF(self)));
}

static VALUE
rg_eject_with_operation(int argc, VALUE *argv, VALUE self)
{
        return unmount_eject_method(g_mount_eject_with_operation, argc, argv, self);
}

static VALUE
rg_eject_with_operation_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_mount_eject_with_operation_finish, self, result);
}

static VALUE
rg_guess_content_type(int argc, VALUE *argv, VALUE self)
{
        VALUE rbforce_rescan, rbcancellable, block;
        gboolean force_rescan;
        GCancellable *cancellable;

        rb_scan_args(argc, argv, "02&", &rbforce_rescan, &rbcancellable, &block);
        force_rescan = RVAL2CBOOL(rbforce_rescan);
        cancellable = RVAL2GCANCELLABLE(rbcancellable);
        SAVE_BLOCK(block);
        g_mount_guess_content_type(_SELF(self),
                                   force_rescan,
                                   cancellable,
                                   rbgio_async_ready_callback,
                                   (gpointer)block);

        return self;
}

static VALUE
rg_guess_content_type_finish(VALUE self, VALUE result)
{
        GError *error = NULL;
        gchar **content_types;

        content_types = g_mount_guess_content_type_finish(_SELF(self),
                                                          RVAL2GASYNCRESULT(result),
                                                          &error);
        if (content_types == NULL)
                rbgio_raise_error(error);

        return STRV2RVAL_FREE(content_types);
}

static VALUE
rg_guess_content_type_sync(int argc, VALUE *argv, VALUE self)
{
        VALUE force_rescan, cancellable;
        GError *error = NULL;
        gchar **content_types;

        rb_scan_args(argc, argv, "02", &force_rescan, &cancellable);
        content_types = g_mount_guess_content_type_sync(_SELF(self),
                                                        RVAL2CBOOL(force_rescan),
                                                        RVAL2GCANCELLABLE(cancellable),
                                                        &error);
        if (content_types == NULL)
                rbgio_raise_error(error);

        return STRV2RVAL_FREE(content_types);
}

static VALUE
rg_shadowed_p(VALUE self)
{
        return CBOOL2RVAL(g_mount_is_shadowed(_SELF(self)));
}

static VALUE
rg_shadow(VALUE self)
{
        g_mount_shadow(_SELF(self));

        return self;
}

static VALUE
rg_unshadow(VALUE self)
{
        g_mount_unshadow(_SELF(self));

        return self;
}

void
Init_gmount(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_MOUNT, "Mount", mGio);

        G_DEF_CLASS(G_TYPE_MOUNT_MOUNT_FLAGS, "MountFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_MOUNT_MOUNT_FLAGS, "G_MOUNT_");

        G_DEF_CLASS(G_TYPE_MOUNT_UNMOUNT_FLAGS, "UnmountFlags", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_MOUNT_UNMOUNT_FLAGS, "G_MOUNT_");

        RG_DEF_METHOD(name, 0);
        RG_DEF_METHOD(uuid, 0);
        RG_DEF_METHOD(icon, 0);
        RG_DEF_METHOD(drive, 0);
        RG_DEF_METHOD(root, 0);
        RG_DEF_METHOD(volume, 0);
        RG_DEF_METHOD(default_location, 0);
        RG_DEF_METHOD_P(can_unmount, 0);
        RG_DEF_METHOD(unmount_with_operation, -1);
        RG_DEF_METHOD(unmount_with_operation_finish, 1);
        RG_DEF_METHOD(remount, -1);
        RG_DEF_METHOD(remount_finish, 1);
        RG_DEF_METHOD_P(can_eject, 0);
        RG_DEF_METHOD(eject_with_operation, -1);
        RG_DEF_METHOD(eject_with_operation_finish, 1);
        RG_DEF_METHOD(guess_content_type, -1);
        RG_DEF_METHOD(guess_content_type_finish, 1);
        RG_DEF_METHOD(guess_content_type_sync, -1);
        RG_DEF_METHOD_P(shadowed, 0);
        RG_DEF_METHOD(shadow, 0);
        RG_DEF_METHOD(unshadow, 0);
}
