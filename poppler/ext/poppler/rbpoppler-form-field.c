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

#define RG_TARGET_NAMESPACE cFormField
#define _SELF(obj) RVAL2POPPLERFORMFIELD(obj)

static VALUE cButtonField, cTextField, cChoiceField;
static VALUE cUnknownField, cSignatureField;

VALUE
rb_poppler_ruby_object_from_form_field(PopplerFormField *field)
{
    VALUE obj;

    obj = rbgobj_ruby_object_from_instance2(field, FALSE);
    if (NIL_P(obj)) {
      switch (poppler_form_field_get_field_type(field)) {
        case POPPLER_FORM_FIELD_UNKNOWN:
          obj = rbgobj_create_object(cUnknownField);
          break;
        case POPPLER_FORM_FIELD_BUTTON:
          obj = rbgobj_create_object(cButtonField);
          break;
        case POPPLER_FORM_FIELD_TEXT:
          obj = rbgobj_create_object(cTextField);
          break;
        case POPPLER_FORM_FIELD_CHOICE:
          obj = rbgobj_create_object(cChoiceField);
          break;
        case POPPLER_FORM_FIELD_SIGNATURE:
          obj = rbgobj_create_object(cSignatureField);
          break;
      }
      g_object_ref(field);
      G_INITIALIZE(obj, (gpointer)field);
    }

    return obj;
}

static VALUE
rg_id(VALUE self)
{
    return INT2NUM(poppler_form_field_get_id(_SELF(self)));
}

static VALUE
rg_font_size(VALUE self)
{
    return rb_float_new(poppler_form_field_get_font_size(_SELF(self)));
}

static VALUE
rg_read_only_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_is_read_only(_SELF(self)));
}

void
Init_poppler_form_field(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD, "FormField", mPoppler);
    cUnknownField = rb_define_class_under(mPoppler, "UnknownField", RG_TARGET_NAMESPACE);
    cSignatureField = rb_define_class_under(mPoppler, "SignatureField",
                                            RG_TARGET_NAMESPACE);

    RG_DEF_METHOD(id, 0);
    RG_DEF_METHOD(font_size, 0);
    RG_DEF_METHOD_P(read_only, 0);

    Init_poppler_button_field(mPoppler, RG_TARGET_NAMESPACE);
    Init_poppler_text_field(mPoppler, RG_TARGET_NAMESPACE);
    Init_poppler_choice_field(mPoppler, RG_TARGET_NAMESPACE);

    cButtonField = rb_const_get(mPoppler, rb_intern("ButtonField"));
    cTextField = rb_const_get(mPoppler, rb_intern("TextField"));
    cChoiceField = rb_const_get(mPoppler, rb_intern("ChoiceField"));
}
