/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004 Masao Mutoh
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

#include "rbatk.h"

#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#define _SELF(r) ((AtkTextRange*)RVAL2BOXED(r, ATK_TYPE_TEXT_RANGE))

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
atktextrange_bounds(VALUE self)
{
    return BOXED2RVAL(&_SELF(self)->bounds, ATK_TYPE_TEXT_RECTANGLE);
}

static VALUE
atktextrange_start_offset(VALUE self)
{
    return INT2NUM(_SELF(self)->start_offset);
}

static VALUE
atktextrange_end_offset(VALUE self)
{
    return INT2NUM(_SELF(self)->end_offset);
}

static VALUE
atktextrange_content(VALUE self)
{
    return CSTR2RVAL(_SELF(self)->content);
}
#endif

void
Init_atk_text_range()
{
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
    VALUE range = G_DEF_CLASS(ATK_TYPE_TEXT_RANGE, "TextRange",  mAtk);

    rb_define_method(range, "bounds", atktextrange_bounds, 0);
    rb_define_method(range, "start_offset", atktextrange_start_offset, 0);
    rb_define_method(range, "end_offset", atktextrange_end_offset, 0);
    rb_define_method(range, "content", atktextrange_content, 0);
#endif
}
