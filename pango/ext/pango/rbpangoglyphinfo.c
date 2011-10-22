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

#define _SELF(self) ((PangoGlyphInfo*)RVAL2BOXED(self, PANGO_TYPE_GLYPH_INFO))


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
gi_initialize(VALUE self)
{
    G_INITIALIZE(self, g_new0(PangoGlyphInfo, 1));
    return Qnil;
}

static VALUE
gi_glyph(VALUE self)
{
    return UINT2NUM(_SELF(self)->glyph);
}

static VALUE
gi_set_glyph(VALUE self, VALUE val)
{
    _SELF(self)->glyph = NUM2UINT(val);
    return self;
}

static VALUE
gi_geometry(VALUE self)
{
    PangoGlyphGeometry geo = _SELF(self)->geometry;

    return rb_ary_new3(3, INT2NUM(geo.width), 
                       INT2NUM(geo.x_offset), INT2NUM(geo.y_offset));
}

static VALUE
gi_set_geometry(VALUE self, VALUE width, VALUE x_offset, VALUE y_offset)
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
gi_attr_is_cluster_start(VALUE self)
{
    return CBOOL2RVAL(_SELF(self)->attr.is_cluster_start);
}

static VALUE
gi_attr_set_is_cluster_start(VALUE self, VALUE val)
{
    _SELF(self)->attr.is_cluster_start = RVAL2CBOOL(val);
    return self;
}

void
Init_pango_glyph_info(void)
{
    VALUE pInfo = G_DEF_CLASS(PANGO_TYPE_GLYPH_INFO, "GlyphInfo", mPango);
    
    rb_define_method(pInfo, "initialize", gi_initialize, 0);
    rb_define_method(pInfo, "glyph", gi_glyph, 0);
    rb_define_method(pInfo, "set_glyph", gi_set_glyph, 1);
    rb_define_method(pInfo, "geometry", gi_geometry, 0);
    rb_define_method(pInfo, "set_geometry", gi_set_geometry, 3);
    rb_define_method(pInfo, "cluster_start?", gi_attr_is_cluster_start, 0);
    rb_define_method(pInfo, "set_cluster_start", gi_attr_set_is_cluster_start, 1);

    G_DEF_SETTERS(pInfo);
}
