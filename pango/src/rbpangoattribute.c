/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoattribute.c -

  $Author: mutoh $
  $Date: 2005/11/16 16:02:18 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

VALUE pattr, attrstring, pattrint, pattrfloat, pattrcolor, pattrbool;
static VALUE type_to_klass;

/***********************************************/
void
pango_add_attribute(attr_type, klass)
    int attr_type;
    VALUE klass;
{
    rb_hash_aset(type_to_klass, INT2FIX(attr_type), klass);
}

/* This is for Attributes which has PangoAttrType. */
VALUE
pango_get_attribute_klass(attr_type)
    VALUE attr_type;
{
    VALUE type = Qnil;
    if (TYPE(attr_type) == T_STRING){
        char* strtype = RVAL2CSTR(attr_type);
        if (strcmp(strtype, "Attribute") == 0){
            type = pattr;
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
pango_make_attribute(attr)
    PangoAttribute* attr;
{
    if (attr == NULL) return Qnil;
    return Data_Wrap_Struct(rb_hash_aref(type_to_klass, INT2FIX(attr->klass->type)), 0, 
                            pango_attribute_destroy, 
                            pango_attribute_copy(attr));
}

PangoAttribute*
pango_get_attribute(attr)
    VALUE attr;
{
    PangoAttribute *gattr;

    if (NIL_P(attr)) return NULL;

    if (!rb_obj_is_kind_of(attr, pattr)) {
        rb_raise(rb_eTypeError, "not a Pango::Attribute...");
    }
    Data_Get_Struct(attr, PangoAttribute, gattr);
    return gattr;
}

/***********************************************/
static VALUE
attr_s_allocate(klass)
    VALUE klass;
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
attr_s_new(argc, argv, klass)
    int argc;
    VALUE* argv;
    VALUE klass;
{
    VALUE obj = attr_s_allocate(klass);
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
attr_s_type_register(self, name)
    VALUE self, name;
{
    return INT2NUM(pango_attr_type_register(RVAL2CSTR(name)));
}

static VALUE
attr_equal(self, other)
    VALUE self, other;
{
    return CBOOL2RVAL(pango_attribute_equal(RVAL2ATTR(self), RVAL2ATTR(other)));
}

/*
 * Structure accessors 
 */
/* PangoAttribute */
static VALUE
attr_start_index(self)
    VALUE self;
{
    return UINT2NUM(RVAL2ATTR(self)->start_index);
}

static VALUE
attr_end_index(self)
    VALUE self;
{
    return UINT2NUM(RVAL2ATTR(self)->end_index);
}

/* PangoAttrString */
static VALUE
attr_string_value(self)
    VALUE self;
{
    return CSTR2RVAL(((PangoAttrString*)RVAL2ATTR(self))->value);
}

/* PangoAttrLanguage */
static VALUE
attr_language_value(self)
    VALUE self;
{
    return BOXED2RVAL(((PangoAttrLanguage*)RVAL2ATTR(self))->value, PANGO_TYPE_LANGUAGE);
}

/* PangoAttrColor */
static VALUE
attr_color_value(self)
    VALUE self;
{
    PangoColor color = ((PangoAttrColor*)RVAL2ATTR(self))->color;
    return BOXED2RVAL(&color, PANGO_TYPE_COLOR);
}

/* PangoAttrInt */
static VALUE
attr_int_value(self)
    VALUE self;
{
    return INT2NUM(((PangoAttrInt*)RVAL2ATTR(self))->value);
}

/* PangoAttrFloat */
static VALUE
attr_float_value(self)
    VALUE self;
{
    return rb_float_new(((PangoAttrFloat*)RVAL2ATTR(self))->value);
}

/* PangoAttrBool(This is Ruby/Pango's original class) */
static VALUE
attr_bool_value(self)
    VALUE self;
{
    return CBOOL2RVAL(((PangoAttrInt*)RVAL2ATTR(self))->value);
}

/* PangoAttrFontDesc */
static VALUE
attr_fontdesc_value(self)
    VALUE self;
{
    return BOXED2RVAL(((PangoAttrFontDesc*)RVAL2ATTR(self))->desc, PANGO_TYPE_FONT_DESCRIPTION);
}

/* PangoAttrShape */
static VALUE
attr_shape_ink_rect(self)
    VALUE self;
{
    PangoRectangle rect = ((PangoAttrShape*)RVAL2ATTR(self))->ink_rect;
    return BOXED2RVAL(&rect, PANGO_TYPE_RECTANGLE);
}

static VALUE
attr_shape_logical_rect(self)
    VALUE self;
{
    PangoRectangle rect = ((PangoAttrShape*)RVAL2ATTR(self))->logical_rect;
    return BOXED2RVAL(&rect, PANGO_TYPE_RECTANGLE);
}

static VALUE
attr_shape_value(self)
    VALUE self;
{
    return rb_ary_new3(2, attr_shape_ink_rect(self), attr_shape_logical_rect(self));
}

/*
 * Initialize methods
 */
#define MAKE_ATTRINT_INIT(klassname, funcname)\
static VALUE \
attr_ ## klassname ## _initialize(self, val)\
    VALUE self, val;\
{\
    DATA_PTR(self) = pango_attr_ ## funcname ## _new(NUM2INT(val));\
    return Qnil;\
}

#define MAKE_ATTRENUM_INIT(klassname, funcname, type)\
static VALUE \
attr_ ## klassname ## _initialize(self, val)\
    VALUE self, val;\
{\
    DATA_PTR(self) = pango_attr_ ## funcname ## _new(RVAL2GENUM(val, type));\
    return Qnil;\
}

static VALUE
attr_AttrLanguage_initialize(self, lang)
    VALUE self, lang;
{
    DATA_PTR(self) = pango_attr_language_new(
                     (PangoLanguage*)RVAL2BOXED(lang, PANGO_TYPE_LANGUAGE));
    return Qnil;
}

static VALUE
attr_AttrFamily_initialize(self, family)
    VALUE self, family;
{
    DATA_PTR(self) = pango_attr_family_new(RVAL2CSTR(family));
    return Qnil;
}

MAKE_ATTRENUM_INIT(AttrStyle, style, PANGO_TYPE_STYLE); 
MAKE_ATTRENUM_INIT(AttrVariant, variant, PANGO_TYPE_VARIANT); 
MAKE_ATTRENUM_INIT(AttrStretch, stretch, PANGO_TYPE_STRETCH); 
MAKE_ATTRENUM_INIT(AttrWeight, weight, PANGO_TYPE_WEIGHT); 

static VALUE
attr_AttrSize_initialize(self, size)
    VALUE self, size;
{
    DATA_PTR(self) = pango_attr_size_new(NUM2INT(size));
    return Qnil;
}
#if PANGO_CHECK_VERSION(1,8,1)
static VALUE
attr_AttrAbsoluteSize_initialize(self, size)
    VALUE self, size;
{
    DATA_PTR(self) = pango_attr_size_new_absolute(NUM2INT(size));
    return Qnil;
}
#endif

static VALUE
attr_AttrFontDescription_initialize(self, fontdescription)
    VALUE self, fontdescription;
{
    DATA_PTR(self) = pango_attr_font_desc_new(
        (PangoFontDescription*)RVAL2BOXED(fontdescription, 
                                          PANGO_TYPE_FONT_DESCRIPTION));

    return Qnil;
}

static VALUE
attr_AttrForeground_initialize(self, r, g, b)
    VALUE self, r, g, b;
{
    DATA_PTR(self) = pango_attr_foreground_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}

static VALUE
attr_AttrBackground_initialize(self, r, g, b)
    VALUE self, r, g, b;
{
    DATA_PTR(self) = pango_attr_background_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}

static VALUE
attr_AttrStrikethrough_initialize(self, strikethrough)
    VALUE self, strikethrough;
{
    DATA_PTR(self) = pango_attr_strikethrough_new(RTEST(strikethrough));
    return Qnil;
}
#if HAVE_PANGO_ATTR_STRIKETHROUGH_COLOR_NEW
static VALUE
attr_AttrStrikethroughColor_initialize(self, r, g, b)
    VALUE self, r, g, b;
{
    DATA_PTR(self) = pango_attr_strikethrough_color_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}
#endif

MAKE_ATTRENUM_INIT(AttrUnderline, underline, PANGO_TYPE_UNDERLINE); 
#if HAVE_PANGO_ATTR_UNDERLINE_COLOR_NEW
static VALUE
attr_AttrUnderlineColor_initialize(self, r, g, b)
    VALUE self, r, g, b;
{
    DATA_PTR(self) = pango_attr_underline_color_new(FIX2UINT(r), FIX2UINT(g), FIX2UINT(b));
    return Qnil;
}
#endif

static VALUE
attr_AttrShape_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE ink_rect, logical_rect, data;
    
    rb_scan_args(argc, argv, "21", &ink_rect, &logical_rect, &data);

    if (NIL_P(data)){
        DATA_PTR(self) = pango_attr_shape_new(
            (PangoRectangle*)RVAL2BOXED(ink_rect, PANGO_TYPE_RECTANGLE),
            (PangoRectangle*)RVAL2BOXED(logical_rect, PANGO_TYPE_RECTANGLE));
    } else {
#if PANGO_CHECK_VERSION(1,8,0)
        G_RELATIVE(self, data);
        DATA_PTR(self) = pango_attr_shape_new_with_data(
            (PangoRectangle*)RVAL2BOXED(ink_rect, PANGO_TYPE_RECTANGLE),
            (PangoRectangle*)RVAL2BOXED(logical_rect, PANGO_TYPE_RECTANGLE),
            (gpointer)data, NULL, NULL);
#else
        rb_warning("not supported in Pango-1.6.x. the 3rd parameter was ignored.");
        DATA_PTR(self) = pango_attr_shape_new(
            (PangoRectangle*)RVAL2BOXED(ink_rect, PANGO_TYPE_RECTANGLE),
            (PangoRectangle*)RVAL2BOXED(logical_rect, PANGO_TYPE_RECTANGLE));     
#endif
    }

    return Qnil;
}

static VALUE
attr_AttrScale_initialize(self, scale)
    VALUE self, scale;
{
    DATA_PTR(self) = pango_attr_scale_new(NUM2DBL(scale));
    return Qnil;
}

#if PANGO_CHECK_VERSION(1,4,0)
static VALUE
attr_AttrFallback_initialize(self, enable_fallback)
    VALUE self, enable_fallback;
{
    DATA_PTR(self) = pango_attr_fallback_new(RTEST(enable_fallback));
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
rb_define_method(tmpklass, "initialize", attr_## name ## _initialize , num);

void
Init_pango_attribute()
{
    VALUE tmpklass;
    pattr = rb_define_class_under(mPango, "Attribute", rb_cData);

    rb_define_singleton_method(pattr, "type_register", attr_s_type_register, 1);
    rb_define_method(pattr, "==", attr_equal, 1);
    rb_define_method(pattr, "start_index", attr_start_index, 0);
    rb_define_method(pattr, "end_index", attr_end_index, 0);

#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    rb_define_singleton_method(pattr, "allocate", attr_s_allocate, 0);
#else
    rb_define_alloc_func(pattr, (VALUE(*)_((VALUE)))attr_s_allocate);
#endif
#ifndef HAVE_OBJECT_ALLOCATE
    rb_define_singleton_method(pattr, "new", attr_s_new, -1);
#endif

    attrstring = rb_define_class_under(mPango, "AttrString", pattr);
    rb_define_method(attrstring, "value", attr_string_value, 0);

    pattrint = rb_define_class_under(mPango, "AttrInt", pattr);
    rb_define_method(pattrint, "value", attr_int_value, 0);

    pattrfloat = rb_define_class_under(mPango, "AttrFloat", pattr);
    rb_define_method(pattrfloat, "value", attr_float_value, 0);

    pattrcolor = rb_define_class_under(mPango, "AttrColor", pattr);
    rb_define_method(pattrcolor, "value", attr_color_value, 0);

    pattrbool = rb_define_class_under(mPango, "AttrBool", pattr);
    rb_define_method(pattrbool, "value", attr_bool_value, 0);

    rb_global_variable(&type_to_klass);
    type_to_klass = rb_hash_new();

    MAKE_ATTR(PANGO_ATTR_LANGUAGE, AttrLanguage, pattr, 1);
    rb_define_method(tmpklass, "value", attr_language_value, 0);
    
    MAKE_ATTR(PANGO_ATTR_FAMILY, AttrFamily, attrstring, 1);
    MAKE_ATTR(PANGO_ATTR_STYLE, AttrStyle, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_WEIGHT, AttrWeight, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_VARIANT, AttrVariant, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_STRETCH, AttrStretch, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_SIZE, AttrSize, pattrint, 1);
#if PANGO_CHECK_VERSION(1,8,1)
    MAKE_ATTR(PANGO_ATTR_ABSOLUTE_SIZE, AttrAbsoluteSize, pattrint, 1);
#endif
    MAKE_ATTR(PANGO_ATTR_FONT_DESC, AttrFontDescription, pattr, 1);
    rb_define_method(tmpklass, "value", attr_fontdesc_value, 0);
    MAKE_ATTR(PANGO_ATTR_FOREGROUND, AttrForeground, pattrcolor, 3);
    MAKE_ATTR(PANGO_ATTR_BACKGROUND, AttrBackground, pattrcolor, 3);
    MAKE_ATTR(PANGO_ATTR_UNDERLINE, AttrUnderline, pattrint, 1);
    /* PangoUnderline */
    G_DEF_CLASS(PANGO_TYPE_UNDERLINE, "Underline", tmpklass);
    G_DEF_CONSTANTS(tmpklass, PANGO_TYPE_UNDERLINE, "PANGO_UNDERLINE_");
#if HAVE_PANGO_ATTR_UNDERLINE_COLOR_NEW
    MAKE_ATTR(PANGO_ATTR_UNDERLINE_COLOR, AttrUnderlineColor, pattrcolor, 3);
#endif

    MAKE_ATTR(PANGO_ATTR_STRIKETHROUGH, AttrStrikethrough, pattrbool, 1);
#if HAVE_PANGO_ATTR_STRIKETHROUGH_COLOR_NEW
    MAKE_ATTR(PANGO_ATTR_STRIKETHROUGH_COLOR, AttrStrikethroughColor, pattrcolor, 3);
#endif
    MAKE_ATTR(PANGO_ATTR_RISE, AttrRise, pattrint, 1);
#if PANGO_CHECK_VERSION(1,6,0)
    MAKE_ATTR(PANGO_ATTR_LETTER_SPACING, AttrLetterSpacing, pattrint, 1);
#endif
    MAKE_ATTR(PANGO_ATTR_SHAPE, AttrShape, pattr, -1);
    rb_define_method(tmpklass, "ink_rect", attr_shape_ink_rect, 0);
    rb_define_method(tmpklass, "logical_rect", attr_shape_logical_rect, 0);
    rb_define_method(tmpklass, "value", attr_shape_value, 0);
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
    G_DEF_CLASS(PANGO_TYPE_ATTR_TYPE, "Type", pattr);
#define INT2ATTRTYPE(x) rbgobj_make_enum((x), PANGO_TYPE_ATTR_TYPE)
    rb_define_const(pattr, "TYPE_INVALID", INT2ATTRTYPE(PANGO_ATTR_INVALID));
    rb_define_const(pattr, "TYPE_LANGUAGE", INT2ATTRTYPE(PANGO_ATTR_LANGUAGE));
    rb_define_const(pattr, "TYPE_FAMILY", INT2ATTRTYPE(PANGO_ATTR_FAMILY));
    rb_define_const(pattr, "TYPE_STYLE", INT2ATTRTYPE(PANGO_ATTR_STYLE));
    rb_define_const(pattr, "TYPE_WEIGHT", INT2ATTRTYPE(PANGO_ATTR_WEIGHT));
    rb_define_const(pattr, "TYPE_VARIANT", INT2ATTRTYPE(PANGO_ATTR_VARIANT));
    rb_define_const(pattr, "TYPE_STRETCH", INT2ATTRTYPE(PANGO_ATTR_STRETCH));
    rb_define_const(pattr, "TYPE_SIZE", INT2ATTRTYPE(PANGO_ATTR_SIZE));
    rb_define_const(pattr, "TYPE_FONT_DESC", INT2ATTRTYPE(PANGO_ATTR_FONT_DESC));
    rb_define_const(pattr, "TYPE_FOREGROUND", INT2ATTRTYPE(PANGO_ATTR_FOREGROUND));
    rb_define_const(pattr, "TYPE_BACKGROUND", INT2ATTRTYPE(PANGO_ATTR_BACKGROUND));
    rb_define_const(pattr, "TYPE_UNDERLINE", INT2ATTRTYPE(PANGO_ATTR_UNDERLINE));
    rb_define_const(pattr, "TYPE_STRIKETHROUGH", INT2ATTRTYPE(PANGO_ATTR_STRIKETHROUGH));
#if PANGO_CHECK_VERSION(1,8,0)
    rb_define_const(pattr, "TYPE_STRIKETHROUGH_COLOR", INT2ATTRTYPE(PANGO_ATTR_STRIKETHROUGH_COLOR));
#endif
    rb_define_const(pattr, "TYPE_RISE", INT2ATTRTYPE(PANGO_ATTR_RISE));
    rb_define_const(pattr, "TYPE_SHAPE", INT2ATTRTYPE(PANGO_ATTR_SHAPE));
    rb_define_const(pattr, "TYPE_SCALE", INT2ATTRTYPE(PANGO_ATTR_SCALE));
#if PANGO_CHECK_VERSION(1,8,0)
    rb_define_const(pattr, "TYPE_FALLBACK", INT2ATTRTYPE(PANGO_ATTR_FALLBACK));
#endif
#if PANGO_CHECK_VERSION(1,6,0)
    rb_define_const(pattr, "TYPE_LETTER_SPACING", INT2ATTRTYPE(PANGO_ATTR_LETTER_SPACING));
#endif
}
