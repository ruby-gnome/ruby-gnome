/* -*- c-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-help.c,v 1.3 2002/11/01 15:25:40 tkubo Exp $ */
/* based on libgnome/gnome-help.h */

/* Gnome::Help for Ruby/GNOME2
 * Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>
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
gnohelp_m_display(self, file_name, link_id)
    VALUE self, file_name, link_id;
{
    GError *error = NULL;
    gboolean result = gnome_help_display(RVAL2CSTR(file_name),
                                         NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                         &error);
    if (!result)
        RAISE_GERROR(error);
    return self;
}

static VALUE
gnohelp_m_display_with_doc_id(self, pgm, doc_id, file_name, link_id)
    VALUE self, pgm, doc_id, file_name, link_id;
{
    GError *error = NULL;
    gboolean result = gnome_help_display_with_doc_id(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                                     NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                                     RVAL2CSTR(file_name),
                                                     NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                                     &error);
    if (!result)
        RAISE_GERROR(error);
    return self;
}

static VALUE
gnohelp_m_display_desktop(self, pgm, doc_id, file_name, link_id)
    VALUE self, pgm, doc_id, file_name, link_id;
{
    GError *error = NULL;
    gboolean result = gnome_help_display_desktop(NIL_P(pgm) ? NULL : GNOME_PROGRAM(RVAL2GOBJ(pgm)),
                                                 NIL_P(doc_id) ? NULL : RVAL2CSTR(doc_id),
                                                 RVAL2CSTR(file_name),
                                                 NIL_P(link_id) ? NULL : RVAL2CSTR(link_id),
                                                 &error);
    if (!result)
        RAISE_GERROR(error);
    return self;
}

#if 0 /* use Gnome::URL.show() instead */
gboolean gnome_help_display_uri (const char *help_uri,
                                 GError     **error);
#endif

void
Init_gnome_help(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeHelp = rb_define_module_under(mGnome, "Help");

    rb_define_module_function(mGnomeHelp, "display", gnohelp_m_display, 2);
    rb_define_module_function(mGnomeHelp, "display_with_doc_id", gnohelp_m_display_with_doc_id, 4);
    rb_define_module_function(mGnomeHelp, "display_desktop", gnohelp_m_display_desktop, 4);
}
