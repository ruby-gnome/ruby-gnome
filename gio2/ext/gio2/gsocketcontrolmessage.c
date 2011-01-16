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

#define _SELF(value) G_SOCKET_CONTROL_MESSAGE(RVAL2GOBJ(value))

static VALUE
socketcontrolmessage_deserialize(G_GNUC_UNUSED VALUE self, VALUE level, VALUE type, VALUE data)
{
        StringValue(data);
        return GOBJ2RVAL(g_socket_control_message_deserialize(FIX2INT(level),
                                                              FIX2INT(type),
                                                              RSTRING_LEN(data),
                                                              RSTRING_PTR(data)));
}

static VALUE
socketcontrolmessage_get_level(VALUE self)
{
        return INT2FIX(g_socket_control_message_get_level(_SELF(self)));
}

static VALUE
socketcontrolmessage_get_msg_type(VALUE self)
{
        return INT2FIX(g_socket_control_message_get_msg_type(_SELF(self)));
}

static VALUE
socketcontrolmessage_get_size(VALUE self)
{
        return GSIZE2RVAL(g_socket_control_message_get_size(_SELF(self)));
}

static VALUE
socketcontrolmessage_serialize(VALUE self)
{
        gsize size;
        VALUE result;
 
        /* TODO: Is tainting really necessary?  Where does the message come
         * from? */
        size = g_socket_control_message_get_size(_SELF(self));
        result = rb_str_new(NULL, size);
        g_socket_control_message_serialize(_SELF(self), RSTRING_PTR(result));
        OBJ_TAINT(result);

        return result;
}

void
Init_gsocketcontrolmessage(VALUE glib)
{
        VALUE socketcontrolmessage = G_DEF_CLASS(G_TYPE_SOCKET_CONTROL_MESSAGE, "SocketControlMessage", glib);

        rb_define_singleton_method(socketcontrolmessage, "deserialize", socketcontrolmessage_deserialize, 3);

        rb_define_method(socketcontrolmessage, "level", socketcontrolmessage_get_level, 0);
        rb_define_method(socketcontrolmessage, "msg_type", socketcontrolmessage_get_msg_type, 0);
        rb_define_method(socketcontrolmessage, "size", socketcontrolmessage_get_size, 0);
        rb_define_method(socketcontrolmessage, "serialize", socketcontrolmessage_serialize, 0);
}
