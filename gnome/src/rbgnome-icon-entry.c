/* $Id: rbgnome-icon-entry.c,v 1.1 2002/05/19 12:39:27 mutoh Exp $ */

/* Gnome::IconEntry widget for Ruby/Gnome
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

VALUE gnoIconEntry;

static VALUE
ientry_initialize(self, history_id, browse_dialog_title)
    VALUE self, history_id, browse_dialog_title;
{
    GtkWidget *icon_entry;
    icon_entry = gnome_icon_entry_new(STR2CSTR(history_id),
				      STR2CSTR(browse_dialog_title));
    set_widget(self, icon_entry);
    return Qnil;
}

static VALUE
ientry_set_pixmap_subdir(self, subdir)
    VALUE self, subdir;
{
    gnome_icon_entry_set_pixmap_subdir(GNOME_ICON_ENTRY(get_widget(self)),
				       STR2CSTR(subdir));
    return self;
}

static VALUE
ientry_set_icon(self, filename)
    VALUE self, filename;
{
    gnome_icon_entry_set_icon(GNOME_ICON_ENTRY(get_widget(self)),
			      STR2CSTR(filename));
    return self;
}

static VALUE
ientry_gnome_file_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_icon_entry_gnome_file_entry(
		GNOME_ICON_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
ientry_gnome_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_icon_entry_gnome_entry(
		GNOME_ICON_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
ientry_gtk_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_icon_entry_gtk_entry(
		GNOME_ICON_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
ientry_get_filename(self)
    VALUE self;
{
    char *filename;
    filename = gnome_icon_entry_get_filename(
		   GNOME_ICON_ENTRY(get_widget(self)));
    return CSTR2OBJ(filename);
}

void
Init_gnome_icon_entry()
{
    gnoIconEntry = rb_define_class_under(mGnome, "IconEntry", gVBox);

    /* Instance methods */
    rb_define_method(gnoIconEntry, "initialize", ientry_initialize, 2);
    rb_define_method(gnoIconEntry, "set_pixmap_subdir",
		     ientry_set_pixmap_subdir, 1);
    rb_define_method(gnoIconEntry, "set_icon", ientry_set_icon, 1);
    rb_define_method(gnoIconEntry, "gnome_file_entry",
		     ientry_gnome_file_entry, 0);
    rb_define_method(gnoIconEntry, "gnome_entry", ientry_gnome_entry, 0);
    rb_define_method(gnoIconEntry, "gtk_entry", ientry_gtk_entry, 0);
    rb_define_method(gnoIconEntry, "get_filename", ientry_get_filename, 0);
}
