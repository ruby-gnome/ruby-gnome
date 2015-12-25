/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbgprivate.h"

#ifdef G_OS_WIN32

#define RG_TARGET_NAMESPACE cIOChannelWin32Socket

static VALUE
rg_initialize(VALUE self, VALUE socket)
{
    GIOChannel *io = NULL;
    int fd;

    /* TODO: support IO object */
    fd = NUM2INT(socket);
    io = g_io_channel_win32_new_socket(rb_w32_get_osfhandle(fd));
    G_INITIALIZE(self, io);

    return Qnil;
}
#endif

void
Init_glib_io_channel_win32_socket(void)
{
#ifdef G_OS_WIN32
    /* GIOWin32Channel */
    VALUE RG_TARGET_NAMESPACE;
    RG_TARGET_NAMESPACE =
        rb_define_class_under(mGLib,
                              "IOChannelWin32Socket",
                              rb_const_get(mGLib, rb_intern("IOChannel")));
    RG_DEF_METHOD(initialize, 1);
#endif
}
