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
#include <gio/gfiledescriptorbased.h>

#define RG_TARGET_NAMESPACE mFileDescriptorBased
#define _SELF(value) G_FILE_DESCRIPTOR_BASED(RVAL2GOBJ(value))

static VALUE
rg_fd(VALUE self)
{
        return FD2RVAL(g_file_descriptor_based_get_fd(_SELF(self)));
}
#endif

void
Init_gfiledescriptorbased(G_GNUC_UNUSED VALUE mGio)
{
#ifdef HAVE_GIO_UNIX
        VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(G_TYPE_FILE_DESCRIPTOR_BASED, "FileDescriptorBased", mGio);

        RG_DEF_METHOD(fd, 0);
#endif
}
