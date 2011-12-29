/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#define RG_TARGET_NAMESPACE mDocument
#define _SELF(s) (RVAL2ATKDOCUMENT(s))

static VALUE
rg_document_type(VALUE self)
{
    return CSTR2RVAL(atk_document_get_document_type(_SELF(self)));
}

/*
How can I implement this?
static VALUE
rg_document(VALUE self)
{
    return GOBJ2RVAL(atk_document_get_document(_SELF(self)));
}
*/

#if ATK_CHECK_VERSION(1,12,0)
static VALUE
rg_get_attribute_value(VALUE self, VALUE name)
{
    return CSTR2RVAL(atk_document_get_attribute_value(_SELF(self), 
                                                      RVAL2CSTR(name)));
}

static VALUE
rg_set_attribute_value(VALUE self, VALUE name, VALUE value)
{
    gboolean ret = atk_document_set_attribute_value(_SELF(self),
                                                    RVAL2CSTR(name),
                                                    RVAL2CSTR(value));

    if (! ret) rb_raise(rb_eRuntimeError, "Can't set attribute value: %s, %s", 
                        RVAL2CSTR(name), RVAL2CSTR(value));

    return self;
}

static VALUE
rg_attributes(VALUE self)
{
    AtkAttributeSet* list = atk_document_get_attributes(_SELF(self));
    VALUE ary = rb_ary_new();
    while (list) {
        AtkAttribute* attr = list->data;
        rb_ary_push(ary, rb_assoc_new(CSTR2RVAL(attr->name), CSTR2RVAL(attr->value)));
        list = list->next;
    }
    return ary;
}

static VALUE
rg_locale(VALUE self)
{
    return CSTR2RVAL(atk_document_get_locale(_SELF(self)));
}
#endif

void
Init_atk_document(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_DOCUMENT, "Document", mAtk);

    RG_DEF_METHOD(document_type, 0);
/*
    RG_DEF_METHOD(document, 0);
*/
#if ATK_CHECK_VERSION(1,12,0)
    RG_DEF_METHOD(get_attribute_value, 1);
    RG_DEF_ALIAS("[]", "get_attribute_value");
    RG_DEF_METHOD(set_attribute_value, 2);
    RG_DEF_ALIAS("[]=", "set_attribute_value");
    RG_DEF_METHOD(attributes, 0);
    RG_DEF_METHOD(locale, 0);
#endif
}
