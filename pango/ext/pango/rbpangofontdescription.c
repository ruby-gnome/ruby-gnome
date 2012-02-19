/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cFontDescription
#define _SELF(self) (RVAL2PANGOFONTDESCRIPTION(self))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE str;
    PangoFontDescription *description;

    rb_scan_args(argc, argv, "01", &str);

    if (NIL_P(str)) {
        description = pango_font_description_new();
    } else {
        description = pango_font_description_from_string(RVAL2CSTR(str));
    }

    G_INITIALIZE(self, description);
    pango_font_description_free(description);

    return Qnil;
}
/*
PangoFontDescription* pango_font_description_copy
                                            (const PangoFontDescription *desc);
PangoFontDescription* pango_font_description_copy_static
                                            (const PangoFontDescription *desc);
*/

static VALUE
rg_hash(VALUE self)
{
    return UINT2NUM(pango_font_description_hash(_SELF(self)));
}

static VALUE
rg_operator_font_desc_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(pango_font_description_equal(_SELF(self), _SELF(other)));
}

/* Don't we need this?
void        pango_font_descriptions_free    (PangoFontDescription **descs,
                                             int n_descs);
*/

static VALUE
rg_set_family(VALUE self, VALUE family)
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
rg_family(VALUE self)
{
    return CSTR2RVAL(pango_font_description_get_family(_SELF(self)));
}

static VALUE
rg_set_style(VALUE self, VALUE style)
{
    pango_font_description_set_style(_SELF(self), RVAL2PANGOSTYLE(style));
    return self;
}

static VALUE
rg_style(VALUE self)
{
    return PANGOSTYLE2RVAL(pango_font_description_get_style(_SELF(self)));
}

static VALUE
rg_set_variant(VALUE self, VALUE variant)
{
    pango_font_description_set_variant(_SELF(self), RVAL2PANGOVARIANT(variant));
    return self;
}

static VALUE
rg_variant(VALUE self)
{
    return PANGOVARIANT2RVAL(pango_font_description_get_variant(_SELF(self)));
}

static VALUE
rg_set_weight(VALUE self, VALUE weight)
{
    pango_font_description_set_weight(_SELF(self),
                      RVAL2PANGOWEIGHT(weight));
    return self;
}

static VALUE
rg_weight(VALUE self)
{
    return PANGOWEIGHT2RVAL(pango_font_description_get_weight(_SELF(self)));
}

static VALUE
rg_set_stretch(VALUE self, VALUE stretch)
{
    pango_font_description_set_stretch(_SELF(self), RVAL2PANGOSTRETCH(stretch));
    return self;
}

static VALUE
rg_stretch(VALUE self)
{
    return PANGOSTRETCH2RVAL(pango_font_description_get_stretch(_SELF(self)));
}

static VALUE
rg_set_size(VALUE self, VALUE size)
{
    pango_font_description_set_size(_SELF(self), NUM2INT(size));
    return self;
}

static VALUE
rg_size(VALUE self)
{
    return INT2NUM(pango_font_description_get_size(_SELF(self)));
}

#if PANGO_CHECK_VERSION(1,8,0)
static VALUE
rg_set_absolute_size(VALUE self, VALUE size)
{
    pango_font_description_set_absolute_size(_SELF(self), NUM2INT(size));
    return self;
}

static VALUE
rg_size_is_absolute_p(VALUE self)
{
    return CBOOL2RVAL(pango_font_description_get_size_is_absolute(_SELF(self)));
}
#endif

#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
rg_gravity(VALUE self)
{
    return PANGOGRAVITY2RVAL(pango_font_description_get_gravity(_SELF(self)));
}

static VALUE
rg_set_gravity(VALUE self, VALUE gravity)
{
    pango_font_description_set_gravity(_SELF(self), RVAL2PANGOGRAVITY(gravity));
    return self;
}
#endif

static VALUE
rg_set_fields(VALUE self)
{
    return PANGOFONTMASK2RVAL(pango_font_description_get_set_fields(_SELF(self)));
}

static VALUE
rg_unset_fields(VALUE self, VALUE to_unset)
{
    pango_font_description_unset_fields(_SELF(self), RVAL2PANGOFONTMASK(to_unset));
    return self;
}

static VALUE
rg_merge(VALUE self, VALUE desc_to_merge, VALUE replace_existing)
{
    pango_font_description_merge(_SELF(self), _SELF(desc_to_merge),
                                 RVAL2CBOOL(replace_existing));
    return self;
}

/* Don't we need this?
void        pango_font_description_merge_static
                                            (PangoFontDescription *desc,
                                             const PangoFontDescription *desc_to_merge,
                                             gboolean replace_existing);
*/

static VALUE
rg_better_match(VALUE self, VALUE old_match, VALUE new_match)
{
    return CBOOL2RVAL(pango_font_description_better_match(_SELF(self), 
                                                          _SELF(old_match),
                                                          _SELF(new_match)));
}

static VALUE
rg_to_str(VALUE self)
{
    return CSTR2RVAL(pango_font_description_to_string(_SELF(self)));
}

static VALUE
rg_to_filename(VALUE self)
{
    return CSTR2RVAL(pango_font_description_to_filename(_SELF(self)));
}

void
Init_pango_font_description(VALUE mPango)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_FONT_DESCRIPTION, "FontDescription", mPango);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(hash, 0);
    RG_DEF_METHOD_OPERATOR("==", font_desc_equal, 1);
    RG_DEF_METHOD(set_family, 1);
    RG_DEF_METHOD(family, 0);
    RG_DEF_METHOD(set_style, 1);
    RG_DEF_METHOD(style, 0);
    RG_DEF_METHOD(set_variant, 1);
    RG_DEF_METHOD(variant, 0);
    RG_DEF_METHOD(set_weight, 1);
    RG_DEF_METHOD(weight, 0);
    RG_DEF_METHOD(set_stretch, 1);
    RG_DEF_METHOD(stretch, 0);
    RG_DEF_METHOD(set_size, 1);
    RG_DEF_METHOD(size, 0);

#if PANGO_CHECK_VERSION(1,8,0)
    RG_DEF_METHOD(set_absolute_size, 1);
    RG_DEF_METHOD_P(size_is_absolute, 0);
#endif
#if PANGO_CHECK_VERSION(1,16,0)
    RG_DEF_METHOD(set_gravity, 1);
    RG_DEF_METHOD(gravity, 0);
#endif
    RG_DEF_METHOD(set_fields, 0);
    RG_DEF_METHOD(unset_fields, 1);
    RG_DEF_METHOD(merge, 2);
    RG_DEF_METHOD(better_match, 2);
    RG_DEF_METHOD(to_str, 0);
    RG_DEF_ALIAS("to_s", "to_str");
    RG_DEF_METHOD(to_filename, 0);

    /* PangoStyle */
    G_DEF_CLASS(PANGO_TYPE_STYLE, "Style", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_STYLE, "PANGO_");

    /* PangoWeight */
    G_DEF_CLASS(PANGO_TYPE_WEIGHT, "Weight", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_WEIGHT, "PANGO_");

    /* PangoVariant */
    G_DEF_CLASS(PANGO_TYPE_VARIANT, "Variant", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_VARIANT, "PANGO_");

    /* PangoStretch */
    G_DEF_CLASS(PANGO_TYPE_STRETCH, "Stretch", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_STRETCH, "PANGO_");

    /* PangoFontMask */
    G_DEF_CLASS(PANGO_TYPE_FONT_MASK, "FontMask", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, PANGO_TYPE_FONT_MASK, "PANGO_");
}
