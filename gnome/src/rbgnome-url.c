/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-url.c,v 1.5 2004/08/26 17:47:45 mutoh Exp $ */

/* Gnome::URL module for Ruby/Gnome
 * Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
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
url_show(self, url)
    VALUE self, url;
{
    GError *error = NULL;

    if (! gnome_url_show(RVAL2CSTR(url), &error)) {
        RAISE_GERROR(error);
    }
    return Qnil;
}

void
Init_gnome_url(mGnome)
     VALUE mGnome;
{
    VALUE mGnomeURL = rb_define_module_under(mGnome, "URL");
    rb_define_module_function(mGnomeURL, "show", url_show, 1);

    G_DEF_ERROR(GNOME_URL_ERROR, "URLError", mGnome, rb_eRuntimeError,
                GNOME_TYPE_URL_ERROR);

}
