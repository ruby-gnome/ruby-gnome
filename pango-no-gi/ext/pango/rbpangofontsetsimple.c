/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#ifdef PANGO_ENABLE_BACKEND

#define RG_TARGET_NAMESPACE cFontsetSimple
#define _SELF(self) (RVAL2PANGOFONTSETSIMPLE(self))

static VALUE
rg_initialize(VALUE self, VALUE lang)
{
    G_INITIALIZE(self, pango_fontset_simple_new(RVAL2PANGOLANGUAGE(lang)));
    return Qnil;
}

static VALUE
rg_append(VALUE self, VALUE font)
{
    pango_fontset_simple_append(_SELF(self), RVAL2PANGOFONT(font));
    return self;
}

static VALUE
rg_size(VALUE self)
{
    return INT2NUM(pango_fontset_simple_size(_SELF(self)));
}

#endif
void
Init_pango_fontset_simple(VALUE mPango)
{
#ifdef PANGO_ENABLE_BACKEND
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_FONTSET_SIMPLE, "FontsetSimple", mPango);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(append, 1);
    RG_DEF_METHOD(size, 0);
#endif
}
