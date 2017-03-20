/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#include "rbpangoprivate.h"

#define RG_TARGET_NAMESPACE cAttrIterator
#define _SELF(self) (RVAL2PANGOATTRITERATOR(self))

/**********************************/
GType
pango_attr_iter_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoAttrIterator",
                    (GBoxedCopyFunc)pango_attr_iterator_copy,
                    (GBoxedFreeFunc)pango_attr_iterator_destroy);
    return our_type;
}
/**********************************/

static VALUE
rg_next_bang(VALUE self)
{
    return CBOOL2RVAL(pango_attr_iterator_next(_SELF(self)));
}

static VALUE
rg_range(VALUE self)
{
    gint start = 0;
    gint end = 0;
    pango_attr_iterator_range(_SELF(self), &start, &end);
    return rb_ary_new3(2, INT2NUM(start), INT2NUM(end));
}

static VALUE
rg_get(int argc, VALUE *argv, VALUE self)
{
    VALUE type, ret;
    PangoAttribute* attr;
    int i;

    rb_scan_args(argc, argv, "01", &type);

    if (NIL_P(type)){
        PangoAttrIterator* iter = _SELF(self);
        ret = rb_ary_new();
        for (i = 0; i < PANGO_ATTR_SCALE + 1; i++){
            attr = pango_attr_iterator_get(iter, i);
            if (attr)
                rb_ary_push(ret, ATTR2RVAL(attr));
        }
    } else {
        attr = pango_attr_iterator_get(_SELF(self), FIX2INT(type));
        ret = attr ? ATTR2RVAL(attr) : Qnil;
    }
    return ret;
}

static VALUE
rg_font(VALUE self)
{
    PangoFontDescription* desc;
    PangoLanguage* lang;
    GSList* extra_attrs;
    VALUE ary, ret;

    desc = pango_font_description_new();

    pango_attr_iterator_get_font(_SELF(self), desc, &lang, &extra_attrs);

    ary = rb_ary_new();
    while(extra_attrs) {
        rb_ary_push(ary, ATTR2RVAL(extra_attrs->data));
        extra_attrs = extra_attrs->next;
    }

    ret = rb_ary_new3(3, PANGOFONTDESCRIPTION2RVAL(desc),
                      PANGOLANGUAGE2RVAL(lang),
                      ary);

    pango_font_description_free(desc);
    return ret;
}

#ifdef HAVE_PANGO_ATTR_ITERATOR_GET_ATTRS
static VALUE
rg_attrs(VALUE self)
{
    GSList* list = pango_attr_iterator_get_attrs(_SELF(self));
    GSList* base = list;
    GSList* old = list;
    VALUE ary = rb_ary_new();

    while (list) {
        rb_ary_push(ary, ATTR2RVAL(list->data));
        list = list->next;
    }
    while (old) {
        pango_attribute_destroy((PangoAttribute*)old);
        old = old->next;
    }
    g_slist_free(base);

    return ary;
}  
#endif

void
Init_pango_attriterator(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_ATTR_ITERATOR, "AttrIterator", mPango);

    RG_DEF_METHOD_BANG(next, 0);
    RG_DEF_METHOD(range, 0);
    RG_DEF_METHOD(get, -1);
    RG_DEF_METHOD(font, 0);
#ifdef HAVE_PANGO_ATTR_ITERATOR_GET_ATTRS
    RG_DEF_METHOD(attrs, 0);
#endif
}
