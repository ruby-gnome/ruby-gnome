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

#define RG_TARGET_NAMESPACE cCanvasStyle

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, goo_canvas_style_new());
    return Qnil;
}

/* TODO: make it more generic, with maybe some part in Ruby */
static VALUE
rg_set_fill_pattern(VALUE self, VALUE value)
{
    GValue gval = {0,};
    cairo_pattern_t *pattern;

    g_value_init(&gval, GOO_TYPE_CAIRO_PATTERN);
    pattern = RVAL2CRPATTERN(value);
    g_value_take_boxed(&gval, pattern);
    goo_canvas_style_set_property(RVAL2GOOCANVASSTYLE(self),
                                  goo_canvas_style_fill_pattern_id,
                                  &gval);
    g_value_unset(&gval);

    G_CHILD_SET(self, rb_intern("fill_pattern"), value);
    return self;
}

void
Init_goocanvasstyle(VALUE mGoo)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GOO_TYPE_CANVAS_STYLE, "CanvasStyle", mGoo);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_fill_pattern, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
