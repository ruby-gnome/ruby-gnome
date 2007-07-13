/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-util.c,v 1.9 2007/07/13 16:07:30 ggc Exp $ */
/* based on libgnome/gnome-util.h */

/* Utility functions for Ruby/GNOME2
 * Copyright (C) 2002-2005 Ruby-GNOME2 Project Team
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

/* Deprecated
#define     gnome_is_program_in_path        (program)
*/

static VALUE
rgutil_extension_pointer(self, path)
    VALUE self, path;
{
    return CSTR2RVAL(g_extension_pointer((const char*)RVAL2CSTR(path)));
}

static VALUE
rgutil_prepend_user_home(self, filename)
    VALUE self, filename;
{
    char *result = gnome_util_prepend_user_home(RVAL2CSTR(filename));
    VALUE obj = rb_str_new2(result);
    g_free(result);
    return obj;
}

static VALUE
rgutil_home_file(self, filename)
    VALUE self, filename;
{
    char *result = gnome_util_home_file(RVAL2CSTR(filename));
    VALUE obj = rb_str_new2(result);
    g_free(result);
    return obj;
}

static VALUE
rgutil_user_shell(self)
    VALUE self;
{
    char *result = gnome_util_user_shell();
    VALUE obj = result ? rb_str_new2(result) : Qnil;
    if (result)
        g_free(result);
    return obj;
}

static VALUE
rgutil_setenv(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE name, value, overwrite;

    rb_scan_args(argc, argv, "21", &name, &value, &overwrite);

    if (NIL_P(overwrite)) overwrite = Qtrue;

    return CBOOL2RVAL(gnome_setenv((const char*)RVAL2CSTR(name), 
                                   (const char*)RVAL2CSTR(value), 
                                   RVAL2CBOOL(overwrite)) == 0);
}

static VALUE
rgutil_unsetenv(self, name)
    VALUE self, name;
{
    gnome_unsetenv((const char*)RVAL2CSTR(name));
    return self;
}

static VALUE
rgutil_clearenv(self)
    VALUE self;
{
    gnome_clearenv();
    return self;
}

/* Deprecated.
#define     g_concat_dir_and_file           (dir,file)
#define     g_file_exists                   (filename)
#define     g_unix_error_string             (error_num)
#define     gnome_util_user_home            ()
#define     g_copy_vector                   (vec)
#define     gnome_libdir_file               (f)
#define     gnome_datadir_file              (f)
#define     gnome_sound_file                (f)
#define     gnome_pixmap_file               (f)
#define     gnome_config_file               (f)
#define     gnome_unconditional_libdir_file (f)
#define     gnome_unconditional_datadir_file(f)
#define     gnome_unconditional_sound_file  (f)
#define     gnome_unconditional_pixmap_file (f)
#define     gnome_unconditional_config_file (f)
 */

void
Init_gnome_util(mGnome)
    VALUE mGnome;
{
    VALUE mUtil = rb_define_module_under(mGnome, "Util");

    rb_define_module_function(mUtil, "extension_pointer", rgutil_extension_pointer, 1);
    rb_define_module_function(mUtil, "prepend_user_home", rgutil_prepend_user_home, 1);
    rb_define_module_function(mUtil, "home_file", rgutil_home_file, 1);
    rb_define_module_function(mUtil, "user_shell", rgutil_user_shell,0);
    rb_define_module_function(mUtil, "setenv", rgutil_setenv,-1);
    rb_define_module_function(mUtil, "unsetenv", rgutil_unsetenv, 1);
    rb_define_module_function(mUtil, "clearenv", rgutil_clearenv, 0);
}
