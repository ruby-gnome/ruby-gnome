/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-url.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

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

static VALUE
url_show(self, url)
    VALUE self, url;
{
    GError *error = NULL;
    VALUE exc;

    if (gnome_url_show(RVAL2CSTR(url), &error)) {
        /* success */
        return Qnil;
    }
    /* fail */
    if (error && error->message != NULL) {
        exc = rb_exc_new2(rb_eRuntimeError, error->message);
    } else {
        exc = rb_exc_new2(rb_eRuntimeError, "url_show error");
    }
    if (error)
        g_error_free(error);
    rb_exc_raise(exc);
}

void
Init_gnome_url(mGnome)
     VALUE mGnome;
{
    VALUE mGnomeURL = rb_define_module_under(mGnome, "URL");
    rb_define_module_function(mGnomeURL, "show", url_show, 1);
}
