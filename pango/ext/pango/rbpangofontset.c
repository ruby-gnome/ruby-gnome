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

#define _SELF(self) (PANGO_FONTSET(RVAL2GOBJ(self)))

static ID id_call;

static VALUE
fontset_get_font(VALUE self, VALUE wc)
{
    return GOBJ2RVAL(pango_fontset_get_font(_SELF(self), NUM2UINT(wc)));
}

static VALUE
fontset_get_metrics(VALUE self)
{
    return BOXED2RVAL(pango_fontset_get_metrics(_SELF(self)), PANGO_TYPE_FONT_METRICS);
}

#if PANGO_CHECK_VERSION(1,4,0)
static gboolean
fontset_each(PangoFontset *fontset, PangoFont *font, gpointer func)
{
    return RVAL2CBOOL(rb_funcall((VALUE)func, id_call, 
                            2, GOBJ2RVAL(fontset), GOBJ2RVAL(font)));
}

static VALUE
fontset_foreach(VALUE self)
{
    VALUE func = rb_block_proc();
    G_RELATIVE(self, func);
    pango_fontset_foreach(_SELF(self), 
                          (PangoFontsetForeachFunc)fontset_each, 
                          (gpointer)func);
    return self;
}
#endif

void
Init_pango_fontset(void)
{
    VALUE pFontSet = G_DEF_CLASS(PANGO_TYPE_FONTSET, "Fontset", mPango);
    
    id_call = rb_intern("call");

    rb_define_method(pFontSet, "get_font", fontset_get_font, 1);
    rb_define_method(pFontSet, "metrics", fontset_get_metrics, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    rb_define_method(pFontSet, "each", fontset_foreach, 0);
#endif
}
