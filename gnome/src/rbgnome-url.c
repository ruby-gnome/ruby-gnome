/* $Id: rbgnome-url.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::URL module for Ruby/Gnome
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "rbgnome.h"

VALUE mGnomeURL;

static VALUE
url_show(self, url)
    VALUE self, url;
{
    gnome_url_show(STR2CSTR(url));
    return Qnil;
}

void
Init_gnome_url()
{
    mGnomeURL = rb_define_module_under(mGnome, "URL");
    rb_define_module_function(mGnomeURL, "show", url_show, 1);
}
