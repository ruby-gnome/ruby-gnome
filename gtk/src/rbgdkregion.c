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
gdkregion_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE points_or_rectangle, fill_rule;
    GdkRegion* region;
    GdkPoint *gpoints;
    int i;

    rb_scan_args(argc, argv, "02", &points_or_rectangle, &fill_rule);
    if (NIL_P(points_or_rectangle)){
        region = gdk_region_new();
    } else if (TYPE(points_or_rectangle) == T_ARRAY){
        gpoints = ALLOCA_N(GdkPoint, RARRAY(points_or_rectangle)->len);

        for (i = 0; i < RARRAY(points_or_rectangle)->len; i++) {
            Check_Type(RARRAY(points_or_rectangle)->ptr[i], T_ARRAY);
            if (RARRAY(RARRAY(points_or_rectangle)->ptr[i])->len < 2) {
                rb_raise(rb_eArgError, "point %d should be array of size 2", i);
            }
            gpoints[i].x = NUM2INT(RARRAY(RARRAY(points_or_rectangle)->ptr[i])->ptr[0]);
            gpoints[i].y = NUM2INT(RARRAY(RARRAY(points_or_rectangle)->ptr[i])->ptr[1]);
        }
        region = gdk_region_polygon(gpoints, RARRAY(points_or_rectangle)->len, 
                                    RVAL2GENUM(fill_rule, GDK_TYPE_FILL_RULE));
    } else if (RVAL2GTYPE(points_or_rectangle) == GDK_TYPE_RECTANGLE){
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
gdkregion_get_rectangles(self)
    VALUE self;
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
gdkregion_span_func(span, func)
    GdkSpan* span;
    gpointer func;
{
    rb_funcall((VALUE)func, id_call, 3, 
               INT2NUM(span->x), INT2NUM(span->y), INT2NUM(span->width));
}

static VALUE
gdkregion_spans_intersect_foreach(self, spans, sorted)
    VALUE self, spans, sorted;
{
    int i;
    GdkSpan* gspans = ALLOCA_N(GdkSpan, RARRAY(spans)->len);
    volatile VALUE func = rb_block_proc();

    G_RELATIVE(self, func);
    for (i = 0; i < RARRAY(spans)->len; i++) {
        gspans[i].x = NUM2INT(RARRAY(RARRAY(spans)->ptr[i])->ptr[0]);
        gspans[i].y = NUM2INT(RARRAY(RARRAY(spans)->ptr[i])->ptr[1]);
        gspans[i].width = NUM2INT(RARRAY(RARRAY(spans)->ptr[i])->ptr[2]);
    }
    gdk_region_spans_intersect_foreach(_SELF(self), 
                                       gspans, RARRAY(spans)->len, RVAL2CBOOL(sorted), 
                                       (GdkSpanFunc)gdkregion_span_func, (gpointer)func);
    return self;
}

static VALUE
gdkregion_get_clipbox(self)
    VALUE self;
{
    GdkRectangle rect;
    gdk_region_get_clipbox(_SELF(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
gdkregion_empty(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_region_empty(_SELF(self)));
}

static VALUE
gdkregion_equal(self, obj)
    VALUE self, obj;
{
    if (!rb_obj_is_instance_of(obj, GTYPE2CLASS(GDK_TYPE_REGION))) {
        return Qnil;
    }
    return CBOOL2RVAL(gdk_region_equal(_SELF(self), _SELF(obj)));
}

static VALUE
gdkregion_point_in(self, x, y)
    VALUE self, x, y;
{
    return CBOOL2RVAL(gdk_region_point_in(_SELF(self), NUM2INT(x),
                                          NUM2INT(y)));
}

static VALUE
gdkregion_rect_in(self, rect)
    VALUE self, rect;
{
    return GENUM2RVAL(gdk_region_rect_in(
                          _SELF(self),
                          (GdkRectangle*)RVAL2BOXED(rect, GDK_TYPE_RECTANGLE)),
                      GDK_TYPE_OVERLAP_TYPE);
}

static VALUE
gdkregion_offset(self, dx, dy)
    VALUE self, dx, dy;
{
    gdk_region_offset(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
gdkregion_shrink(self, dx, dy)
    VALUE self, dx, dy;
{
    gdk_region_shrink(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return self;
}

static VALUE
gdkregion_intersect(self, region)
    VALUE self, region;
{
    gdk_region_intersect(_SELF(self), _SELF(region));
    return self;
}

static VALUE
gdkregion_union(self, other)
    VALUE self, other;
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
gdkregion_subtract(self, region)
    VALUE self, region;
{
    gdk_region_subtract(_SELF(self), _SELF(region));
    return self;
}

static VALUE
gdkregion_xor(self, region)
    VALUE self, region;
{
    gdk_region_xor(_SELF(self), _SELF(region));
    return self;
}


void
Init_gtk_gdk_region()
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
