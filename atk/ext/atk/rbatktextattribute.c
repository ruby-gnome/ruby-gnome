/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#include "rbatkprivate.h"

#define RG_TARGET_NAMESPACE cAttribute

static VALUE
rg_s_type_register(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return ATKTEXTATTRIBUTE2RVAL(atk_text_attribute_register(RVAL2CSTR(name)));
}

static VALUE
rg_s_for_name(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return ATKTEXTATTRIBUTE2RVAL(atk_text_attribute_for_name(RVAL2CSTR(name)));
}

static VALUE
rg_get_value(VALUE self, VALUE index)
{
    return CSTR2RVAL(atk_text_attribute_get_value(RVAL2ATKTEXTATTRIBUTE(self), 
                                                  NUM2INT(index)));
}

void
Init_atk_text_attribute(VALUE mText)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_TEXT_ATTRIBUTE, "Attribute", mText);
    G_DEF_CONSTANTS(mText, ATK_TYPE_TEXT_ATTRIBUTE, "ATK_TEXT_");

    RG_DEF_SMETHOD(type_register, 1);
    RG_DEF_SMETHOD(for_name, 1);
    RG_DEF_METHOD(get_value, 1);   
}
