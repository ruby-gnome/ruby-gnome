/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-property-box.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::PropertyBox widget for Ruby/Gnome
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

#define _SELF(self) GNOME_PROPERTY_BOX(RVAL2GOBJ(self))

static VALUE
propbox_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gnome_property_box_new());
    return Qnil;
}

static VALUE
propbox_changed(self)
    VALUE self;
{
    gnome_property_box_changed(_SELF(self));
    return self;
}

static VALUE
propbox_set_state(self, state)
    VALUE self, state;
{
    gnome_property_box_set_state(_SELF(self),
                                 RTEST(state));
    return self;
}

static VALUE
propbox_append_page(self, child, label)
    VALUE self, child, label;
{
    return NUM2INT(gnome_property_box_append_page(_SELF(self),
                                                  GTK_WIDGET(RVAL2GOBJ(child)),
                                                  GTK_WIDGET(RVAL2GOBJ(label))));
}

void
Init_gnome_property_box(mGnome)
    VALUE mGnome;
{
    VALUE gnoPropertyBox = G_DEF_CLASS(GNOME_TYPE_PROPERTY_BOX, "PropertyBox", mGnome);

    rb_define_method(gnoPropertyBox, "initialize", propbox_initialize, 0);
    rb_define_method(gnoPropertyBox, "changed", propbox_changed, 0);
    rb_define_method(gnoPropertyBox, "set_state", propbox_set_state, 1);
    rb_define_method(gnoPropertyBox, "append_page", propbox_append_page, 2);
}
