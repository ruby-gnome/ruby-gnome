/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cAttrIterator
#define _SELF(self) (RVAL2PANGOATTRITERATOR(self))

G_DEFINE_BOXED_TYPE(PangoAttrIterator,
                    pango_attr_iterator,
                    pango_attr_iterator_copy,
                    pango_attr_iterator_destroy);

static VALUE
rg_attrs(VALUE self)
{
    GSList *attrs;
    GSList *node;
    VALUE rb_attrs;

    attrs = pango_attr_iterator_get_attrs(_SELF(self));
    rb_attrs = rb_ary_new();
    for (node = attrs; node; node = g_slist_next(node)) {
        PangoAttribute *attr = node->data;
        VALUE rb_attr;

        rb_attr = rbpango_attribute_to_ruby(attr);
        rb_ary_push(rb_attrs, rb_attr);
    }
    g_slist_free(attrs);

    return rb_attrs;
}

void
rbpango_attr_iterator_init(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE =
        G_DEF_CLASS(PANGO_TYPE_ATTR_ITERATOR, "AttrIterator", mPango);
    RG_DEF_METHOD(attrs, 0);
}
