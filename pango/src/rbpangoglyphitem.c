/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoglyphitem.c -

  $Author: mutoh $
  $Date: 2005/03/05 16:23:46 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "rbpango.h"


#define _SELF(r) ((PangoGlyphItem*)RVAL2BOXED(r, PANGO_TYPE_GLYPH_ITEM))

/**********************************/
static PangoGlyphItem*
glyph_item_copy(ref)
    const PangoGlyphItem* ref;
{
  PangoGlyphItem* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(PangoGlyphItem, 1);
  *new_ref = *ref;
  return new_ref;
}

#if ! HAVE_PANGO_GLYPH_ITEM_FREE
static void
pango_glyph_item_free(PangoGlyphItem* glyph_item)
{
    if (glyph_item->item)
        pango_item_free(glyph_item->item);
    if (glyph_item->glyphs)
        pango_glyph_string_free(glyph_item->glyphs);
    g_free(glyph_item);
}
#endif

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
/**********************************/

static VALUE
glyph_item_get_item(self)
    VALUE self;
{
    PangoItem* item = _SELF(self)->item;
    return item ? BOXED2RVAL(item, PANGO_TYPE_ITEM) : Qnil;
}

static VALUE
glyph_item_get_glyphs(self)
    VALUE self;
{
    PangoGlyphString* glyphs = _SELF(self)->glyphs;
    return glyphs ? BOXED2RVAL(glyphs, PANGO_TYPE_GLYPH_STRING) : Qnil;
}

void
Init_pango_glyph_item()
{
    VALUE pItem = G_DEF_CLASS(PANGO_TYPE_GLYPH_ITEM, "GlyphItem", mPango);

    rb_define_method(pItem, "item", glyph_item_get_item, 0);
    rb_define_method(pItem, "glyphs", glyph_item_get_glyphs, 0);
}
