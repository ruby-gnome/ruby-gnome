/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-pixmap-entry.c,v 1.7 2003/06/26 15:16:20 mutoh Exp $ */
/* based on libgnomeui/gnome-pixmap-entry.h */

/* Gnome::PixmapEntry widget for Ruby/GNOME2
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

#define _SELF(self) GNOME_PIXMAP_ENTRY(RVAL2GOBJ(self))

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
    pentry = gnome_pixmap_entry_new(NIL_P(history_id)?0:RVAL2CSTR(history_id),
                                    NIL_P(browse_dialog_title)?0:RVAL2CSTR(browse_dialog_title),
                                    RTEST(do_preview));
    RBGTK_INITIALIZE(self, pentry);
    return Qnil;
}

static VALUE
pentry_set_pixmap_subdir(self, subdir)
    VALUE self, subdir;
{
    gnome_pixmap_entry_set_pixmap_subdir(_SELF(self), RVAL2CSTR(subdir));
    return self;
}

static VALUE
pentry_scrolled_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_pixmap_entry_scrolled_window(_SELF(self)));
}

static VALUE
pentry_preview_widget(self)
    VALUE self;
{
    return GOBJ2RVAL(gnome_pixmap_entry_preview_widget(_SELF(self)));
}

static VALUE
pentry_set_preview(self, do_preview)
    VALUE self, do_preview;
{
    gnome_pixmap_entry_set_preview(_SELF(self), RTEST(do_preview));
    return self;
}

static VALUE
pentry_set_preview_size(self, preview_w, preview_h)
    VALUE self, preview_w, preview_h;
{
    gnome_pixmap_entry_set_preview_size(_SELF(self),
                                        NUM2INT(preview_w),
                                        NUM2INT(preview_h));
    return self;
}

static VALUE
pentry_get_filename(self)
    VALUE self;
{
    char *filename;
    VALUE obj;

    filename = gnome_pixmap_entry_get_filename(_SELF(self));
    SET_STR_AND_GFREE(obj, filename);
    return obj;
}

void
Init_gnome_pixmap_entry(mGnome)
    VALUE mGnome;
{
    VALUE gnoPixmapEntry = G_DEF_CLASS(GNOME_TYPE_PIXMAP_ENTRY, "PixmapEntry", mGnome);

    /* Instance methods */
    rb_define_method(gnoPixmapEntry, "initialize", pentry_initialize, -1);
    rb_define_method(gnoPixmapEntry, "set_pixmap_subdir", pentry_set_pixmap_subdir, 1);
    rb_define_method(gnoPixmapEntry, "scrolled_window", pentry_scrolled_window, 0);
    rb_define_method(gnoPixmapEntry, "preview_widget", pentry_preview_widget, 0);
    rb_define_method(gnoPixmapEntry, "set_preview", pentry_set_preview, 1);
    rb_define_method(gnoPixmapEntry, "set_preview_size", pentry_set_preview_size, 2);
    rb_define_method(gnoPixmapEntry, "filename", pentry_get_filename, 0);

    G_DEF_SETTERS(gnoPixmapEntry);
}
