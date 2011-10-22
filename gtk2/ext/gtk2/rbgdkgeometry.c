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
geo_initialize(VALUE self)
{
    GdkGeometry g;
    G_INITIALIZE(self, &g);
    return Qnil;
}

static VALUE
geo_set(VALUE self, VALUE min_width, VALUE min_height, VALUE max_width, VALUE max_height, VALUE base_width, VALUE base_height, VALUE width_inc, VALUE height_inc, VALUE min_aspect, VALUE max_aspect, VALUE gravity)
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
geo_min_width(VALUE self)
{
    return INT2NUM(_SELF(self)->min_width);
}

static VALUE
geo_min_height(VALUE self)
{
    return INT2NUM(_SELF(self)->min_height);
}

static VALUE
geo_max_width(VALUE self)
{
    return INT2NUM(_SELF(self)->max_width);
}

static VALUE
geo_max_height(VALUE self)
{
    return INT2NUM(_SELF(self)->max_height);
}

static VALUE
geo_base_width(VALUE self)
{
    return INT2NUM(_SELF(self)->base_width);
}

static VALUE
geo_base_height(VALUE self)
{
    return INT2NUM(_SELF(self)->base_height);
}

static VALUE
geo_width_inc(VALUE self)
{
    return INT2NUM(_SELF(self)->width_inc);
}

static VALUE
geo_height_inc(VALUE self)
{
    return INT2NUM(_SELF(self)->height_inc);
}

static VALUE
geo_min_aspect(VALUE self)
{
    return rb_float_new(_SELF(self)->min_aspect);
}

static VALUE
geo_max_aspect(VALUE self)
{
    return rb_float_new(_SELF(self)->max_aspect);
}

static VALUE
geo_win_gravity(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->win_gravity, GDK_TYPE_GRAVITY);
}

static VALUE
geo_set_min_width(VALUE self, VALUE min_width)
{
    _SELF(self)->min_width = NUM2INT(min_width);
    return self;
}

static VALUE
geo_set_min_height(VALUE self, VALUE min_height)
{
    _SELF(self)->min_height = NUM2INT(min_height);
    return self;
}

static VALUE
geo_set_max_width(VALUE self, VALUE max_width)
{
    _SELF(self)->max_width = NUM2INT(max_width);
    return self;
}

static VALUE
geo_set_max_height(VALUE self, VALUE max_height)
{
    _SELF(self)->max_height = NUM2INT(max_height);
    return self;
}

static VALUE
geo_set_base_width(VALUE self, VALUE base_width)
{
    _SELF(self)->base_width = NUM2INT(base_width);
    return self;
}

static VALUE
geo_set_base_height(VALUE self, VALUE base_height)
{
    _SELF(self)->base_height = NUM2INT(base_height);
    return self;
}

static VALUE
geo_set_width_inc(VALUE self, VALUE width_inc)
{
    _SELF(self)->width_inc = NUM2INT(width_inc);
    return self;
}

static VALUE
geo_set_height_inc(VALUE self, VALUE height_inc)
{
    _SELF(self)->height_inc = NUM2INT(height_inc);
    return self;
}

static VALUE
geo_set_min_aspect(VALUE self, VALUE min_aspect)
{
    _SELF(self)->min_aspect = NUM2DBL(min_aspect);
    return self;
}

static VALUE
geo_set_max_aspect(VALUE self, VALUE max_aspect)
{
    _SELF(self)->max_aspect = NUM2DBL(max_aspect);
    return self;
}

static VALUE
geo_set_win_gravity(VALUE self, VALUE gravity)
{
    _SELF(self)->win_gravity = RVAL2GENUM(gravity, GDK_TYPE_GRAVITY);
    return self;
}

void 
Init_gtk_gdk_geometry(void)
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
