/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-config.c,v 1.3 2002/09/25 17:17:24 tkubo Exp $ */

/* Gnome::Config for Ruby-Gnome
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
config_push_prefix(self, prefix)
    VALUE self, prefix;
{
    gnome_config_push_prefix(RVAL2CSTR(prefix));
    return self;
}

static VALUE
config_set_string(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_set_string(RVAL2CSTR(path), RVAL2CSTR(new_value));
    return self;
}

static VALUE
config_set_translated_string(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_set_translated_string(RVAL2CSTR(path), RVAL2CSTR(new_value));
    return self;
}

static VALUE
config_set_int(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_set_int(RVAL2CSTR(path), NUM2INT(new_value));
    return self;
}

static VALUE
config_set_float(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_set_float(RVAL2CSTR(path), NUM2DBL(new_value));
    return self;
}

static VALUE
config_set_bool(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_set_bool(RVAL2CSTR(path), RTEST(new_value));
    return self;
}

static VALUE
config_pop_prefix(self)
    VALUE self;
{
    gnome_config_pop_prefix();
    return self;
}

static VALUE
config_drop_all(self)
    VALUE self;
{
    gnome_config_drop_all();
    return self;
}

static VALUE
config_sync(self)
    VALUE self;
{
    gnome_config_sync();
    return self;
}

static VALUE
config_sync_file(self, path)
    VALUE self, path;
{
    gnome_config_sync_file(RVAL2CSTR(path));
    return self;
}

static VALUE
config_private_sync_file(self, path)
    VALUE self, path;
{
    gnome_config_private_sync_file(RVAL2CSTR(path));
    return self;
}

static VALUE
config_drop_file(self, path)
    VALUE self, path;
{
    gnome_config_drop_file(RVAL2CSTR(path));
    return self;
}

static VALUE
config_private_drop_file(self, path)
    VALUE self, path;
{
    gnome_config_private_drop_file(RVAL2CSTR(path));
    return self;
}

static VALUE
config_clean_file(self, path)
    VALUE self, path;
{
    gnome_config_clean_file(RVAL2CSTR(path));
    return self;
}

static VALUE
config_private_clean_file(self, path)
    VALUE self, path;
{
    gnome_config_private_clean_file(RVAL2CSTR(path));
    return self;
}

static VALUE
config_clean_section(self, path)
    VALUE self, path;
{
    gnome_config_clean_section(RVAL2CSTR(path));
    return self;
}

static VALUE
config_private_clean_section(self, path)
    VALUE self, path;
{
    gnome_config_private_clean_section(RVAL2CSTR(path));
    return self;
}

static VALUE
config_clean_key(self, path)
    VALUE self, path;
{
    gnome_config_clean_key(RVAL2CSTR(path));
    return self;
}

static VALUE
config_private_clean_key(self, path)
    VALUE self, path;
{
    gnome_config_private_clean_key(RVAL2CSTR(path));
    return self;
}

static VALUE
config_get_real_path(self, path)
    VALUE self, path;
{
    return rb_str_new2(gnome_config_get_real_path(RVAL2CSTR(path)));
}

void
Init_gnome_config(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeConfig = rb_define_module_under(mGnome, "Config");

    rb_define_module_function(mGnomeConfig, "push_prefix", config_push_prefix, 1);
    rb_define_module_function(mGnomeConfig, "set_string", config_set_string, 2);
    rb_define_module_function(mGnomeConfig, "set_translated_string", config_set_translated_string, 2);
    rb_define_module_function(mGnomeConfig, "set_int", config_set_int, 2);
    rb_define_module_function(mGnomeConfig, "set_float", config_set_float, 2);
    rb_define_module_function(mGnomeConfig, "set_bool", config_set_bool, 2);
    rb_define_module_function(mGnomeConfig, "pop_prefix", config_pop_prefix, 0);
    rb_define_module_function(mGnomeConfig, "drop_all", config_drop_all, 0);
    rb_define_module_function(mGnomeConfig, "sync", config_sync, 0);
    rb_define_module_function(mGnomeConfig, "sync_file", config_sync_file, 1);
    rb_define_module_function(mGnomeConfig, "private_sync_file", config_private_sync_file, 1);
    rb_define_module_function(mGnomeConfig, "drop_file", config_drop_file, 1);
    rb_define_module_function(mGnomeConfig, "private_drop_file", config_private_drop_file, 1);
    rb_define_module_function(mGnomeConfig, "clean_file", config_clean_file, 1);
    rb_define_module_function(mGnomeConfig, "private_clean_file", config_private_clean_file, 1);
    rb_define_module_function(mGnomeConfig, "clean_section", config_clean_section, 1);
    rb_define_module_function(mGnomeConfig, "private_clean_section", config_private_clean_section, 1);
    rb_define_module_function(mGnomeConfig, "clean_key", config_clean_key, 1);
    rb_define_module_function(mGnomeConfig, "private_clean_key", config_private_clean_key, 1);
    rb_define_module_function(mGnomeConfig, "get_real_path", config_get_real_path, 1);
}
