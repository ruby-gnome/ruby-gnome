/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Vincent Isambart <vincent.isambart@gmail.com>
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

#include "rbgoocanvasprivate.h"

#define RG_TARGET_NAMESPACE mGoo

VALUE RG_TARGET_NAMESPACE;

void
Init_goo(void)
{
    RG_TARGET_NAMESPACE = rb_define_module("Goo");

    G_DEF_CLASS(GOO_TYPE_CAIRO_MATRIX, "CairoMatrix", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GOO_TYPE_CAIRO_PATTERN, "CairoPattern", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GOO_TYPE_CAIRO_FILL_RULE, "CairoFillRule", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GOO_TYPE_CAIRO_OPERATOR, "CairoOperator", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GOO_TYPE_CAIRO_ANTIALIAS, "CairoAntiAlias", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GOO_TYPE_CAIRO_LINE_CAP, "CairoLineCap", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GOO_TYPE_CAIRO_LINE_JOIN, "CairoLineJoin", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GOO_TYPE_CAIRO_HINT_METRICS, "CairoHintMetrics", RG_TARGET_NAMESPACE);
}
