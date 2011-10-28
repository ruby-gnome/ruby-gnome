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

#define RG_TARGET_NAMESPACE cIOChannelError

static VALUE
rg_s_from_errno(G_GNUC_UNUSED VALUE self, VALUE errno_)
{
    return INT2NUM(g_io_channel_error_from_errno(NUM2INT(errno_)));
}

void
Init_glib_io_channelerror(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_ERROR2(G_IO_CHANNEL_ERROR, "IOChannelError", mGLib, rb_eIOError);

    /* GIOChannelError */
    RG_DEF_SMETHOD(from_errno, 1);

    rb_define_const(RG_TARGET_NAMESPACE, "FBIG", INT2NUM(G_IO_CHANNEL_ERROR_FBIG));
    rb_define_const(RG_TARGET_NAMESPACE, "INVAL", INT2NUM(G_IO_CHANNEL_ERROR_INVAL));
    rb_define_const(RG_TARGET_NAMESPACE, "IO", INT2NUM(G_IO_CHANNEL_ERROR_IO));
    rb_define_const(RG_TARGET_NAMESPACE, "ISDIR", INT2NUM(G_IO_CHANNEL_ERROR_ISDIR));
    rb_define_const(RG_TARGET_NAMESPACE, "NOSPC", INT2NUM(G_IO_CHANNEL_ERROR_NOSPC));
    rb_define_const(RG_TARGET_NAMESPACE, "NXIO", INT2NUM(G_IO_CHANNEL_ERROR_NXIO));
    rb_define_const(RG_TARGET_NAMESPACE, "OVERFLOW", INT2NUM(G_IO_CHANNEL_ERROR_OVERFLOW));
    rb_define_const(RG_TARGET_NAMESPACE, "PIPE", INT2NUM(G_IO_CHANNEL_ERROR_PIPE));
    rb_define_const(RG_TARGET_NAMESPACE, "FAILED", INT2NUM(G_IO_CHANNEL_ERROR_FAILED));
}
