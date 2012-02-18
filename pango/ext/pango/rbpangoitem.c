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

#define RG_TARGET_NAMESPACE cItem
#define _SELF(self) (RVAL2PANGOITEM(self))

#define ATTR_INT(name)\
static VALUE \
item_int_ ## name (VALUE self)\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
item_int_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = NUM2INT(val); \
    return self;\
}

#define DEF_INT_ACCESSOR(name)                                  \
    rbg_define_method(RG_TARGET_NAMESPACE, G_STRINGIFY(name), item_int_ ## name, 0);\
    rbg_define_method(RG_TARGET_NAMESPACE, G_STRINGIFY(set_ ## name), item_int_set_ ## name, 1);

/**********************************/
#if ! PANGO_CHECK_VERSION(1,9,0)
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
#endif
/**********************************/

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, pango_item_new());
    return Qnil;
}

static VALUE
rg_split(VALUE self, VALUE split_index, VALUE split_offset)
{
    return PANGOITEM2RVAL(pango_item_split(_SELF(self), 
                                           NUM2INT(split_index), 
                                           NUM2INT(split_offset)));
}
/* Move to Pango module (rbpangomain.c)
GList*      pango_reorder_items             (GList *logical_items);
*/

ATTR_INT(offset);
ATTR_INT(length);
ATTR_INT(num_chars);

static VALUE
rg_analysis(VALUE self)
{
    PangoAnalysis ana = _SELF(self)->analysis;
    return PANGOANALYSIS2RVAL(&ana);
}

static VALUE
rg_set_analysis(VALUE self, VALUE val)
{
    PangoAnalysis* ana = RVAL2PANGOANALYSIS(val);
    _SELF(self)->analysis = *ana;
    return self;
}

void
Init_pango_item(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_ITEM, "Item", mPango);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(split, 2);

    DEF_INT_ACCESSOR(offset);
    DEF_INT_ACCESSOR(length);
    DEF_INT_ACCESSOR(num_chars);

    RG_DEF_METHOD(analysis, 0);
    RG_DEF_METHOD(set_analysis, 1);
}
