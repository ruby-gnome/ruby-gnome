/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkregion.c -

  $Author: mutoh $
  $Date: 2002/08/29 07:24:40 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

VALUE gdkRegion;

VALUE
make_gdkregion(region)
    GdkRegion *region;
{
    return Data_Wrap_Struct(gdkRegion, 0, gdk_region_destroy, region);
}

GdkRegion*
get_gdkregion(region)
    VALUE region;
{
    GdkRegion *gregion;

    if (NIL_P(region)) return NULL;

    if (!rb_obj_is_instance_of(region, gdkRegion)) {
        rb_raise(rb_eTypeError, "not a GdkRegion");
    }
    Data_Get_Struct(region, GdkRegion, gregion);
    if (gregion == 0) {
        rb_raise(rb_eArgError, "destroyed GdkRegion");
    }

    return gregion;
}

static VALUE
gdkregion_s_new(self)
    VALUE self;
{
    return make_gdkregion(gdk_region_new());
}

/*
static VALUE
gdk_region_s_polygon(points, fill_rule)
    VALUE points, fill_rule
{
}
 */

static VALUE
gdkregion_get_clipbox(self)
    VALUE self;
{
    GdkRectangle rect;
    gdk_region_get_clipbox(get_gdkregion(self), &rect);
    return BOXED2RVAL(&rect, GDK_TYPE_RECTANGLE);
}

static VALUE
gdkregion_empty_p(self)
    VALUE self;
{
    return gdk_region_empty(get_gdkregion(self)) ? Qtrue : Qfalse;
}

static VALUE
gdkregion_equal(self, obj)
    VALUE self, obj;
{
    if (!rb_obj_is_instance_of(obj, gdkRegion)) {
        return Qnil;
    }
    return gdk_region_equal(get_gdkregion(self),
                            get_gdkregion(obj)) ? Qtrue : Qfalse;
}

static VALUE
gdkregion_point_in(self, x, y)
    VALUE self, x, y;
{
    return gdk_region_point_in(get_gdkregion(self), NUM2INT(x),
                               NUM2INT(y)) ? Qtrue : Qfalse;
}

static VALUE
gdkregion_rect_in(self, rect)
    VALUE self, rect;
{
    return INT2FIX(gdk_region_rect_in(get_gdkregion(self),
                                      (GdkRectangle*)RVAL2BOXED(rect)));
}

static VALUE
gdkregion_offset(self, dx, dy)
    VALUE self, dx, dy;
{
    gdk_region_offset(get_gdkregion(self), NUM2INT(dx), NUM2INT(dy));
    return Qnil;
}

static VALUE
gdkregion_shrink(self, dx, dy)
    VALUE self, dx, dy;
{
    gdk_region_shrink(get_gdkregion(self), NUM2INT(dx), NUM2INT(dy));
    return Qnil;
}

static VALUE
gdkregion_union_with_rect(self, rect)
    VALUE self, rect;
{
    gdk_region_union_with_rect(get_gdkregion(self),
                               (GdkRectangle*)RVAL2BOXED(rect));
    return self;
}

static VALUE
gdkregion_intersect(self, region)
    VALUE self, region;
{
    gdk_region_intersect(get_gdkregion(self), get_gdkregion(region));
    return self;
}

static VALUE
gdkregion_union(self, region)
    VALUE self, region;
{
    gdk_region_union(get_gdkregion(self), get_gdkregion(region));
    return self;
}

static VALUE
gdkregion_subtract(self, region)
    VALUE self, region;
{
    gdk_region_subtract(get_gdkregion(self), get_gdkregion(region));
    return self;
}

static VALUE
gdkregion_xor(self, region)
    VALUE self, region;
{
    gdk_region_xor(get_gdkregion(self), get_gdkregion(region));
    return self;
}


void
Init_gtk_gdk_region()
{
    gdkRegion = rb_define_class_under(mGdk, "Region", rb_cData);

    rb_define_singleton_method(gdkRegion, "new", gdkregion_s_new, 0);
    rb_define_method(gdkRegion, "clipbox", gdkregion_get_clipbox, 0);
    rb_define_method(gdkRegion, "get_clipbox", gdkregion_get_clipbox, 0);
    rb_define_method(gdkRegion, "empty?", gdkregion_empty_p, 0);
    rb_define_method(gdkRegion, "==", gdkregion_equal, 1);
    rb_define_method(gdkRegion, "eql?", gdkregion_equal, 1);
    rb_define_method(gdkRegion, "point_in?", gdkregion_point_in, 2);
    rb_define_method(gdkRegion, "rect_in", gdkregion_rect_in, 1);
    rb_define_method(gdkRegion, "offset", gdkregion_offset, 2);
    rb_define_method(gdkRegion, "shrink", gdkregion_shrink, 2);
    rb_define_method(gdkRegion, "union_with_rect", gdkregion_union_with_rect, 1);
    rb_define_method(gdkRegion, "intersect", gdkregion_intersect, 1);
    rb_define_method(gdkRegion, "&", gdkregion_intersect, 1);
    rb_define_method(gdkRegion, "union", gdkregion_union, 1);
    rb_define_method(gdkRegion, "|", gdkregion_union, 1);
    rb_define_method(gdkRegion, "subtract", gdkregion_subtract, 1);
    rb_define_method(gdkRegion, "-", gdkregion_subtract, 1);
    rb_define_method(gdkRegion, "xor", gdkregion_xor, 1);

    /* constants */
    rb_define_const(mGdk, "OVERLAP_RECTANGLE_IN",
                    INT2NUM(GDK_OVERLAP_RECTANGLE_IN));
    rb_define_const(mGdk, "OVERLAP_RECTANGLE_OUT",
                    INT2NUM(GDK_OVERLAP_RECTANGLE_OUT));
    rb_define_const(mGdk, "OVERLAP_RECTANGLE_PART",
                    INT2NUM(GDK_OVERLAP_RECTANGLE_PART));
}
