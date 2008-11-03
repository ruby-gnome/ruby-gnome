/* -*- c-file-style: "ruby" -*- */
/**********************************************************************

  rbpoppler-annotation.c -

  Copyright (C) 2008 Ruby-GNOME2 Project Team

**********************************************************************/

#include "rbpoppler-private.h"

#define SELF(self) (POPPLER_ANNOT(RVAL2GOBJ(self)))

#define TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_TYPE))
#define FLAG2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_FLAG))
#define REPLY_TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_MARKUP_REPLY_TYPE))
#define DATA_TYPE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_EXTERNAL_DATA_TYPE))
#define TEXT_ICON2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_TEXT_ICON))
#define TEXT_STATE2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_TEXT_STATE))
#define QUADDING2RVAL(obj) (GENUM2RVAL(obj, POPPLER_TYPE_ANNOT_FREE_TEXT_QUADDING))
#define LINE2RVAL(obj) (BOXED2RVAL(obj, POPPLER_TYPE_ANNOT_CALLOUT_LINE))
#define RVAL2LINE(obj) ((PopplerAnnotCalloutLine *)RVAL2BOXED(obj, POPPLER_TYPE_ANNOT_CALLOUT_LINE))

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

/* PopplerAnnotText */
#undef SELF
#define SELF(self) (POPPLER_ANNOT_TEXT(RVAL2GOBJ(self)))

static VALUE
annot_text_get_is_open(VALUE self)
{
    return CBOOL2RVAL(poppler_annot_text_get_is_open(SELF(self)));
}

static VALUE
annot_text_get_icon(VALUE self)
{
#if POPPLER_CHECK_VERSION(0, 9, 0)
    return CSTR2RVAL_FREE(poppler_annot_text_get_icon(SELF(self)));
#else
    return TEXT_ICON2RVAL(poppler_annot_text_get_icon(SELF(self)));
#endif
}

static VALUE
annot_text_get_state(VALUE self)
{
    return TEXT_STATE2RVAL(poppler_annot_text_get_state(SELF(self)));
}

/* PopplerAnnotFreeText */
#undef SELF
#define SELF(self) (POPPLER_ANNOT_FREE_TEXT(RVAL2GOBJ(self)))

static VALUE
annot_free_text_get_quadding(VALUE self)
{
    return QUADDING2RVAL(poppler_annot_free_text_get_quadding(SELF(self)));
}

static VALUE
annot_free_text_get_callout_line(VALUE self)
{
    return LINE2RVAL(poppler_annot_free_text_get_callout_line(SELF(self)));
}

/* PopplerCalloutLine */
static VALUE
annot_callout_line_initialize(VALUE self, VALUE multiline, VALUE x1, VALUE y1,
                              VALUE x2, VALUE y2, VALUE x3, VALUE y3)
{
    PopplerAnnotCalloutLine *line;

    line = poppler_annot_callout_line_new();
    line->multiline = RVAL2CBOOL(multiline);
    line->x1 = NUM2DBL(x1);
    line->y1 = NUM2DBL(y1);
    line->x2 = NUM2DBL(x2);
    line->y2 = NUM2DBL(y2);
    line->x3 = NUM2DBL(x3);
    line->y3 = NUM2DBL(y3);

    G_INITIALIZE(self, line);
    return Qnil;
}

DEF_ACCESSOR(annot_callout_line, multiline, RVAL2LINE, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR(annot_callout_line, x1, RVAL2LINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, y1, RVAL2LINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, x2, RVAL2LINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, y2, RVAL2LINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, x3, RVAL2LINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, y3, RVAL2LINE, rb_float_new, NUM2DBL)

static VALUE
annot_callout_line_to_a(VALUE self)
{
    PopplerAnnotCalloutLine *line;

    line = RVAL2LINE(self);
    return rb_ary_new3(7,
                       CBOOL2RVAL(line->multiline),
                       rb_float_new(line->x1),
                       rb_float_new(line->y1),
                       rb_float_new(line->x2),
                       rb_float_new(line->y2),
                       rb_float_new(line->x3),
                       rb_float_new(line->y3));
}

static VALUE
annot_callout_line_inspect(VALUE self)
{
    VALUE inspected;
    gchar *info;
    PopplerAnnotCalloutLine *line;

    line = RVAL2LINE(self);
    inspected = rb_call_super(0, NULL);
    rb_str_resize(inspected, RSTRING_LEN(inspected) - 1);
    info = g_strdup_printf(": [%s, %g, %g, %g, %g, %g, %g]>",
                           line->multiline ? "true" : "false",
                           line->x1, line->y1,
                           line->x2, line->y2,
                           line->x3, line->y3);
    rb_str_cat2(inspected, info);
    g_free(info);
    return inspected;
}

void
Init_poppler_annotation(VALUE mPoppler)
{
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
    rb_define_method(cAnnotationText, "open?", annot_text_get_is_open, 0);
    rb_define_method(cAnnotationText, "icon", annot_text_get_icon, 0);
    rb_define_method(cAnnotationText, "state", annot_text_get_state, 0);

    cAnnotationFreeText = G_DEF_CLASS(POPPLER_TYPE_ANNOT_FREE_TEXT,
                                      "AnnotationFreeText", mPoppler);
    rb_define_method(cAnnotationFreeText, "quadding",
                     annot_free_text_get_quadding, 0);
    rb_define_method(cAnnotationFreeText, "callout_line",
                     annot_free_text_get_callout_line, 0);

    cAnnotationCalloutLine = G_DEF_CLASS(POPPLER_TYPE_ANNOT_CALLOUT_LINE,
                                         "AnnotationCalloutLine", mPoppler);
    rb_define_method(cAnnotationCalloutLine, "initialize",
                     annot_callout_line_initialize, 7);

    rb_define_method(cAnnotationCalloutLine, "multiline?",
                     annot_callout_line_get_multiline, 0);
    rb_define_method(cAnnotationCalloutLine, "set_multiline",
                     annot_callout_line_set_multiline, 1);
    rb_define_method(cAnnotationCalloutLine, "x1",
                     annot_callout_line_get_x1, 0);
    rb_define_method(cAnnotationCalloutLine, "set_x1",
                     annot_callout_line_set_x1, 1);
    rb_define_method(cAnnotationCalloutLine, "y1",
                     annot_callout_line_get_y1, 0);
    rb_define_method(cAnnotationCalloutLine, "set_y1",
                     annot_callout_line_set_y1, 1);
    rb_define_method(cAnnotationCalloutLine, "x2",
                     annot_callout_line_get_x2, 0);
    rb_define_method(cAnnotationCalloutLine, "set_x2",
                     annot_callout_line_set_x2, 2);
    rb_define_method(cAnnotationCalloutLine, "y2",
                     annot_callout_line_get_y2, 0);
    rb_define_method(cAnnotationCalloutLine, "set_y2",
                     annot_callout_line_set_y2, 2);
    rb_define_method(cAnnotationCalloutLine, "x3",
                     annot_callout_line_get_x3, 0);
    rb_define_method(cAnnotationCalloutLine, "set_x3",
                     annot_callout_line_set_x3, 3);
    rb_define_method(cAnnotationCalloutLine, "y3",
                     annot_callout_line_get_y3, 0);
    rb_define_method(cAnnotationCalloutLine, "set_y3",
                     annot_callout_line_set_y3, 3);

    rb_define_method(cAnnotationCalloutLine, "to_a", annot_callout_line_to_a, 0);
    rb_define_method(cAnnotationCalloutLine, "inspect",
                     annot_callout_line_inspect, 0);

    G_DEF_SETTERS(cAnnotationCalloutLine);
}
