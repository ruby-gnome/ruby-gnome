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

#ifdef HAVE_GIO_UNIX
#include <gio/gfiledescriptorbased.h>

#define _SELF(value) G_FILE_DESCRIPTOR_BASED(RVAL2GOBJ(value))

static VALUE
filedescriptorbased_get_fd(VALUE self)
{
        return FD2RVAL(g_file_descriptor_based_get_fd(_SELF(self)));
}
#endif

void
Init_gfiledescriptorbased(G_GNUC_UNUSED VALUE glib)
{
#ifdef HAVE_GIO_UNIX
        VALUE filedescriptorbased = G_DEF_INTERFACE(G_TYPE_FILE_DESCRIPTOR_BASED, "FileDescriptorBased", glib);

        rb_define_method(filedescriptorbased, "fd", filedescriptorbased_get_fd, 0);
#endif
}
