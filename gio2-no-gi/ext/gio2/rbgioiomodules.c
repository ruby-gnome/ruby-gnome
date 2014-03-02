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

#define RG_TARGET_NAMESPACE mIOModules

static VALUE
rg_s_load_all_in_directory(G_GNUC_UNUSED VALUE self, VALUE dirname)
{
        return GLIST2ARY_FREE(g_io_modules_load_all_in_directory(RVAL2CSTR(dirname)));
}

static VALUE
rg_s_scan_all_in_directory(VALUE self, VALUE dirname)
{
        g_io_modules_scan_all_in_directory(RVAL2CSTR(dirname));

        return self;
}

void
Init_giomodules(VALUE mGio)
{
        VALUE RG_TARGET_NAMESPACE = rb_define_module_under(mGio, "IOModules");

        RG_DEF_SMETHOD(load_all_in_directory, 1);
        RG_DEF_SMETHOD(scan_all_in_directory, 1);
}
