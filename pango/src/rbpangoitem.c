/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoitem.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoItem*)RVAL2BOXED(self, PANGO_TYPE_ITEM))

#define ATTR_INT(name)\
static VALUE \
item_int_ ## name (self)\
    VALUE self;\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
item_int_set_ ## name (self, val)\
    VALUE self, val;\
{\
    _SELF(self)->name = NUM2INT(val); \
    return self;\
}

#define DEF_INT_ACCESSOR(name)                                  \
    rb_define_method(pItem, G_STRINGIFY(name), item_int_ ## name, 0);\
    rb_define_method(pItem, G_STRINGIFY(set_ ## name), item_int_set_ ## name, 1);

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
/* Move to Pango module (rbpangomain.c)
GList*      pango_reorder_items             (GList *logical_items);
*/

ATTR_INT(offset);
ATTR_INT(length);
ATTR_INT(num_chars);


static VALUE
item_get_analysis(self)
    VALUE self;
{
    PangoAnalysis ana = _SELF(self)->analysis;
    return BOXED2RVAL(&ana, PANGO_TYPE_ANALYSIS);
}

static VALUE
item_set_analysis(self, val)
    VALUE self, val;
{
    PangoAnalysis* ana = (PangoAnalysis*)RVAL2BOXED(val, PANGO_TYPE_ANALYSIS);
    _SELF(self)->analysis = *ana;
    return self;
}

void
Init_pango_item()
{
    VALUE pItem = G_DEF_CLASS(PANGO_TYPE_ITEM, "Item", mPango);
    
    rb_define_method(pItem, "initialize", item_initialize, 0);
    rb_define_method(pItem, "split", item_split, 2);

    DEF_INT_ACCESSOR(offset);
    DEF_INT_ACCESSOR(length);
    DEF_INT_ACCESSOR(num_chars);

    rb_define_method(pItem, "analysis", item_get_analysis, 0);
    rb_define_method(pItem, "set_analysis", item_set_analysis, 1);

    G_DEF_SETTERS(pItem);
}
