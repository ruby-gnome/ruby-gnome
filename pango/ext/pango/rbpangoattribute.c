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

#define RG_TARGET_NAMESPACE pattr

VALUE RG_TARGET_NAMESPACE, attrstring, pattrint, pattrfloat, pattrcolor, pattrbool;
static VALUE type_to_klass;

/***********************************************/
void
pango_add_attribute(int attr_type, VALUE klass)
{
    rb_hash_aset(type_to_klass, INT2FIX(attr_type), klass);
}

/* This is for Attributes which has PangoAttrType. */
VALUE
pango_get_attribute_klass(VALUE attr_type)
{
    VALUE type = Qnil;
    if (TYPE(attr_type) == T_STRING){
        const char *strtype = RVAL2CSTR(attr_type);
        if (strcmp(strtype, "Attribute") == 0){
            type = RG_TARGET_NAMESPACE;
        } else if (strcmp(strtype, "AttrString") == 0){
            type = attrstring;
        } else if (strcmp(strtype, "AttrInt") == 0){
            type = pattrint;
        } else if (strcmp(strtype, "AttrFloat") == 0){
            type = pattrfloat;
        } else if (strcmp(strtype, "AttrColor") == 0){
            type = pattrcolor;
        } else if (strcmp(strtype, "AttrBool") == 0){
            type = pattrbool;
        }
    } else {
        type = rb_hash_aref(type_to_klass, INT2FIX(attr_type));
    }
    return type;
}

VALUE
pango_make_attribute(PangoAttribute *attr)
{
    if (attr == NULL) return Qnil;
    return Data_Wrap_Struct(rb_hash_aref(type_to_klass, INT2FIX(attr->klass->type)), 0, 
                            pango_attribute_destroy, 
                            pango_attribute_copy(attr));
}

PangoAttribute*
pango_get_attribute(VALUE attr)
{
    PangoAttribute *gattr;

    if (NIL_P(attr)) return NULL;

    if (!rb_obj_is_kind_of(attr, RG_TARGET_NAMESPACE)) {
        rb_raise(rb_eTypeError, "not a Pango::Attribute...");
    }
    Data_Get_Struct(attr, PangoAttribute, gattr);
    return gattr;
}

/***********************************************/
static VALUE
rg_s_allocate(VALUE klass)
{
    /* Don't define destroy method here.
       return Data_Wrap_Struct(klass, 0, pango_attribute_destroy, 0);
    */
    return Data_Wrap_Struct(klass, NULL, NULL, 0);
}

#ifdef HAVE_OBJECT_ALLOCATE
#define attr_s_new rb_class_new_instance
#else
static VALUE
rg_s_new(int argc, VALUE *argv, VALUE klass)
{
    VALUE obj = rg_s_allocate(klass);
    rb_obj_call_init(obj, argc, argv);
    return obj;
}
#endif

/***********************************************/

/* This method implemented at rbpangomain.c
gboolean    pango_parse_markup              (const char *markup_text,
                                             int length,
                                             gunichar accel_marker,
                                             PangoAttrList **attr_list,
                                             char **text,
                                             gunichar *accel_char,
                                             GError **error);
*/

static VALUE
rg_s_type_register(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return INT2NUM(pango_attr_type_register(RVAL2CSTR(name)));
}

static VALUE
rg_operator_attr_equal(VALUE self, VALUE other)
{
    return CBOOL2RVAL(pango_attribute_equal(RVAL2ATTR(self), RVAL2ATTR(other)));
}

/*
 * Structure accessors 
 */
/* PangoAttribute */
static VALUE
rg_start_index(VALUE self)
{
    return UINT2NUM(RVAL2ATTR(self)->start_index);
}

static VALUE
rg_set_start_index(VALUE self, VALUE value)
{
    RVAL2ATTR(self)->start_index = NUM2UINT(value);
    return Qnil;
}

static VALUE
rg_end_index(VALUE self)
{
    return UINT2NUM(RVAL2ATTR(self)->end_index);
}

static VALUE
rg_set_end_index(VALUE self, VALUE value)
{
    RVAL2ATTR(self)->end_index = NUM2UINT(value);
    return Qnil;
}

/* PangoAttrString */
static VALUE
attr_string_value(VALUE self)
{
    return CSTR2RVAL(((PangoAttrString*)RVAL2ATTR(self))->value);
}

/* PangoAttrLanguage */
static VALUE
attr_language_value(VALUE self)
{
    return PANGOLANGUAGE2RVAL(((PangoAttrLanguage*)RVAL2ATTR(self))->value);
}

/* PangoAttrColor */
static VALUE
attr_color_value(VALUE self)
{
    PangoColor color = ((PangoAttrColor*)RVAL2ATTR(self))->color;
    return PANGOCOLOR2RVAL(&color);
}

/* PangoAttrInt */
static VALUE
attr_int_value(VALUE self)
{
    return INT2NUM(((PangoAttrInt*)RVAL2ATTR(self))->value);
}

/* PangoAttrFloat */
static VALUE
attr_float_value(VALUE self)
{
    return rb_float_new(((PangoAttrFloat*)RVAL2ATTR(self))->value);
}

/* PangoAttrBool(This is Ruby/Pango's original class) */
static VALUE
attr_bool_value(VALUE self)
{
    return CBOOL2RVAL(((PangoAttrInt*)RVAL2ATTR(self))->value);
}

/* PangoAttrFontDesc */
static VALUE
attr_fontdesc_value(VALUE self)
{
    return PANGOFONTDESCRIPTION2RVAL(((PangoAttrFontDesc*)RVAL2ATTR(self))->desc);
}

/* PangoAttrShape */
static VALUE
attr_shape_ink_rect(VALUE self)
{
    PangoRectangle rect = ((PangoAttrShape*)RVAL2ATTR(self))->ink_rect;
    return PANGORECTANGLE2RVAL(&rect);
}

static VALUE
attr_shape_logical_rect(VALUE self)
{
    PangoRectangle rect = ((PangoAttrShape*)RVAL2ATTR(self))->logical_rect;
    return PANGORECTANGLE2RVAL(&rect);
}

static VALUE
attr_shape_value(VALUE self)
{
    return rb_ary_new3(2, attr_shape_ink_rect(self), attr_shape_logical_rect(self));
}

/*
 * Initialize methods
 */
#define MAKE_ATTRINT_INIT(klassname, funcname)\
static VALUE \
attr_ ## klassname ## _initialize(VALUE self, VALUE val)\
{\
    DATA_PTR(self) = pango_attr_ ## funcname ## _new(NUM2INT(val));\
    return Qnil;\
}

#define MAKE_ATTRENUM_INIT(klassname, funcname, type)\
static VALUE \
attr_ ## klassname ## _initialize(VALUE self, VALUE val)\
{\
    DATA_PTR(self) = pango_attr_ ## funcname ## _new(RVAL2GENUM(val, type));\
    return Qnil;\
}

static VALUE
attr_AttrLanguage_initialize(VALUE self, VALUE lang)
{
    DATA_PTR(self) = pango_attr_language_new(
                     RVAL2PANGOLANGUAGE(lang));
    return Qnil;
}

static VALUE
attr_AttrFamily_initialize(VALUE self, VALUE family)
{
    DATA_PTR(self) = pango_attr_family_new(RVAL2CSTR(family));
    return Qnil;
}

MAKE_ATTRENUM_INIT(AttrStyle, style, PANGO_TYPE_STYLE); 
MAKE_ATTRENUM_INIT(AttrVariant, variant, PANGO_TYPE_VARIANT); 
MAKE_ATTRENUM_INIT(AttrStretch, stretch, PANGO_TYPE_STRETCH); 
MAKE_ATTRENUM_INIT(AttrWeight, weight, PANGO_TYPE_WEIGHT); 

static VALUE
attr_AttrSize_initialize(VALUE self, VALUE size)
{
    DATA_PTR(self) = pango_attr_size_new(NUM2INT(size));
    return Qnil;
}
#if PANGO_CHECK_VERSION(1,8,1)
static VALUE
attr_AttrAbsoluteSize_initialize(VALUE self, VALUE size)
{
    DATA_PTR(self) = pango_attr_size_new_absolute(NUM2INT(size));
    return Qnil;
}
#endif

#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
attr_AttrGravity_initialize(VALUE self, VALUE gravity)
{
    DATA_PTR(self) = pango_attr_gravity_new(RVAL2PANGOGRAVITY(gravity));
    return Qnil;
}

static VALUE
attr_AttrGravityHint_initialize(VALUE self, VALUE gravity_hint)
{
    DATA_PTR(self) = pango_attr_gravity_hint_new(RVAL2PANGOGRAVITYHINT(gravity_hint));
    return Qnil;
}
#endif

static VALUE
attr_AttrFontDescription_initialize(VALUE self, VALUE fontdescription)
{
    DATA_PTR(self) = pango_attr_font_desc_new(RVAL2PANGOFONTDESCRIPTION(fontdescription));
    return Qnil;
}

static VALUE
attr_AttrForeground_initialize(VALUE self, VALUE r, VALUE g, VALUE b)
{
    DATA_PTR(self) = pango_attr_foreground_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}

static VALUE
attr_AttrBackground_initialize(VALUE self, VALUE r, VALUE g, VALUE b)
{
    DATA_PTR(self) = pango_attr_background_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}

static VALUE
attr_AttrStrikethrough_initialize(VALUE self, VALUE strikethrough)
{
    DATA_PTR(self) = pango_attr_strikethrough_new(RVAL2CBOOL(strikethrough));
    return Qnil;
}
#ifdef HAVE_PANGO_ATTR_STRIKETHROUGH_COLOR_NEW
static VALUE
attr_AttrStrikethroughColor_initialize(VALUE self, VALUE r, VALUE g, VALUE b)
{
    DATA_PTR(self) = pango_attr_strikethrough_color_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}
#endif

MAKE_ATTRENUM_INIT(AttrUnderline, underline, PANGO_TYPE_UNDERLINE); 
#ifdef HAVE_PANGO_ATTR_UNDERLINE_COLOR_NEW
static VALUE
attr_AttrUnderlineColor_initialize(VALUE self, VALUE r, VALUE g, VALUE b)
{
    DATA_PTR(self) = pango_attr_underline_color_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}
#endif

static VALUE
attr_AttrShape_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE ink_rect, logical_rect, data;

    rb_scan_args(argc, argv, "21", &ink_rect, &logical_rect, &data);

    if (NIL_P(data)){
        DATA_PTR(self) = pango_attr_shape_new(
            RVAL2PANGORECTANGLE(ink_rect),
            RVAL2PANGORECTANGLE(logical_rect));
    } else {
#if PANGO_CHECK_VERSION(1,8,0)
        G_RELATIVE(self, data);
        DATA_PTR(self) = pango_attr_shape_new_with_data(
            RVAL2PANGORECTANGLE(ink_rect),
            RVAL2PANGORECTANGLE(logical_rect),
            (gpointer)data, NULL, NULL);
#else
        rb_warning("not supported in Pango-1.6.x. the 3rd parameter was ignored.");
        DATA_PTR(self) = pango_attr_shape_new(
            RVAL2PANGORECTANGLE(ink_rect),
            RVAL2PANGORECTANGLE(logical_rect));     
#endif
    }

    return Qnil;
}

static VALUE
attr_AttrScale_initialize(VALUE self, VALUE scale)
{
    DATA_PTR(self) = pango_attr_scale_new(NUM2DBL(scale));
    return Qnil;
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
attr_AttrFallback_initialize(VALUE self, VALUE enable_fallback)
{
    DATA_PTR(self) = pango_attr_fallback_new(RVAL2CBOOL(enable_fallback));
    return Qnil;
}
#endif

MAKE_ATTRINT_INIT(AttrRise, rise);

#if PANGO_CHECK_VERSION(1,6,0)
MAKE_ATTRINT_INIT(AttrLetterSpacing, letter_spacing);
#endif

#define MAKE_ATTR(gtype, name, parent, num)\
tmpklass = rb_define_class_under(mPango, #name, parent);\
rb_hash_aset(type_to_klass, INT2FIX(gtype), tmpklass);\
rbg_define_method(tmpklass, "initialize", attr_## name ## _initialize , num);

void
Init_pango_attribute(VALUE mPango)
{
    VALUE tmpklass;
    RG_TARGET_NAMESPACE = rb_define_class_under(mPango, "Attribute", rb_cData);

    RG_DEF_SMETHOD(type_register, 1);
    RG_DEF_METHOD_OPERATOR("==", attr_equal, 1);
    RG_DEF_METHOD(start_index, 0);
    RG_DEF_METHOD(set_start_index, 1);
    RG_DEF_METHOD(end_index, 0);
    RG_DEF_METHOD(set_end_index, 1);

#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    RG_DEF_SMETHOD(allocate, 0);
#else
    rb_define_alloc_func(RG_TARGET_NAMESPACE, (VALUE(*)_((VALUE)))rg_s_allocate);
#endif
#ifndef HAVE_OBJECT_ALLOCATE
    RG_DEF_SMETHOD(new, -1);
#endif

    attrstring = rb_define_class_under(mPango, "AttrString", RG_TARGET_NAMESPACE);
    rbg_define_method(attrstring, "value", attr_string_value, 0);

    pattrint = rb_define_class_under(mPango, "AttrInt", RG_TARGET_NAMESPACE);
    rbg_define_method(pattrint, "value", attr_int_value, 0);

    pattrfloat = rb_define_class_under(mPango, "AttrFloat", RG_TARGET_NAMESPACE);
    rbg_define_method(pattrfloat, "value", attr_float_value, 0);

    pattrcolor = rb_define_class_under(mPango, "AttrColor", RG_TARGET_NAMESPACE);
    rbg_define_method(pattrcolor, "value", attr_color_value, 0);

    pattrbool = rb_define_class_under(mPango, "AttrBool", RG_TARGET_NAMESPACE);
    rbg_define_method(pattrbool, "value", attr_bool_value, 0);

    rb_global_variable(&type_to_klass);
    type_to_klass = rb_hash_new();

    MAKE_ATTR(PANGO_ATTR_LANGUAGE, AttrLanguage, RG_TARGET_NAMESPACE, 1);
    rbg_define_method(tmpklass, "value", attr_language_value, 0);

    MAKE_ATTR(PANGO_ATTR_FAMILY, AttrFamily, attrstring, 1);
    MAKE_ATTR(PANGO_ATTR_STYLE, AttrStyle, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_WEIGHT, AttrWeight, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_VARIANT, AttrVariant, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_STRETCH, AttrStretch, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_SIZE, AttrSize, pattrint, 1);
#if PANGO_CHECK_VERSION(1,8,1)
    MAKE_ATTR(PANGO_ATTR_ABSOLUTE_SIZE, AttrAbsoluteSize, pattrint, 1);
#endif
#if PANGO_CHECK_VERSION(1,16,0)
    MAKE_ATTR(PANGO_ATTR_GRAVITY, AttrGravity, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_GRAVITY_HINT, AttrGravityHint, pattrint, 1);
#endif
    MAKE_ATTR(PANGO_ATTR_FONT_DESC, AttrFontDescription, RG_TARGET_NAMESPACE, 1);
    rbg_define_method(tmpklass, "value", attr_fontdesc_value, 0);
    MAKE_ATTR(PANGO_ATTR_FOREGROUND, AttrForeground, pattrcolor, 3);
    MAKE_ATTR(PANGO_ATTR_BACKGROUND, AttrBackground, pattrcolor, 3);
    MAKE_ATTR(PANGO_ATTR_UNDERLINE, AttrUnderline, pattrint, 1);
    /* PangoUnderline */
    G_DEF_CLASS(PANGO_TYPE_UNDERLINE, "Underline", tmpklass);
    G_DEF_CONSTANTS(tmpklass, PANGO_TYPE_UNDERLINE, "PANGO_UNDERLINE_");
#ifdef HAVE_PANGO_ATTR_UNDERLINE_COLOR_NEW
    MAKE_ATTR(PANGO_ATTR_UNDERLINE_COLOR, AttrUnderlineColor, pattrcolor, 3);
#endif

    MAKE_ATTR(PANGO_ATTR_STRIKETHROUGH, AttrStrikethrough, pattrbool, 1);
#ifdef HAVE_PANGO_ATTR_STRIKETHROUGH_COLOR_NEW
    MAKE_ATTR(PANGO_ATTR_STRIKETHROUGH_COLOR, AttrStrikethroughColor, pattrcolor, 3);
#endif
    MAKE_ATTR(PANGO_ATTR_RISE, AttrRise, pattrint, 1);
#if PANGO_CHECK_VERSION(1,6,0)
    MAKE_ATTR(PANGO_ATTR_LETTER_SPACING, AttrLetterSpacing, pattrint, 1);
#endif
    MAKE_ATTR(PANGO_ATTR_SHAPE, AttrShape, RG_TARGET_NAMESPACE, -1);
    rbg_define_method(tmpklass, "ink_rect", attr_shape_ink_rect, 0);
    rbg_define_method(tmpklass, "logical_rect", attr_shape_logical_rect, 0);
    rbg_define_method(tmpklass, "value", attr_shape_value, 0);
    MAKE_ATTR(PANGO_ATTR_SCALE, AttrScale, pattrfloat, 1);
    /* PangoScale */
    rb_define_const(tmpklass, "XX_SMALL", rb_float_new(PANGO_SCALE_XX_SMALL));
    rb_define_const(tmpklass, "X_SMALL", rb_float_new(PANGO_SCALE_X_SMALL));
    rb_define_const(tmpklass, "SMALL", rb_float_new(PANGO_SCALE_SMALL));
    rb_define_const(tmpklass, "MEDIUM", rb_float_new(PANGO_SCALE_MEDIUM));
    rb_define_const(tmpklass, "LARGE", rb_float_new(PANGO_SCALE_LARGE));
    rb_define_const(tmpklass, "X_LARGE", rb_float_new(PANGO_SCALE_X_LARGE));
    rb_define_const(tmpklass, "XX_LARGE", rb_float_new(PANGO_SCALE_XX_LARGE));
#if PANGO_CHECK_VERSION(1,4,0)
    MAKE_ATTR(PANGO_ATTR_FALLBACK, AttrFallback, pattrbool, 1);
#endif
    /* PangoAttrType */
    G_DEF_CLASS(PANGO_TYPE_ATTR_TYPE, "Type", RG_TARGET_NAMESPACE);
#define INT2ATTRTYPE(x) rbgobj_make_enum((x), PANGO_TYPE_ATTR_TYPE)
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_INVALID", INT2ATTRTYPE(PANGO_ATTR_INVALID));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_LANGUAGE", INT2ATTRTYPE(PANGO_ATTR_LANGUAGE));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_FAMILY", INT2ATTRTYPE(PANGO_ATTR_FAMILY));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_STYLE", INT2ATTRTYPE(PANGO_ATTR_STYLE));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_WEIGHT", INT2ATTRTYPE(PANGO_ATTR_WEIGHT));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_VARIANT", INT2ATTRTYPE(PANGO_ATTR_VARIANT));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_STRETCH", INT2ATTRTYPE(PANGO_ATTR_STRETCH));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_SIZE", INT2ATTRTYPE(PANGO_ATTR_SIZE));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_FONT_DESC", INT2ATTRTYPE(PANGO_ATTR_FONT_DESC));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_FOREGROUND", INT2ATTRTYPE(PANGO_ATTR_FOREGROUND));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_BACKGROUND", INT2ATTRTYPE(PANGO_ATTR_BACKGROUND));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_UNDERLINE", INT2ATTRTYPE(PANGO_ATTR_UNDERLINE));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_STRIKETHROUGH", INT2ATTRTYPE(PANGO_ATTR_STRIKETHROUGH));
#if PANGO_CHECK_VERSION(1,8,0)
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_STRIKETHROUGH_COLOR", INT2ATTRTYPE(PANGO_ATTR_STRIKETHROUGH_COLOR));
#endif
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_RISE", INT2ATTRTYPE(PANGO_ATTR_RISE));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_SHAPE", INT2ATTRTYPE(PANGO_ATTR_SHAPE));
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_SCALE", INT2ATTRTYPE(PANGO_ATTR_SCALE));
#if PANGO_CHECK_VERSION(1,8,0)
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_FALLBACK", INT2ATTRTYPE(PANGO_ATTR_FALLBACK));
#endif
#if PANGO_CHECK_VERSION(1,6,0)
    rb_define_const(RG_TARGET_NAMESPACE, "TYPE_LETTER_SPACING", INT2ATTRTYPE(PANGO_ATTR_LETTER_SPACING));
#endif
}
