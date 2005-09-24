/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-gconf.c,v 1.1 2005/09/24 18:02:43 mutoh Exp $ */

/* GNOME GConf module for Ruby/GNOME2
 *
 * Copyright (C) 2005 Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
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

#include "rbgnome.h"

static VALUE
gconf_get_gnome_libs_settings_relative(self, subkey)
    VALUE self, subkey;
{
    return CSTR2RVAL(gnome_gconf_get_gnome_libs_settings_relative((const gchar*)RVAL2CSTR(subkey)));
}

static VALUE
gconf_get_app_settings_relative(self, program, subkey)
    VALUE self, program, subkey;
{
    return CSTR2RVAL(gnome_gconf_get_app_settings_relative(GNOME_PROGRAM(RVAL2GOBJ(program)),
                                                           (const gchar*)RVAL2CSTR(subkey)));
}

void
Init_gnome_gconf(mGnome)
    VALUE mGnome;
{
    VALUE gconf = rb_define_module_under(mGnome, "GConf");

    rb_define_module_function(gconf, "get_gnome_libs_settings_relative", gconf_get_gnome_libs_settings_relative, 1);
    rb_define_module_function(gconf, "get_app_settings_relative", gconf_get_app_settings_relative, 2);
}
