/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontdescription.c -

  $Author: mutoh $
  $Date: 2003/01/10 19:22:13 $

  Copyright (C) 2002 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoFontDescription*)RVAL2BOXED(self, PANGO_TYPE_FONT_DESCRIPTION))

static VALUE
font_desc_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE str;

    rb_scan_args(argc, argv, "01", &str);

    if (NIL_P(str)) {
        G_INITIALIZE(self, pango_font_description_new());
    } else {
        G_INITIALIZE(self, pango_font_description_from_string(RVAL2CSTR(str)));
    }
    return Qnil;
}
/*
PangoFontDescription* pango_font_description_copy
                                            (const PangoFontDescription *desc);
PangoFontDescription* pango_font_description_copy_static
                                            (const PangoFontDescription *desc);
*/

static VALUE
font_desc_hash(self)
    VALUE self;
{
    return UINT2NUM(pango_font_description_hash(_SELF(self)));
}

static VALUE
font_desc_equal(self, other)
    VALUE self, other;
{
    return CBOOL2RVAL(pango_font_description_equal(_SELF(self), _SELF(other)));
}

/* Don't we need this?
void        pango_font_descriptions_free    (PangoFontDescription **descs,
                                             int n_descs);
*/

static VALUE
font_desc_set_family(self, family)
    VALUE self, family;
{
    pango_font_description_set_family(_SELF(self), RVAL2CSTR(family));
    return self;
}
/* Don't we need this?
void        pango_font_description_set_family_static
                                            (PangoFontDescription *desc,
                                             const char *family);
*/

static VALUE
font_desc_get_family(self)
    VALUE self;
{
    return CSTR2RVAL(pango_font_description_get_family(_SELF(self)));
}

static VALUE
font_desc_set_style(self, style)
    VALUE self, style;
{
    pango_font_description_set_style(_SELF(self), FIX2INT(style));
    return self;
}

static VALUE
font_desc_get_style(self)
    VALUE self;
{
    return INT2FIX(pango_font_description_get_style(_SELF(self)));
}

static VALUE
font_desc_set_variant(self, variant)
    VALUE self, variant;
{
    pango_font_description_set_variant(_SELF(self), FIX2INT(variant));
    return self;
}

static VALUE
font_desc_get_variant(self)
    VALUE self;
{
    return INT2FIX(pango_font_description_get_variant(_SELF(self)));
}

static VALUE
font_desc_set_weight(self, weight)
    VALUE self;
{
    pango_font_description_set_weight(_SELF(self), FIX2INT(weight));
    return self;
}

static VALUE
font_desc_get_weight(self)
    VALUE self;
{
    return INT2FIX(pango_font_description_get_weight(_SELF(self)));
}

static VALUE
font_desc_set_stretch(self, stretch)
    VALUE self, stretch;
{
    pango_font_description_set_stretch(_SELF(self), FIX2INT(stretch));
    return self;
}

static VALUE
font_desc_get_stretch(self)
    VALUE self;
{
    return INT2FIX(pango_font_description_get_stretch(_SELF(self)));
}

static VALUE
font_desc_set_size(self, size)
    VALUE self, size;
{
    pango_font_description_set_size(_SELF(self), NUM2INT(size));
    return self;
}

static VALUE
font_desc_get_size(self)
    VALUE self;
{
    return INT2NUM(pango_font_description_get_size(_SELF(self)));
}

static VALUE
font_desc_get_set_fields(self)
    VALUE self;
{
    return INT2NUM(pango_font_description_get_set_fields(_SELF(self)));
}

static VALUE
font_desc_unset_fields(self, to_unset)
    VALUE self, to_unset;
{
    pango_font_description_unset_fields(_SELF(self), FIX2INT(to_unset));
    return self;
}

static VALUE
font_desc_merge(self, desc_to_merge, replace_existing)
    VALUE self, desc_to_merge, replace_existing;
{
    pango_font_description_merge(_SELF(self), _SELF(desc_to_merge),
                                 RTEST(replace_existing));
    return self;
}

/* Don't we need this?
void        pango_font_description_merge_static
                                            (PangoFontDescription *desc,
                                             const PangoFontDescription *desc_to_merge,
                                             gboolean replace_existing);
*/

static VALUE
font_desc_better_match(self, old_match, new_match)
    VALUE self, old_match, new_match;
{
    return CBOOL2RVAL(pango_font_description_better_match(_SELF(self), 
                                                          _SELF(old_match),
                                                          _SELF(new_match)));
}

static VALUE
font_desc_to_string(self)
    VALUE self;
{
    return CSTR2RVAL(pango_font_description_to_string(_SELF(self)));
}

static VALUE
font_desc_to_filename(self)
    VALUE self;
{
    return CSTR2RVAL(pango_font_description_to_filename(_SELF(self)));
}

void
Init_pango_font_description()
{
    VALUE pFontDesc = G_DEF_CLASS(PANGO_TYPE_FONT_DESCRIPTION, "FontDescription", mPango);

    rb_define_method(pFontDesc, "initialize", font_desc_initialize, -1);
    rb_define_method(pFontDesc, "hash", font_desc_hash, 0);
    rb_define_method(pFontDesc, "==", font_desc_equal, 1);
    rb_define_method(pFontDesc, "set_family", font_desc_set_family, 1);
    rb_define_method(pFontDesc, "family", font_desc_get_family, 0);
    rb_define_method(pFontDesc, "set_style", font_desc_set_style, 1);
    rb_define_method(pFontDesc, "style", font_desc_get_style, 0);
    rb_define_method(pFontDesc, "set_variant", font_desc_set_variant, 1);
    rb_define_method(pFontDesc, "variant", font_desc_get_variant, 0);
    rb_define_method(pFontDesc, "set_weight", font_desc_set_weight, 1);
    rb_define_method(pFontDesc, "weight", font_desc_get_weight, 0);
    rb_define_method(pFontDesc, "set_stretch", font_desc_set_stretch, 1);
    rb_define_method(pFontDesc, "stretch", font_desc_get_stretch, 0);
    rb_define_method(pFontDesc, "set_size", font_desc_set_size, 1);
    rb_define_method(pFontDesc, "size", font_desc_get_size, 0);
    rb_define_method(pFontDesc, "set_fields", font_desc_get_set_fields, 0);
    rb_define_method(pFontDesc, "unset_fields", font_desc_unset_fields, 1);
    rb_define_method(pFontDesc, "merge", font_desc_merge, 2);
    rb_define_method(pFontDesc, "better_match", font_desc_better_match, 2);
    rb_define_method(pFontDesc, "to_str", font_desc_to_string, 0);
    rb_define_method(pFontDesc, "to_filename", font_desc_to_filename, 0);

    G_DEF_SETTERS(pFontDesc);

    /* PangoStyle */
    rb_define_const(pFontDesc, "STYLE_NORMAL", INT2FIX(PANGO_STYLE_NORMAL));
    rb_define_const(pFontDesc, "STYLE_OBLIQUE", INT2FIX(PANGO_STYLE_OBLIQUE));
    rb_define_const(pFontDesc, "STYLE_ITALIC", INT2FIX(PANGO_STYLE_ITALIC));

    /* PangoWeight */
    rb_define_const(pFontDesc, "WEIGHT_ULTRALIGHT", INT2FIX(PANGO_WEIGHT_ULTRALIGHT));
    rb_define_const(pFontDesc, "WEIGHT_LIGHT", INT2FIX(PANGO_WEIGHT_LIGHT));
    rb_define_const(pFontDesc, "WEIGHT_NORMAL", INT2FIX(PANGO_WEIGHT_NORMAL));
    rb_define_const(pFontDesc, "WEIGHT_BOLD", INT2FIX(PANGO_WEIGHT_BOLD));
    rb_define_const(pFontDesc, "WEIGHT_ULTRABOLD", INT2FIX(PANGO_WEIGHT_ULTRABOLD));
    rb_define_const(pFontDesc, "WEIGHT_HEAVY", INT2FIX(PANGO_WEIGHT_HEAVY));

    /* PangoVariant */
    rb_define_const(pFontDesc, "VARIANT_NORMAL", INT2FIX(PANGO_VARIANT_NORMAL));
    rb_define_const(pFontDesc, "VARIANT_SMALL_CAPS", INT2FIX(PANGO_VARIANT_SMALL_CAPS));

    /* PangoStretch */
    rb_define_const(pFontDesc, "STRETCH_ULTRA_CONDENSED", INT2FIX(PANGO_STRETCH_ULTRA_CONDENSED));
    rb_define_const(pFontDesc, "STRETCH_EXTRA_CONDENSED", INT2FIX(PANGO_STRETCH_EXTRA_CONDENSED));
    rb_define_const(pFontDesc, "STRETCH_CONDENSED", INT2FIX(PANGO_STRETCH_CONDENSED));
    rb_define_const(pFontDesc, "STRETCH_SEMI_CONDENSED", INT2FIX(PANGO_STRETCH_SEMI_CONDENSED));
    rb_define_const(pFontDesc, "STRETCH_NORMAL", INT2FIX(PANGO_STRETCH_NORMAL));
    rb_define_const(pFontDesc, "STRETCH_SEMI_EXPANDED", INT2FIX(PANGO_STRETCH_SEMI_EXPANDED));
    rb_define_const(pFontDesc, "STRETCH_EXPANDED", INT2FIX(PANGO_STRETCH_EXPANDED));
    rb_define_const(pFontDesc, "STRETCH_EXTRA_EXPANDED", INT2FIX(PANGO_STRETCH_EXTRA_EXPANDED));
    rb_define_const(pFontDesc, "STRETCH_ULTRA_EXPANDED", INT2FIX(PANGO_STRETCH_ULTRA_EXPANDED));

    /* PangoFontMask */
    rb_define_const(pFontDesc, "FONT_MASK_FAMILY", INT2FIX(PANGO_FONT_MASK_FAMILY));
    rb_define_const(pFontDesc, "FONT_MASK_STYLE", INT2FIX(PANGO_FONT_MASK_STYLE));
    rb_define_const(pFontDesc, "FONT_MASK_VARIANT", INT2FIX(PANGO_FONT_MASK_VARIANT));
    rb_define_const(pFontDesc, "FONT_MASK_WEIGHT", INT2FIX(PANGO_FONT_MASK_WEIGHT));
    rb_define_const(pFontDesc, "FONT_MASK_STRETCH", INT2FIX(PANGO_FONT_MASK_STRETCH));
    rb_define_const(pFontDesc, "FONT_MASK_SIZE", INT2FIX(PANGO_FONT_MASK_SIZE));

}
