/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkregion.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

#define _SELF(r) ((GdkRegion*)RVAL2BOXED(r))

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
gdkregion_initialize(self)
    VALUE self;
{
    GdkRegion *region = gdk_region_new();
    G_INITIALIZE(self, region);
    return Qnil;
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
gdkregion_empty_p(self)
    VALUE self;
{
    return gdk_region_empty(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
gdkregion_equal(self, obj)
    VALUE self, obj;
{
    if (!rb_obj_is_instance_of(obj, GTYPE2CLASS(GDK_TYPE_REGION))) {
        return Qnil;
    }
    return gdk_region_equal(_SELF(self),
                            _SELF(obj)) ? Qtrue : Qfalse;
}

static VALUE
gdkregion_point_in(self, x, y)
    VALUE self, x, y;
{
    return gdk_region_point_in(_SELF(self), NUM2INT(x),
                               NUM2INT(y)) ? Qtrue : Qfalse;
}

static VALUE
gdkregion_rect_in(self, rect)
    VALUE self, rect;
{
    return INT2FIX(gdk_region_rect_in(_SELF(self),
                                      (GdkRectangle*)RVAL2BOXED(rect)));
}

static VALUE
gdkregion_offset(self, dx, dy)
    VALUE self, dx, dy;
{
    gdk_region_offset(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return Qnil;
}

static VALUE
gdkregion_shrink(self, dx, dy)
    VALUE self, dx, dy;
{
    gdk_region_shrink(_SELF(self), NUM2INT(dx), NUM2INT(dy));
    return Qnil;
}

static VALUE
gdkregion_union_with_rect(self, rect)
    VALUE self, rect;
{
    gdk_region_union_with_rect(_SELF(self),
                               (GdkRectangle*)RVAL2BOXED(rect));
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
gdkregion_union(self, region)
    VALUE self, region;
{
    gdk_region_union(_SELF(self), _SELF(region));
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

    rb_define_method(gdkRegion, "initialize", gdkregion_initialize, 0);
    rb_define_method(gdkRegion, "clipbox", gdkregion_get_clipbox, 0);
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
