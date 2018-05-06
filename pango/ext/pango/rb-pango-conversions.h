/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017-2018  Ruby-GNOME2 Project Team
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

#pragma once

#define PANGO_TYPE_ATTR_ITERATOR pango_attr_iterator_get_type()
extern GType pango_attr_iterator_get_type(void) G_GNUC_CONST;

extern VALUE rbpango_attribute_to_ruby(PangoAttribute *attribute);
extern PangoAttribute *rbpango_attribute_from_ruby(VALUE rb_attribute);

#define ATTR2RVAL(attr)    (rbpango_attribute_to_ruby((attr)))
#define RVAL2ATTR(rb_attr) (rbpango_attribute_from_ruby((rb_attr)))

#define RVAL2PANGOCAIROFONTMAP(o)          (PANGO_CAIRO_FONT_MAP(RVAL2GOBJ(o)))
#define RVAL2PANGOCONTEXT(o)               (PANGO_CONTEXT(RVAL2GOBJ(o)))
#define RVAL2PANGOENGINE(o)                (PANGO_ENGINE(RVAL2GOBJ(o)))
#define RVAL2PANGOENGINELANG(o)            (PANGO_ENGINE_LANG(RVAL2GOBJ(o)))
#define RVAL2PANGOENGINESHAPE(o)           (PANGO_ENGINE_SHAPE(RVAL2GOBJ(o)))
#define RVAL2PANGOFONT(o)                  (PANGO_FONT(RVAL2GOBJ(o)))
#define RVAL2PANGOFONTSET(o)               (PANGO_FONTSET(RVAL2GOBJ(o)))
#define RVAL2PANGOFONTSETSIMPLE(o)         (PANGO_FONTSET_SIMPLE(RVAL2GOBJ(o)))
#define RVAL2PANGOFONTFACE(o)              (PANGO_FONT_FACE(RVAL2GOBJ(o)))
#define RVAL2PANGOFONTFAMILY(o)            (PANGO_FONT_FAMILY(RVAL2GOBJ(o)))
#define RVAL2PANGOFONTMAP(o)               (PANGO_FONT_MAP(RVAL2GOBJ(o)))
#define RVAL2PANGOLAYOUT(o)                (PANGO_LAYOUT(RVAL2GOBJ(o)))
#define RVAL2PANGORENDERER(o)              (PANGO_RENDERER(RVAL2GOBJ(o)))

#define RVAL2PANGOANALYSIS(o)              ((PangoAnalysis*)RVAL2BOXED(o, PANGO_TYPE_ANALYSIS))
#define PANGOANALYSIS2RVAL(o)              (BOXED2RVAL(o, PANGO_TYPE_ANALYSIS))
#define RVAL2PANGOATTRITERATOR(o)          ((PangoAttrIterator*)RVAL2BOXED(o, PANGO_TYPE_ATTR_ITERATOR))
#define PANGOATTRITERATOR2RVAL(o)          (BOXED2RVAL(o, PANGO_TYPE_ATTR_ITERATOR))
#define RVAL2PANGOATTRLIST(o)              ((PangoAttrList*)RVAL2BOXED(o, PANGO_TYPE_ATTR_LIST))
#define PANGOATTRLIST2RVAL(o)              (BOXED2RVAL(o, PANGO_TYPE_ATTR_LIST))
#define RVAL2PANGOCOLOR(o)                 ((PangoColor*)RVAL2BOXED(o, PANGO_TYPE_COLOR))
#define PANGOCOLOR2RVAL(o)                 (BOXED2RVAL(o, PANGO_TYPE_COLOR))
#define RVAL2PANGOCOVERAGE(o)              ((PangoCoverage*)RVAL2BOXED(o, PANGO_TYPE_COVERAGE))
#define PANGOCOVERAGE2RVAL(o)              (BOXED2RVAL(o, PANGO_TYPE_COVERAGE))
#define RVAL2PANGOFONTDESCRIPTION(o)       ((PangoFontDescription*)RVAL2BOXED(o, PANGO_TYPE_FONT_DESCRIPTION))
#define PANGOFONTDESCRIPTION2RVAL(o)       (BOXED2RVAL(o, PANGO_TYPE_FONT_DESCRIPTION))
#define RVAL2PANGOFONTMETRICS(o)           ((PangoFontMetrics*)RVAL2BOXED(o, PANGO_TYPE_FONT_METRICS))
#define PANGOFONTMETRICS2RVAL(o)           (BOXED2RVAL(o, PANGO_TYPE_FONT_METRICS))
#define RVAL2PANGOGLYPHINFO(o)             ((PangoGlyphInfo*)RVAL2BOXED(o, PANGO_TYPE_GLYPH_INFO))
#define PANGOGLYPHINFO2RVAL(o)             (BOXED2RVAL(o, PANGO_TYPE_GLYPH_INFO))
#define RVAL2PANGOGLYPHITEM(o)             ((PangoGlyphItem*)RVAL2BOXED(o, PANGO_TYPE_GLYPH_ITEM))
#define PANGOGLYPHITEM2RVAL(o)             (BOXED2RVAL(o, PANGO_TYPE_GLYPH_ITEM))
#define RVAL2PANGOGLYPHSTRING(o)           ((PangoGlyphString*)RVAL2BOXED(o, PANGO_TYPE_GLYPH_STRING))
#define PANGOGLYPHSTRING2RVAL(o)           (BOXED2RVAL(o, PANGO_TYPE_GLYPH_STRING))
#define RVAL2PANGOITEM(o)                  ((PangoItem*)RVAL2BOXED(o, PANGO_TYPE_ITEM))
#define PANGOITEM2RVAL(o)                  (BOXED2RVAL(o, PANGO_TYPE_ITEM))
#define RVAL2PANGOLANGUAGE(o)              ((PangoLanguage*)RVAL2BOXED(o, PANGO_TYPE_LANGUAGE))
#define PANGOLANGUAGE2RVAL(o)              (BOXED2RVAL(o, PANGO_TYPE_LANGUAGE))
#define RVAL2PANGOLAYOUTITER(o)            ((PangoLayoutIter*)RVAL2BOXED(o, PANGO_TYPE_LAYOUT_ITER))
#define PANGOLAYOUTITER2RVAL(o)            (BOXED2RVAL(o, PANGO_TYPE_LAYOUT_ITER))
#define RVAL2PANGOLAYOUTLINE(o)            ((PangoLayoutLine*)RVAL2BOXED(o, PANGO_TYPE_LAYOUT_LINE))
#define PANGOLAYOUTLINE2RVAL(o)            (BOXED2RVAL(o, PANGO_TYPE_LAYOUT_LINE))
#define RVAL2PANGOLOGATTR(o)               ((PangoLogAttr*)RVAL2BOXED(o, PANGO_TYPE_LOG_ATTR))
#define PANGOLOGATTR2RVAL(o)               (BOXED2RVAL(o, PANGO_TYPE_LOG_ATTR))
#define RVAL2PANGOMATRIX(o)                ((PangoMatrix*)RVAL2BOXED(o, PANGO_TYPE_MATRIX))
#define PANGOMATRIX2RVAL(o)                (BOXED2RVAL(o, PANGO_TYPE_MATRIX))
#define RVAL2PANGORECTANGLE(o)             ((PangoRectangle *)rb_gi_struct_get_raw(o, G_TYPE_NONE))
#define RVAL2PANGOSCRIPTITER(o)            ((PangoScriptIter*)RVAL2BOXED(o, PANGO_TYPE_SCRIPT_ITER))
#define PANGOSCRIPTITER2RVAL(o)            (BOXED2RVAL(o, PANGO_TYPE_SCRIPT_ITER))
#define RVAL2PANGOTABARRAY(o)              ((PangoTabArray*)RVAL2BOXED(o, PANGO_TYPE_TAB_ARRAY))
#define PANGOTABARRAY2RVAL(o)              (BOXED2RVAL(o, PANGO_TYPE_TAB_ARRAY))

#define RVAL2PANGOALIGNMENT(o)             (RVAL2GENUM(o, PANGO_TYPE_ALIGNMENT))
#define PANGOALIGNMENT2RVAL(o)             (GENUM2RVAL(o, PANGO_TYPE_ALIGNMENT))
#define RVAL2PANGOCOVERAGELEVEL(o)         (RVAL2GENUM(o, PANGO_TYPE_COVERAGE_LEVEL))
#define PANGOCOVERAGELEVEL2RVAL(o)         (GENUM2RVAL(o, PANGO_TYPE_COVERAGE_LEVEL))
#define RVAL2PANGODIRECTION(o)             (RVAL2GENUM(o, PANGO_TYPE_DIRECTION))
#define PANGODIRECTION2RVAL(o)             (GENUM2RVAL(o, PANGO_TYPE_DIRECTION))
#define RVAL2PANGOELLIPSIZEMODE(o)         (RVAL2GENUM(o, PANGO_TYPE_ELLIPSIZE_MODE))
#define PANGOELLIPSIZEMODE2RVAL(o)         (GENUM2RVAL(o, PANGO_TYPE_ELLIPSIZE_MODE))
#define RVAL2PANGOGRAVITY(o)               (RVAL2GENUM(o, PANGO_TYPE_GRAVITY))
#define PANGOGRAVITY2RVAL(o)               (GENUM2RVAL(o, PANGO_TYPE_GRAVITY))
#define RVAL2PANGOGRAVITYHINT(o)           (RVAL2GENUM(o, PANGO_TYPE_GRAVITY_HINT))
#define PANGOGRAVITYHINT2RVAL(o)           (GENUM2RVAL(o, PANGO_TYPE_GRAVITY_HINT))
#define RVAL2PANGORENDERPART(o)            (RVAL2GENUM(o, PANGO_TYPE_RENDER_PART))
#define PANGORENDERPART2RVAL(o)            (GENUM2RVAL(o, PANGO_TYPE_RENDER_PART))
#define RVAL2PANGOSCRIPT(o)                (RVAL2GENUM(o, PANGO_TYPE_SCRIPT))
#define PANGOSCRIPT2RVAL(o)                (GENUM2RVAL(o, PANGO_TYPE_SCRIPT))
#define RVAL2PANGOSTRETCH(o)               (RVAL2GENUM(o, PANGO_TYPE_STRETCH))
#define PANGOSTRETCH2RVAL(o)               (GENUM2RVAL(o, PANGO_TYPE_STRETCH))
#define RVAL2PANGOSTYLE(o)                 (RVAL2GENUM(o, PANGO_TYPE_STYLE))
#define PANGOSTYLE2RVAL(o)                 (GENUM2RVAL(o, PANGO_TYPE_STYLE))
#define RVAL2PANGOTABALIGN(o)              (RVAL2GENUM(o, PANGO_TYPE_TAB_ALIGN))
#define PANGOTABALIGN2RVAL(o)              (GENUM2RVAL(o, PANGO_TYPE_TAB_ALIGN))
#define RVAL2PANGOUNDERLINE(o)             (RVAL2GENUM(o, PANGO_TYPE_UNDERLINE))
#define PANGOUNDERLINE2RVAL(o)             (GENUM2RVAL(o, PANGO_TYPE_UNDERLINE))
#define RVAL2PANGOVARIANT(o)               (RVAL2GENUM(o, PANGO_TYPE_VARIANT))
#define PANGOVARIANT2RVAL(o)               (GENUM2RVAL(o, PANGO_TYPE_VARIANT))
#define RVAL2PANGOWEIGHT(o)                (RVAL2GENUM(o, PANGO_TYPE_WEIGHT))
#define PANGOWEIGHT2RVAL(o)                (GENUM2RVAL(o, PANGO_TYPE_WEIGHT))
#define RVAL2PANGOWRAPMODE(o)              (RVAL2GENUM(o, PANGO_TYPE_WRAP_MODE))
#define PANGOWRAPMODE2RVAL(o)              (GENUM2RVAL(o, PANGO_TYPE_WRAP_MODE))

#define RVAL2PANGOFONTMASK(o)              (RVAL2GFLAGS(o, PANGO_TYPE_FONT_MASK))
#define PANGOFONTMASK2RVAL(o)              (GFLAGS2RVAL(o, PANGO_TYPE_FONT_MASK))

#define RVAL2PANGOATTR(o)                  (rbpango_attribute_from_ruby(o))
