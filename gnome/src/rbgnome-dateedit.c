/* $Id: rbgnome-dateedit.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::DateEdit widget for Ruby/Gnome
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

VALUE gnoDateEdit;

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
					     NUM2INT(show_time));
    }
    set_widget(self, dateedit);
    return Qnil;
}

static VALUE
dateedit_set_time(self, the_time)
    VALUE self, the_time;
{
    struct timeval tv;

    tv = rb_time_timeval(the_time);
    gnome_date_edit_set_time(GNOME_DATE_EDIT(get_widget(self)),
			     tv.tv_sec);
    return self;
}

static VALUE
dateedit_set_popup_range(self, low_hour, up_hour)
    VALUE self, low_hour, up_hour;
{
    gnome_date_edit_set_popup_range(GNOME_DATE_EDIT(get_widget(self)),
				    NUM2INT(low_hour), NUM2INT(up_hour));
    return self;
}

static VALUE
dateedit_get_date(self)
    VALUE self;
{
    time_t sec;
    sec = gnome_date_edit_get_date(GNOME_DATE_EDIT(get_widget(self)));
    return rb_time_new(sec, 0);
}

static VALUE
dateedit_set_flags(self, flags)
    VALUE self, flags;
{
    gnome_date_edit_set_flags(GNOME_DATE_EDIT(get_widget(self)),
			      NUM2INT(flags));
    return self;
}

static VALUE
dateedit_get_flags(self)
    VALUE self;
{
    int flags;
    flags = gnome_date_edit_get_flags(GNOME_DATE_EDIT(get_widget(self)));
    return INT2NUM(flags);
}

void
Init_gnome_dateedit()
{
    gnoDateEdit = rb_define_class_under(mGnome, "DateEdit", gHBox);

    /* GnomeDateEditFlags */
    rb_define_const(gnoDateEdit, "SHOW_TIME",
		    INT2FIX(GNOME_DATE_EDIT_SHOW_TIME));
    rb_define_const(gnoDateEdit, "USE_24_HR",
		    INT2FIX(GNOME_DATE_EDIT_24_HR));
    rb_define_const(gnoDateEdit, "WEEK_STARTS_ON_MONDAY",
		    INT2FIX(GNOME_DATE_EDIT_WEEK_STARTS_ON_MONDAY));

    /* Instance methods */
    rb_define_method(gnoDateEdit, "initialize", dateedit_initialize, -1);
    rb_define_method(gnoDateEdit, "set_time", dateedit_set_time, 1);
    rb_define_method(gnoDateEdit, "set_popup_range",
		     dateedit_set_popup_range, 2);
    rb_define_method(gnoDateEdit, "get_date", dateedit_get_date, 0);
    rb_define_method(gnoDateEdit, "set_flags", dateedit_set_flags, 1);
    rb_define_method(gnoDateEdit, "get_flags", dateedit_get_flags, 0);

    /* Signals */
    rb_define_const(gnoDateEdit, "SIGNAL_TIME_CHANGED",
		    rb_str_new2("time_changed"));
    rb_define_const(gnoDateEdit, "SIGNAL_DATE_CHANGED",
		    rb_str_new2("date_changed"));
}
