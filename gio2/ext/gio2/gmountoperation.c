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

#define _SELF(value) RVAL2GMOUNTOPERATION(value)

#define GPASSWORDSAVE2RVAL(value) \
        GENUM2RVAL((value), G_TYPE_PASSWORD_SAVE)

#define RVAL2GPASSWORDSAVE(value) \
        RVAL2GENUM((value), G_TYPE_PASSWORD_SAVE)

#define RVAL2GMOUNTOPERATIONRESULT(value) \
        RVAL2GENUM((value), G_TYPE_MOUNT_OPERATION_RESULT)

static VALUE
mountoperation_initialize(VALUE self)
{
        G_INITIALIZE(self, g_mount_operation_new());

        return Qnil;
}

static VALUE
mountoperation_reply(VALUE self, VALUE value)
{
        g_mount_operation_reply(_SELF(self), RVAL2GMOUNTOPERATIONRESULT(value));

        return self;
}

void
Init_gmountoperation(VALUE glib)
{
        VALUE mountoperation = G_DEF_CLASS(G_TYPE_MOUNT_OPERATION, "MountOperation", glib);

        G_DEF_CLASS(G_TYPE_MOUNT_OPERATION_RESULT, "Result", mountoperation);
        G_DEF_CONSTANTS(mountoperation, G_TYPE_MOUNT_OPERATION_RESULT, "G_MOUNT_OPERATION_");

        G_DEF_CLASS(G_TYPE_ASK_PASSWORD_FLAGS, "AskPasswordFlags", glib);
        G_DEF_CONSTANTS(glib, G_TYPE_ASK_PASSWORD_FLAGS, "G_");

        G_DEF_CLASS(G_TYPE_PASSWORD_SAVE, "PasswordSave", glib);
        G_DEF_CONSTANTS(glib, G_TYPE_PASSWORD_SAVE, "G_");

        rb_define_method(mountoperation, "initialize", mountoperation_initialize, 0);
        rb_define_method(mountoperation, "reply", mountoperation_reply, 1);
}
