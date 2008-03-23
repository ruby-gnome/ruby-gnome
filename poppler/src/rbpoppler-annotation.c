/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbpoppler-annotation.c -

  Copyright (C) 2008 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

#if POPPLER_CHECK_VERSION(0, 7, 2)

#define SELF(self) (POPPLER_ANNOT(RVAL2GOBJ(self)))

#define TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_TYPE))
#define FLAG2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_FLAG))

static VALUE
annot_get_annot_type(VALUE self)
{
    return TYPE2RVAL(poppler_annot_get_annot_type(SELF(self)));
}

static VALUE
annot_get_contents(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_contents(SELF(self)));
}

static VALUE
annot_get_name(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_name(SELF(self)));
}

static VALUE
annot_get_modified(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_get_modified(SELF(self)));
}

static VALUE
annot_get_flags(VALUE self)
{
    return FLAG2RVAL(poppler_annot_get_flags(SELF(self)));
}

static VALUE
annot_get_color(VALUE self)
{
    return POPPLER_COLOR2RVAL_FREE(poppler_annot_get_color(SELF(self)));
}
#endif

void
Init_poppler_annotation(VALUE mPoppler)
{
#if POPPLER_CHECK_VERSION(0, 7, 2)
    VALUE cAnnotation, cAnnotationMarkup, cAnnotationText;
    VALUE cAnnotationFreeText, cAnnotationCalloutLine;

    cAnnotation = G_DEF_CLASS(POPPLER_TYPE_ANNOT, "Annotation", mPoppler);
    rb_define_method(cAnnotation, "type", annot_get_annot_type, 0);
    rb_define_method(cAnnotation, "contents", annot_get_contents, 0);
    rb_define_method(cAnnotation, "name", annot_get_name, 0);
    rb_define_method(cAnnotation, "modified", annot_get_modified, 0);
    rb_define_method(cAnnotation, "flags", annot_get_flags, 0);
    rb_define_method(cAnnotation, "color", annot_get_color, 0);

    cAnnotationMarkup = G_DEF_CLASS(POPPLER_TYPE_ANNOT_MARKUP,
                                    "AnnotationMarkup", mPoppler);

    cAnnotationText = G_DEF_CLASS(POPPLER_TYPE_ANNOT_TEXT,
                                  "AnnotationText", mPoppler);

    cAnnotationFreeText = G_DEF_CLASS(POPPLER_TYPE_ANNOT_FREE_TEXT,
                                      "AnnotationFreeText", mPoppler);

    cAnnotationCalloutLine = G_DEF_CLASS(POPPLER_TYPE_ANNOT_CALLOUT_LINE,
                                         "AnnotationCalloutLine", mPoppler);
#endif
}
