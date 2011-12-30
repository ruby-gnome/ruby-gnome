/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
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

#define RG_TARGET_NAMESPACE cFontFace
#define _SELF(self) (RVAL2PANGOFONTFACE(self))

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(pango_font_face_get_face_name(_SELF(self)));
}

static VALUE
rg_describe(VALUE self)
{
    return PANGOFONTDESCRIPTION2RVAL(pango_font_face_describe(_SELF(self)));
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
rg_sizes(VALUE self)
{
    int n_sizes;
    int* sizes;
    int i;
    VALUE result;

    pango_font_face_list_sizes(_SELF(self),
                               &sizes,
                               &n_sizes);

    result = rb_ary_new();
    for (i = 0; i < n_sizes; i++)
      rb_ary_push(result, GOBJ2RVAL(&sizes[i]));

    g_free(sizes);
    return result;
}
#endif

void
Init_pango_font_face(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_FONT_FACE, "FontFace", mPango);

    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(describe, 0);
#if PANGO_CHECK_VERSION(1,4,0)
    RG_DEF_METHOD(sizes, 0);
#endif
    G_DEF_CLASS3("PangoFcFace", "FcFace", mPango);
    G_DEF_CLASS3("PangoFT2Face", "FT2Face", mPango);
    G_DEF_CLASS3("PangoXftFace", "XftFace", mPango);
    G_DEF_CLASS3("PangoXFace", "XFace", mPango);
    G_DEF_CLASS3("PangoWin32Face", "Win32Face", mPango);
#if PANGO_CHECK_VERSION(1,12,0)
    G_DEF_CLASS3("PangoATSUIFace", "ATSUIFace", mPango);
#endif
}
