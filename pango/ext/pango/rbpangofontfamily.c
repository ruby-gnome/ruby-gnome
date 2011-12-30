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

#define RG_TARGET_NAMESPACE cFontFamily
#define _SELF(self) (RVAL2PANGOFONTFAMILY(self))

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(pango_font_family_get_name(_SELF(self)));
}

#ifdef HAVE_PANGO_FONT_FAMILY_IS_MONOSPACE
static VALUE
rg_monospace_p(VALUE self)
{
    return CBOOL2RVAL(pango_font_family_is_monospace(_SELF(self)));
}
#endif

static VALUE
rg_faces(VALUE self)
{
    int n_faces;
    PangoFontFace** faces;
    int i;
    VALUE result;

    pango_font_family_list_faces(_SELF(self),
                                 &faces,
                                 &n_faces);

    result = rb_ary_new2(n_faces);
    for (i = 0; i < n_faces; i++)
      rb_ary_store(result, i, GOBJ2RVAL(faces[i]));

    g_free(faces);

    return result;
}

void
Init_pango_font_family(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_FONT_FAMILY, "FontFamily", mPango);

    RG_DEF_METHOD(name, 0);
#ifdef HAVE_PANGO_FONT_FAMILY_IS_MONOSPACE
    RG_DEF_METHOD_P(monospace, 0);
#endif
    RG_DEF_METHOD(faces, 0);

    G_DEF_CLASS3("PangoFcFamily", "FcFamily", mPango);
    G_DEF_CLASS3("PangoFT2Family", "FT2Family", mPango);
    G_DEF_CLASS3("PangoXftFamily", "XftFamily", mPango);
    G_DEF_CLASS3("PangoXFamily", "XFamily", mPango);
    G_DEF_CLASS3("PangoWin32Family", "Win32Family", mPango);
#if PANGO_CHECK_VERSION(1,12,0)
    G_DEF_CLASS3("PangoATSUIFamily", "ATSUIFamily", mPango);
#endif

}
