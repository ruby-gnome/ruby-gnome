/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontdescription.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2002-2005 Masao Mutoh
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
    pango_font_description_set_style(_SELF(self), RVAL2GENUM(style, PANGO_TYPE_STYLE));
    return self;
}

static VALUE
font_desc_get_style(self)
    VALUE self;
{
    return GENUM2RVAL(pango_font_description_get_style(_SELF(self)), PANGO_TYPE_STYLE);
}

static VALUE
font_desc_set_variant(self, variant)
    VALUE self, variant;
{
    pango_font_description_set_variant(_SELF(self), RVAL2GENUM(variant, PANGO_TYPE_VARIANT));
    return self;
}

static VALUE
font_desc_get_variant(self)
    VALUE self;
{
    return GENUM2RVAL(pango_font_description_get_variant(_SELF(self)), PANGO_TYPE_VARIANT);
}

static VALUE
font_desc_set_weight(self, weight)
    VALUE self;
{
    pango_font_description_set_weight(_SELF(self), RVAL2GENUM(weight, PANGO_TYPE_WEIGHT));
    return self;
}

static VALUE
font_desc_get_weight(self)
    VALUE self;
{
    return GENUM2RVAL(pango_font_description_get_weight(_SELF(self)), PANGO_TYPE_WEIGHT);
}

static VALUE
font_desc_set_stretch(self, stretch)
    VALUE self, stretch;
{
    pango_font_description_set_stretch(_SELF(self), RVAL2GENUM(stretch, PANGO_TYPE_STRETCH));
    return self;
}

static VALUE
font_desc_get_stretch(self)
    VALUE self;
{
    return GENUM2RVAL(pango_font_description_get_stretch(_SELF(self)), PANGO_TYPE_STRETCH);
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

#if PANGO_CHECK_VERSION(1,8,0)
static VALUE
font_desc_set_absolute_size(self, size)
    VALUE self, size;
{
    pango_font_description_set_absolute_size(_SELF(self), NUM2INT(size));
    return self;
}

static VALUE
font_desc_get_size_is_absolute(self)
    VALUE self;
{
    return CBOOL2RVAL(pango_font_description_get_size_is_absolute(_SELF(self)));
}
#endif

static VALUE
font_desc_get_set_fields(self)
    VALUE self;
{
    return GFLAGS2RVAL(pango_font_description_get_set_fields(_SELF(self)), PANGO_TYPE_FONT_MASK);
}

static VALUE
font_desc_unset_fields(self, to_unset)
    VALUE self, to_unset;
{
    pango_font_description_unset_fields(_SELF(self), RVAL2GFLAGS(to_unset, PANGO_TYPE_FONT_MASK));
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

#if PANGO_CHECK_VERSION(1,8,0)
    rb_define_method(pFontDesc, "set_absolute_size", font_desc_set_absolute_size, 1);
    rb_define_method(pFontDesc, "size_is_absolute?", font_desc_get_size_is_absolute, 0);
#endif
    rb_define_method(pFontDesc, "set_fields", font_desc_get_set_fields, 0);
    rb_define_method(pFontDesc, "unset_fields", font_desc_unset_fields, 1);
    rb_define_method(pFontDesc, "merge", font_desc_merge, 2);
    rb_define_method(pFontDesc, "better_match", font_desc_better_match, 2);
    rb_define_method(pFontDesc, "to_str", font_desc_to_string, 0);
    rb_define_alias(pFontDesc, "to_s", "to_str");
    rb_define_method(pFontDesc, "to_filename", font_desc_to_filename, 0);

    G_DEF_SETTERS(pFontDesc);

    /* PangoStyle */
    G_DEF_CLASS(PANGO_TYPE_STYLE, "Style", pFontDesc);
    G_DEF_CONSTANTS(pFontDesc, PANGO_TYPE_STYLE, "PANGO_");

    /* PangoWeight */
    G_DEF_CLASS(PANGO_TYPE_WEIGHT, "Weight", pFontDesc);
    G_DEF_CONSTANTS(pFontDesc, PANGO_TYPE_WEIGHT, "PANGO_");

    /* PangoVariant */
    G_DEF_CLASS(PANGO_TYPE_VARIANT, "Variant", pFontDesc);
    G_DEF_CONSTANTS(pFontDesc, PANGO_TYPE_VARIANT, "PANGO_");

    /* PangoStretch */
    G_DEF_CLASS(PANGO_TYPE_STRETCH, "Stretch", pFontDesc);
    G_DEF_CONSTANTS(pFontDesc, PANGO_TYPE_STRETCH, "PANGO_");

    /* PangoFontMask */
    G_DEF_CLASS(PANGO_TYPE_FONT_MASK, "FontMask", pFontDesc);
    G_DEF_CONSTANTS(pFontDesc, PANGO_TYPE_FONT_MASK, "PANGO_");
}
