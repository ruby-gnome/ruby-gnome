/* $Id: rbgnome-util.c,v 1.2 2002/05/19 15:48:28 mutoh Exp $ */

/* Utility functions for Ruby/Gnome
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

static VALUE
rbgnome_libdir_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_libdir_file(STR2CSTR(filename));
    ret = CSTR2OBJ(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_datadir_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_datadir_file(STR2CSTR(filename));
    ret = CSTR2OBJ(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_sound_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_sound_file(STR2CSTR(filename));
    ret = CSTR2OBJ(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_pixmap_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_pixmap_file(STR2CSTR(filename));
    ret = CSTR2OBJ(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_config_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_config_file(STR2CSTR(filename));
    ret = CSTR2OBJ(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_unconditional_libdir_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_unconditional_libdir_file(STR2CSTR(filename));
    ret = rb_str_new2(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_unconditional_datadir_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_unconditional_datadir_file(STR2CSTR(filename));
    ret = rb_str_new2(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_unconditional_sound_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_unconditional_sound_file(STR2CSTR(filename));
    ret = rb_str_new2(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_unconditional_pixmap_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_unconditional_pixmap_file(STR2CSTR(filename));
    ret = rb_str_new2(file);
    g_free(file);
    return ret;
}

static VALUE
rbgnome_unconditional_config_file(self, filename)
    VALUE self, filename;
{
    VALUE ret;
    char *file = gnome_unconditional_config_file(STR2CSTR(filename));
    ret = rb_str_new2(file);
    g_free(file);
    return ret;
}

void
Init_gnome_util()
{
    rb_define_module_function(mGnome, "libdir_file", rbgnome_libdir_file, 1);
    rb_define_module_function(mGnome, "datadir_file", rbgnome_datadir_file, 1);
    rb_define_module_function(mGnome, "sound_file", rbgnome_sound_file, 1);
    rb_define_module_function(mGnome, "pixmap_file", rbgnome_pixmap_file, 1);
    rb_define_module_function(mGnome, "config_file", rbgnome_config_file, 1);
    rb_define_module_function(mGnome, "unconditional_libdir_file",
                rbgnome_unconditional_libdir_file, 1);
    rb_define_module_function(mGnome, "unconditional_datadir_file",
                rbgnome_unconditional_datadir_file, 1);
    rb_define_module_function(mGnome, "unconditional_sound_file",
                rbgnome_unconditional_sound_file, 1);
    rb_define_module_function(mGnome, "unconditional_pixmap_file",
                rbgnome_unconditional_pixmap_file, 1);
    rb_define_module_function(mGnome, "unconditional_config_file",
                rbgnome_unconditional_config_file, 1);
}
