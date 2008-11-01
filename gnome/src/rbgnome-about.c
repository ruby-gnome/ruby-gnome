/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-about.c,v 1.6 2003/02/02 12:51:05 tkubo Exp $ */
/* based on libgnomeui/gnome-about.h */

/* Gnome::About widget for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
 * Copyright (C) 1999      Minoru Inachi <inachi@earth.interq.or.jp>
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
about_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE name, version, copyright, comments, authors, documenters;
    VALUE translator_credits, logo_pixbuf;
    const gchar **c_authors;
    const gchar **c_documenters;
    int i;
    GtkWidget* result;

    rb_scan_args(argc, argv, "71",
                 &name, &version, &copyright, &comments, &authors,
                 &documenters, &translator_credits, &logo_pixbuf);

    Check_Type(authors, T_ARRAY);
    c_authors = ALLOCA_N(const gchar*, RARRAY_LEN(authors)+1);
    for (i=0; i<RARRAY_LEN(authors); i++) {
        c_authors[i] = RVAL2CSTR(RARRAY_PTR(authors)[i]);
    }
    c_authors[i] = NULL; /* we need to terminate the array with a NULL. */

    Check_Type(documenters, T_ARRAY);
    c_documenters = ALLOCA_N(const gchar*, RARRAY_LEN(documenters)+1);
    for (i=0; i<RARRAY_LEN(documenters); i++) {
        c_documenters[i] = RVAL2CSTR(RARRAY_PTR(documenters)[i]);
    }
    c_documenters[i] = NULL; /* we need to terminate the array with a NULL. */

    result = gnome_about_new(RVAL2CSTR(name),
                             RVAL2CSTR(version),
                             RVAL2CSTR(copyright),
                             RVAL2CSTR(comments),
                             c_authors,
                             c_documenters,
                             NIL_P(translator_credits) ? NULL : RVAL2CSTR(translator_credits),
                             NIL_P(logo_pixbuf) ? NULL : GDK_PIXBUF(RVAL2GOBJ(logo_pixbuf)));
    RBGTK_INITIALIZE(self, result);
    return Qnil;
}

void
Init_gnome_about(mGnome)
    VALUE mGnome;
{
    VALUE gnoAbout = G_DEF_CLASS(GNOME_TYPE_ABOUT, "About", mGnome);

    /*
     * instance methods
     */
    rb_define_method(gnoAbout, "initialize", about_initialize, -1);
}
