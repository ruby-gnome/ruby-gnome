/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-href.c,v 1.4 2002/10/14 13:56:22 tkubo Exp $ */
/* based on libgnomeui/gnome-href.h */

/* Gnome::HRef widget for Ruby/GNOME2
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *               2002 KUBO Takehiro <kubo@jiubao.org>
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

static VALUE
href_set_url(self, url)
    VALUE self, url;
{
    gnome_href_set_url(_SELF(self), RVAL2CSTR(url));
    return self;
}

static VALUE
href_get_url(self)
    VALUE self;
{
    const gchar *result = gnome_href_get_url(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
href_set_text(self, text)
    VALUE self, text;
{
    gnome_href_set_text(_SELF(self), RVAL2CSTR(text));
    return self;
}

static VALUE
href_get_text(self)
    VALUE self;
{
    const gchar *result = gnome_href_get_text(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

void
Init_gnome_href(mGnome)
    VALUE mGnome;
{
    VALUE gnoHRef = G_DEF_CLASS(GNOME_TYPE_HREF, "HRef", mGnome);

    rb_define_method(gnoHRef, "initialize", href_initialize, -1);
    rb_define_method(gnoHRef, "set_url", href_set_url, 1);
    rb_define_method(gnoHRef, "url", href_get_url, 0);
    rb_define_method(gnoHRef, "set_text", href_set_text, 1);
    rb_define_method(gnoHRef, "text", href_get_text, 0);

    G_DEF_SETTER(gnoHRef, "url");
    G_DEF_SETTER(gnoHRef, "text");
}

