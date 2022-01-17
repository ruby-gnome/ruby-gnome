/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2013-2019  Ruby-GNOME Project Team
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

#include "rb-cairo-gobject.h"

#define RG_TARGET_NAMESPACE rb_mCairoGObject

static VALUE RG_TARGET_NAMESPACE;

#define DEFINE_CONVERSION(prefix,                                       \
                          gtype,                                        \
                          ClassName,                                    \
                          rb_klass,                                     \
                          RVAL2CR,                                      \
                          CR2RVAL)                                      \
static gpointer                                                         \
prefix ## _robj2instance(VALUE rb_object,                               \
                         G_GNUC_UNUSED gpointer user_data)              \
{                                                                       \
    return RVAL2CR(rb_object);                                          \
}                                                                       \
                                                                        \
static VALUE                                                            \
prefix ## _instance2robj(gpointer cr_object,                            \
                         G_GNUC_UNUSED gpointer user_data)              \
{                                                                       \
    return CR2RVAL(cr_object);                                          \
}                                                                       \
                                                                        \
static VALUE                                                            \
prefix ## _s_gtype(G_GNUC_UNUSED VALUE klass)                           \
{                                                                       \
    return rbgobj_gtype_new(gtype);                                     \
}                                                                       \
                                                                        \
static VALUE                                                            \
rg_ ## prefix ## _s_try_convert(VALUE klass, VALUE value)               \
{                                                                       \
    if (RTEST(rb_obj_is_kind_of(value, klass))) {                       \
        return value;                                                   \
    } else {                                                            \
        gpointer raw_object = RVAL2CR(value);                           \
        return BOXED2RVAL(raw_object, gtype);                           \
    }                                                                   \
}                                                                       \
                                                                        \
static void                                                             \
define_ ## prefix ## _conversion(void)                                  \
{                                                                       \
    RGConvertTable table;                                               \
    VALUE rg_klass;                                                     \
                                                                        \
    memset(&table, 0, sizeof(RGConvertTable));                          \
    table.type          = gtype;                                        \
    table.klass         = rb_klass;                                     \
    table.robj2instance = prefix ## _robj2instance;                     \
    table.instance2robj = prefix ## _instance2robj;                     \
                                                                        \
    RG_DEF_CONVERSION(&table);                                          \
                                                                        \
    rb_define_singleton_method(rb_klass,                                \
                               "gtype",                                 \
                               prefix ## _s_gtype,                      \
                               0);                                      \
                                                                        \
    rg_klass = G_DEF_CLASS(gtype, ClassName, RG_TARGET_NAMESPACE);      \
    rb_define_singleton_method(rg_klass,                                \
                               "try_convert",                           \
                               rg_ ## prefix ## _s_try_convert,         \
                               1);                                      \
}

DEFINE_CONVERSION(context,
                  CAIRO_GOBJECT_TYPE_CONTEXT,
                  "Context",
                  rb_cCairo_Context,
                  RVAL2CRCONTEXT,
                  CRCONTEXT2RVAL)
DEFINE_CONVERSION(device,
                  CAIRO_GOBJECT_TYPE_DEVICE,
                  "Device",
                  rb_cCairo_Device,
                  RVAL2CRDEVICE,
                  CRDEVICE2RVAL)
DEFINE_CONVERSION(pattern,
                  CAIRO_GOBJECT_TYPE_PATTERN,
                  "Pattern",
                  rb_cCairo_Pattern,
                  RVAL2CRPATTERN,
                  CRPATTERN2RVAL)
DEFINE_CONVERSION(surface,
                  CAIRO_GOBJECT_TYPE_SURFACE,
                  "Surface",
                  rb_cCairo_Surface,
                  RVAL2CRSURFACE,
                  CRSURFACE2RVAL)
DEFINE_CONVERSION(scaled_font,
                  CAIRO_GOBJECT_TYPE_SCALED_FONT,
                  "ScaledFont",
                  rb_cCairo_ScaledFont,
                  RVAL2CRSCALEDFONT,
                  CRSCALEDFONT2RVAL)
DEFINE_CONVERSION(font_face,
                  CAIRO_GOBJECT_TYPE_FONT_FACE,
                  "FontFace",
                  rb_cCairo_FontFace,
                  RVAL2CRFONTFACE,
                  CRFONTFACE2RVAL)
DEFINE_CONVERSION(font_options,
                  CAIRO_GOBJECT_TYPE_FONT_OPTIONS,
                  "FontOptions",
                  rb_cCairo_FontOptions,
                  RVAL2CRFONTOPTIONS,
                  CRFONTOPTIONS2RVAL)
DEFINE_CONVERSION(region,
                  CAIRO_GOBJECT_TYPE_REGION,
                  "Region",
                  rb_cCairo_Region,
                  RVAL2CRREGION,
                  CRREGION2RVAL)

void
Init_cairo_gobject(void)
{
    RG_TARGET_NAMESPACE = rb_define_module("CairoGObject");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(CAIRO_VERSION_MAJOR),
                                INT2FIX(CAIRO_VERSION_MINOR),
                                INT2FIX(CAIRO_VERSION_MICRO)));

    define_context_conversion();
    define_device_conversion();
    define_pattern_conversion();
    define_surface_conversion();
    define_scaled_font_conversion();
    define_font_face_conversion();
    define_font_options_conversion();
    define_region_conversion();

    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_STATUS,             "Status",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_CONTENT,            "Content",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_OPERATOR,           "Operator",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_ANTIALIAS,          "Antialias",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_FILL_RULE,          "FillRule",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_LINE_CAP,           "LineCap",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_LINE_JOIN,          "LineJoin",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_TEXT_CLUSTER_FLAGS, "TextClusterFlags",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_FONT_SLANT,         "FontSlant",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_FONT_WEIGHT,        "FontWeight",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_SUBPIXEL_ORDER,     "SubpixelOrder",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_HINT_STYLE,         "HintStyle",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_HINT_METRICS,       "HintMetrics",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_FONT_TYPE,          "FontType",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_PATH_DATA_TYPE,     "PathDataType",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_DEVICE_TYPE,        "DeviceType",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_SURFACE_TYPE,       "SurfaceType",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_FORMAT,             "Format",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_PATTERN_TYPE,       "PatternType",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_EXTEND,             "Extend",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_FILTER,             "Filter",
                RG_TARGET_NAMESPACE);
    G_DEF_CLASS(CAIRO_GOBJECT_TYPE_REGION_OVERLAP,     "RegionOverlap",
                RG_TARGET_NAMESPACE);
}
