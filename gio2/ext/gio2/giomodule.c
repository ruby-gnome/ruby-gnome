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

#define _SELF(value) G_IO_MODULE(RVAL2GOBJ(value))

static VALUE
iomodule_initialize(VALUE self, VALUE filename)
{
        G_INITIALIZE(self, g_io_module_new(RVAL2CSTR(filename)));

        return Qnil;
}

static VALUE
iomodule_load_all_in_directory(G_GNUC_UNUSED VALUE self, VALUE dirname)
{
        return GLIST2ARY_FREE(g_io_modules_load_all_in_directory(RVAL2CSTR(dirname)));
}

static VALUE
iomodule_scan_all_in_directory(VALUE self, VALUE dirname)
{
        g_io_modules_scan_all_in_directory(RVAL2CSTR(dirname));

        return self;
}

/* NOTE: No point in implementing g_io_module_query. */

void
Init_giomodule(VALUE glib)
{
        VALUE iomodule, iomodules;

        iomodule = G_DEF_CLASS(G_IO_TYPE_MODULE, "IOModule", glib);

        rb_define_method(iomodule, "initialize", iomodule_initialize, 1);

        iomodules = rb_define_module_under(glib, "IOModules");

        rb_define_module_function(iomodules, "load_all_in_directory", iomodule_load_all_in_directory, 1);
        rb_define_module_function(iomodules, "scan_all_in_directory", iomodule_scan_all_in_directory, 1);

}
