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

#define RG_TARGET_NAMESPACE mVolume
#define _SELF(value) G_VOLUME(RVAL2GOBJ(value))

static VALUE
volume_get_name(VALUE self)
{
        return CSTR2RVAL_FREE(g_volume_get_name(_SELF(self)));
}

static VALUE
volume_get_uuid(VALUE self)
{
        return CSTR2RVAL_FREE(g_volume_get_uuid(_SELF(self)));
}

static VALUE
volume_get_icon(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_volume_get_icon(_SELF(self)));
}

static VALUE
volume_get_drive(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_volume_get_drive(_SELF(self)));
}

static VALUE
volume_get_mount(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_volume_get_mount(_SELF(self)));
}

static VALUE
volume_can_mount(VALUE self)
{
        return CBOOL2RVAL(g_volume_can_mount(_SELF(self)));
}

static VALUE
volume_should_automount(VALUE self)
{
        return CBOOL2RVAL(g_volume_should_automount(_SELF(self)));
}

static VALUE
volume_get_activation_root(VALUE self)
{
        return GOBJ2RVAL_UNREF(g_volume_get_activation_root(_SELF(self)));
}

static VALUE
volume_mount(int argc, VALUE *argv, VALUE self)
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
        g_volume_mount(_SELF(self),
                       flags,
                       mount_operation,
                       cancellable,
                       rbgio_async_ready_callback,
                       (gpointer)block);

        return self;
}

typedef gboolean (*BooleanFinishMethod)(GVolume *,
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
volume_mount_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_volume_mount_finish, self, result);
}

static VALUE
volume_can_eject(VALUE self)
{
        return CBOOL2RVAL(g_volume_can_eject(_SELF(self)));
}

static VALUE
volume_eject_with_operation(int argc, VALUE *argv, VALUE self)
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
        g_volume_eject_with_operation(_SELF(self),
                                      flags,
                                      mount_operation,
                                      cancellable,
                                      rbgio_async_ready_callback,
                                      (gpointer)block);

        return self;
}

static VALUE
volume_eject_with_operation_finish(VALUE self, VALUE result)
{
        return boolean_finish_method(g_volume_eject_with_operation_finish, self, result);
}

static VALUE
volume_enumerate_identifiers(VALUE self)
{
        return STRV2RVAL_FREE(g_volume_enumerate_identifiers(_SELF(self)));
}

static VALUE
volume_get_identifier(VALUE self, VALUE kind)
{
        return CSTR2RVAL_FREE(g_volume_get_identifier(_SELF(self), RVAL2CSTR(kind)));
}

void
Init_gvolume(VALUE glib)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_VOLUME, "Volume", glib);

        rb_define_const(RG_TARGET_NAMESPACE, "IDENTIFIER_KIND_HAL_UDI", CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_HAL_UDI));
        rb_define_const(RG_TARGET_NAMESPACE, "IDENTIFIER_KIND_LABEL", CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_LABEL));
        rb_define_const(RG_TARGET_NAMESPACE, "IDENTIFIER_KIND_NFS_MOUNT", CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_NFS_MOUNT));
        rb_define_const(RG_TARGET_NAMESPACE, "IDENTIFIER_KIND_UNIX_DEVICE", CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_UNIX_DEVICE));
        rb_define_const(RG_TARGET_NAMESPACE, "IDENTIFIER_KIND_UUID", CSTR2RVAL(G_VOLUME_IDENTIFIER_KIND_UUID));

        rb_define_method(RG_TARGET_NAMESPACE, "name", volume_get_name, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "uuid", volume_get_uuid, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "icon", volume_get_icon, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "drive", volume_get_drive, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "get_mount", volume_get_mount, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "can_mount?", volume_can_mount, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "should_automount?", volume_should_automount, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "activation_root", volume_get_activation_root, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "mount", volume_mount, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "mount_finish", volume_mount_finish, 1);
        rb_define_method(RG_TARGET_NAMESPACE, "can_eject?", volume_can_eject, 0);
        rb_define_method(RG_TARGET_NAMESPACE, "eject_with_operation", volume_eject_with_operation, -1);
        rb_define_method(RG_TARGET_NAMESPACE, "eject_with_operation_finish", volume_eject_with_operation_finish, 1);
        rb_define_method(RG_TARGET_NAMESPACE, "enumerate_identifiers", volume_enumerate_identifiers, -1);
        rb_define_alias(RG_TARGET_NAMESPACE, "identifiers", "enumerate_identifiers");
        rb_define_method(RG_TARGET_NAMESPACE, "get_identifier", volume_get_identifier, 1);
}
