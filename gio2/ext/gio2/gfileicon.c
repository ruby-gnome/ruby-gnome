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

#define _SELF(value) G_FILE_ICON(RVAL2GOBJ(value))

static VALUE
fileicon_initialize(VALUE self, VALUE file)
{
        G_INITIALIZE(self, g_file_icon_new(RVAL2GFILE(file)));

        return Qnil;
}

void
Init_gfileicon(VALUE glib)
{
        VALUE fileicon = G_DEF_CLASS(G_TYPE_FILE_ICON, "FileIcon", glib);

        rb_define_method(fileicon, "initialize", fileicon_initialize, 1);
}
