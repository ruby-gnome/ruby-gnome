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

#define RG_TARGET_NAMESPACE cFontMap
#define _SELF(self) (RVAL2PANGOFONTMAP(self))

/*
static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, pango_ft2_font_map_new());
    return Qnil;
}
*/

static VALUE
rg_load_font(VALUE self, VALUE context, VALUE desc)
{
    return GOBJ2RVAL(pango_font_map_load_font(_SELF(self), 
                                              RVAL2PANGOCONTEXT(context),
                                              RVAL2PANGOFONTDESCRIPTION(desc)));
}

static VALUE
rg_load_fontset(VALUE self, VALUE context, VALUE desc, VALUE lang)
{
    return GOBJ2RVAL(pango_font_map_load_fontset(_SELF(self),
                                                 RVAL2PANGOCONTEXT(context),
                                                 RVAL2PANGOFONTDESCRIPTION(desc),
                                                 RVAL2PANGOLANGUAGE(lang)));
}

static VALUE
rg_families(VALUE self)
{
    int n_families;
    PangoFontFamily** families;
    int i;
    VALUE result;

    pango_font_map_list_families(_SELF(self),
                                 &families,
                                 &n_families);

    result = rb_ary_new2(n_families);
    for (i = 0; i < n_families; i++)
      rb_ary_store(result, i, GOBJ2RVAL(families[i]));

    g_free(families);

    return result;
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
rg_shape_engine_type(VALUE self)
{
    return CSTR2RVAL(pango_font_map_get_shape_engine_type(_SELF(self)));
}
#endif

void
Init_pango_font_map(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_FONT_MAP, "FontMap", mPango);

     /*
    RG_DEF_METHOD(initialize, 0);
     */
    RG_DEF_METHOD(load_font, 2);
    RG_DEF_METHOD(load_fontset, 3);
    RG_DEF_METHOD(families, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    RG_DEF_METHOD(shape_engine_type, 0);
#endif

    G_DEF_CLASS3("PangoFcFontMap", "FcFontMap", mPango);
    G_DEF_CLASS3("PangoXftFontMap", "XftFontMap", mPango);
    G_DEF_CLASS3("PangoFT2FontMap", "FT2FontMap", mPango);
    G_DEF_CLASS3("PangoXFontMap", "XFontMap", mPango);
    G_DEF_CLASS3("PangoWin32FontMap", "Win32FontMap", mPango);
    G_DEF_CLASS3("PangoCairoFcFontMap", "CairoFcFontMap", mPango);
    G_DEF_CLASS3("PangoCairoWin32FontMap", "CairoWin32FontMap", mPango);
#if PANGO_CHECK_VERSION(1,12,0)
    G_DEF_CLASS3("PangoATSUIFontMap", "ATSUIFontMap", mPango);
#endif
}
