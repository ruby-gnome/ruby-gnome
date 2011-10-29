/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE rg_cTextField
#define _SELF(obj) RVAL2POPPLER_FORM_FIELD(obj)

#define FTT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_TEXT_TYPE))

VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_type(VALUE self)
{
    return FTT2RVAL(poppler_form_field_text_get_text_type(_SELF(self)));
}

static VALUE
rg_text(VALUE self)
{
    return CSTR2RVAL(poppler_form_field_text_get_text(_SELF(self)));
}

static VALUE
rg_set_text(VALUE self, VALUE text)
{
    poppler_form_field_text_set_text(_SELF(self), RVAL2CSTR_ACCEPT_NIL(text));
    return Qnil;
}

static VALUE
rg_max_length(VALUE self)
{
    return INT2NUM(poppler_form_field_text_get_max_len(_SELF(self)));
}

static VALUE
rg_spell_check_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_do_spell_check(_SELF(self)));
}

static VALUE
rg_scroll_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_do_scroll(_SELF(self)));
}

static VALUE
rg_rich_text_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_is_rich_text(_SELF(self)));
}

static VALUE
rg_password_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_text_is_password(_SELF(self)));
}

void
Init_poppler_text_field(VALUE mPoppler, VALUE cFormField)
{
    RG_TARGET_NAMESPACE = rb_define_class_under(mPoppler, "TextField", cFormField);

    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD(text, 0);
    RG_DEF_METHOD(set_text, 1);
    RG_DEF_METHOD(max_length, 0);
    RG_DEF_METHOD_P(spell_check, 0);
    RG_DEF_METHOD_P(scroll, 0);
    RG_DEF_METHOD_P(rich_text, 0);
    RG_DEF_METHOD_P(password, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
