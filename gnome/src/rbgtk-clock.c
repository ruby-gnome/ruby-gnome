/* $Id: rbgtk-clock.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gtk::Clock widget for Ruby/Gnome
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

VALUE gClock;

static VALUE
clock_initialize(self, type)
    VALUE self, type;
{
    GtkWidget *clock;
    clock = gtk_clock_new(NUM2INT(type));
    set_widget(self, clock);
    return Qnil;
}

static VALUE
clock_set_format(self, fmt)
    VALUE self, fmt;
{
    gtk_clock_set_format(GTK_CLOCK(get_widget(self)), STR2CSTR(fmt));
    return self;
}

static VALUE
clock_set_seconds(self, seconds)
    VALUE self, seconds;
{
    gtk_clock_set_seconds(GTK_CLOCK(get_widget(self)), NUM2INT(seconds));
    return self;
}

#if 0
static VALUE
clock_set_time(self, the_time)
    VALUE self, the_time;
{
    struct timeval tv;
    tv = rb_time_timeval(the_time);
    gtk_clock_set_seconds(GTK_CLOCK(get_widget(self)), tv->tv_sec);
    return self;
}
#endif

static VALUE
clock_set_update_interval(self, seconds)
    VALUE self, seconds;
{
    gtk_clock_set_update_interval(GTK_CLOCK(get_widget(self)),
				  NUM2INT(seconds));
    return self;
}

static VALUE
clock_start(self)
    VALUE self;
{
    gtk_clock_start(GTK_CLOCK(get_widget(self)));
    return self;
}

static VALUE
clock_stop(self)
    VALUE self;
{
    gtk_clock_stop(GTK_CLOCK(get_widget(self)));
    return self;
}

void
Init_gtk_clock()
{
    gClock = rb_define_class_under(mGtk, "Clock", gLabel);

    /* GtkClockType */
    rb_define_const(gClock, "INCREASING", INT2FIX(GTK_CLOCK_INCREASING));
    rb_define_const(gClock, "DECREASING", INT2FIX(GTK_CLOCK_DECREASING));
    rb_define_const(gClock, "REALTIME", INT2FIX(GTK_CLOCK_REALTIME));

    /* Instance methods */
    rb_define_method(gClock, "initialize", clock_initialize, 1);
    rb_define_method(gClock, "set_format", clock_set_format, 1);
    rb_define_method(gClock, "set_seconds", clock_set_seconds, 1);
    rb_define_method(gClock, "set_update_interval",
		     clock_set_update_interval, 1);
    rb_define_method(gClock, "start", clock_start, 0);
    rb_define_method(gClock, "stop", clock_stop, 0);
}
