/* $Id: rbgnome-about.c,v 1.1 2002/05/19 12:39:26 mutoh Exp $ */

/* Gnome::About widget for Ruby/Gnome
 * Copyright (C) 1999 Minoru Inachi <inachi@earth.interq.or.jp>
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

VALUE gnoAbout;

static VALUE
about_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE title, version, copyright, authors, comments, logo;
    gchar *c_logo;
    const gchar **c_authors;
    int i;
    GtkWidget* result;

    rb_scan_args(argc, argv, "51",
		 &title, &version, &copyright, &authors, &comments, &logo);

    Check_Type(authors, T_ARRAY);
    c_authors = ALLOCA_N(const gchar*, RARRAY(authors)->len+1);
    for (i=0; i<RARRAY(authors)->len; i++) {
        c_authors[i] = STR2CSTR(RARRAY(authors)->ptr[i]);
    }
    c_authors[i] = NULL; // we need to terminate the array with a NULL

    // this is optional; if not specified, it's NULL
    c_logo = NIL_P(logo)?NULL:STR2CSTR(logo);

    result = gnome_about_new(STR2CSTR(title),
                             STR2CSTR(version),
                             STR2CSTR(copyright),
                             c_authors,
                             STR2CSTR(comments),
                             c_logo);
    set_widget(self, result);

    return Qnil;
}

void
Init_gnome_about()
{
    gnoAbout = rb_define_class_under(mGnome, "About", gnoDialog);

    /*
     * instance methods
     */
    rb_define_method(gnoAbout, "initialize", about_initialize, -1);
}

