/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-file-entry.c,v 1.6 2003/02/02 12:51:06 tkubo Exp $ */
/* based on libgnomeui/gnome-file-entry.h */

/* Gnome::FileEntry widget for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
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
fentry_set_title(self, browse_dialog_title)
    VALUE self, browse_dialog_title;
{
    gnome_file_entry_set_title(_SELF(self), RVAL2CSTR(browse_dialog_title));
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
    char *filename;
    VALUE obj;

    filename = gnome_file_entry_get_full_path(_SELF(self),
                                               RTEST(file_must_exist));
    SET_STR_AND_GFREE(obj, filename);
    return obj;
}

void
Init_gnome_file_entry(mGnome)
    VALUE mGnome;
{
    VALUE gnoFileEntry = G_DEF_CLASS(GNOME_TYPE_FILE_ENTRY, "FileEntry", mGnome);

    /* Instance methods */
    rb_define_method(gnoFileEntry, "initialize", fentry_initialize, -1);
    rb_define_method(gnoFileEntry, "set_title", fentry_set_title, 1);
    rb_define_method(gnoFileEntry, "get_full_path", fentry_get_full_path, 1);

    G_DEF_SETTER(gnoFileEntry, "title");
}

