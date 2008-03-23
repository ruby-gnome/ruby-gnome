/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbpoppler-annotation.c -

  Copyright (C) 2008 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler.h"

#if POPPLER_CHECK_VERSION(0, 7, 2)

#define SELF(self) (POPPLER_ANNOT(RVAL2GOBJ(self)))

#define TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_TYPE))

static VALUE
annot_get_annot_type(VALUE self)
{
    return TYPE2RVAL(poppler_annot_get_annot_type(SELF(self)));
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
