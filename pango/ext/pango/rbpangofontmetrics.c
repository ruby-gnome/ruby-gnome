/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangofontmetrics.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(self) ((PangoFontMetrics*)RVAL2BOXED(self, PANGO_TYPE_FONT_METRICS))

static VALUE
font_metrics_get_ascent(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_ascent(_SELF(self)));
}

static VALUE
font_metrics_get_descent(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_descent(_SELF(self)));
}

static VALUE
font_metrics_get_approximate_char_width(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_approximate_char_width(_SELF(self)));
}

static VALUE
font_metrics_get_approximate_digit_width(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_approximate_digit_width(_SELF(self)));
}

#if PANGO_CHECK_VERSION(1,6,0)
static VALUE
font_metrics_get_underline_thickness(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_underline_thickness(_SELF(self)));
}
static VALUE
font_metrics_get_underline_position(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_underline_position(_SELF(self)));
}
static VALUE
font_metrics_get_strikethrough_thickness(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_strikethrough_thickness(_SELF(self)));
}
static VALUE
font_metrics_get_strikethrough_position(VALUE self)
{
    return INT2NUM(pango_font_metrics_get_strikethrough_position(_SELF(self)));
}
#endif
void
Init_pango_font_metrics()
{
    VALUE pMetrics = G_DEF_CLASS(PANGO_TYPE_FONT_METRICS, "FontMetrics", mPango);
    
    rb_define_method(pMetrics, "ascent", font_metrics_get_ascent, 0);
    rb_define_method(pMetrics, "descent", font_metrics_get_descent, 0);
    rb_define_method(pMetrics, "approximate_char_width", font_metrics_get_approximate_char_width, 0);
    rb_define_method(pMetrics, "approximate_digit_width", font_metrics_get_approximate_digit_width, 0);
#if PANGO_CHECK_VERSION(1,6,0)
    rb_define_method(pMetrics, "underline_thickness", font_metrics_get_underline_thickness, 0);
    rb_define_method(pMetrics, "underline_position", font_metrics_get_underline_position, 0);
    rb_define_method(pMetrics, "strikethrough_thickness", font_metrics_get_strikethrough_thickness, 0);
    rb_define_method(pMetrics, "strikethrough_position", font_metrics_get_strikethrough_position, 0);
#endif
}
