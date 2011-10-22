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

#if PANGO_CHECK_VERSION(1,2,0)
#define _SELF(r) ((PangoGlyphItem*)RVAL2BOXED(r, PANGO_TYPE_GLYPH_ITEM))

/**********************************/
#ifndef HAVE_PANGO_GLYPH_ITEM_GET_TYPE
static PangoGlyphItem*
glyph_item_copy(const PangoGlyphItem *ref)
{
  PangoGlyphItem* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(PangoGlyphItem, 1);
  *new_ref = *ref;
  return new_ref;
}

#  ifndef HAVE_PANGO_GLYPH_ITEM_FREE
void
pango_glyph_item_free(PangoGlyphItem* glyph_item)
{
    if (glyph_item->item)
        pango_item_free(glyph_item->item);
    if (glyph_item->glyphs)
        pango_glyph_string_free(glyph_item->glyphs);
    g_free(glyph_item);
}
#  endif

GType
pango_glyph_item_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoGlyphItem",
                    (GBoxedCopyFunc)glyph_item_copy,
                    (GBoxedFreeFunc)pango_glyph_item_free);
    return our_type;
}
#endif
/**********************************/

static VALUE
glyph_item_get_item(VALUE self)
{
    PangoItem* item = _SELF(self)->item;
    return BOXED2RVAL(item, PANGO_TYPE_ITEM);
}

static VALUE
glyph_item_get_glyphs(VALUE self)
{
    PangoGlyphString* glyphs = _SELF(self)->glyphs;
    return BOXED2RVAL(glyphs, PANGO_TYPE_GLYPH_STRING);
}

static VALUE
glyph_item_split(VALUE self, VALUE text, VALUE split_index)
{
    return BOXED2RVAL(pango_glyph_item_split(_SELF(self), RVAL2CSTR(text),
                                             NUM2INT(split_index)), PANGO_TYPE_GLYPH_ITEM);
}

static VALUE
glyph_item_apply_attrs(VALUE self, VALUE text, VALUE attrs)
{
    GSList* list = pango_glyph_item_apply_attrs(_SELF(self), RVAL2CSTR(text),
                                                (PangoAttrList*)RVAL2BOXED(attrs, PANGO_TYPE_ATTR_LIST));

    VALUE ret = rb_ary_new();

    while (list) {
        rb_ary_push(ret, BOXED2RVAL(list->data, PANGO_TYPE_GLYPH_ITEM));
        pango_glyph_item_free(list->data); 
        list = list->next;
    }
   
    g_slist_free(list);
    return ret;
}

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
glyph_item_letter_space(VALUE self, VALUE text, VALUE log_attrs, VALUE letter_spacing)
{
    pango_glyph_item_letter_space(_SELF(self), RVAL2CSTR(text),
                                  (PangoLogAttr*)RVAL2BOXED(log_attrs, PANGO_TYPE_LOG_ATTR),
                                  NUM2INT(letter_spacing));
    return self;
}
#endif
#endif

void
Init_pango_glyph_item(void)
{
#if PANGO_CHECK_VERSION(1,2,0)
    VALUE pItem = G_DEF_CLASS(PANGO_TYPE_GLYPH_ITEM, "GlyphItem", mPango);

    rb_define_method(pItem, "item", glyph_item_get_item, 0);
    rb_define_method(pItem, "glyphs", glyph_item_get_glyphs, 0);

    rb_define_method(pItem, "split", glyph_item_split, 2);
    rb_define_method(pItem, "appy_attrs", glyph_item_apply_attrs, 2);

#if PANGO_CHECK_VERSION(1,6,0)
    rb_define_method(pItem, "letter_space", glyph_item_letter_space, 3);
#endif
#endif
}

