/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoitem.c -

  $Author: geoff_youngs $
  $Date: 2004/11/06 10:33:40 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoItem*)RVAL2BOXED(self, PANGO_TYPE_ITEM))

/**********************************/
GType
pango_item_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoItem",
                    (GBoxedCopyFunc)pango_item_copy,
                    (GBoxedFreeFunc)pango_item_free);
    return our_type;
}
/**********************************/

static VALUE
item_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, pango_item_new());
    return Qnil;
}

static VALUE
item_split(self, split_index, split_offset)
    VALUE self, split_index, split_offset;
{
    return BOXED2RVAL(pango_item_split(_SELF(self), 
                                       NUM2INT(split_index), 
                                       NUM2INT(split_offset)), 
                      PANGO_TYPE_ITEM);
}
/*
GList*      pango_reorder_items             (GList *logical_items);
*/
void
Init_pango_item()
{
    VALUE pItem = G_DEF_CLASS(PANGO_TYPE_ITEM, "Item", mPango);
    
    rb_define_method(pItem, "initialize", item_initialize, 1);
    rb_define_method(pItem, "split", item_split, 2);

}
