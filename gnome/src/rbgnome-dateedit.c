/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-dateedit.c,v 1.8 2003/11/08 18:49:45 mutoh Exp $ */
/* based on libgnomeui/gnome-dateedit.h */

/* Gnome::DateEdit widget for Ruby/GNOME2
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

static ID id_at;

#define _SELF(self) GNOME_DATE_EDIT(RVAL2GOBJ(self))

static VALUE
dateedit_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE the_time, show_time, use_24_format;
    struct timeval tv;
    GtkWidget *dateedit;

    rb_scan_args(argc, argv, "21", &the_time, &show_time, &use_24_format);
    tv = rb_time_timeval(the_time);

    if (argc == 3) {
	dateedit = gnome_date_edit_new(tv.tv_sec,
                                   RTEST(show_time),
                                   RTEST(use_24_format));
    } else {
	dateedit = gnome_date_edit_new_flags(tv.tv_sec,
                                             RVAL2GFLAGS(show_time, GNOME_TYPE_DATE_EDIT_FLAGS));
    }
    RBGTK_INITIALIZE(self, dateedit);
    return Qnil;
}

static VALUE
dateedit_set_popup_range(self, low_hour, up_hour)
    VALUE self, low_hour, up_hour;
{
    gnome_date_edit_set_popup_range(_SELF(self),
                                    NUM2INT(low_hour), NUM2INT(up_hour));
    return self;
}

void
Init_gnome_dateedit(mGnome)
     VALUE mGnome;
{
    VALUE gnoDateEdit = G_DEF_CLASS(GNOME_TYPE_DATE_EDIT, "DateEdit", mGnome);

    id_at = rb_intern("at");

    /* GnomeDateEditFlags */
    G_RENAME_CONSTANT("24_HR", "USE_24_HR");
    G_RENAME_NICK("24-hr", "use-24-hr");
    G_DEF_CLASS(GNOME_TYPE_DATE_EDIT_FLAGS, "Flags", gnoDateEdit);
    G_RENAME_CONSTANT("24_HR", "USE_24_HR");
    G_DEF_CONSTANTS(gnoDateEdit, GNOME_TYPE_DATE_EDIT_FLAGS, "GNOME_DATE_EDIT_");

    /* Instance methods */
    rb_define_method(gnoDateEdit, "initialize", dateedit_initialize, -1);
    rb_define_method(gnoDateEdit, "set_popup_range",  dateedit_set_popup_range, 2);
}
