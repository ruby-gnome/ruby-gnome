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

#define _SELF(self) (PANGO_FONTSET_SIMPLE(RVAL2GOBJ(self)))
#define RVAL2LANG(v) ((PangoLanguage*)RVAL2BOXED(v, PANGO_TYPE_LANGUAGE))

static VALUE
fontset_simple_initialize(VALUE self, VALUE lang)
{
    G_INITIALIZE(self, pango_fontset_simple_new(RVAL2LANG(lang)));
    return Qnil;
}

static VALUE
fontset_simple_append(VALUE self, VALUE font)
{
    pango_fontset_simple_append(_SELF(self), PANGO_FONT(RVAL2GOBJ(font)));
    return self;
}

static VALUE
fontset_simple_size(VALUE self)
{
    return INT2NUM(pango_fontset_simple_size(_SELF(self)));
}

#endif
void
Init_pango_fontset_simple(void)
{
#ifdef PANGO_ENABLE_BACKEND
    VALUE pFSimple = G_DEF_CLASS(PANGO_TYPE_FONTSET_SIMPLE, "FontsetSimple", mPango);
    
    rb_define_method(pFSimple, "initialize", fontset_simple_initialize, 1);
    rb_define_method(pFSimple, "append", fontset_simple_append, 1);
    rb_define_method(pFSimple, "size", fontset_simple_size, 0);
#endif
}
