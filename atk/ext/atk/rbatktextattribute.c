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
rbatk_tattr_s_register(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_text_attribute_register(RVAL2CSTR(name)), ATK_TYPE_TEXT_ATTRIBUTE);
}

static VALUE
rbatk_tattr_s_for_name(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GENUM2RVAL(atk_text_attribute_for_name(RVAL2CSTR(name)), ATK_TYPE_TEXT_ATTRIBUTE);
}

static VALUE
rbatk_tattr_get_value(VALUE self, VALUE index)
{
    return CSTR2RVAL(atk_text_attribute_get_value(RVAL2GENUM(self, ATK_TYPE_TEXT_ATTRIBUTE), 
                                                  NUM2INT(index)));
}

void
Init_atk_text_attribute(VALUE mText)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_TEXT_ATTRIBUTE, "Attribute", mText);
    G_DEF_CONSTANTS(mText, ATK_TYPE_TEXT_ATTRIBUTE, "ATK_TEXT_");

    rb_define_singleton_method(RG_TARGET_NAMESPACE, "type_register", rbatk_tattr_s_register, 1);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "for_name", rbatk_tattr_s_for_name, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "get_value", rbatk_tattr_get_value, 1);   
}
