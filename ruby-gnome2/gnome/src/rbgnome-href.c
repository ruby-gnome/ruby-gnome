/* $Id: rbgnome-href.c,v 1.1 2002/05/19 12:29:54 mutoh Exp $ */

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

VALUE gnoHRef;

static VALUE
href_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE url, label;
    GtkWidget *href;

    rb_scan_args(argc, argv, "02", &url, &label);
    href = gnome_href_new(NIL_P(url)?0:STR2CSTR(url),
			  NIL_P(label)?0:STR2CSTR(label));
    set_widget(self, href);
    return Qnil;
}

static VALUE
href_set_url(self, url)
    VALUE self, url;
{
    gnome_href_set_url(GNOME_HREF(get_widget(self)), STR2CSTR(url));
    return self;
}

static VALUE
href_get_url(self)
    VALUE self;
{
    return rb_str_new2(gnome_href_get_url(GNOME_HREF(get_widget(self))));
}

static VALUE
href_set_label(self, label)
    VALUE self, label;
{
    gnome_href_set_label(GNOME_HREF(get_widget(self)), STR2CSTR(label));
    return self;
}

static VALUE
href_get_label(self)
    VALUE self;
{
    return rb_str_new2(gnome_href_get_label(GNOME_HREF(get_widget(self))));
}

void
Init_gnome_href()
{
    gnoHRef = rb_define_class_under(mGnome, "HRef", gButton);

    rb_define_method(gnoHRef, "initialize", href_initialize, -1);
    rb_define_method(gnoHRef, "set_url", href_set_url, 1);
    rb_define_method(gnoHRef, "get_url", href_get_url, 0);
    rb_define_method(gnoHRef, "set_label", href_set_label, 1);
    rb_define_method(gnoHRef, "get_label", href_get_label, 0);
}

