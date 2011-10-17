/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpango.c -

  $Author: mutoh $
  $Date: 2005/09/28 18:04:21 $

  Copyright (C) 2002-2005 Masao Mutoh
************************************************/

#include "rbpangoprivate.h"

VALUE mPango;

extern void Init_pango(void);

void
Init_pango(void)
{
    mPango = rb_define_module("Pango");

    rb_define_const(mPango, "BUILD_VERSION", 
                    rb_ary_new3(3, 
                                INT2FIX(PANGO_MAJOR_VERSION), 
                                INT2FIX(PANGO_MINOR_VERSION), 
                                INT2FIX(PANGO_MICRO_VERSION)));

    Init_pango_analysis();
    Init_pango_attribute();
    Init_pango_attriterator();
    Init_pango_attrlist();
    Init_pango_cairo();
    Init_pango_color();
    Init_pango_context();
    Init_pango_coverage();
    Init_pango_engine();
    Init_pango_font();
    Init_pango_font_description();
    Init_pango_font_face();
    Init_pango_font_family();
    Init_pango_font_map();
    Init_pango_font_metrics();
    Init_pango_fontset();
    Init_pango_fontset_simple();
    Init_pango_glyph_info();
    Init_pango_glyph_item();
    Init_pango_glyph_string();
    Init_pango_gravity();
    Init_pango_item();
    Init_pango_language();
    Init_pango_layout();
    Init_pango_layout_iter();
    Init_pango_layout_line();
    Init_pango_logattr();
    Init_pango_main();
    Init_pango_matrix();
    Init_pango_rectangle();
    Init_pangorenderer();
    Init_pango_script();
    Init_pango_script_iter();
    Init_pango_array();
}
