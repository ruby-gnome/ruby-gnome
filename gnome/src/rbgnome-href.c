/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-href.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::HRef widget for Ruby-Gnome
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
    return rb_str_new2(gnome_href_get_url(_SELF(self)));
}

static VALUE
href_set_label(self, label)
    VALUE self, label;
{
    gnome_href_set_label(_SELF(self), RVAL2CSTR(label));
    return self;
}

static VALUE
href_get_label(self)
    VALUE self;
{
    return rb_str_new2(gnome_href_get_label(_SELF(self)));
}

void
Init_gnome_href(mGnome)
    VALUE mGnome;
{
    VALUE gnoHRef = G_DEF_CLASS(GNOME_TYPE_HREF, "HRef", mGnome);

    rb_define_method(gnoHRef, "initialize", href_initialize, -1);
    rb_define_method(gnoHRef, "set_url", href_set_url, 1);
    rb_define_method(gnoHRef, "get_url", href_get_url, 0);
    rb_define_method(gnoHRef, "set_label", href_set_label, 1);
    rb_define_method(gnoHRef, "get_label", href_get_label, 0);
}

