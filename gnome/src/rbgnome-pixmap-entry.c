/* $Id: rbgnome-pixmap-entry.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::PixmapEntry widget for Ruby/Gnome
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

VALUE gnoPixmapEntry;

static VALUE
pentry_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE history_id, browse_dialog_title, do_preview;
    GtkWidget* pentry;

    rb_scan_args(argc, argv, "03",
		 &history_id, &browse_dialog_title, &do_preview);
    pentry = gnome_pixmap_entry_new(NIL_P(history_id)?0:STR2CSTR(history_id),
		NIL_P(browse_dialog_title)?0:STR2CSTR(browse_dialog_title),
		RTEST(do_preview));
    set_widget(self, pentry);
    return Qnil;
}

static VALUE
pentry_set_pixmap_subdir(self, subdir)
    VALUE self, subdir;
{
    gnome_pixmap_entry_set_pixmap_subdir(
	GNOME_PIXMAP_ENTRY(get_widget(self)),
	STR2CSTR(subdir));
    return self;
}

static VALUE
pentry_gnome_file_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_pixmap_entry_gnome_file_entry(
		GNOME_PIXMAP_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
pentry_gnome_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_pixmap_entry_gnome_entry(
		GNOME_PIXMAP_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
pentry_gtk_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_pixmap_entry_gtk_entry(
		GNOME_PIXMAP_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
pentry_set_preview(self, do_preview)
    VALUE self, do_preview;
{
    gnome_pixmap_entry_set_preview(GNOME_PIXMAP_ENTRY(get_widget(self)),
				   RTEST(do_preview));
    return self;
}

static VALUE
pentry_set_preview_size(self, preview_w, preview_h)
    VALUE self, preview_w, preview_h;
{
    gnome_pixmap_entry_set_preview_size(GNOME_PIXMAP_ENTRY(get_widget(self)),
				NUM2INT(preview_w), NUM2INT(preview_h));
    return self;
}

static VALUE
pentry_get_filename(self)
    VALUE self;
{
    return CSTR2OBJ(gnome_pixmap_entry_get_filename(
                GNOME_PIXMAP_ENTRY(get_widget(self))));
}

void
Init_gnome_pixmap_entry()
{
    gnoPixmapEntry = rb_define_class_under(mGnome, "PixmapEntry", gVBox);

    /* Instance methods */
    rb_define_method(gnoPixmapEntry, "initialize", pentry_initialize, -1);
    rb_define_method(gnoPixmapEntry, "set_pixmap_subdir",
		     pentry_set_pixmap_subdir, 1);
    rb_define_method(gnoPixmapEntry, "gnome_file_entry",
		     pentry_gnome_file_entry, 0);
    rb_define_method(gnoPixmapEntry, "gnome_entry", pentry_gnome_entry, 0);
    rb_define_method(gnoPixmapEntry, "gtk_entry", pentry_gtk_entry, 0);
    rb_define_method(gnoPixmapEntry, "set_preview", pentry_set_preview, 1);
    rb_define_method(gnoPixmapEntry, "set_preview_szie",
		     pentry_set_preview_size, 2);
    rb_define_method(gnoPixmapEntry, "get_filename", pentry_get_filename, 0);
}
