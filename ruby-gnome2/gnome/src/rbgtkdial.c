/* $Id: rbgtkdial.c,v 1.1 2002/05/19 12:29:53 mutoh Exp $ */

/* Gtk::Dial widget for Ruby-Gnome
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

VALUE gDial;

static VALUE
dial_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE adjustment;
    GtkWidget* dial;
    
    rb_scan_args(argc, argv, "01", &adjustment);
    dial = gtk_dial_new(NIL_P(adjustment)
			?NULL:GTK_ADJUSTMENT(get_gobject(adjustment)));
    set_widget(self, dial);
    return Qnil;
}

static VALUE
dial_get_adjustment(self)
    VALUE self;
{
    GtkAdjustment* adj;
    adj = gtk_dial_get_adjustment(GTK_DIAL(get_widget(self)));
    return make_gobject(gAdjustment, GTK_OBJECT(adj));
}

static VALUE
dial_set_update_policy(self, policy)
    VALUE self, policy;
{
    gtk_dial_set_update_policy(GTK_DIAL(get_widget(self)), NUM2INT(policy));
    return self;
}

static VALUE
dial_set_adjustment(self, adjustment)
    VALUE self, adjustment;
{
    gtk_dial_set_adjustment(GTK_DIAL(get_widget(self)),
			    GTK_ADJUSTMENT(get_gobject(adjustment)));
    return self;
}

static VALUE
dial_set_percentage(self, percent)
    VALUE self, percent;
{
    gfloat percentage;
    percentage = gtk_dial_set_percentage(GTK_DIAL(get_widget(self)),
					 (gfloat)NUM2DBL(percent));
    return rb_float_new((double)percentage);
}

static VALUE
dial_get_percentage(self)
    VALUE self;
{
    gfloat percentage;
    percentage = gtk_dial_get_percentage(GTK_DIAL(get_widget(self)));
    return rb_float_new((double)percentage);
}

static VALUE
dial_set_value(self, value)
    VALUE self, value;
{
	return rb_float_new(gtk_dial_set_value(GTK_DIAL(get_widget(self)),
										   (gfloat)NUM2DBL(value)));
}

static VALUE
dial_get_value(self)
    VALUE self;
{
    return rb_float_new(gtk_dial_get_value(GTK_DIAL(get_widget(self))));
}

static VALUE
dial_set_view_only(self, view_only)
	VALUE self, view_only;
{
    gtk_dial_set_view_only(GTK_DIAL(get_widget(self)), RTEST(view_only));
    return self;
}

void
Init_gtk_dial()
{
    gDial = rb_define_class_under(mGtk, "Dial", gWidget);

    rb_define_method(gDial, "initialize", dial_initialize, -1);
    rb_define_method(gDial, "get_adjustment", dial_get_adjustment, 0);
    rb_define_method(gDial, "set_update_policy", dial_set_update_policy, 1);
    rb_define_method(gDial, "set_adjustment", dial_set_adjustment, 1);
    rb_define_method(gDial, "set_percentage", dial_set_percentage, 1);
    rb_define_method(gDial, "get_percentage", dial_get_percentage, 0);
    rb_define_method(gDial, "set_value", dial_set_value, 1);
    rb_define_method(gDial, "get_value", dial_get_value, 0);
    rb_define_method(gDial, "set_view_only", dial_set_view_only, 1);
}

