/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2013  Ruby-GNOME2 Project Team
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
#include "rbglib.h"

#include <libintl.h>

#define RG_TARGET_NAMESPACE mGetText

static VALUE
rg_s_bindtextdomain(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE domain_name;
    VALUE directory;
    const char *current_directory = NULL;

    rb_scan_args(argc, argv, "11", &domain_name, &directory);
    current_directory = bindtextdomain(RVAL2CSTR(domain_name),
                                       RVAL2CSTR_ACCEPT_NIL(directory));
    return CSTR2RVAL(current_directory);
}

void
Init_glib_gettext(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_module_under(mGLib, "GetText");
    RG_DEF_SMETHOD(bindtextdomain, -1);
}
