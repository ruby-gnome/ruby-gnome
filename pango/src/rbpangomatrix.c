/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangomatrix.c -

  $Author: mutoh $
  $Date: 2005/01/28 09:48:46 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "rbpango.h"

#if PANGO_CHECK_VERSION(1,6,0)
#define _SELF(self) ((PangoMatrix*)(RVAL2BOXED(self, PANGO_TYPE_MATRIX)))

static VALUE
matrix_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
matrix_translate(self, tx, ty)
    VALUE self, tx, ty;
{
    pango_matrix_translate(_SELF(self), NUM2DBL(tx), NUM2DBL(ty));
    return self;
}

static VALUE
matrix_scale(self, scale_x, scale_y)
    VALUE self, scale_x, scale_y;
{
    pango_matrix_scale(_SELF(self), NUM2DBL(scale_x), NUM2DBL(scale_y));
    return self;
}

static VALUE
matrix_rotate(self, degrees)
    VALUE self, degrees;
{
    pango_matrix_rotate(_SELF(self), NUM2DBL(degrees));
    return self;
}

static VALUE
matrix_concat(self, new_matrix)
    VALUE self, new_matrix;
{
    pango_matrix_concat(_SELF(self), _SELF(new_matrix));
    return self;
}
#endif

void
Init_pango_matrix()
{
#if PANGO_CHECK_VERSION(1,6,0)
    VALUE matrix = G_DEF_CLASS(PANGO_TYPE_MATRIX, "Matrix", mPango);
    
    rb_define_method(matrix, "initialize", matrix_initialize, -1);
    rb_define_method(matrix, "translate!", matrix_translate, 2);
    rb_define_method(matrix, "scale!", matrix_scale, 2);
    rb_define_method(matrix, "rotate!", matrix_rotate, 1);
    rb_define_method(matrix, "concat!", matrix_concat, 1);

#endif
}
