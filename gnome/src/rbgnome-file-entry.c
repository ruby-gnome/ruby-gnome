/* $Id: rbgnome-file-entry.c,v 1.1 2002/05/19 12:39:28 mutoh Exp $ */

/* Gnome::FileEntry widget for Ruby/Gnome
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

VALUE gnoFileEntry;

static VALUE
fentry_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE history_id, title;
    GtkWidget* fentry;

    rb_scan_args(argc, argv, "02", &history_id, &title);
    fentry = gnome_file_entry_new(NIL_P(history_id)?0:STR2CSTR(history_id),
                                  NIL_P(title)?0:STR2CSTR(title));

    set_widget(self, fentry);
    return Qnil;
}

static VALUE
fentry_gnome_entry(self)
    VALUE self;
{
    GtkWidget *entry = gnome_file_entry_gnome_entry(GNOME_FILE_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
fentry_gtk_entry(self)
    VALUE self;
{
    GtkWidget *entry = gnome_file_entry_gtk_entry(GNOME_FILE_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
fentry_set_title(self, browse_dialog_title)
    VALUE self, browse_dialog_title;
{
    gnome_file_entry_set_title(GNOME_FILE_ENTRY(get_widget(self)),
                               STR2CSTR(browse_dialog_title));
    return self;
}

/*set default path for the browse dialog*/
static VALUE
fentry_set_default_path(self, path)
    VALUE self, path;
{
    gnome_file_entry_set_default_path(GNOME_FILE_ENTRY(get_widget(self)),
                      STR2CSTR(path));
    return self;
}

/*sets up the file entry to be a directory picker rather then a file picker*/
static VALUE
fentry_set_directory(self, directory_entry)
    VALUE self, directory_entry;
{
    gnome_file_entry_set_directory(GNOME_FILE_ENTRY(get_widget(self)),
                                   RTEST(directory_entry));
    return self;
}

/*returns a filename which is a full path with WD or the default
  directory prepended if it's not an absolute path, returns
  NULL on empty entry or if the file doesn't exist and that was
  a requirement*/
static VALUE
fentry_get_full_path(self, file_must_exist)
    VALUE self, file_must_exist;
{
    return CSTR2OBJ(gnome_file_entry_get_full_path(GNOME_FILE_ENTRY(get_widget(self)),
                                                   RTEST(file_must_exist)));
}

/*set modality of the file browse dialog, only applies for the
  next time a dialog is created*/
static VALUE
fentry_set_modal(self, is_modal)
    VALUE self, is_modal;
{
    gnome_file_entry_set_modal(GNOME_FILE_ENTRY(get_widget(self)),
                               RTEST(is_modal));
    return self;
}

void
Init_gnome_file_entry()
{
    gnoFileEntry = rb_define_class_under(mGnome, "FileEntry", gHBox);

    /* Instance methods */
    rb_define_method(gnoFileEntry, "initialize", fentry_initialize, -1);
    rb_define_method(gnoFileEntry, "gnome_entry", fentry_gnome_entry, 0);
    rb_define_method(gnoFileEntry, "gtk_entry", fentry_gtk_entry, 0);
    rb_define_method(gnoFileEntry, "set_title", fentry_set_title, 1);
    rb_define_method(gnoFileEntry, "set_default_path",
                     fentry_set_default_path, 1);
    rb_define_method(gnoFileEntry, "set_directory", fentry_set_directory, 1);
    rb_define_method(gnoFileEntry, "get_full_path", fentry_get_full_path, 1);
    rb_define_method(gnoFileEntry, "set_modal", fentry_set_modal, 1);

    /* Signals */
    rb_define_const(gnoFileEntry, "SIGNAL_BROWSE_CLICKED",
                    rb_str_new2("browse_clicked"));
}

