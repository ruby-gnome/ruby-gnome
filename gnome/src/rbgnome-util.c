/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-util.c,v 1.5 2002/10/14 13:56:24 tkubo Exp $ */
/* based on libgnome/gnome-util.h */

/* Utility functions for Ruby/GNOME2
 * Copyright (C) 2001 Neil Conway <neilconway@rogers.com>
 *               2002 KUBO Takehiro <kubo@jiubao.org>
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

void
Init_gnome_util(mGnome)
    VALUE mGnome;
{
    VALUE mUtil = rb_define_module_under(mGnome, "Util");

    rb_define_module_function(mUtil, "prepend_user_home", rgutil_prepend_user_home, 1);
    rb_define_module_function(mUtil, "home_file", rgutil_home_file, 1);
    rb_define_module_function(mUtil, "user_shell", rgutil_user_shell,0);
}
