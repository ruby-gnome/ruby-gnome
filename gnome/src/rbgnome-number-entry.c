/* $Id: rbgnome-number-entry.c,v 1.1 2002/05/19 12:39:26 mutoh Exp $ */

/* Gnome::NumberEntry widget for Ruby/Gnome
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

VALUE gnoNumberEntry;

static VALUE
nentry_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE history_id, calc_dialog_title;
    GtkWidget* nentry;

    rb_scan_args(argc, argv, "02", &history_id, &calc_dialog_title);
    nentry = gnome_number_entry_new(NIL_P(history_id)?0:STR2CSTR(history_id),
		NIL_P(calc_dialog_title)?0:STR2CSTR(calc_dialog_title));

    set_widget(self, nentry);
    return Qnil;
}

static VALUE
nentry_gnome_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_number_entry_gnome_entry(
		GNOME_NUMBER_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
nentry_gtk_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_number_entry_gtk_entry(
		GNOME_NUMBER_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
nentry_set_title(self, calc_dialog_title)
    VALUE self, calc_dialog_title;
{
    gnome_number_entry_set_title(
		GNOME_NUMBER_ENTRY(get_widget(self)),
		STR2CSTR(calc_dialog_title));
    return self;
}

static VALUE
nentry_get_number(self)
    VALUE self;
{
    gdouble number;
    number = gnome_number_entry_get_number(
		GNOME_NUMBER_ENTRY(get_widget(self)));
    return rb_float_new(number);
}

void
Init_gnome_number_entry()
{
    gnoNumberEntry = rb_define_class_under(mGnome, "NumberEntry", gHBox);

    /* Instance methods */
    rb_define_method(gnoNumberEntry, "initialize", nentry_initialize, -1);
    rb_define_method(gnoNumberEntry, "gnome_entry", nentry_gnome_entry, 0);
    rb_define_method(gnoNumberEntry, "gtk_entry", nentry_gtk_entry, 0);
    rb_define_method(gnoNumberEntry, "set_title", nentry_set_title, 1);
    rb_define_method(gnoNumberEntry, "get_number", nentry_get_number, 0);
}

