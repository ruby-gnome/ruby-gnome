/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-href.c,v 1.7 2005/09/25 17:53:07 mutoh Exp $ */
/* based on libgnomeui/gnome-href.h */

/* Gnome::HRef widget for Ruby/GNOME2
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
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

#define _SELF(self) GNOME_HREF(RVAL2GOBJ(self))

/*
 * Class Gnome::HRef
 *
 * Description:
 *   A clickable URL button.
 *
 * Hierarchy:
 *
 *   Object
 *     +----Gtk::Object
 *            +----Gtk::Widget
 *                   +----Gtk::Container
 *                          +----Gtk::Bin
 *                                 +----Gtk::Button
 *                                        +----Gnome::HRef
 */

static VALUE
href_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE url, label;
    GtkWidget *href;

    rb_scan_args(argc, argv, "02", &url, &label);
    href = gnome_href_new(NIL_P(url)?0:RVAL2CSTR(url),
			  NIL_P(label)?0:RVAL2CSTR(label));
    RBGTK_INITIALIZE(self, href);
    return Qnil;
}

/* Defined as properties
void        gnome_href_set_url              (GnomeHRef *href,
                                             const gchar *url);
const gchar* gnome_href_get_url             (GnomeHRef *href);
void        gnome_href_set_text             (GnomeHRef *href,
                                             const gchar *text);
const gchar* gnome_href_get_text            (GnomeHRef *href);
*/

/* Deprecated.
void        gnome_href_set_label            (GnomeHRef *href,
                                             const gchar *label);
const gchar* gnome_href_get_label           (GnomeHRef *href);
*/

void
Init_gnome_href(mGnome)
    VALUE mGnome;
{
    VALUE gnoHRef = G_DEF_CLASS(GNOME_TYPE_HREF, "HRef", mGnome);

    rb_define_method(gnoHRef, "initialize", href_initialize, -1);
}

