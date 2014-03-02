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

#ifdef HAVE_GIO_UNIX
#include <gio/gunixoutputstream.h>

#define RG_TARGET_NAMESPACE cUnixOutputStream
#define _SELF(value) RVAL2GUNIXOUTPUTSTREAM(value)

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
        VALUE fd, close_fd;

        rb_scan_args(argc, argv, "11", &fd, &close_fd);
        G_INITIALIZE(self, g_unix_output_stream_new(RVAL2FD(fd),
                                                    RVAL2CBOOL(close_fd)));

        return Qnil;
}
#endif

void
Init_gunixoutputstream(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(G_TYPE_UNIX_OUTPUT_STREAM, "UnixOutputStream", mGio);

        RG_DEF_METHOD(initialize, -1);
#endif
}
