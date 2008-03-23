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
#define REPLY_TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_MARKUP_REPLY_TYPE))
#define DATA_TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_EXTERNAL_DATA_TYPE))

static VALUE rb_cDate;
static ID id_new;


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

/* PopplerAnnotMarkup */
#undef SELF
#define SELF(self) (POPPLER_ANNOT_MARKUP(RVAL2GOBJ(self)))

static VALUE
annot_markup_get_label(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_markup_get_label(SELF(self)));
}

static VALUE
annot_markup_get_popup_is_open(VALUE self)
{
    return CBOOL2RVAL(poppler_annot_markup_get_popup_is_open(SELF(self)));
}

static VALUE
annot_markup_get_opacity(VALUE self)
{
    return rb_float_new(poppler_annot_markup_get_opacity(SELF(self)));
}

static VALUE
annot_markup_get_date(VALUE self)
{
    GDate *date;
    VALUE rb_date;

    date = poppler_annot_markup_get_date(SELF(self));
    rb_date = rb_funcall(rb_cDate, id_new, 3,
                         UINT2NUM(g_date_get_year(date)),
                         UINT2NUM(g_date_get_month(date)),
                         UINT2NUM(g_date_get_day(date)));
    g_date_free(date);
    return rb_date;
}

static VALUE
annot_markup_get_subject(VALUE self)
{
    return CSTR2RVAL_FREE(poppler_annot_markup_get_subject(SELF(self)));
}

static VALUE
annot_markup_get_reply_to(VALUE self)
{
    return REPLY_TYPE2RVAL(poppler_annot_markup_get_reply_to(SELF(self)));
}

static VALUE
annot_markup_get_external_data(VALUE self)
{
    return DATA_TYPE2RVAL(poppler_annot_markup_get_external_data(SELF(self)));
}
#endif

void
Init_poppler_annotation(VALUE mPoppler)
{
#if POPPLER_CHECK_VERSION(0, 7, 2)
    VALUE cAnnotation, cAnnotationMarkup, cAnnotationText;
    VALUE cAnnotationFreeText, cAnnotationCalloutLine;

    id_new = rb_intern("new");
    rb_cDate = rb_const_get(rb_cObject, rb_intern("Date"));

    cAnnotation = G_DEF_CLASS(POPPLER_TYPE_ANNOT, "Annotation", mPoppler);
    rb_define_method(cAnnotation, "type", annot_get_annot_type, 0);
    rb_define_method(cAnnotation, "contents", annot_get_contents, 0);
    rb_define_method(cAnnotation, "name", annot_get_name, 0);
    rb_define_method(cAnnotation, "modified", annot_get_modified, 0);
    rb_define_method(cAnnotation, "flags", annot_get_flags, 0);
    rb_define_method(cAnnotation, "color", annot_get_color, 0);

    cAnnotationMarkup = G_DEF_CLASS(POPPLER_TYPE_ANNOT_MARKUP,
                                    "AnnotationMarkup", mPoppler);
    rb_define_method(cAnnotationMarkup, "label", annot_markup_get_label, 0);
    rb_define_method(cAnnotationMarkup, "popup_is_open?",
                     annot_markup_get_popup_is_open, 0);
    rb_define_method(cAnnotationMarkup, "opacity", annot_markup_get_opacity, 0);
    rb_define_method(cAnnotationMarkup, "date", annot_markup_get_date, 0);
    rb_define_method(cAnnotationMarkup, "subject", annot_markup_get_subject, 0);
    rb_define_method(cAnnotationMarkup, "reply_to",
                     annot_markup_get_reply_to, 0);
    rb_define_method(cAnnotationMarkup, "external_data",
                     annot_markup_get_external_data, 0);

    cAnnotationText = G_DEF_CLASS(POPPLER_TYPE_ANNOT_TEXT,
                                  "AnnotationText", mPoppler);

    cAnnotationFreeText = G_DEF_CLASS(POPPLER_TYPE_ANNOT_FREE_TEXT,
                                      "AnnotationFreeText", mPoppler);

    cAnnotationCalloutLine = G_DEF_CLASS(POPPLER_TYPE_ANNOT_CALLOUT_LINE,
                                         "AnnotationCalloutLine", mPoppler);
#endif
}
