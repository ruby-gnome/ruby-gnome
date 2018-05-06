/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017-2018  Ruby-GNOME2 Project Team
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

#include "rb-pango-private.h"

#define RG_TARGET_NAMESPACE cAttrList

static VALUE
rg_insert(VALUE self, VALUE rb_attribute)
{
    PangoAttribute *attribute;

    if (NIL_P(rb_attribute))
        return self;

    attribute = rbpango_attribute_from_ruby(rb_attribute);
    pango_attr_list_insert(RVAL2PANGOATTRLIST(self),
                           pango_attribute_copy(attribute));

    return self;
}

static VALUE
rg_insert_before(VALUE self, VALUE rb_attribute)
{
    PangoAttribute *attribute;

    if (NIL_P(rb_attribute))
        return self;

    attribute = rbpango_attribute_from_ruby(rb_attribute);
    pango_attr_list_insert_before(RVAL2PANGOATTRLIST(self),
                                  pango_attribute_copy(attribute));

    return self;
}

static VALUE
rg_iterator(VALUE self)
{
    PangoAttrIterator *iterator;

    iterator = pango_attr_list_get_iterator(RVAL2PANGOATTRLIST(self));
    return PANGOATTRITERATOR2RVAL(iterator);
}

void
rbpango_attr_list_init(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_ATTR_LIST, "AttrList", mPango);

    RG_DEF_METHOD(insert, 1);
    RG_DEF_METHOD(insert_before, 1);
    RG_DEF_METHOD(iterator, 0);
}
