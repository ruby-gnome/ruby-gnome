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

#include "rb-pango-private.h"

#ifndef PANGO_CHECK_VERSION
#  define PANGO_CHECK_VERSION(major, minor, micro)                      \
    (PANGO_VERSION_MAJOR > (major) ||                                   \
     (PANGO_VERSION_MAJOR == (major) && PANGO_VERSION_MINOR > (minor)) || \
     (PANGO_VERSION_MAJOR == (major) && PANGO_VERSION_MINOR == (minor) && \
      PANGO_VERSION_MICRO >= (micro)))
#endif

#if !PANGO_CHECK_VERSION(1, 44, 0)
static GType
pango_attribute_get_type(void)
{
    static GType our_type = 0;
    if (our_type == 0)
        our_type = g_boxed_type_register_static("PangoAttribute",
                                                (GBoxedCopyFunc)pango_attribute_copy,
                                                (GBoxedFreeFunc)pango_attribute_destroy);
    return our_type;
}
#endif

VALUE
rbpango_attribute_to_ruby(PangoAttribute *attribute)
{
    VALUE rb_attr_type;
    ID id_to_class;
    VALUE klass;

    rb_attr_type = GENUM2RVAL(attribute->klass->type, PANGO_TYPE_ATTR_TYPE);
    CONST_ID(id_to_class, "to_class");
    klass = rb_funcall(rb_attr_type, id_to_class, 0);
    return rbgobj_make_boxed_raw(pango_attribute_copy(attribute),
                                 pango_attribute_get_type(),
                                 klass,
                                 0);
}

PangoAttribute *
rbpango_attribute_from_ruby(VALUE rb_attribute)
{
    return rbgobj_boxed_get(rb_attribute, pango_attribute_get_type());
}

static VALUE
rbpango_attr_bool_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return CBOOL2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_language_initialize(VALUE self, VALUE rb_language)
{
    PangoLanguage *language;

    language = RVAL2PANGOLANGUAGE(rb_language);
    G_INITIALIZE(self, pango_attr_language_new(language));

    return Qnil;
}

static VALUE
rbpango_attr_family_initialize(VALUE self, VALUE rb_family)
{
    G_INITIALIZE(self, pango_attr_family_new(RVAL2CSTR(rb_family)));

    return Qnil;
}

static VALUE
rbpango_attr_style_initialize(VALUE self, VALUE rb_style)
{
    G_INITIALIZE(self, pango_attr_style_new(RVAL2PANGOSTYLE(rb_style)));

    return Qnil;
}

static VALUE
rbpango_attr_style_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return PANGOSTYLE2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_weight_initialize(VALUE self, VALUE rb_weight)
{
    G_INITIALIZE(self, pango_attr_weight_new(RVAL2PANGOWEIGHT(rb_weight)));

    return Qnil;
}

static VALUE
rbpango_attr_weight_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return PANGOWEIGHT2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_variant_initialize(VALUE self, VALUE rb_variant)
{
    G_INITIALIZE(self, pango_attr_variant_new(RVAL2PANGOVARIANT(rb_variant)));

    return Qnil;
}

static VALUE
rbpango_attr_variant_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return PANGOVARIANT2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_stretch_initialize(VALUE self, VALUE rb_stretch)
{
    G_INITIALIZE(self, pango_attr_stretch_new(RVAL2PANGOSTRETCH(rb_stretch)));

    return Qnil;
}

static VALUE
rbpango_attr_stretch_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return PANGOSTRETCH2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_size_initialize(VALUE self, VALUE rb_size)
{
    gint size;

    size = NUM2INT(rb_size);
    G_INITIALIZE(self, pango_attr_size_new(size));

    return Qnil;
}

static VALUE
rbpango_attr_font_desc_initialize(VALUE self, VALUE rb_font_desc)
{
    PangoFontDescription *font_desc;

    if (RB_TYPE_P(rb_font_desc, RUBY_T_STRING)) {
        VALUE mPango;
        VALUE cPangoFontDescription;

        mPango = rb_const_get(rb_cObject, rb_intern("Pango"));
        cPangoFontDescription = rb_const_get(mPango,
                                             rb_intern("FontDescription"));
        rb_font_desc = rb_funcall(cPangoFontDescription,
                                  rb_intern("new"), 1, rb_font_desc);
    }
    font_desc = RVAL2PANGOFONTDESCRIPTION(rb_font_desc);
    G_INITIALIZE(self, pango_attr_font_desc_new(font_desc));

    return Qnil;
}

static VALUE
rbpango_attr_foregound_initialize(VALUE self,
                                  VALUE rb_red,
                                  VALUE rb_green,
                                  VALUE rb_blue)
{
    G_INITIALIZE(self,
                 pango_attr_foreground_new(NUM2UINT(rb_red),
                                           NUM2UINT(rb_green),
                                           NUM2UINT(rb_blue)));

    return Qnil;
}

static VALUE
rbpango_attr_background_initialize(VALUE self,
                                   VALUE rb_red,
                                   VALUE rb_green,
                                   VALUE rb_blue)
{
    G_INITIALIZE(self,
                 pango_attr_background_new(NUM2UINT(rb_red),
                                           NUM2UINT(rb_green),
                                           NUM2UINT(rb_blue)));

    return Qnil;
}

static VALUE
rbpango_attr_underline_initialize(VALUE self, VALUE rb_underline)
{
    G_INITIALIZE(self,
                 pango_attr_underline_new(RVAL2PANGOUNDERLINE(rb_underline)));

    return Qnil;
}

static VALUE
rbpango_attr_underline_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return PANGOUNDERLINE2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_strikethrough_initialize(VALUE self, VALUE rb_strikethrough)
{
    G_INITIALIZE(self,
                 pango_attr_strikethrough_new(RVAL2CBOOL(rb_strikethrough)));

    return Qnil;
}

static VALUE
rbpango_attr_rise_initialize(VALUE self, VALUE rb_rise)
{
    G_INITIALIZE(self, pango_attr_rise_new(NUM2INT(rb_rise)));

    return Qnil;
}

static VALUE
rbpango_attr_shape_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE rb_ink_rect;
    VALUE rb_logical_rect;
    VALUE rb_data;
    PangoRectangle *ink_rect;
    PangoRectangle *logical_rect;

    rb_scan_args(argc, argv, "21", &rb_ink_rect, &rb_logical_rect, &rb_data);

    ink_rect = RVAL2PANGORECTANGLE(rb_ink_rect);
    logical_rect = RVAL2PANGORECTANGLE(rb_logical_rect);

    if (NIL_P(rb_data)) {
        G_INITIALIZE(self, pango_attr_shape_new(ink_rect, logical_rect));
    } else {
        G_RELATIVE(self, rb_data);
        G_INITIALIZE(self,
                     pango_attr_shape_new_with_data(ink_rect,
                                                    logical_rect,
                                                    (gpointer)rb_data,
                                                    NULL,
                                                    NULL));
    }

    return Qnil;
}

static VALUE
rbpango_attr_shape_get_data(VALUE self)
{
    PangoAttrShape *attr_shape;
    VALUE rb_data;

    attr_shape = (PangoAttrShape *)rbpango_attribute_from_ruby(self);
    rb_data = (VALUE)(attr_shape->data);
    return rb_data;
}

static VALUE
rbpango_attr_scale_initialize(VALUE self, VALUE rb_scale)
{
    G_INITIALIZE(self, pango_attr_scale_new(NUM2DBL(rb_scale)));

    return Qnil;
}

static VALUE
rbpango_attr_fallback_initialize(VALUE self, VALUE rb_enable_fallback)
{
    G_INITIALIZE(self, pango_attr_fallback_new(RVAL2CBOOL(rb_enable_fallback)));

    return Qnil;
}

static VALUE
rbpango_attr_letter_spacing_initialize(VALUE self, VALUE rb_letter_spacing)
{
    G_INITIALIZE(self,
                 pango_attr_letter_spacing_new(NUM2INT(rb_letter_spacing)));

    return Qnil;
}

static VALUE
rbpango_attr_underline_color_initialize(VALUE self,
                                        VALUE rb_red,
                                        VALUE rb_green,
                                        VALUE rb_blue)
{
    G_INITIALIZE(self,
                 pango_attr_underline_color_new(NUM2UINT(rb_red),
                                                NUM2UINT(rb_green),
                                                NUM2UINT(rb_blue)));

    return Qnil;
}

static VALUE
rbpango_attr_strikethrough_color_initialize(VALUE self,
                                            VALUE rb_red,
                                            VALUE rb_green,
                                            VALUE rb_blue)
{
    G_INITIALIZE(self,
                 pango_attr_strikethrough_color_new(NUM2UINT(rb_red),
                                                    NUM2UINT(rb_green),
                                                    NUM2UINT(rb_blue)));

    return Qnil;
}

static VALUE
rbpango_attr_absolute_size_initialize(VALUE self, VALUE rb_absolute_size)
{
    G_INITIALIZE(self,
                 pango_attr_size_new_absolute(NUM2INT(rb_absolute_size)));

    return Qnil;
}

static VALUE
rbpango_attr_gravity_initialize(VALUE self, VALUE rb_gravity)
{
    G_INITIALIZE(self, pango_attr_gravity_new(RVAL2PANGOGRAVITY(rb_gravity)));

    return Qnil;
}

static VALUE
rbpango_attr_gravity_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return PANGOGRAVITY2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_gravity_hint_initialize(VALUE self, VALUE rb_gravity_hint)
{
    PangoGravityHint hint;

    hint = RVAL2PANGOGRAVITYHINT(rb_gravity_hint);
    G_INITIALIZE(self, pango_attr_gravity_hint_new(hint));

    return Qnil;
}

static VALUE
rbpango_attr_gravity_hint_get_value(VALUE self)
{
    PangoAttribute *attribute;

    attribute = rbpango_attribute_from_ruby(self);

    return PANGOGRAVITYHINT2RVAL(((PangoAttrInt *)attribute)->value);
}

static VALUE
rbpango_attr_font_features_initialize(VALUE self, VALUE rb_font_features)
{
    G_INITIALIZE(self,
                 pango_attr_font_features_new(RVAL2CSTR(rb_font_features)));

    return Qnil;
}

static VALUE
rbpango_attr_foreground_alpha_initialize(VALUE self, VALUE rb_alpha)
{
    G_INITIALIZE(self, pango_attr_foreground_alpha_new(NUM2UINT(rb_alpha)));

    return Qnil;
}

static VALUE
rbpango_attr_background_alpha_initialize(VALUE self, VALUE rb_alpha)
{
    G_INITIALIZE(self, pango_attr_background_alpha_new(NUM2UINT(rb_alpha)));

    return Qnil;
}

void
rbpango_attribute_init(VALUE mPango)
{
    VALUE cAttribute;
    VALUE cAttrString;
    VALUE cAttrColor;
    VALUE cAttrInt;
    VALUE cAttrBool;
    VALUE cAttrFloat;
    VALUE cAttrLanguage;
    VALUE cAttrFamily;
    VALUE cAttrStyle;
    VALUE cAttrWeight;
    VALUE cAttrVariant;
    VALUE cAttrStretch;
    VALUE cAttrSize;
    VALUE cAttrFontDesc;
    VALUE cAttrForeground;
    VALUE cAttrBackground;
    VALUE cAttrUnderline;
    VALUE cAttrStrikethrough;
    VALUE cAttrRise;
    VALUE cAttrShape;
    VALUE cAttrScale;
    VALUE cAttrFallback;
    VALUE cAttrLetterSpacing;
    VALUE cAttrUnderlineColor;
    VALUE cAttrStrikethroughColor;
    VALUE cAttrAbsoluteSize;
    VALUE cAttrGravity;
    VALUE cAttrGravityHint;
    VALUE cAttrFontFeatures;
    VALUE cAttrForegroundAlpha;
    VALUE cAttrBackgroundAlpha;

    cAttribute = G_DEF_CLASS(pango_attribute_get_type(), "Attribute", mPango);

#define DEFINE_ABSTRACT_ATTRIBUTE_BEGIN(ClassName, parent) do { \
        c ## ClassName = rb_define_class_under(mPango,          \
                                               #ClassName,      \
                                               parent);

#define DEFINE_ABSTRACT_ATTRIBUTE_END()         \
    } while (FALSE)

#define DEFINE_ABSTRACT_ATTRIBUTE(ClassName, parent)                    \
    DEFINE_ABSTRACT_ATTRIBUTE_BEGIN(ClassName, parent) {                \
    } DEFINE_ABSTRACT_ATTRIBUTE_END()

#define DEFINE_ATTRIBUTE_BEGIN(ClassName,                               \
                               parent,                                  \
                               method_name,                             \
                               n_initialize_args) do {                  \
        c ## ClassName = rb_define_class_under(mPango,                  \
                                               #ClassName,              \
                                               parent);                 \
        rb_define_method(c ## ClassName, "initialize",                  \
                         rbpango_ ## method_name ## _initialize,        \
                         n_initialize_args);                            \

#define DEFINE_ATTRIBUTE_END()                  \
    } while (FALSE)

#define DEFINE_ATTRIBUTE(ClassName,             \
                         parent,                \
                         method_name,           \
                         n_initialize_args)     \
    DEFINE_ATTRIBUTE_BEGIN(ClassName,           \
                           parent,              \
                           method_name,         \
                           n_initialize_args) { \
    } DEFINE_ATTRIBUTE_END();

    DEFINE_ABSTRACT_ATTRIBUTE(AttrString, cAttribute);
    DEFINE_ABSTRACT_ATTRIBUTE(AttrColor, cAttribute);
    DEFINE_ABSTRACT_ATTRIBUTE(AttrInt, cAttribute);
    DEFINE_ABSTRACT_ATTRIBUTE_BEGIN(AttrBool, cAttrInt) {
        rb_define_method(cAttrBool, "value", rbpango_attr_bool_get_value, 0);
    } DEFINE_ABSTRACT_ATTRIBUTE_END();
    DEFINE_ABSTRACT_ATTRIBUTE(AttrFloat, cAttribute);

    DEFINE_ATTRIBUTE(AttrLanguage, cAttribute, attr_language, 1);
    DEFINE_ATTRIBUTE(AttrFamily, cAttrString, attr_family, 1);
    DEFINE_ATTRIBUTE_BEGIN(AttrStyle, cAttrInt, attr_style, 1) {
        rb_define_method(cAttrStyle, "value", rbpango_attr_style_get_value, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE_BEGIN(AttrWeight, cAttrInt, attr_weight, 1) {
        rb_define_method(cAttrWeight, "value", rbpango_attr_weight_get_value, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE_BEGIN(AttrVariant, cAttrInt, attr_variant, 1) {
        rb_define_method(cAttrVariant,
                         "value", rbpango_attr_variant_get_value, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE_BEGIN(AttrStretch, cAttrInt, attr_stretch, 1) {
        rb_define_method(cAttrStretch,
                         "value", rbpango_attr_stretch_get_value, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE(AttrSize, cAttribute, attr_size, 1);
    DEFINE_ATTRIBUTE(AttrFontDesc, cAttribute, attr_font_desc, 1);
    DEFINE_ATTRIBUTE(AttrForeground, cAttrColor, attr_foregound, 3);
    DEFINE_ATTRIBUTE(AttrBackground, cAttrColor, attr_background, 3);
    DEFINE_ATTRIBUTE_BEGIN(AttrUnderline, cAttrInt, attr_underline, 1) {
        rb_define_method(cAttrUnderline,
                         "value", rbpango_attr_underline_get_value, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE(AttrStrikethrough, cAttrBool, attr_strikethrough, 1);
    DEFINE_ATTRIBUTE(AttrRise, cAttrInt, attr_rise, 1);
    DEFINE_ATTRIBUTE_BEGIN(AttrShape, cAttribute, attr_shape, -1) {
        rb_define_method(cAttrShape, "data", rbpango_attr_shape_get_data, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE(AttrScale, cAttrFloat, attr_scale, 1);
    DEFINE_ATTRIBUTE(AttrFallback, cAttrBool, attr_fallback, 1);
    DEFINE_ATTRIBUTE(AttrLetterSpacing, cAttrInt, attr_letter_spacing, 1);
    DEFINE_ATTRIBUTE(AttrUnderlineColor, cAttrColor, attr_underline_color, 3);
    DEFINE_ATTRIBUTE(AttrStrikethroughColor, cAttrColor,
                     attr_strikethrough_color, 3);
    DEFINE_ATTRIBUTE(AttrAbsoluteSize, cAttrSize, attr_absolute_size, 1);
    DEFINE_ATTRIBUTE_BEGIN(AttrGravity, cAttrInt, attr_gravity, 1) {
        rb_define_method(cAttrGravity,
                         "value", rbpango_attr_gravity_get_value, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE_BEGIN(AttrGravityHint, cAttrInt, attr_gravity_hint, 1) {
        rb_define_method(cAttrGravityHint,
                         "value", rbpango_attr_gravity_hint_get_value, 0);
    } DEFINE_ATTRIBUTE_END();
    DEFINE_ATTRIBUTE(AttrFontFeatures, cAttrString, attr_font_features, 1);
    DEFINE_ATTRIBUTE(AttrForegroundAlpha, cAttrInt, attr_foreground_alpha, 1);
    DEFINE_ATTRIBUTE(AttrBackgroundAlpha, cAttrInt, attr_background_alpha, 1);

#undef DEFINE_ABSTRACT_ATTRIBUTE
#undef DEFINE_ABSTRACT_ATTRIBUTE_BEGIN
#undef DEFINE_ABSTRACT_ATTRIBUTE_END
#undef DEFINE_ATTRIBUTE
#undef DEFINE_ATTRIBUTE_BEGIN
#undef DEFINE_ATTRIBUTE_END
}
