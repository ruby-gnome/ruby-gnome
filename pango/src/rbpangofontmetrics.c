/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontmetrics.c -

  $Author: mutoh $
  $Date: 2003/02/01 17:13:25 $

  Copyright (C) 2002,2003 Masao Mutoh <mutoh@highway.ne.jp>
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoFontMetrics*)RVAL2BOXED(self, PANGO_TYPE_FONT_METRICS))

static VALUE
font_metrics_get_ascent(self)
    VALUE self;
{
    return INT2NUM(pango_font_metrics_get_ascent(_SELF(self)));
}

static VALUE
font_metrics_get_descent(self)
    VALUE self;
{
    return INT2NUM(pango_font_metrics_get_descent(_SELF(self)));
}

static VALUE
font_metrics_get_approximate_char_width(self)
    VALUE self;
{
    return INT2NUM(pango_font_metrics_get_approximate_char_width(_SELF(self)));
}

static VALUE
font_metrics_get_approximate_digit_width(self)
    VALUE self;
{
    return INT2NUM(pango_font_metrics_get_approximate_digit_width(_SELF(self)));
}

void
Init_pango_font_metrics()
{
    VALUE pMetrics = G_DEF_CLASS(PANGO_TYPE_FONT_METRICS, "FontMetrics", mPango);
    
    rb_define_method(pMetrics, "ascent", font_metrics_get_ascent, 0);
    rb_define_method(pMetrics, "descent", font_metrics_get_descent, 0);
    rb_define_method(pMetrics, "approximate_char_width", font_metrics_get_approximate_char_width, 0);
    rb_define_method(pMetrics, "approximate_digit_width", font_metrics_get_approximate_digit_width, 0);
}
