/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#include "rbpangoprivate.h"

#if PANGO_CHECK_VERSION(1,6,0)

#define RG_TARGET_NAMESPACE cMatrix
#define _SELF(self) (RVAL2PANGOMATRIX(self))

#define ATTR_FLOAT(name)\
static VALUE \
matrix_get_ ## name (VALUE self)\
{\
    return rb_float_new(_SELF(self)->name);\
}\
static VALUE \
matrix_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = NUM2DBL(val);\
    return self;\
}

#define DEFINE_ACCESSOR(name) \
    rbg_define_method(RG_TARGET_NAMESPACE, G_STRINGIFY(name), matrix_get_ ## name, 0);\
    rbg_define_method(RG_TARGET_NAMESPACE, G_STRINGIFY(set_ ## name), matrix_set_## name, 1);

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    PangoMatrix matrix = PANGO_MATRIX_INIT;
    VALUE xx, xy, yx, yy, x0, y0;

    rb_scan_args(argc, argv, "06", &xx, &xy, &yx, &yy, &x0, &y0);

    if (argc > 0){
        matrix.xx = NUM2DBL(xx);
        matrix.xy = NUM2DBL(xy);
        matrix.yx = NUM2DBL(yx);
        matrix.yy = NUM2DBL(yy);
        matrix.x0 = NUM2DBL(x0);
        matrix.y0 = NUM2DBL(y0);
    }
    G_INITIALIZE(self, &matrix);
    return Qnil;
}

static VALUE
rg_translate_bang(VALUE self, VALUE tx, VALUE ty)
{
    pango_matrix_translate(_SELF(self), NUM2DBL(tx), NUM2DBL(ty));
    return self;
}

static VALUE
rg_scale_bang(VALUE self, VALUE scale_x, VALUE scale_y)
{
    pango_matrix_scale(_SELF(self), NUM2DBL(scale_x), NUM2DBL(scale_y));
    return self;
}

static VALUE
rg_rotate_bang(VALUE self, VALUE degrees)
{
    pango_matrix_rotate(_SELF(self), NUM2DBL(degrees));
    return self;
}

#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
rg_gravity(VALUE self)
{
    return PANGOGRAVITY2RVAL(pango_gravity_get_for_matrix(_SELF(self)));
}
#endif

static VALUE
rg_concat_bang(VALUE self, VALUE new_matrix)
{
    pango_matrix_concat(_SELF(self), _SELF(new_matrix));
    return self;
}

#if PANGO_CHECK_VERSION(1,12,0)
static VALUE
rg_font_scale_factor(VALUE self)
{
    return rb_float_new(pango_matrix_get_font_scale_factor(_SELF(self)));
}
#endif

ATTR_FLOAT(xx);
ATTR_FLOAT(xy);
ATTR_FLOAT(yx);
ATTR_FLOAT(yy);
ATTR_FLOAT(x0);
ATTR_FLOAT(y0);

static VALUE
rg_to_a(VALUE self)
{
    PangoMatrix* matrix = _SELF(self);
    return rb_ary_new3(6, INT2NUM(matrix->xx), INT2NUM(matrix->xy), INT2NUM(matrix->yx),
                       INT2NUM(matrix->yy), INT2NUM(matrix->x0), INT2NUM(matrix->y0));
}

#endif

void
Init_pango_matrix(VALUE mPango)
{
#if PANGO_CHECK_VERSION(1,6,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(PANGO_TYPE_MATRIX, "Matrix", mPango);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD_BANG(translate, 2);
    RG_DEF_METHOD_BANG(scale, 2);
    RG_DEF_METHOD_BANG(rotate, 1);
    RG_DEF_METHOD_BANG(concat, 1);
#if PANGO_CHECK_VERSION(1,12,0)
    RG_DEF_METHOD(font_scale_factor, 0);
#endif
#if PANGO_CHECK_VERSION(1,16,0)
    RG_DEF_METHOD(gravity, 0);
#endif
    RG_DEF_METHOD(to_a, 0);

    DEFINE_ACCESSOR(xx);
    DEFINE_ACCESSOR(xy);
    DEFINE_ACCESSOR(yx);
    DEFINE_ACCESSOR(yy);
    DEFINE_ACCESSOR(x0);
    DEFINE_ACCESSOR(y0);
#endif
}
