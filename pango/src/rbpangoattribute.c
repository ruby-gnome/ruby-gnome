/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangoattribute.c -

  $Author: mutoh $
  $Date: 2003/02/01 17:13:25 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
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
    printf("gattr type = %d\n", gattr->klass->type);
    return gattr;
}

/***********************************************/
static VALUE
attr_s_allocate(klass)
    VALUE klass;
{
    return Data_Wrap_Struct(klass, 0, pango_attribute_destroy, 0);
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
attr_s_type_register(name)
    VALUE name;
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

MAKE_ATTRINT_INIT(AttrStyle, style); 
MAKE_ATTRINT_INIT(AttrVariant, variant); 
MAKE_ATTRINT_INIT(AttrStretch, stretch); 
MAKE_ATTRINT_INIT(AttrWeight, weight); 
MAKE_ATTRINT_INIT(AttrSize, size); 

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

MAKE_ATTRINT_INIT(AttrUnderline, underline); 

static VALUE
attr_AttrShape_initialize(self, ink_rect, logical_rect)
    VALUE self, ink_rect, logical_rect;
{
    DATA_PTR(self) = pango_attr_shape_new(
        (PangoRectangle*)RVAL2BOXED(ink_rect, PANGO_TYPE_RECTANGLE),
        (PangoRectangle*)RVAL2BOXED(logical_rect, PANGO_TYPE_RECTANGLE));
    return Qnil;
}

static VALUE
attr_AttrScale_initialize(self, scale)
    VALUE self, scale;
{
    DATA_PTR(self) = pango_attr_scale_new(NUM2DBL(scale));
    return Qnil;
}

MAKE_ATTRINT_INIT(AttrRise, rise);

#define MAKE_ATTR(gtype, name, parent, num)\
tmpklass = rb_define_class_under(mPango, #name, parent);\
rb_hash_aset(type_to_klass, INT2FIX(gtype), tmpklass);\
rb_define_method(tmpklass, "initialize", attr_## name ## _initialize , num);

void
Init_pango_attribute()
{
    VALUE tmpklass;

    pattr = rb_define_class_under(mPango, "Attribute", GTYPE2CLASS(G_TYPE_BOXED));

    rb_define_singleton_method(pattr, "type_register", attr_s_type_register, 1);
    rb_define_method(pattr, "==", attr_equal, 1);
    rb_define_method(pattr, "start_index", attr_start_index, 0);
    rb_define_method(pattr, "end_index", attr_end_index, 0);

#ifndef HAVE_RB_DEFINE_ALLOC_FUNC
    rb_define_singleton_method(pattr, "allocate", attr_s_allocate, 0);
#else
    rb_define_alloc_func(pattr, attr_s_allocate);
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
    MAKE_ATTR(PANGO_ATTR_FONT_DESC, AttrFontDescription, pattr, 1);
    rb_define_method(tmpklass, "value", attr_fontdesc_value, 0);
    MAKE_ATTR(PANGO_ATTR_FOREGROUND, AttrForeground, pattrcolor, 3);
    MAKE_ATTR(PANGO_ATTR_BACKGROUND, AttrBackground, pattrcolor, 3);
    MAKE_ATTR(PANGO_ATTR_UNDERLINE, AttrUnderline, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_STRIKETHROUGH, AttrStrikethrough, pattrbool, 1);
    MAKE_ATTR(PANGO_ATTR_RISE, AttrRise, pattrint, 1);
    MAKE_ATTR(PANGO_ATTR_SHAPE, AttrShape, pattr, 2);
    rb_define_method(tmpklass, "ink_rect", attr_shape_ink_rect, 0);
    rb_define_method(tmpklass, "logical_rect", attr_shape_logical_rect, 0);
    rb_define_method(tmpklass, "value", attr_shape_value, 0);
    MAKE_ATTR(PANGO_ATTR_SCALE, AttrScale, pattrfloat, 1);

    rb_define_const(pattr, "SCALE_XX_SMALL", rb_float_new(PANGO_SCALE_XX_SMALL));
    rb_define_const(pattr, "SCALE_X_SMALL", rb_float_new(PANGO_SCALE_X_SMALL));
    rb_define_const(pattr, "SCALE_SMALL", rb_float_new(PANGO_SCALE_SMALL));
    rb_define_const(pattr, "SCALE_MEDIUM", rb_float_new(PANGO_SCALE_MEDIUM));
    rb_define_const(pattr, "SCALE_LARGE", rb_float_new(PANGO_SCALE_LARGE));
    rb_define_const(pattr, "SCALE_X_LARGE", rb_float_new(PANGO_SCALE_X_LARGE));
    rb_define_const(pattr, "SCALE_XX_LARGE", rb_float_new(PANGO_SCALE_XX_LARGE));

    /* PangoUnderline */
    rb_define_const(pattr, "UNDERLINE_NONE", INT2FIX(PANGO_UNDERLINE_NONE));
    rb_define_const(pattr, "UNDERLINE_SINGLE", INT2FIX(PANGO_UNDERLINE_SINGLE));
    rb_define_const(pattr, "UNDERLINE_DOUBLE", INT2FIX(PANGO_UNDERLINE_DOUBLE));
    rb_define_const(pattr, "UNDERLINE_LOW", INT2FIX(PANGO_UNDERLINE_LOW));

    rb_define_const(pattr, "TYPE_INVALID", INT2FIX(PANGO_ATTR_INVALID));
    rb_define_const(pattr, "TYPE_LANGUAGE", INT2FIX(PANGO_ATTR_LANGUAGE));
    rb_define_const(pattr, "TYPE_FAMILY", INT2FIX(PANGO_ATTR_FAMILY));
    rb_define_const(pattr, "TYPE_STYLE", INT2FIX(PANGO_ATTR_STYLE));
    rb_define_const(pattr, "TYPE_WEIGHT", INT2FIX(PANGO_ATTR_WEIGHT));
    rb_define_const(pattr, "TYPE_VARIANT", INT2FIX(PANGO_ATTR_VARIANT));
    rb_define_const(pattr, "TYPE_STRETCH", INT2FIX(PANGO_ATTR_STRETCH));
    rb_define_const(pattr, "TYPE_SIZE", INT2FIX(PANGO_ATTR_SIZE));
    rb_define_const(pattr, "TYPE_FONT_DESC", INT2FIX(PANGO_ATTR_FONT_DESC));
    rb_define_const(pattr, "TYPE_FOREGROUND", INT2FIX(PANGO_ATTR_FOREGROUND));
    rb_define_const(pattr, "TYPE_BACKGROUND", INT2FIX(PANGO_ATTR_BACKGROUND));
    rb_define_const(pattr, "TYPE_UNDERLINE", INT2FIX(PANGO_ATTR_UNDERLINE));
    rb_define_const(pattr, "TYPE_STRIKETHROUGH", INT2FIX(PANGO_ATTR_STRIKETHROUGH));
    rb_define_const(pattr, "TYPE_RISE", INT2FIX(PANGO_ATTR_RISE));
    rb_define_const(pattr, "TYPE_SHAPE", INT2FIX(PANGO_ATTR_SHAPE));
    rb_define_const(pattr, "TYPE_SCALE", INT2FIX(PANGO_ATTR_SCALE));

}
