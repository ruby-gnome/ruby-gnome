/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005-2006 Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Kouhei Sutou <kou@cozmixng.org>
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

#include "rsvg2.h"

#ifdef HAVE_TYPE_RSVGDIMENSIONDATA

#define RG_TARGET_NAMESPACE rg_cDimensionData

#define RVAL2DIM(obj) ((RsvgDimensionData *)DATA_PTR(obj))

VALUE RG_TARGET_NAMESPACE;

static ID id_closed;
static ID id_to_s;

static VALUE
to_s(VALUE obj)
{
    return rb_funcall(obj, id_to_s, 0);
}

static void
rb_rsvg_dim_free(RsvgDimensionData *dimp)
{
    if (dimp) {
	free(dimp);
    }
}

static VALUE
rb_rsvg_dim_alloc(VALUE klass)
{
    RsvgDimensionData *dimp;
    return Data_Make_Struct(klass, RsvgDimensionData, 0,
                            rb_rsvg_dim_free, dimp);
}

static VALUE
rb_rsvg_dim_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE width, height, em, ex;
    RsvgDimensionData *dimp;

    dimp = RVAL2DIM(self);
    rb_scan_args(argc, argv, "04", &width, &height, &em, &ex);

    if (!NIL_P(width))
        dimp->width = NUM2INT(width);
    if (!NIL_P(height))
        dimp->height = NUM2INT(height);
    if (!NIL_P(em))
        dimp->em = NUM2DBL(em);
    if (!NIL_P(ex))
        dimp->ex = NUM2DBL(ex);

    return Qnil;
}

static VALUE
rb_rsvg_dim_get_width(VALUE self)
{
    return INT2NUM(RVAL2DIM(self)->width);
}

static VALUE
rb_rsvg_dim_set_width(VALUE self, VALUE width)
{
    RVAL2DIM(self)->width = NUM2INT(width);
    return Qnil;
}

static VALUE
rb_rsvg_dim_get_height(VALUE self)
{
    return INT2NUM(RVAL2DIM(self)->height);
}

static VALUE
rb_rsvg_dim_set_height(VALUE self, VALUE height)
{
    RVAL2DIM(self)->height = NUM2INT(height);
    return Qnil;
}

static VALUE
rb_rsvg_dim_get_em(VALUE self)
{
    return rb_float_new(RVAL2DIM(self)->em);
}

static VALUE
rb_rsvg_dim_set_em(VALUE self, VALUE em)
{
    RVAL2DIM(self)->em = NUM2DBL(em);
    return Qnil;
}

static VALUE
rb_rsvg_dim_get_ex(VALUE self)
{
    return rb_float_new(RVAL2DIM(self)->ex);
}

static VALUE
rb_rsvg_dim_set_ex(VALUE self, VALUE ex)
{
    RVAL2DIM(self)->ex = NUM2DBL(ex);
    return Qnil;
}

static VALUE
rb_rsvg_dim_to_a(VALUE self)
{
    return rb_ary_new3(4,
                       rb_rsvg_dim_get_width(self),
                       rb_rsvg_dim_get_height(self),
                       rb_rsvg_dim_get_em(self),
                       rb_rsvg_dim_get_ex(self));
}

static VALUE
rb_rsvg_dim_to_s(VALUE self)
{
    VALUE ret;

    ret = rb_str_new2("#<");
    rb_str_cat2(ret, rb_obj_classname(self));
    rb_str_cat2(ret, ":");
    rb_str_concat(ret, rb_funcall(INT2NUM(self), id_to_s, 0));
    rb_str_cat2(ret, " ");

    rb_str_cat2(ret, "width=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_width(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "height=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_height(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "em=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_em(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "ex=");
    rb_str_concat(ret, to_s(rb_rsvg_dim_get_ex(self)));
    rb_str_cat2(ret, ">");

    return ret;
}
#endif

void
Init_rsvg_dimensiondata(VALUE mRSVG)
{
#ifdef HAVE_TYPE_RSVGDIMENSIONDATA
    id_closed = rb_intern("closed");
    id_to_s = rb_intern("to_s");

    RG_TARGET_NAMESPACE = rb_define_class_under(mRSVG, "DimensionData", rb_cObject);

    rb_define_alloc_func(RG_TARGET_NAMESPACE, rb_rsvg_dim_alloc);
    rb_define_method(RG_TARGET_NAMESPACE, "initialize", rb_rsvg_dim_initialize, -1);

    rb_define_method(RG_TARGET_NAMESPACE, "width", rb_rsvg_dim_get_width, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_width", rb_rsvg_dim_set_width, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "height", rb_rsvg_dim_get_height, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_height", rb_rsvg_dim_set_height, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "em", rb_rsvg_dim_get_em, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_em", rb_rsvg_dim_set_em, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "ex", rb_rsvg_dim_get_ex, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "set_ex", rb_rsvg_dim_set_ex, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "to_s", rb_rsvg_dim_to_s, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "to_a", rb_rsvg_dim_to_a, 0);
    rb_define_alias(RG_TARGET_NAMESPACE, "to_ary", "to_a");

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}

