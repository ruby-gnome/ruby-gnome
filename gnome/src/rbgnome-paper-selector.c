/* $Id: rbgnome-paper-selector.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::PaperSelector widget for Ruby/Gnome
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

VALUE gnoPaperSelector;

static VALUE
pselector_initialize(self)
    VALUE self;
{
    set_widget(self, gnome_paper_selector_new());
    return Qnil;
}

static VALUE
pselector_get_name(self)
    VALUE self;
{
    gchar *name;
    name = gnome_paper_selector_get_name(
		GNOME_PAPER_SELECTOR(get_widget(self)));
    return rb_str_new2(name);
}

static VALUE
pselector_get_width(self)
    VALUE self;
{
    gfloat width;
    width = gnome_paper_selector_get_width(
		GNOME_PAPER_SELECTOR(get_widget(self)));
    return rb_float_new((double)width);
}

static VALUE
pselector_get_height(self)
    VALUE self;
{
    gfloat height;
    height = gnome_paper_selector_get_height(
		GNOME_PAPER_SELECTOR(get_widget(self)));
    return rb_float_new((double)height);
}

static VALUE
pselector_get_left_margin(self)
    VALUE self;
{
    gfloat left_margin;
    left_margin = gnome_paper_selector_get_left_margin(
			GNOME_PAPER_SELECTOR(get_widget(self)));
    return rb_float_new((double)left_margin);
}

static VALUE
pselector_get_right_margin(self)
    VALUE self;
{
    gfloat right_margin;
    right_margin = gnome_paper_selector_get_right_margin(
			GNOME_PAPER_SELECTOR(get_widget(self)));
    return rb_float_new((double)right_margin);
}

static VALUE
pselector_get_top_margin(self)
    VALUE self;
{
    gfloat top_margin;
    top_margin = gnome_paper_selector_get_top_margin(
			GNOME_PAPER_SELECTOR(get_widget(self)));
    return rb_float_new((double)top_margin);
}

static VALUE
pselector_get_bottom_margin(self)
    VALUE self;
{
    gfloat bottom_margin;
    bottom_margin = gnome_paper_selector_get_bottom_margin(
			GNOME_PAPER_SELECTOR(get_widget(self)));
    return rb_float_new((double)bottom_margin);
}

void
Init_gnome_paper_selector()
{
    gnoPaperSelector = rb_define_class_under(mGnome, "PaperSelector", gVBox);

    /* Instance methods */
    rb_define_method(gnoPaperSelector, "initialize",
		     pselector_initialize, 0);
    rb_define_method(gnoPaperSelector, "get_name",
		     pselector_get_name, 0);
    rb_define_method(gnoPaperSelector, "get_width",
		     pselector_get_width, 0);
    rb_define_method(gnoPaperSelector, "get_height",
		     pselector_get_height, 0);
    rb_define_method(gnoPaperSelector, "get_left_margin",
		     pselector_get_left_margin, 0);
    rb_define_method(gnoPaperSelector, "get_right_margin",
		     pselector_get_right_margin, 0);
    rb_define_method(gnoPaperSelector, "get_top_margin",
		     pselector_get_top_margin, 0);
    rb_define_method(gnoPaperSelector, "get_bottom_margin",
		     pselector_get_bottom_margin, 0);
}

