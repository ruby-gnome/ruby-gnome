/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-config.c,v 1.8 2007/07/13 16:07:29 ggc Exp $ */
/* based on libgnome/gnome-config.h */

/* Gnome::Config for Ruby/GNOME2
 * Copyright (C) 2002-2003 Ruby-GNOME2 Project Team
 * Copyright (C) 2002      KUBO Takehiro <kubo@jiubao.org>
 * Copyright (C) 2001      Neil Conway <neilconway@rogers.com>
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

/* normal get functions */
static VALUE
config_get_string_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    char *result = gnome_config_get_string_with_default(RVAL2CSTR(path), &def);
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_get_translated_string_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    char *result = gnome_config_get_translated_string_with_default(RVAL2CSTR(path), &def);
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_get_int_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    VALUE obj = INT2NUM(gnome_config_get_int_with_default(RVAL2CSTR(path), &def));
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_get_float_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    VALUE obj = rb_float_new(gnome_config_get_float_with_default(RVAL2CSTR(path), &def));
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_get_bool_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    VALUE obj = CBOOL2RVAL(gnome_config_get_bool_with_default(RVAL2CSTR(path), &def));
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_get_vector_with_default(self, path)
    VALUE self, path;
{
    int argc;
    char **argv;
    int i;
    gboolean def;
    VALUE obj;
    gnome_config_get_vector_with_default(RVAL2CSTR(path), &argc, &argv, &def);
    if (argc == 0)
        return rb_ary_new3(2, Qnil, CBOOL2RVAL(def));
    obj = rb_ary_new2(argc);
    for (i = 0; i < argc; i++) {
        rb_ary_push(obj, rb_str_new2(argv[i]));
        g_free(argv[i]);
    }
    g_free(argv);
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

/* private get functions */
static VALUE
config_private_get_string_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    char *result = gnome_config_private_get_string_with_default(RVAL2CSTR(path), &def);
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_private_get_translated_string_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    char *result = gnome_config_private_get_translated_string_with_default(RVAL2CSTR(path), &def);
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_private_get_int_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    VALUE obj = INT2NUM(gnome_config_private_get_int_with_default(RVAL2CSTR(path), &def));
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_private_get_float_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    VALUE obj = rb_float_new(gnome_config_private_get_float_with_default(RVAL2CSTR(path), &def));
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_private_get_bool_with_default(self, path)
    VALUE self, path;
{
    gboolean def;
    VALUE obj = CBOOL2RVAL(gnome_config_private_get_bool_with_default(RVAL2CSTR(path), &def));
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

static VALUE
config_private_get_vector_with_default(self, path)
    VALUE self, path;
{
    int argc;
    char **argv;
    int i;
    gboolean def;
    VALUE obj;
    gnome_config_private_get_vector_with_default(RVAL2CSTR(path), &argc, &argv, &def);
    if (argc == 0)
        return rb_ary_new3(2, Qnil, CBOOL2RVAL(def));
    obj = rb_ary_new2(argc);
    for (i = 0; i < argc; i++) {
        rb_ary_push(obj, rb_str_new2(argv[i]));
        g_free(argv[i]);
    }
    g_free(argv);
    return rb_ary_new3(2, obj, CBOOL2RVAL(def));
}

/* normal get functions */
static VALUE
config_get_string(self, path)
    VALUE self, path;
{
    char *result = gnome_config_get_string(RVAL2CSTR(path));
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

static VALUE
config_get_translated_string(self, path)
    VALUE self, path;
{
    char *result = gnome_config_get_translated_string(RVAL2CSTR(path));
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

static VALUE
config_get_int(self, path)
    VALUE self, path;
{
    return INT2NUM(gnome_config_get_int(RVAL2CSTR(path)));
}

static VALUE
config_get_float(self, path)
    VALUE self, path;
{
    return rb_float_new(gnome_config_get_float(RVAL2CSTR(path)));
}

static VALUE
config_get_bool(self, path)
    VALUE self, path;
{
    return CBOOL2RVAL(gnome_config_get_bool(RVAL2CSTR(path)));
}

static VALUE
config_get_vector(self, path)
    VALUE self, path;
{
    int argc;
    char **argv;
    int i;
    VALUE obj;
    gnome_config_get_vector(RVAL2CSTR(path), &argc, &argv);
    if (argc == 0)
        return Qnil;
    obj = rb_ary_new2(argc);
    for (i = 0; i < argc; i++) {
        rb_ary_push(obj, rb_str_new2(argv[i]));
        g_free(argv[i]);
    }
    g_free(argv);
    return obj;
}

/* private get functions */
static VALUE
config_private_get_string(self, path)
    VALUE self, path;
{
    char *result = gnome_config_private_get_string(RVAL2CSTR(path));
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

static VALUE
config_private_get_translated_string(self, path)
    VALUE self, path;
{
    char *result = gnome_config_private_get_translated_string(RVAL2CSTR(path));
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

static VALUE
config_private_get_int(self, path)
    VALUE self, path;
{
    return INT2NUM(gnome_config_private_get_int(RVAL2CSTR(path)));
}

static VALUE
config_private_get_float(self, path)
    VALUE self, path;
{
    return rb_float_new(gnome_config_private_get_float(RVAL2CSTR(path)));
}

static VALUE
config_private_get_bool(self, path)
    VALUE self, path;
{
    return CBOOL2RVAL(gnome_config_private_get_bool(RVAL2CSTR(path)));
}

static VALUE
config_private_get_vector(self, path)
    VALUE self, path;
{
    int argc;
    char **argv;
    int i;
    VALUE obj;
    gnome_config_private_get_vector(RVAL2CSTR(path), &argc, &argv);
    if (argc == 0)
        return Qnil;
    obj = rb_ary_new2(argc);
    for (i = 0; i < argc; i++) {
        rb_ary_push(obj, rb_str_new2(argv[i]));
        g_free(argv[i]);
    }
    g_free(argv);
    return obj;
}

/* normal set functions */
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
    gnome_config_set_bool(RVAL2CSTR(path), RVAL2CBOOL(new_value));
    return self;
}

static VALUE
config_set_vector(self, path, new_value)
    VALUE self, path, new_value;
{
    int argc;
    const char **argv;
    int i;

    Check_Type(new_value, T_ARRAY);
    argc = RARRAY_LEN(new_value);
    argv = ALLOCA_N(char const *, argc);
    for (i = 0; i < argc; i++) {
        argv[i] = RVAL2CSTR(RARRAY_PTR(new_value)[i]);
    }
    gnome_config_set_vector(RVAL2CSTR(path), argc, argv);
    return self;
}

/* private set functions */
static VALUE
config_private_set_string(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_private_set_string(RVAL2CSTR(path), RVAL2CSTR(new_value));
    return self;
}

static VALUE
config_private_set_translated_string(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_private_set_translated_string(RVAL2CSTR(path), RVAL2CSTR(new_value));
    return self;
}

static VALUE
config_private_set_int(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_private_set_int(RVAL2CSTR(path), NUM2INT(new_value));
    return self;
}

static VALUE
config_private_set_float(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_private_set_float(RVAL2CSTR(path), NUM2DBL(new_value));
    return self;
}

static VALUE
config_private_set_bool(self, path, new_value)
    VALUE self, path, new_value;
{
    gnome_config_private_set_bool(RVAL2CSTR(path), RVAL2CBOOL(new_value));
    return self;
}

static VALUE
config_private_set_vector(self, path, new_value)
    VALUE self, path, new_value;
{
    int argc;
    const char **argv;
    int i;

    Check_Type(new_value, T_ARRAY);
    argc = RARRAY_LEN(new_value);
    argv = ALLOCA_N(char const *, argc);
    for (i = 0; i < argc; i++) {
        argv[i] = RVAL2CSTR(RARRAY_PTR(new_value)[i]);
    }
    gnome_config_private_set_vector(RVAL2CSTR(path), argc, argv);
    return self;
}


/* Returns true if /path/section is defined */
static VALUE
config_has_section(self, prefix)
    VALUE self, prefix;
{
    return CBOOL2RVAL(gnome_config_has_section(RVAL2CSTR(prefix)));
}

static VALUE
config_private_has_section(self, prefix)
    VALUE self, prefix;
{
    return CBOOL2RVAL(gnome_config_private_has_section(RVAL2CSTR(prefix)));
}

#if 0 /* FIXME */
/* Returns a pointer for iterating on /file/section contents */
void *gnome_config_init_iterator_ (const char *path, gboolean priv);
#define gnome_config_init_iterator(path) \
	(gnome_config_init_iterator_((path),FALSE))
#define gnome_config_private_init_iterator(path) \
	(gnome_config_init_iterator_((path),TRUE))

/* Returns a pointer for iterating on /file contents */
void *gnome_config_init_iterator_sections_ (const char *path, gboolean priv);
#define gnome_config_init_iterator_sections(path) \
	(gnome_config_init_iterator_sections_((path),FALSE))
#define gnome_config_private_init_iterator_sections(path) \
	(gnome_config_init_iterator_sections_((path),TRUE))

/* Get next key and value value from a section */
void *gnome_config_iterator_next (void *iterator_handle, char **key, char **value);
#endif

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

/* sync's data for one file only */
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

/* This routine drops the information about /file, meaning changes
   done to this file will be dropped, it will no delete the file */
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

/* This routine actually removes /file on sync (not right away, you
   can still save it by dropping it)*/
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

/* This routine drops all of the information related to /file/section 
   this will actually remove the section */
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

/* Drops the information for a specific key, this will actually remove
   the key */
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

/* returns the true filename of the config file */
static VALUE
config_get_real_path(self, path)
    VALUE self, path;
{
    char *result = gnome_config_get_real_path(RVAL2CSTR(path));
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

static VALUE
config_private_get_real_path(self, path)
    VALUE self, path;
{
    char *result = gnome_config_private_get_real_path(RVAL2CSTR(path));
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

/* Set an active prefix and remove an active prefix */
static VALUE
config_push_prefix(self, prefix)
    VALUE self, prefix;
{
    gnome_config_push_prefix(RVAL2CSTR(prefix));
    return self;
}

static VALUE
config_pop_prefix(self)
    VALUE self;
{
    gnome_config_pop_prefix();
    return self;
}

void
Init_gnome_config(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeConfig = rb_define_module_under(mGnome, "Config");

    /* normal get functions with default */
    rb_define_module_function(mGnomeConfig, "get_string_with_default", config_get_string_with_default, 1);
    rb_define_module_function(mGnomeConfig, "get_translated_string_with_default", config_get_translated_string_with_default, 1);
    rb_define_module_function(mGnomeConfig, "get_int_with_default", config_get_int_with_default, 1);
    rb_define_module_function(mGnomeConfig, "get_float_with_default", config_get_float_with_default, 1);
    rb_define_module_function(mGnomeConfig, "get_bool_with_default", config_get_bool_with_default, 1);
    rb_define_module_function(mGnomeConfig, "get_vector_with_default", config_get_vector_with_default, 1);

    /* private get functions with default */
    rb_define_module_function(mGnomeConfig, "private_get_string_with_default", config_private_get_string_with_default, 1);
    rb_define_module_function(mGnomeConfig, "private_get_translated_string_with_default", config_private_get_translated_string_with_default, 1);
    rb_define_module_function(mGnomeConfig, "private_get_int_with_default", config_private_get_int_with_default, 1);
    rb_define_module_function(mGnomeConfig, "private_get_float_with_default", config_private_get_float_with_default, 1);
    rb_define_module_function(mGnomeConfig, "private_get_bool_with_default", config_private_get_bool_with_default, 1);
    rb_define_module_function(mGnomeConfig, "private_get_vector_with_default", config_private_get_vector_with_default, 1);

    /* normal get functions */
    rb_define_module_function(mGnomeConfig, "get_string", config_get_string, 1);
    rb_define_module_function(mGnomeConfig, "get_translated_string", config_get_translated_string, 1);
    rb_define_module_function(mGnomeConfig, "get_int", config_get_int, 1);
    rb_define_module_function(mGnomeConfig, "get_float", config_get_float, 1);
    rb_define_module_function(mGnomeConfig, "get_bool", config_get_bool, 1);
    rb_define_module_function(mGnomeConfig, "get_vector", config_get_vector, 1);

    /* private get functions */
    rb_define_module_function(mGnomeConfig, "private_get_string", config_private_get_string, 1);
    rb_define_module_function(mGnomeConfig, "private_get_translated_string", config_private_get_translated_string, 1);
    rb_define_module_function(mGnomeConfig, "private_get_int", config_private_get_int, 1);
    rb_define_module_function(mGnomeConfig, "private_get_float", config_private_get_float, 1);
    rb_define_module_function(mGnomeConfig, "private_get_bool", config_private_get_bool, 1);
    rb_define_module_function(mGnomeConfig, "private_get_vector", config_private_get_vector, 1);

    /* normal set functions */
    rb_define_module_function(mGnomeConfig, "set_string", config_set_string, 2);
    rb_define_module_function(mGnomeConfig, "set_translated_string", config_set_translated_string, 2);
    rb_define_module_function(mGnomeConfig, "set_int", config_set_int, 2);
    rb_define_module_function(mGnomeConfig, "set_float", config_set_float, 2);
    rb_define_module_function(mGnomeConfig, "set_bool", config_set_bool, 2);
    rb_define_module_function(mGnomeConfig, "set_vector", config_set_vector, 2);

    /* private set functions */
    rb_define_module_function(mGnomeConfig, "private_set_string", config_private_set_string, 2);
    rb_define_module_function(mGnomeConfig, "private_set_translated_string", config_private_set_translated_string, 2);
    rb_define_module_function(mGnomeConfig, "private_set_int", config_private_set_int, 2);
    rb_define_module_function(mGnomeConfig, "private_set_float", config_private_set_float, 2);
    rb_define_module_function(mGnomeConfig, "private_set_bool", config_private_set_bool, 2);
    rb_define_module_function(mGnomeConfig, "private_set_vector", config_private_set_vector, 2);

    /* Returns true if /path/section is defined */
    rb_define_module_function(mGnomeConfig, "has_section?", config_has_section, 1);
    rb_define_module_function(mGnomeConfig, "private_has_section?", config_private_has_section, 1);

    rb_define_module_function(mGnomeConfig, "drop_all", config_drop_all, 0);
    rb_define_module_function(mGnomeConfig, "sync", config_sync, 0);

    /* sync's data for one file only */
    rb_define_module_function(mGnomeConfig, "sync_file", config_sync_file, 1);
    rb_define_module_function(mGnomeConfig, "private_sync_file", config_private_sync_file, 1);

    /* This routine drops the information about /file, meaning changes
       done to this file will be dropped, it will no delete the file */
    rb_define_module_function(mGnomeConfig, "drop_file", config_drop_file, 1);
    rb_define_module_function(mGnomeConfig, "private_drop_file", config_private_drop_file, 1);

    /* This routine actually removes /file on sync (not right away, you
       can still save it by dropping it)*/
    rb_define_module_function(mGnomeConfig, "clean_file", config_clean_file, 1);
    rb_define_module_function(mGnomeConfig, "private_clean_file", config_private_clean_file, 1);

    /* This routine drops all of the information related to /file/section 
       this will actually remove the section */
    rb_define_module_function(mGnomeConfig, "clean_section", config_clean_section, 1);
    rb_define_module_function(mGnomeConfig, "private_clean_section", config_private_clean_section, 1);

    /* Drops the information for a specific key, this will actually remove
       the key */
    rb_define_module_function(mGnomeConfig, "clean_key", config_clean_key, 1);
    rb_define_module_function(mGnomeConfig, "private_clean_key", config_private_clean_key, 1);

    /* returns the true filename of the config file */
    rb_define_module_function(mGnomeConfig, "get_real_path", config_get_real_path, 1);
    rb_define_module_function(mGnomeConfig, "private_get_real_path", config_private_get_real_path, 1);

    /* Set an active prefix and remove an active prefix */
    rb_define_module_function(mGnomeConfig, "push_prefix", config_push_prefix, 1);
    rb_define_module_function(mGnomeConfig, "pop_prefix", config_pop_prefix, 0);
}
