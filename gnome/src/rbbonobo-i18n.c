/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbbonobo-i18n.c,v 1.1 2002/10/26 06:22:24 tkubo Exp $ */
/* based on libbonobo/bonobo/bonobo-i18n.h */
/* renamed from rbgnome-i18n.c */

/* Bonobo::I18n module for Ruby/GNOME2
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

VALUE mBonoboI18n;

static VALUE
bnbi18n_bindtextdomain(self, domain, dirname)
    VALUE self, domain, dirname;
{
    char* new_dirname;

    switch (TYPE(dirname)) {
      case T_STRING:
        new_dirname = bindtextdomain(RVAL2CSTR(domain), RVAL2CSTR(dirname));
        break;
      case T_NIL:
        new_dirname = bindtextdomain(RVAL2CSTR(domain), 0);
        break;
      default:
        rb_raise(rb_eTypeError, "dirname is Not String or Null");
    }

    return rb_str_new2(new_dirname);
}

static VALUE
bnbi18n_textdomain(self, domain)
    VALUE self, domain;
{
    char* new_domain;
    new_domain = textdomain(RVAL2CSTR(domain));
    return rb_str_new2(new_domain);
}

static VALUE
bnbi18n_gettext(self, msgid)
    VALUE self, msgid;
{
    char* msgstr;
    msgstr = gettext(RVAL2CSTR(msgid));
    return rb_str_new2(msgstr);
}

static VALUE
bnbi18n_gettext_noop(self, msgid)
    VALUE self, msgid;
{
    return msgid;
}

static VALUE
bnbi18n_dgettext(self, domain, msgid)
    VALUE self, domain, msgid;
{
    char* msgstr;
    msgstr = dgettext(RVAL2CSTR(domain), RVAL2CSTR(msgid));

    return rb_str_new2(msgstr);
}

static VALUE
bnbi18n_dcgettext(self, domain, msgid, category)
    VALUE self, domain, msgid, category;
{
    char* msgstr;
    msgstr = dcgettext(RVAL2CSTR(domain), RVAL2CSTR(msgid), FIX2INT(category));

    return rb_str_new2(msgstr);
}

void
Init_bonobo_i18n(mBonobo)
    VALUE mBonobo;
{
    mBonoboI18n = rb_define_module_under(mBonobo, "I18n");

    rb_define_module_function(mBonoboI18n, "bindtextdomain", bnbi18n_bindtextdomain, 2);
    rb_define_module_function(mBonoboI18n, "textdomain", bnbi18n_textdomain, 1);
    rb_define_module_function(mBonoboI18n, "gettext", bnbi18n_gettext, 1);
    rb_define_module_function(mBonoboI18n, "_", bnbi18n_gettext, 1);
    rb_define_module_function(mBonoboI18n, "gettext_noop", bnbi18n_gettext_noop, 1);
    rb_define_module_function(mBonoboI18n, "N_", bnbi18n_gettext_noop, 1);
    rb_define_module_function(mBonoboI18n, "dgettext", bnbi18n_dgettext, 2);
    rb_define_module_function(mBonoboI18n, "dcgettext", bnbi18n_dcgettext, 3);
}
