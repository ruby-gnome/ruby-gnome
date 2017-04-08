/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2017  Ruby-GNOME2 Project Team
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

static VALUE
rbpango_attribute_allocate(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, pango_attribute_destroy, 0);
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
    DATA_PTR(self) = pango_attr_font_desc_new(font_desc);

    return Qnil;
}

static VALUE
rbpango_attr_font_features_initialize(VALUE self, VALUE rb_font_features)
{
    DATA_PTR(self) = pango_attr_font_features_new(RVAL2CSTR(rb_font_features));

    return Qnil;
}

static VALUE
rbpango_attr_language_initialize(VALUE self, VALUE rb_language)
{
    PangoLanguage *language;

    language = RVAL2PANGOLANGUAGE(rb_language);
    DATA_PTR(self) = pango_attr_language_new(language);

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
        DATA_PTR(self) = pango_attr_shape_new(ink_rect, logical_rect);
    } else {
        G_RELATIVE(self, rb_data);
        DATA_PTR(self) = pango_attr_shape_new_with_data(ink_rect,
                                                        logical_rect,
                                                        (gpointer)rb_data,
                                                        NULL,
                                                        NULL);
    }

    return Qnil;
}

static VALUE
rbpango_attr_size_initialize(VALUE self, VALUE rb_size)
{
    gint size;

    size = NUM2INT(rb_size);
    DATA_PTR(self) = pango_attr_size_new(size);

    return Qnil;
}

void
rbpango_attribute_init(VALUE mPango)
{
    VALUE cAttribute;

    cAttribute = rb_define_class_under(mPango, "Attribute", rb_cData);
    rb_define_alloc_func(cAttribute, rbpango_attribute_allocate);

#define DEFINE_ABSTRACT_ATTRIBUTE(ClassName)                            \
    do {                                                                \
        rb_define_class_under(mPango,                                   \
                              #ClassName,                               \
                              cAttribute);                              \
    } while (FALSE)

#define DEFINE_ATTRIBUTE(ClassName, method_name, n_initialize_args)     \
    do {                                                                \
        VALUE c ## ClassName;                                           \
        c ## ClassName = rb_define_class_under(mPango,                  \
                                               #ClassName,              \
            cAttribute);                                                \
        rb_define_method(c ## ClassName, "initialize",                  \
                         rbpango_ ## method_name ## _initialize,        \
                         n_initialize_args);                            \
    } while (FALSE)

    DEFINE_ABSTRACT_ATTRIBUTE(AttrColor);
    DEFINE_ABSTRACT_ATTRIBUTE(AttrFloat);
    DEFINE_ATTRIBUTE(AttrFontDesc, attr_font_desc, 1);
    DEFINE_ATTRIBUTE(AttrFontFeatures, attr_font_features, 1);
    DEFINE_ABSTRACT_ATTRIBUTE(AttrInt);
    DEFINE_ATTRIBUTE(AttrLanguage, attr_language, 1);
    DEFINE_ATTRIBUTE(AttrShape, attr_shape, -1);
    DEFINE_ATTRIBUTE(AttrSize, attr_size, 1);
    DEFINE_ABSTRACT_ATTRIBUTE(AttrString);

#undef DEFINE_ABSTRACT_ATTRIBUTE
#undef DEFINE_ATTRIBUTE
}
