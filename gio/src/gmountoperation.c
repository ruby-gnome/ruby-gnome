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
        RVAL2GMOUNTOPERATION(value)

DECLARE(initialize)(VALUE self)
{
        G_INITIALIZE(self, g_mount_operation_new());

        return Qnil;
}

#define DEFGETMETHOD(name, type) \
        DECLARE(name)(VALUE self) \
        { \
                return type(g_mount_operation_get_##name(_SELF(self))); \
        }

#define DEFSETMETHOD(name, type) \
        DECLARE(name)(VALUE self, VALUE value) \
        { \
                g_mount_operation_##name(_SELF(self), type(value)); \
                return self; \
        }

DEFGETMETHOD(username, CSTR2RVAL)
DEFSETMETHOD(set_username, RVAL2CSTR)
DEFGETMETHOD(password, CSTR2RVAL)
DEFSETMETHOD(set_password, RVAL2CSTR)
DEFGETMETHOD(anonymous, CBOOL2RVAL)
DEFSETMETHOD(set_anonymous, RVAL2CBOOL)
DEFGETMETHOD(domain, CSTR2RVAL)
DEFSETMETHOD(set_domain, RVAL2CSTR)
DEFGETMETHOD(password_save, GPASSWORDSAVE2RVAL)
DEFSETMETHOD(set_password_save, RVAL2GPASSWORDSAVE)
DEFGETMETHOD(choice, INT2NUM)
DEFSETMETHOD(set_choice, NUM2INT)

DEFSETMETHOD(reply, RVAL2GMOUNTOPERATIONRESULT)

void
Init_gmountoperation(VALUE glib)
{
        VALUE mountoperation = G_DEF_CLASS(G_TYPE_MOUNT_OPERATION,
                                           "MountOperation",
                                           glib);

        /* TODO: Are these necessary? */
        DEF_CONSTANTS_CLASS(G_TYPE_MOUNT_OPERATION_RESULT,
                            "Result",
                            "G_MOUNT_OPERATION_",
                            mountoperation);

        DEF_CONSTANTS_CLASS(G_TYPE_ASK_PASSWORD_FLAGS,
                            "AskPasswordFlags",
                            "G_",
                            glib);
        DEF_CONSTANTS_CLASS(G_TYPE_PASSWORD_SAVE,
                            "PasswordSave",
                            "G_",
                            glib);


        DEF_METHOD(mountoperation, initialize, 0);
        DEF_ATTRIBUTE(mountoperation, username);
        DEF_ATTRIBUTE(mountoperation, password);
        DEF_ATTRIBUTE(mountoperation, anonymous);
        DEF_ATTRIBUTE(mountoperation, domain);
        DEF_ATTRIBUTE(mountoperation, password_save);
        DEF_ATTRIBUTE(mountoperation, choice);
        DEF_METHOD(mountoperation, reply, 1);
}
