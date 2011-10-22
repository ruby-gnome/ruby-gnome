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
