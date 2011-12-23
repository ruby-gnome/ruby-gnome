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

#define RG_TARGET_NAMESPACE cMountOperation
#define _SELF(value) RVAL2GMOUNTOPERATION(value)

static VALUE
rg_initialize(VALUE self)
{
        G_INITIALIZE(self, g_mount_operation_new());

        return Qnil;
}

static VALUE
rg_reply(VALUE self, VALUE value)
{
        g_mount_operation_reply(_SELF(self), RVAL2GMOUNTOPERATIONRESULT(value));

        return self;
}

void
Init_gmountoperation(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_MOUNT_OPERATION, "MountOperation", mGio);

        G_DEF_CLASS(G_TYPE_MOUNT_OPERATION_RESULT, "Result", RG_TARGET_NAMESPACE);
        G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, G_TYPE_MOUNT_OPERATION_RESULT, "G_MOUNT_OPERATION_");

        RG_DEF_METHOD(initialize, 0);
        RG_DEF_METHOD(reply, 1);
}
