/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-file-entry.c,v 1.4 2002/10/13 14:11:42 tkubo Exp $ */
/* based on libgnomeui/gnome-file-entry.h */

/* Gnome::FileEntry widget for Ruby/GNOME2
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

#define _SELF(self) GNOME_FILE_ENTRY(RVAL2GOBJ(self))

static VALUE
fentry_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE history_id, title;
    GtkWidget* fentry;

    rb_scan_args(argc, argv, "02", &history_id, &title);
    fentry = gnome_file_entry_new(NIL_P(history_id)?0:RVAL2CSTR(history_id),
                                  NIL_P(title)?0:RVAL2CSTR(title));

    RBGTK_INITIALIZE(self, fentry);
    return Qnil;
}

static VALUE
fentry_gnome_entry(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_file_entry_gnome_entry(_SELF(self)));
}

static VALUE
fentry_gtk_entry(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_file_entry_gtk_entry(_SELF(self)));
}

static VALUE
fentry_set_title(self, browse_dialog_title)
    VALUE self, browse_dialog_title;
{
    gnome_file_entry_set_title(_SELF(self), RVAL2CSTR(browse_dialog_title));
    return self;
}

/*set default path for the browse dialog*/
static VALUE
fentry_set_default_path(self, path)
    VALUE self, path;
{
    gnome_file_entry_set_default_path(_SELF(self), RVAL2CSTR(path));
    return self;
}

/*sets up the file entry to be a directory picker rather then a file picker*/
static VALUE
fentry_set_directory_entry(self, directory_entry)
    VALUE self, directory_entry;
{
    gnome_file_entry_set_directory_entry(_SELF(self), RTEST(directory_entry));
    return self;
}

static VALUE
fentry_get_directory_entry(self)
    VALUE self;
{
    return gnome_file_entry_get_directory_entry(_SELF(self)) ? Qtrue : Qfalse;
}

/*returns a filename which is a full path with WD or the default
  directory prepended if it's not an absolute path, returns
  NULL on empty entry or if the file doesn't exist and that was
  a requirement*/
static VALUE
fentry_get_full_path(self, file_must_exist)
    VALUE self, file_must_exist;
{
    char *filename;
    VALUE obj;

    filename = gnome_file_entry_get_full_path(_SELF(self),
                                               RTEST(file_must_exist));
    SET_STR_AND_GFREE(obj, filename);
    return obj;
}

/* set the filename to something, this is like setting the internal
 * GtkEntry */
static VALUE
fentry_set_filename(self, filename)
    VALUE self, filename;
{
    gnome_file_entry_set_filename(_SELF(self), RVAL2CSTR(filename));
    return self;
}

/*set modality of the file browse dialog, only applies for the
  next time a dialog is created*/
static VALUE
fentry_set_modal(self, is_modal)
    VALUE self, is_modal;
{
    gnome_file_entry_set_modal(_SELF(self),
                               RTEST(is_modal));
    return self;
}

static VALUE
fentry_get_modal(self)
    VALUE self;
{
    return gnome_file_entry_get_modal(_SELF(self)) ? Qtrue : Qfalse;
}

void
Init_gnome_file_entry(mGnome)
    VALUE mGnome;
{
    VALUE gnoFileEntry = G_DEF_CLASS(GNOME_TYPE_FILE_ENTRY, "FileEntry", mGnome);

    /* Instance methods */
    rb_define_method(gnoFileEntry, "initialize", fentry_initialize, -1);
    rb_define_method(gnoFileEntry, "gnome_entry", fentry_gnome_entry, 0);
    rb_define_method(gnoFileEntry, "gtk_entry", fentry_gtk_entry, 0);
    rb_define_method(gnoFileEntry, "set_title", fentry_set_title, 1);
    rb_define_method(gnoFileEntry, "set_default_path", fentry_set_default_path, 1);
    rb_define_method(gnoFileEntry, "set_directory_entry", fentry_set_directory_entry, 1);
    rb_define_method(gnoFileEntry, "directory_entry?", fentry_get_directory_entry, 0);
    rb_define_method(gnoFileEntry, "get_full_path", fentry_get_full_path, 1);
    rb_define_method(gnoFileEntry, "set_filename", fentry_set_filename, 1);
    rb_define_method(gnoFileEntry, "set_modal", fentry_set_modal, 1);
    rb_define_method(gnoFileEntry, "modal?", fentry_get_modal, 0);

    G_DEF_SETTER(gnoFileEntry, "title");
    G_DEF_SETTER(gnoFileEntry, "default_path");
    G_DEF_SETTER(gnoFileEntry, "directory_entry");
    G_DEF_SETTER(gnoFileEntry, "filename");
    G_DEF_SETTER(gnoFileEntry, "modal");
}

