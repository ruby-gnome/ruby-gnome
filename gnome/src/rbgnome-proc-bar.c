/* $Id: rbgnome-proc-bar.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::ProcBar widget for Ruby/Gnome
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

VALUE gnoProcBar;

static VALUE
procbar_initialize(self, colors, cb)
    VALUE self, colors, cb;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

static VALUE
procbar_set_values(self, values)
    VALUE self, values;
{
    guint* c_values;
    int i;

    Check_Type(values, T_ARRAY);
    c_values = ALLOCA_N(guint, RARRAY(values)->len+1);
    for (i = 0; i < RARRAY(values)->len; ++i) {
        c_values[i] = NUM2INT(RARRAY(values)->ptr[i]);
    }

    gnome_proc_bar_set_values(GNOME_PROC_BAR(get_widget(self)),
                              c_values);
    return Qnil;
}

static VALUE
procbar_set_orient(self, vertical)
    VALUE self, vertical;
{
    gnome_proc_bar_set_orient(GNOME_PROC_BAR(get_widget(self)),
                              RTEST(vertical));
    return Qnil;
}

static VALUE
procbar_start(self, gtime, data)
    VALUE self, gtime, data;
{
    /* TODO */
    rb_notimplement();
    return Qnil;
}

static VALUE
procbar_stop(self)
    VALUE self;
{
    gnome_proc_bar_stop(GNOME_PROC_BAR(get_widget(self)));
    return Qnil;
}

/* FIXME: should this take an array of Gdk::Color instances? The
 * function is supposed to be passed a "GdkColor *colors" -- does this
 * mean an array of color structs or a single one?
 */
static VALUE
procbar_update(self, colors)
    VALUE self, colors;
{
    gnome_proc_bar_update(GNOME_PROC_BAR(get_widget(self)),
                          get_gdkcolor(colors));
    return Qnil;
}

void
Init_gnome_proc_bar()
{
    gnoProcBar = rb_define_class_under(mGnome, "ProcBar", gHBox);
    rb_define_method(gnoProcBar, "initialize", procbar_initialize, 2);
    rb_define_method(gnoProcBar, "set_values", procbar_set_values, 1);
    rb_define_method(gnoProcBar, "set_orient", procbar_set_orient, 1);
    rb_define_method(gnoProcBar, "start", procbar_start, 2);
    rb_define_method(gnoProcBar, "stop", procbar_stop, 0);
    rb_define_method(gnoProcBar, "update", procbar_update, 1);
}
