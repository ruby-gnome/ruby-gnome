/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE rb_cPopplerColor

#ifdef POPPLER_TYPE_COLOR
VALUE RG_TARGET_NAMESPACE;

PopplerColor *
rb_poppler_ruby_object_to_color(VALUE color)
{
#ifdef POPPLER_WITH_GDK
    if (RTEST(rb_obj_is_kind_of(color, rb_cGdkColor))) {
        GdkColor *gdk_color;

        gdk_color = RVAL2GDKCOLOR(color);
        color = rb_funcall(RG_TARGET_NAMESPACE, rb_intern("new"),
                           3,
                           UINT2NUM(gdk_color->red),
                           UINT2NUM(gdk_color->green),
                           UINT2NUM(gdk_color->blue));
    }
#endif

    return RVAL2BOXED(color, POPPLER_TYPE_COLOR);
}

VALUE
rb_poppler_ruby_object_from_color_with_free(PopplerColor *color)
{
    VALUE rb_color;

    rb_color = POPPLERCOLOR2RVAL(color);
    g_free(color);
    return rb_color;
}

static VALUE
rg_initialize(VALUE self, VALUE red, VALUE green, VALUE blue)
{
    PopplerColor color;

    color.red = NUM2UINT(red);
    color.green = NUM2UINT(green);
    color.blue = NUM2UINT(blue);

    G_INITIALIZE(self, &color);
    return Qnil;
}

DEF_ACCESSOR(color, red, RVAL2POPPLERCOLOR, UINT2NUM, NUM2UINT)
DEF_ACCESSOR(color, green, RVAL2POPPLERCOLOR, UINT2NUM, NUM2UINT)
DEF_ACCESSOR(color, blue, RVAL2POPPLERCOLOR, UINT2NUM, NUM2UINT)

static VALUE
rg_to_a(VALUE self)
{
    PopplerColor *color;
    color = RVAL2POPPLERCOLOR(self);
    return rb_ary_new3(3,
                       UINT2NUM(color->red),
                       UINT2NUM(color->green),
                       UINT2NUM(color->blue));
}

static VALUE
rg_inspect(VALUE self)
{
    VALUE inspected;
    gchar *rgb;
    PopplerColor *color;

    color = RVAL2POPPLERCOLOR(self);
    inspected = rb_call_super(0, NULL);
    rb_str_resize(inspected, RSTRING_LEN(inspected) - 1);
    rgb = g_strdup_printf(": [%u, %u, %u]>",
                          color->red, color->green, color->blue);
    rb_str_cat2(inspected, rgb);
    g_free(rgb);
    return inspected;
}
#endif

void
Init_poppler_color(VALUE mPoppler)
{
#ifdef POPPLER_TYPE_COLOR
    RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_COLOR, "Color", mPoppler);

    RG_DEF_METHOD(initialize, 3);
    rbg_define_method(RG_TARGET_NAMESPACE, "red", color_get_red, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "green", color_get_green, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "blue", color_get_blue, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_red", color_set_red, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_green", color_set_green, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_blue", color_set_blue, 1);
    RG_DEF_METHOD(to_a, 0);
    RG_DEF_METHOD(inspect, 0);
#endif
}
