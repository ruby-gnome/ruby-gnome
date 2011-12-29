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
#define RG_TARGET_NAMESPACE cGlyphItem
#define _SELF(r) (RVAL2PANGOGLYPHITEM(r))

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
rg_item(VALUE self)
{
    PangoItem* item = _SELF(self)->item;
    return PANGOITEM2RVAL(item);
}

static VALUE
rg_glyphs(VALUE self)
{
    PangoGlyphString* glyphs = _SELF(self)->glyphs;
    return PANGOGLYPHSTRING2RVAL(glyphs);
}

static VALUE
rg_split(VALUE self, VALUE text, VALUE split_index)
{
    return PANGOGLYPHITEM2RVAL(pango_glyph_item_split(_SELF(self), RVAL2CSTR(text),
                                                      NUM2INT(split_index)));
}

static VALUE
rg_appy_attrs(VALUE self, VALUE text, VALUE attrs)
{
    GSList* list = pango_glyph_item_apply_attrs(_SELF(self), RVAL2CSTR(text),
                                                RVAL2PANGOATTRLIST(attrs));

    VALUE ret = rb_ary_new();

    while (list) {
        rb_ary_push(ret, PANGOGLYPHITEM2RVAL(list->data));
        pango_glyph_item_free(list->data); 
        list = list->next;
    }

    g_slist_free(list);
    return ret;
}

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
rg_letter_space(VALUE self, VALUE text, VALUE log_attrs, VALUE letter_spacing)
{
    pango_glyph_item_letter_space(_SELF(self), RVAL2CSTR(text),
                                  RVAL2PANGOLOGATTR(log_attrs),
                                  NUM2INT(letter_spacing));
    return self;
}
#endif
#endif

void
Init_pango_glyph_item(VALUE mPango)
{
#if PANGO_CHECK_VERSION(1,2,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_GLYPH_ITEM, "GlyphItem", mPango);

    RG_DEF_METHOD(item, 0);
    RG_DEF_METHOD(glyphs, 0);

    RG_DEF_METHOD(split, 2);
    RG_DEF_METHOD(appy_attrs, 2);

#if PANGO_CHECK_VERSION(1,6,0)
    RG_DEF_METHOD(letter_space, 3);
#endif
#endif
}
