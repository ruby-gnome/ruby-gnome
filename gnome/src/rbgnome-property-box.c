/* $Id: rbgnome-property-box.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Gnome::Animator widget for Ruby/Gnome
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

VALUE gnoPropertyBox;

static VALUE
propbox_initialize(self)
    VALUE self;
{
    GtkWidget* pb = gnome_property_box_new();
    set_widget(self, pb);
    return Qnil;
}

static VALUE
propbox_changed(self)
    VALUE self;
{
    gnome_property_box_changed(GNOME_PROPERTY_BOX(get_widget(self)));
    return Qnil;
}

static VALUE
propbox_set_state(self, state)
    VALUE self, state;
{
    gnome_property_box_set_state(GNOME_PROPERTY_BOX(get_widget(self)),
                                 RTEST(state));
    return Qnil;
}

static VALUE
propbox_append_page(self, child, label)
    VALUE self, child, label;
{
    return NUM2INT(
            gnome_property_box_append_page(GNOME_PROPERTY_BOX(get_widget(self)),
                                           get_widget(child),
                                           get_widget(label)));
}

void
Init_gnome_property_box()
{
    gnoPropertyBox = rb_define_class_under(mGnome, "PropertyBox", gnoDialog);

    rb_define_method(gnoPropertyBox, "initialize", propbox_initialize, 0);
    rb_define_method(gnoPropertyBox, "changed", propbox_changed, 0);
    rb_define_method(gnoPropertyBox, "set_state", propbox_set_state, 1);
    rb_define_method(gnoPropertyBox, "append_page", propbox_append_page, 2);

    rb_define_const(gnoPropertyBox, "SIGNAL_APPLY", rb_str_new2("apply"));
    rb_define_const(gnoPropertyBox, "SIGNAL_HELP", rb_str_new2("help"));
}
