/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkgeometry.c -

  $Author: mutoh $
  $Date: 2003/10/17 16:30:51 $

  Copyright (C) 2002,2003 Masao Mutoh

  This file is derived from rbgdkregion.c.
  rbgdkregion.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/
#include "global.h"

/*****************************************/
static GdkGeometry*
geo_copy(const GdkGeometry* geo)
{
  GdkGeometry* new_geo;
  g_return_val_if_fail (geo != NULL, NULL);
  new_geo = g_new(GdkGeometry, 1);
  *new_geo = *geo;
  return new_geo;
}

GType
gdk_geometry_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GdkGeometry",
                    (GBoxedCopyFunc)geo_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}
/*****************************************/

#define _SELF(g) ((GdkGeometry*)RVAL2BOXED(g, GDK_TYPE_GEOMETRY))

static VALUE
geo_initialize(self)
    VALUE self;
{
    GdkGeometry g;
    G_INITIALIZE(self, &g);
    return Qnil;
}

static VALUE
geo_set(self, min_width, min_height, max_width,	max_height,
		base_width, base_height, width_inc, height_inc,	min_aspect, max_aspect, gravity)
     VALUE self, min_width, min_height, max_width, max_height,
     base_width, base_height, width_inc, height_inc, min_aspect, max_aspect, gravity;
{
    GdkGeometry *geo = _SELF(self);
    geo->min_width = NUM2INT(min_width);
    geo->min_height = NUM2INT(min_height);
    geo->max_width = NUM2INT(max_width);
    geo->max_height = NUM2INT(max_height);
    geo->base_width = NUM2INT(base_width);
    geo->base_height = NUM2INT(base_height);
    geo->width_inc = NUM2INT(width_inc);
    geo->height_inc = NUM2INT(height_inc);
    geo->min_aspect = NUM2DBL(min_aspect);
    geo->max_aspect = NUM2DBL(max_aspect);
    geo->win_gravity = RVAL2GENUM(gravity, GDK_TYPE_GRAVITY);

    return self;
}

static VALUE
geo_min_width(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->min_width);
}

static VALUE
geo_min_height(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->min_height);
}

static VALUE
geo_max_width(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->max_width);
}

static VALUE
geo_max_height(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->max_height);
}

static VALUE
geo_base_width(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->base_width);
}

static VALUE
geo_base_height(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->base_height);
}

static VALUE
geo_width_inc(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width_inc);
}

static VALUE
geo_height_inc(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->height_inc);
}

static VALUE
geo_min_aspect(self)
    VALUE self;
{
    return rb_float_new(_SELF(self)->min_aspect);
}

static VALUE
geo_max_aspect(self)
    VALUE self;
{
    return rb_float_new(_SELF(self)->max_aspect);
}

static VALUE
geo_win_gravity(self)
    VALUE self;
{
    return GENUM2RVAL(_SELF(self)->win_gravity, GDK_TYPE_GRAVITY);
}

static VALUE
geo_set_min_width(self, min_width)
    VALUE self, min_width;
{
    _SELF(self)->min_width = NUM2INT(min_width);
    return self;
}

static VALUE
geo_set_min_height(self, min_height)
    VALUE self, min_height;
{
    _SELF(self)->min_height = NUM2INT(min_height);
    return self;
}

static VALUE
geo_set_max_width(self, max_width)
    VALUE self, max_width;
{
    _SELF(self)->max_width = NUM2INT(max_width);
    return self;
}

static VALUE
geo_set_max_height(self, max_height)
    VALUE self, max_height;
{
    _SELF(self)->max_height = NUM2INT(max_height);
    return self;
}

static VALUE
geo_set_base_width(self, base_width)
    VALUE self, base_width;
{
    _SELF(self)->base_width = NUM2INT(base_width);
    return self;
}

static VALUE
geo_set_base_height(self, base_height)
    VALUE self, base_height;
{
    _SELF(self)->base_height = NUM2INT(base_height);
    return self;
}

static VALUE
geo_set_width_inc(self, width_inc)
    VALUE self, width_inc;
{
    _SELF(self)->width_inc = NUM2INT(width_inc);
    return self;
}

static VALUE
geo_set_height_inc(self, height_inc)
    VALUE self, height_inc;
{
    _SELF(self)->height_inc = NUM2INT(height_inc);
    return self;
}

static VALUE
geo_set_min_aspect(self, min_aspect)
    VALUE self, min_aspect;
{
    _SELF(self)->min_aspect = NUM2DBL(min_aspect);
    return self;
}

static VALUE
geo_set_max_aspect(self, max_aspect)
    VALUE self, max_aspect;
{
    _SELF(self)->max_aspect = NUM2DBL(max_aspect);
    return self;
}

static VALUE
geo_set_win_gravity(self, gravity)
    VALUE self, gravity;
{
    _SELF(self)->win_gravity = RVAL2GENUM(gravity, GDK_TYPE_GRAVITY);
    return self;
}

void 
Init_gtk_gdk_geometry()
{
    VALUE gdkGeometry = G_DEF_CLASS(GDK_TYPE_GEOMETRY, "Geometry", mGdk);

    rb_define_method(gdkGeometry, "initialize", geo_initialize, 0);
    rb_define_method(gdkGeometry, "min_width", geo_min_width, 0);
    rb_define_method(gdkGeometry, "min_height", geo_min_height, 0);
    rb_define_method(gdkGeometry, "max_width", geo_max_width, 0);
    rb_define_method(gdkGeometry, "max_height", geo_max_height, 0);
    rb_define_method(gdkGeometry, "base_width", geo_base_width, 0);
    rb_define_method(gdkGeometry, "base_height", geo_base_height, 0);
    rb_define_method(gdkGeometry, "width_inc", geo_width_inc, 0);
    rb_define_method(gdkGeometry, "height_inc", geo_height_inc, 0);
    rb_define_method(gdkGeometry, "min_aspect", geo_min_aspect, 0);
    rb_define_method(gdkGeometry, "max_aspect", geo_max_aspect, 0);
    rb_define_method(gdkGeometry, "win_gravity", geo_win_gravity, 0);
    rb_define_method(gdkGeometry, "set", geo_set, 11);
    rb_define_method(gdkGeometry, "set_min_width", geo_set_min_width, 1);
    rb_define_method(gdkGeometry, "set_min_height", geo_set_min_height, 1);
    rb_define_method(gdkGeometry, "set_max_width", geo_set_max_width, 1);
    rb_define_method(gdkGeometry, "set_max_height", geo_set_max_height, 1);
    rb_define_method(gdkGeometry, "set_base_width", geo_set_base_width, 1);
    rb_define_method(gdkGeometry, "set_base_height", geo_set_base_height, 1);
    rb_define_method(gdkGeometry, "set_width_inc", geo_set_width_inc, 1);
    rb_define_method(gdkGeometry, "set_height_inc", geo_set_height_inc, 1);
    rb_define_method(gdkGeometry, "set_min_aspect", geo_set_min_aspect, 1);
    rb_define_method(gdkGeometry, "set_max_aspect", geo_set_max_aspect, 1);
    rb_define_method(gdkGeometry, "set_win_gravity", geo_set_win_gravity, 1);

    G_DEF_SETTERS(gdkGeometry);

}
