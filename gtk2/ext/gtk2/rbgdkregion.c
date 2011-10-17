/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkregion.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002,2003 Masao Mutoh
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

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
gdkregion_initialize(int argc, VALUE *argv, VALUE self)
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
gdkregion_get_rectangles(VALUE self)
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

static VALUE
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
gdkregion_spans_intersect_foreach(VALUE self, VALUE rbspans, VALUE rbsorted)
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
gdkregion_get_clipbox(VALUE self)
{
    GdkRectangle rect;
    gdk_region_get_clipbox(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
gdkregion_empty(VALUE self)
{
    return CBOOL2RVAL(gdk_region_empty(_SELF(self)));
}

static VALUE
gdkregion_equal(VALUE self, VALUE obj)
{
    if (!rb_obj_is_instance_of(obj, GTYPE2CLASS(GDK_TYPE_REGION))) {
        return Qnil;
    }
    return CBOOL2RVAL(gdk_region_equal(_SELF(self), _SELF(obj)));
}

static VALUE
gdkregion_point_in(VALUE self, VALUE x, VALUE y)
{
    return CBOOL2RVAL(gdk_region_point_in(_SELF(self), NUM2INT(x),
                                          NUM2INT(y)));
}

static VALUE
gdkregion_rect_in(VALUE self, VALUE rect)
{
    return GENUM2RVAL(gdk_region_rect_in(
                          _SELF(self),
                          (GdkRectangle*)RVAL2BOXED(rect, GDK_TYPE_RECTANGLE)),
                      GDK_TYPE_OVERLAP_TYPE);
}

static VALUE
gdkregion_offset(VALUE self, VALUE dx, VALUE dy)
{
    gdk_region_offset(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
gdkregion_shrink(VALUE self, VALUE dx, VALUE dy)
{
    gdk_region_shrink(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
gdkregion_intersect(VALUE self, VALUE region)
{
    gdk_region_intersect(_SELF(self), _SELF(region));
    return self;
}

static VALUE
gdkregion_union(VALUE self, VALUE other)
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
gdkregion_subtract(VALUE self, VALUE region)
{
    gdk_region_subtract(_SELF(self), _SELF(region));
    return self;
}

static VALUE
gdkregion_xor(VALUE self, VALUE region)
{
    gdk_region_xor(_SELF(self), _SELF(region));
    return self;
}


void
Init_gtk_gdk_region(void)
{
    VALUE gdkRegion = G_DEF_CLASS(GDK_TYPE_REGION, "Region", mGdk);

    rb_define_method(gdkRegion, "initialize", gdkregion_initialize, -1);
    rb_define_method(gdkRegion, "rectangles", gdkregion_get_rectangles, 0);
    rb_define_method(gdkRegion, "spans_intersect_each", gdkregion_spans_intersect_foreach, 2);
    rb_define_method(gdkRegion, "clipbox", gdkregion_get_clipbox, 0);
    rb_define_method(gdkRegion, "empty?", gdkregion_empty, 0);
    rb_define_method(gdkRegion, "==", gdkregion_equal, 1);
    rb_define_method(gdkRegion, "point_in?", gdkregion_point_in, 2);
    rb_define_method(gdkRegion, "rect_in", gdkregion_rect_in, 1);
    rb_define_method(gdkRegion, "offset", gdkregion_offset, 2);
    rb_define_method(gdkRegion, "shrink", gdkregion_shrink, 2);
    rb_define_method(gdkRegion, "intersect", gdkregion_intersect, 1);
    rb_define_method(gdkRegion, "union", gdkregion_union, 1);
    rb_define_method(gdkRegion, "subtract", gdkregion_subtract, 1);
    rb_define_method(gdkRegion, "xor", gdkregion_xor, 1);

    /* GdkOverlapType */
    G_DEF_CLASS(GDK_TYPE_OVERLAP_TYPE, "OverlapType", gdkRegion);
    G_DEF_CONSTANTS(gdkRegion, GDK_TYPE_OVERLAP_TYPE, "GDK_");

    /* GdkFillRule */
    G_DEF_CLASS(GDK_TYPE_FILL_RULE, "FillRule", gdkRegion);
    G_DEF_CONSTANTS(gdkRegion, GDK_TYPE_FILL_RULE, "GDK_");
}
