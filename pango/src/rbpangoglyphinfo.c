/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoglyphinfo.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoGlyphInfo*)RVAL2BOXED(self, PANGO_TYPE_GLYPH_INFO))


/**********************************/
PangoGlyphInfo*
pango_glyph_info_copy (const PangoGlyphInfo* info)
{
  PangoGlyphInfo* data;
  g_return_val_if_fail (info != NULL, NULL);
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
gi_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, g_new0(PangoGlyphInfo, 1));
    return Qnil;
}

static VALUE
gi_glyph(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->glyph);
}

static VALUE
gi_set_glyph(self, val)
    VALUE self, val;
{
    _SELF(self)->glyph = NUM2UINT(val);
}

static VALUE
gi_geometry(self)
    VALUE self;
{
    PangoGlyphGeometry geo = _SELF(self)->geometry;

    return rb_ary_new3(3, INT2NUM(geo.width), 
                       INT2NUM(geo.x_offset), INT2NUM(geo.y_offset));
}

static VALUE
gi_set_geometry(self, width, x_offset, y_offset)
    VALUE self;
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
gi_attr_is_cluster_start(self)
    VALUE self;
{
    return CBOOL2RVAL(_SELF(self)->attr.is_cluster_start);
}

static VALUE
gi_attr_set_is_cluster_start(self, val)
    VALUE self, val;
{
    _SELF(self)->attr.is_cluster_start = RTEST(val);
    return self;
}

void
Init_pango_glyph_info()
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
