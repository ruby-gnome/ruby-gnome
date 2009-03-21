/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-dateedit.c,v 1.10 2007/07/13 16:07:29 ggc Exp $ */
/* based on libgnomeui/gnome-dateedit.h */

/* Gnome::DateEdit widget for Ruby/GNOME2
 * Copyright (C) 2002-2005, 2009 Ruby-GNOME2 Project Team
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
#include <time.h>

struct timeval rb_time_timeval(VALUE);

static ID id_at;

#define _SELF(self) GNOME_DATE_EDIT(RVAL2GOBJ(self))

static VALUE
dateedit_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE the_time, show_time, use_24_format;
    struct timeval tv;
    GtkWidget *dateedit;

    rb_scan_args(argc, argv, "21", &the_time, &show_time, &use_24_format);
    tv = rb_time_timeval(the_time);

    if (argc == 3) {
	dateedit = gnome_date_edit_new(tv.tv_sec,
                                   RVAL2CBOOL(show_time),
                                   RVAL2CBOOL(use_24_format));
    } else {
	dateedit = gnome_date_edit_new_flags(tv.tv_sec,
                                             RVAL2GFLAGS(show_time, GNOME_TYPE_DATE_EDIT_FLAGS));
    }
    RBGTK_INITIALIZE(self, dateedit);
    return Qnil;
}

/* Don't need this
void        gnome_date_edit_construct       (GnomeDateEdit *gde,
                                             time_t the_time,
                                             GnomeDateEditFlags flags);
*/

/* Defined as properties.
void        gnome_date_edit_set_time        (GnomeDateEdit *gde,
                                             time_t the_time);
time_t      gnome_date_edit_get_time        (GnomeDateEdit *gde);
void        gnome_date_edit_set_flags       (GnomeDateEdit *gde,
                                             GnomeDateEditFlags flags);
int         gnome_date_edit_get_flags       (GnomeDateEdit *gde);
time_t      gnome_date_edit_get_initial_time
                                            (GnomeDateEdit *gde);
*/

/* Deprecated.
time_t      gnome_date_edit_get_date        (GnomeDateEdit *gde);
*/

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
