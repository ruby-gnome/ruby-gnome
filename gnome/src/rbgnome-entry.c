/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-entry.c,v 1.4 2002/10/13 14:11:42 tkubo Exp $ */
/* based on libgnomeui/gnome-entry.h */

/* Gnome::Entry widget for Ruby/GNOME2
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

#define _SELF(self) GNOME_ENTRY(RVAL2GOBJ(self))

static VALUE
entry_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE history_id;
    GtkWidget *entry;

    rb_scan_args(argc, argv, "01", &history_id);
    entry = gnome_entry_new(NIL_P(history_id) ? NULL : RVAL2CSTR(history_id));
    RBGTK_INITIALIZE(self, entry);
    return Qnil;
}

static VALUE
entry_gtk_entry(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_entry_gtk_entry(_SELF(self)));
}

static VALUE
entry_set_history_id(self, history_id)
    VALUE self, history_id;
{
    gnome_entry_set_history_id(_SELF(self), RVAL2CSTR(history_id));
    return self;
}

static VALUE
entry_get_history_id(self)
    VALUE self;
{
    const gchar *result = gnome_entry_get_history_id(_SELF(self));
    return result ? rb_str_new2(result) : Qnil;
}

static VALUE
entry_set_max_saved(self, max_saved)
    VALUE self, max_saved;
{
    gnome_entry_set_max_saved(_SELF(self), (guint)NUM2INT(max_saved));
    return self;
}

static VALUE
entry_get_max_saved(self)
    VALUE self;
{
    return UINT2NUM(gnome_entry_get_max_saved(_SELF(self)));
}

static VALUE
entry_prepend_history(self, save, text)
    VALUE self, save, text;
{
    gnome_entry_prepend_history(_SELF(self), NUM2INT(save), RVAL2CSTR(text));
    return self;
}

static VALUE
entry_append_history(self, save, text)
    VALUE self, save, text;
{
    gnome_entry_append_history(_SELF(self), NUM2INT(save), RVAL2CSTR(text));
    return self;
}

static VALUE
entry_clear_history(self)
    VALUE self;
{
    gnome_entry_clear_history(_SELF(self));
    return self;
}

void
Init_gnome_entry(mGnome)
    VALUE mGnome;
{
    VALUE gnoEntry = G_DEF_CLASS(GNOME_TYPE_ENTRY, "Entry", mGnome);

    rb_define_method(gnoEntry, "initialize", entry_initialize, -1);
    rb_define_method(gnoEntry, "gtk_entry", entry_gtk_entry, 0);
    rb_define_method(gnoEntry, "set_history_id", entry_set_history_id, 1);
    rb_define_method(gnoEntry, "history_id", entry_get_history_id, 0);
    rb_define_method(gnoEntry, "set_max_saved", entry_set_max_saved, 1);
    rb_define_method(gnoEntry, "max_saved", entry_get_max_saved, 0);
    rb_define_method(gnoEntry, "prepend_history", entry_prepend_history, 2);
    rb_define_method(gnoEntry, "append_history", entry_append_history, 2);
    rb_define_method(gnoEntry, "clear_history", entry_clear_history, 0);

    G_DEF_SETTER(gnoEntry, "history_id");
    G_DEF_SETTER(gnoEntry, "max_saved");
}
