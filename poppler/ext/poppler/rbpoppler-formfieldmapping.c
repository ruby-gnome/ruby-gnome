/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cFormFieldMapping

DEF_ACCESSOR_WITH_SETTER(form_field_mapping, area,
                         RVAL2POPPLERFORMFIELDMAPPING, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(form_field_mapping, field, RVAL2POPPLERFORMFIELDMAPPING,
             POPPLERFORMFIELD2RVAL, RVAL2POPPLERFORMFIELD)

void
Init_poppler_formfieldmapping(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_FORM_FIELD_MAPPING,
                                    "FormFieldMapping", mPoppler);

    rb_define_method(RG_TARGET_NAMESPACE, "area", form_field_mapping_get_area, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "field", form_field_mapping_get_field,
                     0);

    rb_define_method(RG_TARGET_NAMESPACE, "set_area",
                     form_field_mapping_set_area, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_field",
                     form_field_mapping_set_field, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
