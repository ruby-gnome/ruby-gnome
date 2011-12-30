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

#define RG_TARGET_NAMESPACE cButtonField
#define _SELF(obj) RVAL2POPPLERFORMFIELD(obj)

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_type(VALUE self)
{
    return POPPLERFORMBUTTONTYPE2RVAL(poppler_form_field_button_get_button_type(_SELF(self)));
}

static VALUE
rg_active_p(VALUE self)
{
    return CBOOL2RVAL(poppler_form_field_button_get_state(_SELF(self)));
}

static VALUE
rg_set_active(VALUE self, VALUE state)
{
    poppler_form_field_button_set_state(_SELF(self), RVAL2CBOOL(state));
    return Qnil;
}

void
Init_poppler_button_field(VALUE mPoppler, VALUE cFormField)
{
    RG_TARGET_NAMESPACE = rb_define_class_under(mPoppler, "ButtonField", cFormField);

    RG_DEF_METHOD(type, 0);
    RG_DEF_METHOD_P(active, 0);
    RG_DEF_METHOD(set_active, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
