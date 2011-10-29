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
#define _SELF(self) ((PangoMatrix*)(RVAL2BOXED(self, PANGO_TYPE_MATRIX)))

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
    rb_define_method(matrix, G_STRINGIFY(name), matrix_get_ ## name, 0);\
    rb_define_method(matrix, G_STRINGIFY(set_ ## name), matrix_set_## name, 1);

static VALUE
matrix_initialize(int argc, VALUE *argv, VALUE self)
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
matrix_translate(VALUE self, VALUE tx, VALUE ty)
{
    pango_matrix_translate(_SELF(self), NUM2DBL(tx), NUM2DBL(ty));
    return self;
}

static VALUE
matrix_scale(VALUE self, VALUE scale_x, VALUE scale_y)
{
    pango_matrix_scale(_SELF(self), NUM2DBL(scale_x), NUM2DBL(scale_y));
    return self;
}

static VALUE
matrix_rotate(VALUE self, VALUE degrees)
{
    pango_matrix_rotate(_SELF(self), NUM2DBL(degrees));
    return self;
}

#if PANGO_CHECK_VERSION(1,16,0)
static VALUE
matrix_get_gravity(VALUE self)
{
    return GENUM2RVAL(pango_gravity_get_for_matrix(_SELF(self)), PANGO_TYPE_GRAVITY);
}
#endif

static VALUE
matrix_concat(VALUE self, VALUE new_matrix)
{
    pango_matrix_concat(_SELF(self), _SELF(new_matrix));
    return self;
}

#if PANGO_CHECK_VERSION(1,12,0)
static VALUE
matrix_get_font_scale_factor(VALUE self)
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
matrix_to_a(VALUE self)
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
    VALUE matrix = G_DEF_CLASS(PANGO_TYPE_MATRIX, "Matrix", mPango);
    
    rb_define_method(matrix, "initialize", matrix_initialize, -1);
    rb_define_method(matrix, "translate!", matrix_translate, 2);
    rb_define_method(matrix, "scale!", matrix_scale, 2);
    rb_define_method(matrix, "rotate!", matrix_rotate, 1);
    rb_define_method(matrix, "concat!", matrix_concat, 1);
#if PANGO_CHECK_VERSION(1,12,0)
    rb_define_method(matrix, "font_scale_factor", matrix_get_font_scale_factor, 0);
#endif
#if PANGO_CHECK_VERSION(1,16,0)
    rb_define_method(matrix, "gravity", matrix_get_gravity, 0);
#endif
    rb_define_method(matrix, "to_a", matrix_to_a, 0);

    DEFINE_ACCESSOR(xx);
    DEFINE_ACCESSOR(xy);
    DEFINE_ACCESSOR(yx);
    DEFINE_ACCESSOR(yy);
    DEFINE_ACCESSOR(x0);
    DEFINE_ACCESSOR(y0);

    G_DEF_SETTERS(matrix);

#endif
}
