/* $Id: rbgnome-less.c,v 1.1 2002/05/19 12:29:54 mutoh Exp $ */

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

VALUE gnoLess;

static VALUE
less_initialize(self)
    VALUE self;
{
    GtkWidget *less;
    less = gnome_less_new();
    set_widget(self, less);
    return Qnil;
}

static VALUE
less_clear(self)
    VALUE self;
{
    gnome_less_clear(GNOME_LESS(get_widget(self)));
    return self;
}

/* All these clear any existing text and show whatever you pass in. 
   When applicable, they return TRUE on success, FALSE and set errno 
   on failure. */
static VALUE
less_show_file(self, path)
    VALUE self, path;
{
    gboolean result;
    result = gnome_less_show_file(GNOME_LESS(get_widget(self)),
				  STR2CSTR(path));
    return result?Qtrue:Qfalse;
}

static VALUE
less_show_command(self, command_line)
    VALUE self, command_line;
{
    gboolean result;
    result = gnome_less_show_command(GNOME_LESS(get_widget(self)),
				     STR2CSTR(command_line));
    return result?Qtrue:Qfalse;
}

static VALUE
less_show_string(self, s)
    VALUE self, s;
{
    gnome_less_show_string(GNOME_LESS(get_widget(self)), STR2CSTR(s));
    return self;
}

static VALUE
less_show_filestream(self, f)
    VALUE self, f;
{
    gboolean result;
    
    result = gnome_less_show_filestream(GNOME_LESS(get_widget(self)),
					RFILE(f)->fptr->f);
    return result?Qtrue:Qfalse;
}

static VALUE
less_show_fd(self, fd)
    VALUE self, fd;
{
    gboolean result;
    
    result = gnome_less_show_fd(GNOME_LESS(get_widget(self)),
				NUM2INT(fd));
    return result?Qtrue:Qfalse;
}

/* Write a file; returns FALSE and sets errno if either open
   or close fails on the file. write_file overwrites any existing file. */
static VALUE
less_write_file(self, path)
    VALUE self, path;
{
    gboolean result;
    result = gnome_less_write_file(GNOME_LESS(get_widget(self)),
				   STR2CSTR(path));
    return result?Qtrue:Qfalse;
}

/* Write a file; returns FALSE and sets errno if either open
   or close fails on the file. write_file overwrites any existing file. */
static VALUE
less_write_fd(self, fd)
    VALUE self, fd;
{
    gboolean result;
    result = gnome_less_write_fd(GNOME_LESS(get_widget(self)),
				 NUM2INT(fd));
    return result?Qtrue:Qfalse;
}

/* Set an arbitrary font */
static VALUE
less_set_font(self, font)
    VALUE self, font;
{
    gnome_less_set_font(GNOME_LESS(get_widget(self)),
			get_gdkfont(font));
    return self;
}

/*
 * Whether to use a fixed font for any future showings. 
 * Recommended for anything that comes in columns, program code,
 * etc. Just loads a fixed font and calls set_font above.
 */
static VALUE
less_set_fixed_font(self, fixed)
    VALUE self, fixed;
{
    gnome_less_set_fixed_font(GNOME_LESS(get_widget(self)),
			      RTEST(fixed));
    return self;
}

/* Re-insert the text with the current font settings. */
static VALUE
less_reshow(self)
    VALUE self;
{
    gnome_less_reshow(GNOME_LESS(get_widget(self)));
    return self;
}

/* Replaced by the more versatile set_fixed_font - avoid. */
static VALUE
less_fixed_font(self)
    VALUE self;
{
    gnome_less_fixed_font(GNOME_LESS(get_widget(self)));
    return self;
}

void
Init_gnome_less()
{
    gnoLess = rb_define_class_under(mGnome, "Less", gVBox);

    rb_define_method(gnoLess, "initialize", less_initialize, 0);
    rb_define_method(gnoLess, "clear", less_clear, 0);
    rb_define_method(gnoLess, "show_file", less_show_file, 1);
    rb_define_method(gnoLess, "show_command", less_show_command, 1);
    rb_define_method(gnoLess, "show_string", less_show_string, 1);
    rb_define_method(gnoLess, "show_filestream", less_show_filestream, 1);
    rb_define_method(gnoLess, "show_fd", less_show_fd, 1);
    rb_define_method(gnoLess, "write_file", less_write_file, 1);
    rb_define_method(gnoLess, "write_fd", less_write_fd, 1);
    rb_define_method(gnoLess, "set_font", less_set_font, 1);
    rb_define_method(gnoLess, "set_fixed_font", less_set_fixed_font, 1);
    rb_define_method(gnoLess, "reshow", less_reshow, 0);
    rb_define_method(gnoLess, "fixed_font", less_fixed_font, 0);
}
