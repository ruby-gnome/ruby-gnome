/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-i18n.c,v 1.4 2002/10/26 06:22:24 tkubo Exp $ */
/* based on libgnome/gnome-i18n.h */

/* Gnome::I18n module for Ruby/GNOME2
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
gnoi18n_get_language_list(self, category_name)
    VALUE self, category_name;
{
    const GList* lang_list;
    VALUE ary;
    lang_list = gnome_i18n_get_language_list(RVAL2CSTR(category_name));

    ary = rb_ary_new();
    while (lang_list) {
        char* lang = (char*)lang_list->data;
        lang_list = lang_list->next;
        rb_ary_push(ary, rb_str_new2(lang));
    }

    return ary;
}

static VALUE
gnoi18n_push_c_numeric_locale(self)
    VALUE self;
{
    gnome_i18n_push_c_numeric_locale();
    return self;
}

static VALUE
gnoi18n_pop_c_numeric_locale(self)
    VALUE self;
{
    gnome_i18n_pop_c_numeric_locale();
    return self;
}

void
Init_gnome_i18n(mGnome)
    VALUE mGnome;
{
    VALUE mGnomeI18n = rb_define_module_under(mGnome, "I18n");

    rb_define_module_function(mGnomeI18n, "get_language_list", gnoi18n_get_language_list, 1);
    rb_define_module_function(mGnomeI18n, "push_c_numeric_locale", gnoi18n_push_c_numeric_locale, 0);
    rb_define_module_function(mGnomeI18n, "pop_c_numeric_locale", gnoi18n_pop_c_numeric_locale, 0);
}
