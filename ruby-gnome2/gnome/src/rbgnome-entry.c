/* $Id: rbgnome-entry.c,v 1.1 2002/05/19 12:29:54 mutoh Exp $ */

/* Gnome::Entry widget for Ruby/Gnome
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

VALUE gnoEntry;

static VALUE
entry_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE history_id;
    GtkWidget *entry;

    rb_scan_args(argc, argv, "01", &history_id);
    entry = gnome_entry_new(NIL_P(history_id)?0:STR2CSTR(history_id));
    set_widget(self, entry);
    return Qnil;
}

static VALUE
entry_gtk_entry(self)
    VALUE self;
{
    GtkWidget *entry;
    entry = gnome_entry_gtk_entry(GNOME_ENTRY(get_widget(self)));
    return make_gnobject_auto_type(GTK_OBJECT(entry));
}

static VALUE
entry_set_history_id(self, history_id)
    VALUE self, history_id;
{
    gnome_entry_set_history_id(GNOME_ENTRY(get_widget(self)),
			       STR2CSTR(history_id));
    return self;
}

static VALUE
entry_set_max_saved(self, max_saved)
    VALUE self, max_saved;
{
    gnome_entry_set_max_saved(GNOME_ENTRY(get_widget(self)),
			      (guint)NUM2INT(max_saved));
    return self;
}

static VALUE
entry_prepend_history(self, save, text)
    VALUE self, save, text;
{
    gnome_entry_prepend_history(GNOME_ENTRY(get_widget(self)),
				NUM2INT(save),
				STR2CSTR(text));
    return self;
}

static VALUE
entry_append_history(self, save, text)
    VALUE self, save, text;
{
    gnome_entry_append_history(GNOME_ENTRY(get_widget(self)),
				NUM2INT(save),
				STR2CSTR(text));
    return self;
}

static VALUE
entry_load_history(self)
    VALUE self;
{
    gnome_entry_load_history(GNOME_ENTRY(get_widget(self)));
    return self;
}

static VALUE
entry_save_history(self)
    VALUE self;
{
    gnome_entry_save_history(GNOME_ENTRY(get_widget(self)));
    return self;
}

void
Init_gnome_entry()
{
    gnoEntry = rb_define_class_under(mGnome, "Entry", gCombo);

    rb_define_method(gnoEntry, "initialize", entry_initialize, -1);
    rb_define_method(gnoEntry, "gtk_entry", entry_gtk_entry, 0);
    rb_define_method(gnoEntry, "set_history_id", entry_set_history_id, 1);
    rb_define_method(gnoEntry, "set_max_saved", entry_set_max_saved, 1);
    rb_define_method(gnoEntry, "prepend_history", entry_prepend_history, 2);
    rb_define_method(gnoEntry, "append_history", entry_append_history, 2);
    rb_define_method(gnoEntry, "load_history", entry_load_history, 0);
    rb_define_method(gnoEntry, "save_history", entry_save_history, 0);
}
