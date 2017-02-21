/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
 *  Copyright (C) 2002,2003 Masao Mutoh
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "rbatkprivate.h"

#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#define RG_TARGET_NAMESPACE cTextRange
#define _SELF(r) (RVAL2ATKTEXTRANGE(r))

/**********************************/
static AtkTextRange*
atk_text_range_copy(const AtkTextRange* val)
{
    AtkTextRange* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(AtkTextRange, 1);
    *new_val = *val;
    return new_val;
}

GType
atk_text_range_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("AtkTextRange",
                    (GBoxedCopyFunc)atk_text_range_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/
/* Struct accessors */
static VALUE
rg_bounds(VALUE self)
{
    return ATKTEXTRECTANGLE2RVAL(&_SELF(self)->bounds);
}

static VALUE
rg_start_offset(VALUE self)
{
    return INT2NUM(_SELF(self)->start_offset);
}

static VALUE
rg_end_offset(VALUE self)
{
    return INT2NUM(_SELF(self)->end_offset);
}

static VALUE
rg_content(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->content);
}
#endif

void
Init_atk_text_range(VALUE mAtk)
{
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(ATK_TYPE_TEXT_RANGE, "TextRange",  mAtk);

    RG_DEF_METHOD(bounds, 0);
    RG_DEF_METHOD(start_offset, 0);
    RG_DEF_METHOD(end_offset, 0);
    RG_DEF_METHOD(content, 0);
#endif
}
