/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-icon-entry.c,v 1.5 2002/12/26 15:13:44 mutoh Exp $ */
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

void
Init_gnome_icon_entry(mGnome)
    VALUE mGnome;
{
    VALUE gnoIconEntry = G_DEF_CLASS(GNOME_TYPE_ICON_ENTRY, "IconEntry", mGnome);

    /* Instance methods */
    rb_define_method(gnoIconEntry, "initialize", ientry_initialize, 2);
}
