/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-sound.c,v 1.1 2005/09/24 18:02:43 mutoh Exp $ */

/* Gnome::Sound module for Ruby/GNOME2
 * Copyright (C) 2005  Ruby-GNOME2 Project Team
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

static VALUE
sound_init(self, hostname)
    VALUE self, hostname;
{
    gnome_sound_init((const char*)RVAL2CSTR(hostname));
    return self;
}

static VALUE
sound_shutdown(self)
    VALUE self;
{
    gnome_sound_shutdown();
    return self;
}

static VALUE
sound_play(self, filename)
    VALUE self, filename;
{
    gnome_sound_play((const char*)RVAL2CSTR(filename));
    return self;
}

static VALUE
sound_sample_load(self, sample_name, filename)
    VALUE self, sample_name, filename;
{
    return INT2NUM(gnome_sound_sample_load((const char*)RVAL2CSTR(sample_name),
                                           (const char*)RVAL2CSTR(filename)));
}

static VALUE
sound_connection_get(self)
    VALUE self;
{
    return INT2NUM(gnome_sound_connection_get());
}

void
Init_gnome_sound(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeSound = rb_define_module_under(mGnome, "Sound");

    rb_define_module_function(mGnomeSound, "init", sound_init, 1);
    rb_define_module_function(mGnomeSound, "shutdown", sound_shutdown, 0);
    rb_define_module_function(mGnomeSound, "play", sound_play, 1);
    rb_define_module_function(mGnomeSound, "sample_load", sound_sample_load, 2);
    rb_define_module_function(mGnomeSound, "connection", sound_connection_get, 0);

}
