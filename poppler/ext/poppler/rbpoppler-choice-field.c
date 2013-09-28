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

#define RG_TARGET_NAMESPACE cChoiceField
#define _SELF(obj) RVAL2POPPLERFORMFIELD(obj)

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_type(VALUE self)
{
    return POPPLERFORMCHOICETYPE2RVAL(poppler_form_field_choice_get_choice_type(_SELF(self)));
}

static VALUE
rg_editable_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_editable(_SELF(self)));
}

static VALUE
rg_select_multiple_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_can_select_multiple(_SELF(self)));
}

static VALUE
rg_spell_check_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_do_spell_check(_SELF(self)));
}

static VALUE
rg_commit_on_change_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_commit_on_change(_SELF(self)));
}

static VALUE
rg_n_items(VALUE self)
{
    return INT2NUM(poppler_form_field_choice_get_n_items(_SELF(self)));
}

static VALUE
rg_operator_choice_field_get_item(VALUE self, VALUE index)
{
    return CSTR2RVAL(poppler_form_field_choice_get_item(_SELF(self),
                                                        NUM2INT(index)));
}

static VALUE
rg_selected_p(VALUE self, VALUE index)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_item_selected(_SELF(self),
                                                                 NUM2INT(index)));
}

static VALUE
rg_select(VALUE self, VALUE index)
{
    poppler_form_field_choice_select_item(_SELF(self), NUM2INT(index));
    return Qnil;
}

static VALUE
rg_unselect_all(VALUE self)
{
    poppler_form_field_choice_unselect_all(_SELF(self));
    return Qnil;
}

static VALUE
rg_toggle(VALUE self, VALUE index)
{
    poppler_form_field_choice_toggle_item(_SELF(self), NUM2INT(index));
    return Qnil;
}

static VALUE
rg_set_text(VALUE self, VALUE text)
{
    poppler_form_field_choice_set_text(_SELF(self), RVAL2CSTR_ACCEPT_NIL(text));
    return Qnil;
}

static VALUE
rg_text(VALUE self)
{
    return CSTR2RVAL(poppler_form_field_choice_get_text(_SELF(self)));
}

void
Init_poppler_choice_field(VALUE mPoppler, VALUE cFormField)
{
    RG_TARGET_NAMESPACE = rb_define_class_under(mPoppler, "ChoiceField", cFormField);

    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD_P(editable, 0);
    RG_DEF_METHOD_P(select_multiple, 0);
    RG_DEF_METHOD_P(spell_check, 0);
    RG_DEF_METHOD_P(commit_on_change, 0);
    RG_DEF_METHOD(n_items, 0);
    RG_DEF_METHOD_OPERATOR("[]", choice_field_get_item, 1);
    RG_DEF_METHOD_P(selected, 1);
    RG_DEF_METHOD(select, 1);
    RG_DEF_METHOD(unselect_all, 0);
    RG_DEF_METHOD(toggle, 1);
    RG_DEF_METHOD(text, 0);
    RG_DEF_METHOD(set_text, 1);
}
