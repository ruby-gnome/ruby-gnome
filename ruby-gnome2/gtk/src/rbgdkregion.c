/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkregion.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:35 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Gdk::Point
 */

VALUE gdkPoint;

static VALUE
gdkpoint_s_new(self, x, y)
    VALUE self, x, y;
{
    GdkPoint new;
    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    return make_gdkpoint(&new);
}

static VALUE
gdkpoint_x(self)
    VALUE self;
{
    return INT2NUM(get_gdkpoint(self)->x);
}

static VALUE
gdkpoint_y(self)
    VALUE self;
{
    return INT2NUM(get_gdkpoint(self)->y);
}

static VALUE
gdkpoint_set_x(self, x)
    VALUE self, x;
{
    get_gdkpoint(self)->x = NUM2INT(x);
    return self;
}

static VALUE
gdkpoint_set_y(self, y)
    VALUE self, y;
{
    get_gdkpoint(self)->y = NUM2INT(y);
    return self;
}


/*
 * Gdk::Rectangle
 */

VALUE gdkRectangle;

static VALUE
gdkrect_s_new(self, x, y, width, height)
    VALUE self, x, y, width, height;
{
    GdkRectangle new;
    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);
    return make_gdkrectangle(&new);
}

static VALUE
gdkrect_x(self)
    VALUE self;
{
    return INT2NUM(get_gdkrectangle(self)->x);
}

static VALUE
gdkrect_y(self)
    VALUE self;
{
    return INT2NUM(get_gdkrectangle(self)->y);
}

static VALUE
gdkrect_w(self)
    VALUE self;
{
    return INT2NUM(get_gdkrectangle(self)->width);
}

static VALUE
gdkrect_h(self)
    VALUE self;
{
    return INT2NUM(get_gdkrectangle(self)->height);
}

static VALUE
gdkrect_set_x(self, x)
    VALUE self, x;
{
    get_gdkrectangle(self)->x = NUM2INT(x);
    return self;
}

static VALUE
gdkrect_set_y(self, y)
    VALUE self, y;
{
    get_gdkrectangle(self)->y = NUM2INT(y);
    return self;
}

static VALUE
gdkrect_set_w(self, width)
    VALUE self, width;
{
    get_gdkrectangle(self)->width = NUM2INT(width);
    return self;
}

static VALUE
gdkrect_set_h(self, height)
    VALUE self, height;
{
    get_gdkrectangle(self)->height = NUM2INT(height);
    return self;
}


/*
 * Gdk::Region
 */

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
    return make_gdkrectangle(&rect);
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
                                      get_gdkrectangle(rect)));
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
    return make_gdkregion(gdk_region_union_with_rect(get_gdkregion(self),
                                                     get_gdkrectangle(rect)));
}

static VALUE
gdkregion_intersect(self, region)
    VALUE self, region;
{
    return make_gdkregion(gdk_regions_intersect(get_gdkregion(self),
                                                get_gdkregion(region)));
}

static VALUE
gdkregion_union(self, region)
    VALUE self, region;
{
    return make_gdkregion(gdk_regions_union(get_gdkregion(self),
                                            get_gdkregion(region)));
}

static VALUE
gdkregion_subtract(self, region)
    VALUE self, region;
{
    return make_gdkregion(gdk_regions_subtract(get_gdkregion(self),
                                               get_gdkregion(region)));
}

static VALUE
gdkregion_xor(self, region)
    VALUE self, region;
{
    return make_gdkregion(gdk_regions_xor(get_gdkregion(self),
                                          get_gdkregion(region)));
}


void
Init_gtk_gdk_region()
{
    /* Gdk::Point */
    gdkPoint = rb_define_class_under(mGdk, "Point", rb_cData);

    rb_define_singleton_method(gdkPoint, "new", gdkpoint_s_new, 2);
    rb_define_method(gdkPoint, "x", gdkpoint_x, 0);
    rb_define_method(gdkPoint, "y", gdkpoint_y, 0);
    rb_define_method(gdkPoint, "x=", gdkpoint_set_x, 1);
    rb_define_method(gdkPoint, "y=", gdkpoint_set_y, 1);

    /* Gdk::Rectangle */
    gdkRectangle = rb_define_class_under(mGdk, "Rectangle", rb_cData);

    rb_define_singleton_method(gdkRectangle, "new", gdkrect_s_new, 4);
    rb_define_method(gdkRectangle, "x", gdkrect_x, 0);
    rb_define_method(gdkRectangle, "y", gdkrect_y, 0);
    rb_define_method(gdkRectangle, "width", gdkrect_w, 0);
    rb_define_method(gdkRectangle, "height", gdkrect_h, 0);
    rb_define_method(gdkRectangle, "x=", gdkrect_set_x, 1);
    rb_define_method(gdkRectangle, "y=", gdkrect_set_y, 1);
    rb_define_method(gdkRectangle, "width=", gdkrect_set_w, 1);
    rb_define_method(gdkRectangle, "height=", gdkrect_set_h, 1);

    /* Gdk::Region */
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
