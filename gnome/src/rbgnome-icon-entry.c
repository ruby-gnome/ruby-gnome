/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-icon-entry.c,v 1.7 2005/09/25 17:53:07 mutoh Exp $ */
/* based on libgnomeui/gnome-icon-entry.h */

/* Gnome::IconEntry widget for Ruby/GNOME2
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
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

#define _SELF(self) GNOME_ICON_ENTRY(RVAL2GOBJ(self))

/* Don't need this.
void        gnome_icon_entry_construct      (GnomeIconEntry *ientry,
                                             const gchar *history_id,
                                             const gchar *browse_dialog_title);
*/

/* Defined as properties.
void        gnome_icon_entry_set_pixmap_subdir
                                            (GnomeIconEntry *ientry,
                                             const gchar *subdir);
gchar*      gnome_icon_entry_get_filename   (GnomeIconEntry *ientry);
gboolean    gnome_icon_entry_set_filename   (GnomeIconEntry *ientry,
                                             const gchar *filename);
void        gnome_icon_entry_set_browse_dialog_title
                                            (GnomeIconEntry *ientry,
                                             const gchar *browse_dialog_title);
void        gnome_icon_entry_set_history_id (GnomeIconEntry *ientry,
                                             const gchar *history_id);
GtkWidget*  gnome_icon_entry_pick_dialog    (GnomeIconEntry *ientry);
*/
/* Deprecated.
void        gnome_icon_entry_set_icon       (GnomeIconEntry *ientry,
                                             const gchar *filename);
GtkWidget*  gnome_icon_entry_gnome_file_entry
                                            (GnomeIconEntry *ientry);
GtkWidget*  gnome_icon_entry_gnome_entry    (GnomeIconEntry *ientry);
GtkWidget*  gnome_icon_entry_gtk_entry      (GnomeIconEntry *ientry);
*/

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
