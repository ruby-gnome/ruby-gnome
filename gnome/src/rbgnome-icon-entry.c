/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-icon-entry.c,v 1.4 2002/10/14 13:56:22 tkubo Exp $ */
/* based on libgnomeui/gnome-icon-entry.h */

/* Gnome::IconEntry widget for Ruby/GNOME2
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

#define _SELF(self) GNOME_ICON_ENTRY(RVAL2GOBJ(self))

static VALUE
ientry_initialize(self, history_id, browse_dialog_title)
    VALUE self, history_id, browse_dialog_title;
{
    GtkWidget *icon_entry;
    icon_entry = gnome_icon_entry_new(RVAL2CSTR(history_id),
                                      RVAL2CSTR(browse_dialog_title));
    RBGTK_INITIALIZE(self, icon_entry);
    return Qnil;
}

static VALUE
ientry_set_pixmap_subdir(self, subdir)
    VALUE self, subdir;
{
    gnome_icon_entry_set_pixmap_subdir(_SELF(self), RVAL2CSTR(subdir));
    return self;
}

static VALUE
ientry_get_filename(self)
    VALUE self;
{
    gchar *result = gnome_icon_entry_get_filename(_SELF(self));
    VALUE obj;
    SET_STR_AND_GFREE(obj, result);
    return obj;
}

static VALUE
ientry_set_filename(self, filename)
    VALUE self, filename;
{
    gboolean result = gnome_icon_entry_set_filename(_SELF(self), RVAL2CSTR(filename));
    if (!result)
        rb_raise(rb_eRuntimeError, "cannot load icon %s", RVAL2CSTR(filename));
    return self;
}

static VALUE
ientry_set_browse_dialog_title(self, browse_dialog_title)
    VALUE self, browse_dialog_title;
{
    gnome_icon_entry_set_browse_dialog_title(_SELF(self), RVAL2CSTR(browse_dialog_title));
    return self;
}

static VALUE
ientry_set_history_id(self, history_id)
    VALUE self, history_id;
{
    gnome_icon_entry_set_history_id(_SELF(self), RVAL2CSTR(history_id));
    return self;
}

static VALUE
ientry_pick_dialog(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_icon_entry_pick_dialog(_SELF(self)));
}

void
Init_gnome_icon_entry(mGnome)
    VALUE mGnome;
{
    VALUE gnoIconEntry = G_DEF_CLASS(GNOME_TYPE_ICON_ENTRY, "IconEntry", mGnome);

    /* Instance methods */
    rb_define_method(gnoIconEntry, "initialize", ientry_initialize, 2);
    rb_define_method(gnoIconEntry, "set_pixmap_subdir", ientry_set_pixmap_subdir, 1);
    rb_define_method(gnoIconEntry, "filename", ientry_get_filename, 0);
    rb_define_method(gnoIconEntry, "set_filename", ientry_set_filename, 0);
    rb_define_method(gnoIconEntry, "set_browse_dialog_title", ientry_set_browse_dialog_title, 1);
    rb_define_method(gnoIconEntry, "set_history_id", ientry_set_history_id, 1);
    rb_define_method(gnoIconEntry, "pick_dialog", ientry_pick_dialog, 0);

    G_DEF_SETTER(gnoIconEntry, "pixmap_subdir");
    G_DEF_SETTER(gnoIconEntry, "filename");
    G_DEF_SETTER(gnoIconEntry, "browse_dialog_title");
    G_DEF_SETTER(gnoIconEntry, "history_id");
}
