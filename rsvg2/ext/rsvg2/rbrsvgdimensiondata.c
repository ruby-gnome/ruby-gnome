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

#define RG_TARGET_NAMESPACE cDimensionData

#define RVAL2DIM(obj) ((RsvgDimensionData *)DATA_PTR(obj))

static VALUE RG_TARGET_NAMESPACE;

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
rg_initialize(int argc, VALUE *argv, VALUE self)
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
rg_width(VALUE self)
{
    return INT2NUM(RVAL2DIM(self)->width);
}

static VALUE
rg_set_width(VALUE self, VALUE width)
{
    RVAL2DIM(self)->width = NUM2INT(width);
    return Qnil;
}

static VALUE
rg_height(VALUE self)
{
    return INT2NUM(RVAL2DIM(self)->height);
}

static VALUE
rg_set_height(VALUE self, VALUE height)
{
    RVAL2DIM(self)->height = NUM2INT(height);
    return Qnil;
}

static VALUE
rg_em(VALUE self)
{
    return rb_float_new(RVAL2DIM(self)->em);
}

static VALUE
rg_set_em(VALUE self, VALUE em)
{
    RVAL2DIM(self)->em = NUM2DBL(em);
    return Qnil;
}

static VALUE
rg_ex(VALUE self)
{
    return rb_float_new(RVAL2DIM(self)->ex);
}

static VALUE
rg_set_ex(VALUE self, VALUE ex)
{
    RVAL2DIM(self)->ex = NUM2DBL(ex);
    return Qnil;
}

static VALUE
rg_to_a(VALUE self)
{
    return rb_ary_new3(4,
                       rg_width(self),
                       rg_height(self),
                       rg_em(self),
                       rg_ex(self));
}

static VALUE
rg_to_s(VALUE self)
{
    VALUE ret;

    ret = rb_str_new2("#<");
    rb_str_cat2(ret, rb_obj_classname(self));
    rb_str_cat2(ret, ":");
    rb_str_concat(ret, rb_funcall(INT2NUM(self), id_to_s, 0));
    rb_str_cat2(ret, " ");

    rb_str_cat2(ret, "width=");
    rb_str_concat(ret, to_s(rg_width(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "height=");
    rb_str_concat(ret, to_s(rg_height(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "em=");
    rb_str_concat(ret, to_s(rg_em(self)));
    rb_str_cat2(ret, ", ");
    rb_str_cat2(ret, "ex=");
    rb_str_concat(ret, to_s(rg_ex(self)));
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
    RG_DEF_METHOD(initialize, -1);

    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(set_width, 1);
    RG_DEF_METHOD(height, 0);
    RG_DEF_METHOD(set_height, 1);
    RG_DEF_METHOD(em, 0);
    RG_DEF_METHOD(set_em, 1);
    RG_DEF_METHOD(ex, 0);
    RG_DEF_METHOD(set_ex, 1);

    RG_DEF_METHOD(to_s, 0);
    RG_DEF_METHOD(to_a, 0);
    RG_DEF_ALIAS("to_ary", "to_a");
#endif
}
