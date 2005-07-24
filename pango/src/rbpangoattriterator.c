/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoattriterator.c -

  $Author: mutoh $
  $Date: 2005/07/24 07:00:32 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoAttrIterator*)RVAL2BOXED(self, PANGO_TYPE_ATTR_ITERATOR))

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
attriterator_next(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_attr_iterator_next(_SELF(self)));
}

static VALUE
attriterator_range(self)
    VALUE self;
{
    gint start = 0;
    gint end = 0;
    pango_attr_iterator_range(_SELF(self), &start, &end);
    return rb_ary_new3(2, INT2NUM(start), INT2NUM(end));
}

static VALUE
attriterator_get(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
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
attriterator_get_font(self)
    VALUE self;
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

    ret = rb_ary_new3(3, (PangoFontDescription*)BOXED2RVAL(desc, PANGO_TYPE_FONT_DESCRIPTION),
                      (PangoLanguage*)BOXED2RVAL(lang, PANGO_TYPE_LANGUAGE),
                      ary);

    pango_font_description_free(desc);
    return ret;
}

#ifdef HAVE_PANGO_ATTR_ITERATOR_GET_ATTRS
static VALUE
attriterator_get_attrs(self)
    VALUE self;
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
Init_pango_attriterator()
{
    VALUE pAttriterator = G_DEF_CLASS(PANGO_TYPE_ATTR_ITERATOR, "AttrIterator", mPango);
    
    rb_define_method(pAttriterator, "next!", attriterator_next, 0);
    rb_define_method(pAttriterator, "range", attriterator_range, 0);
    rb_define_method(pAttriterator, "get", attriterator_get, -1);
    rb_define_method(pAttriterator, "font", attriterator_get_font, 0);
#ifdef HAVE_PANGO_ATTR_ITERATOR_GET_ATTRS
    rb_define_method(pAttriterator, "attrs", attriterator_get_attrs, 0);
#endif
}
