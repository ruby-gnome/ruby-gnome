/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#include "global.h"

#define RG_TARGET_NAMESPACE cRegion
#define _SELF(r) ((GdkRegion*)RVAL2BOXED(r, GDK_TYPE_REGION))

/**********************************/
GType
gdk_region_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GdkRegion",
                    (GBoxedCopyFunc)gdk_region_copy,
                    (GBoxedFreeFunc)gdk_region_destroy);
    return our_type;
}
/**********************************/
static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE points_or_rectangle, rbfill_rule;
    GdkRegion *region;

    rb_scan_args(argc, argv, "02", &points_or_rectangle, &rbfill_rule);
    if (NIL_P(points_or_rectangle)) {
        region = gdk_region_new();
    } else if (TYPE(points_or_rectangle) == T_ARRAY) {
        GdkFillRule fill_rule = RVAL2GENUM(rbfill_rule, GDK_TYPE_FILL_RULE);
        long n;
        GdkPoint *points = RVAL2GDKPOINTS(points_or_rectangle, &n);

        region = gdk_region_polygon(points, n, fill_rule);

        g_free(points);
    } else if (RVAL2GTYPE(points_or_rectangle) == GDK_TYPE_RECTANGLE) {
        region = gdk_region_rectangle((GdkRectangle*)RVAL2BOXED(points_or_rectangle, 
                                                                GDK_TYPE_RECTANGLE));
    } else {
        rb_raise(rb_eArgError, 
                 "invalid argument %s (expect array of Gdk::Point or Gdk::Rectangle, nil)",
                 rb_class2name(CLASS_OF(points_or_rectangle)));
    }

    G_INITIALIZE(self, region);

    return Qnil;
}

static VALUE
rg_rectangles(VALUE self)
{
    GdkRectangle* rectangles;
    gint n_rect, i;
    VALUE ary;

    gdk_region_get_rectangles(_SELF(self), &rectangles, &n_rect);
    ary = rb_ary_new2(n_rect);
    for (i = 0; i < n_rect; i++) {
        rb_ary_push(ary, BOXED2RVAL(&rectangles[i], GDK_TYPE_RECTANGLE));
    }
    g_free(rectangles);
    return ary;
}

static void
gdkregion_span_func(GdkSpan *span, gpointer func)
{
    rb_funcall((VALUE)func, id_call, 3, 
               INT2NUM(span->x), INT2NUM(span->y), INT2NUM(span->width));
}

struct rbgdk_rval2gdkspans_args {
    VALUE ary;
    long n;
    GdkSpan *result;
};

static VALUE
rbgdk_rval2gdkspans_body(VALUE value)
{
    long i;
    struct rbgdk_rval2gdkspans_args *args = (struct rbgdk_rval2gdkspans_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE points = rb_ary_to_ary(RARRAY_PTR(args->ary)[i]);

        if (RARRAY_LEN(points) != 2)
            rb_raise(rb_eArgError, "point %ld should be array of size 3", i);

        args->result[i].x = NUM2INT(RARRAY_PTR(points)[0]);
        args->result[i].y = NUM2INT(RARRAY_PTR(points)[1]);
        args->result[i].width = NUM2INT(RARRAY_PTR(points)[1]);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgdk_rval2gdkspans_rescue(VALUE value)
{
    g_free(((struct rbgdk_rval2gdkspans_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GdkSpan *
rbgdk_rval2gdkspans(VALUE value, long *n)
{
    struct rbgdk_rval2gdkspans_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GdkSpan, args.n + 1);

    rb_rescue(rbgdk_rval2gdkspans_body, (VALUE)&args,
              rbgdk_rval2gdkspans_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

#define RVAL2GDKSPANS(value, n) rbgdk_rval2gdkspans(value, n)

static VALUE
rg_spans_intersect_each(VALUE self, VALUE rbspans, VALUE rbsorted)
{
    GdkRegion *region = _SELF(self);
    gboolean sorted = RVAL2CBOOL(rbsorted);
    VALUE func = rb_block_proc();
    long n;
    GdkSpan *spans;

    G_RELATIVE(self, func);

    spans = RVAL2GDKSPANS(rbspans, &n);

    gdk_region_spans_intersect_foreach(region,
                                       spans,
                                       n,
                                       sorted,
                                       (GdkSpanFunc)gdkregion_span_func,
                                       (gpointer)func);

    g_free(spans);

    return self;
}

static VALUE
rg_clipbox(VALUE self)
{
    GdkRectangle rect;
    gdk_region_get_clipbox(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
rg_empty_p(VALUE self)
{
    return CBOOL2RVAL(gdk_region_empty(_SELF(self)));
}

static VALUE
rg_operator_equal(VALUE self, VALUE obj)
{
    if (!rb_obj_is_instance_of(obj, GTYPE2CLASS(GDK_TYPE_REGION))) {
        return Qnil;
    }
    return CBOOL2RVAL(gdk_region_equal(_SELF(self), _SELF(obj)));
}

static VALUE
rg_point_in_p(VALUE self, VALUE x, VALUE y)
{
    return CBOOL2RVAL(gdk_region_point_in(_SELF(self), NUM2INT(x),
                                          NUM2INT(y)));
}

static VALUE
rg_rect_in(VALUE self, VALUE rect)
{
    return GENUM2RVAL(gdk_region_rect_in(
                          _SELF(self),
                          (GdkRectangle*)RVAL2BOXED(rect, GDK_TYPE_RECTANGLE)),
                      GDK_TYPE_OVERLAP_TYPE);
}

static VALUE
rg_offset(VALUE self, VALUE dx, VALUE dy)
{
    gdk_region_offset(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
rg_shrink(VALUE self, VALUE dx, VALUE dy)
{
    gdk_region_shrink(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
rg_intersect(VALUE self, VALUE region)
{
    gdk_region_intersect(_SELF(self), _SELF(region));
    return self;
}

static VALUE
rg_union(VALUE self, VALUE other)
{
    if (RVAL2GTYPE(other) == GDK_TYPE_RECTANGLE){
        gdk_region_union_with_rect(_SELF(self),
                                   (GdkRectangle*)RVAL2BOXED(other, GDK_TYPE_RECTANGLE));
    } else {
        gdk_region_union(_SELF(self), _SELF(other));
    }
    return self;
}

static VALUE
rg_subtract(VALUE self, VALUE region)
{
    gdk_region_subtract(_SELF(self), _SELF(region));
    return self;
}

static VALUE
rg_xor(VALUE self, VALUE region)
{
    gdk_region_xor(_SELF(self), _SELF(region));
    return self;
}

void
Init_gtk_gdk_region(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_REGION, "Region", mGdk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(rectangles, 0);
    RG_DEF_METHOD(spans_intersect_each, 2);
    RG_DEF_METHOD(clipbox, 0);
    RG_DEF_METHOD_P(empty, 0);
    RG_DEF_METHOD_OPERATOR("==", equal, 1);
    RG_DEF_METHOD_P(point_in, 2);
    RG_DEF_METHOD(rect_in, 1);
    RG_DEF_METHOD(offset, 2);
    RG_DEF_METHOD(shrink, 2);
    RG_DEF_METHOD(intersect, 1);
    RG_DEF_METHOD(union, 1);
    RG_DEF_METHOD(subtract, 1);
    RG_DEF_METHOD(xor, 1);

    /* GdkOverlapType */
    G_DEF_CLASS(GDK_TYPE_OVERLAP_TYPE, "OverlapType", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_OVERLAP_TYPE, "GDK_");

    /* GdkFillRule */
    G_DEF_CLASS(GDK_TYPE_FILL_RULE, "FillRule", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_FILL_RULE, "GDK_");
}
