/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

   rbbonobo-i18n.c -

   $Author: mutoh $
   $Date: 2002/12/22 14:27:43 $

	Copyright (C) 2002 KUBO Takehiro <kubo@jiubao.org>

	This file was renamed from rbgnome-i18n.c.
	rbgnome-i18n.c -
	Copyright (C) 1999 Minoru Inachi <inachi@earth.interq.or.jp>
************************************************/

#include "rbbonobo.h"

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
Init_bonobo_i18n()
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
