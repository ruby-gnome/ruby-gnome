/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id$ */
/* convertion from Cairo types to GooCairo types
 * Copyright (C) 2007 Vincent Isambart <vincent.isambart@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgoocanvas.h"

static VALUE
rb_cairo_pattern_to_goo(VALUE self)
{
    GValue gval = {0,};
    VALUE result;

    g_value_init(&gval, GOO_TYPE_CAIRO_PATTERN);
    g_value_set_boxed(&gval, RVAL2CRPATTERN(self));

    result = rbgobj_gvalue_to_rvalue(&gval);
    g_value_unset(&gval);

    return result;
}

static VALUE
rb_cairo_matrix_to_goo(VALUE self)
{
    GValue gval = {0,};
    VALUE result;

    g_value_init(&gval, GOO_TYPE_CAIRO_MATRIX);
    g_value_set_boxed(&gval, RVAL2CRMATRIX(self));

    result = rbgobj_gvalue_to_rvalue(&gval);
    g_value_unset(&gval);

    return result;
}

void
Init_goocairo(void)
{
    VALUE Cairo, CairoPattern, CairoMatrix;

    G_DEF_CLASS(GOO_TYPE_CAIRO_MATRIX, "CairoMatrix", mGoo);
    G_DEF_CLASS(GOO_TYPE_CAIRO_PATTERN, "CairoPattern", mGoo);
    G_DEF_CLASS(GOO_TYPE_CAIRO_FILL_RULE, "CairoFillRule", mGoo);
    G_DEF_CLASS(GOO_TYPE_CAIRO_OPERATOR, "CairoOperator", mGoo);
    G_DEF_CLASS(GOO_TYPE_CAIRO_ANTIALIAS, "CairoAntiAlias", mGoo);
    G_DEF_CLASS(GOO_TYPE_CAIRO_LINE_CAP, "CairoLineCap", mGoo);
    G_DEF_CLASS(GOO_TYPE_CAIRO_LINE_JOIN, "CairoLineJoin", mGoo);
    G_DEF_CLASS(GOO_TYPE_CAIRO_HINT_METRICS, "CairoHintMetrics", mGoo);

    Cairo = rb_const_get(rb_mKernel, rb_intern("Cairo"));

    CairoPattern = rb_const_get(Cairo, rb_intern("Pattern"));
    rb_define_method(CairoPattern, "to_goo", rb_cairo_pattern_to_goo, 0);

    CairoMatrix = rb_const_get(Cairo, rb_intern("Matrix"));
    rb_define_method(CairoMatrix, "to_goo", rb_cairo_matrix_to_goo, 0);
}
