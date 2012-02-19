/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cGlyphInfo
#define _SELF(self) (RVAL2PANGOGLYPHINFO(self))

/**********************************/
static PangoGlyphInfo *
pango_glyph_info_copy(const PangoGlyphInfo *info)
{
  PangoGlyphInfo *data;

  g_return_val_if_fail(info != NULL, NULL);

  data = g_new(PangoGlyphInfo, 1);
  *data = *info;

  return data;
}

GType
pango_glyph_info_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoGlyphInfo",
                    (GBoxedCopyFunc)pango_glyph_info_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, g_new0(PangoGlyphInfo, 1));
    return Qnil;
}

static VALUE
rg_glyph(VALUE self)
{
    return UINT2NUM(_SELF(self)->glyph);
}

static VALUE
rg_set_glyph(VALUE self, VALUE val)
{
    _SELF(self)->glyph = NUM2UINT(val);
    return self;
}

static VALUE
rg_geometry(VALUE self)
{
    PangoGlyphGeometry geo = _SELF(self)->geometry;

    return rb_ary_new3(3, INT2NUM(geo.width), 
                       INT2NUM(geo.x_offset), INT2NUM(geo.y_offset));
}

static VALUE
rg_set_geometry(VALUE self, VALUE width, VALUE x_offset, VALUE y_offset)
{
    PangoGlyphGeometry geo = _SELF(self)->geometry;

    geo.width = NUM2INT(width);
    geo.x_offset = NUM2INT(x_offset);
    geo.y_offset = NUM2INT(y_offset);

    return self;
}

/*
  This method may be changed in the future following Pango implementation.
 */
static VALUE
rg_cluster_start_p(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->attr.is_cluster_start);
}

static VALUE
rg_set_cluster_start(VALUE self, VALUE val)
{
    _SELF(self)->attr.is_cluster_start = RVAL2CBOOL(val);
    return self;
}

void
Init_pango_glyph_info(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_GLYPH_INFO, "GlyphInfo", mPango);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(glyph, 0);
    RG_DEF_METHOD(set_glyph, 1);
    RG_DEF_METHOD(geometry, 0);
    RG_DEF_METHOD(set_geometry, 3);
    RG_DEF_METHOD_P(cluster_start, 0);
    RG_DEF_METHOD(set_cluster_start, 1);
}
