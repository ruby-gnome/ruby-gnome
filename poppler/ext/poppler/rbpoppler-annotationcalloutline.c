/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cAnnotation

static VALUE
rg_initialize(VALUE self, VALUE multiline, VALUE x1, VALUE y1,
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

DEF_ACCESSOR(annot_callout_line, multiline, RVAL2POPPLERANNOTCALLOUTLINE, CBOOL2RVAL, RVAL2CBOOL)
DEF_ACCESSOR(annot_callout_line, x1, RVAL2POPPLERANNOTCALLOUTLINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, y1, RVAL2POPPLERANNOTCALLOUTLINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, x2, RVAL2POPPLERANNOTCALLOUTLINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, y2, RVAL2POPPLERANNOTCALLOUTLINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, x3, RVAL2POPPLERANNOTCALLOUTLINE, rb_float_new, NUM2DBL)
DEF_ACCESSOR(annot_callout_line, y3, RVAL2POPPLERANNOTCALLOUTLINE, rb_float_new, NUM2DBL)

static VALUE
rg_to_a(VALUE self)
{
    PopplerAnnotCalloutLine *line;

    line = RVAL2POPPLERANNOTCALLOUTLINE(self);
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
rg_inspect(VALUE self)
{
    VALUE inspected;
    gchar *info;
    PopplerAnnotCalloutLine *line;

    line = RVAL2POPPLERANNOTCALLOUTLINE(self);
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
Init_poppler_annotationcalloutline(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_ANNOT_CALLOUT_LINE,
                                         "AnnotationCalloutLine", mPoppler);

    RG_DEF_METHOD(initialize, 7);

    rb_define_method(RG_TARGET_NAMESPACE, "multiline?",
                     annot_callout_line_get_multiline, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_multiline",
                     annot_callout_line_set_multiline, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "x1",
                     annot_callout_line_get_x1, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_x1",
                     annot_callout_line_set_x1, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "y1",
                     annot_callout_line_get_y1, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_y1",
                     annot_callout_line_set_y1, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "x2",
                     annot_callout_line_get_x2, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_x2",
                     annot_callout_line_set_x2, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "y2",
                     annot_callout_line_get_y2, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_y2",
                     annot_callout_line_set_y2, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "x3",
                     annot_callout_line_get_x3, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_x3",
                     annot_callout_line_set_x3, 3);
    rb_define_method(RG_TARGET_NAMESPACE, "y3",
                     annot_callout_line_get_y3, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_y3",
                     annot_callout_line_set_y3, 3);

    RG_DEF_METHOD(to_a, 0);
    RG_DEF_METHOD(inspect, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
