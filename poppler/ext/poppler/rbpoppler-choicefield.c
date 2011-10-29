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

#define RG_TARGET_NAMESPACE rg_cChoiceField
#define _SELF(obj) RVAL2POPPLER_FORM_FIELD(obj)

#define FCT2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_FORM_CHOICE_TYPE))

VALUE RG_TARGET_NAMESPACE;

static VALUE
choice_field_get_choice_type(VALUE self)
{
    return FCT2RVAL(poppler_form_field_choice_get_choice_type(_SELF(self)));
}

static VALUE
choice_field_is_editable(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_editable(_SELF(self)));
}

static VALUE
choice_field_can_select_multiple(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_can_select_multiple(_SELF(self)));
}

static VALUE
choice_field_do_spell_check(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_do_spell_check(_SELF(self)));
}

static VALUE
choice_field_commit_on_change(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_choice_commit_on_change(_SELF(self)));
}

static VALUE
choice_field_get_n_items(VALUE self)
{
    return INT2NUM(poppler_form_field_choice_get_n_items(_SELF(self)));
}

static VALUE
choice_field_get_item(VALUE self, VALUE index)
{
    return CSTR2RVAL(poppler_form_field_choice_get_item(_SELF(self),
                                                        NUM2INT(index)));
}

static VALUE
choice_field_is_item_selected(VALUE self, VALUE index)
{
    return CBOOL2RVAL(poppler_form_field_choice_is_item_selected(_SELF(self),
                                                                 NUM2INT(index)));
}

static VALUE
choice_field_select_item(VALUE self, VALUE index)
{
    poppler_form_field_choice_select_item(_SELF(self), NUM2INT(index));
    return Qnil;
}

static VALUE
choice_field_unselect_all(VALUE self)
{
    poppler_form_field_choice_unselect_all(_SELF(self));
    return Qnil;
}

static VALUE
choice_field_toggle_item(VALUE self, VALUE index)
{
    poppler_form_field_choice_toggle_item(_SELF(self), NUM2INT(index));
    return Qnil;
}

static VALUE
choice_field_set_text(VALUE self, VALUE text)
{
    poppler_form_field_choice_set_text(_SELF(self), RVAL2CSTR_ACCEPT_NIL(text));
    return Qnil;
}

static VALUE
choice_field_get_text(VALUE self)
{
    return CSTR2RVAL(poppler_form_field_choice_get_text(_SELF(self)));
}

void
Init_poppler_choice_field(VALUE mPoppler, VALUE cFormField)
{
    RG_TARGET_NAMESPACE = rb_define_class_under(mPoppler, "ChoiceField", cFormField);

    rb_define_method(RG_TARGET_NAMESPACE, "type", choice_field_get_choice_type, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "editable?", choice_field_is_editable, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "select_multiple?",
                     choice_field_can_select_multiple, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "spell_check?",
                     choice_field_do_spell_check, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "commit_on_change?",
                     choice_field_commit_on_change, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "n_items", choice_field_get_n_items, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "[]", choice_field_get_item, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "selected?",
                     choice_field_is_item_selected, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "select", choice_field_select_item, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "unselect_all", choice_field_unselect_all, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "toggle", choice_field_toggle_item, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "text", choice_field_get_text, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_text", choice_field_set_text, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}

