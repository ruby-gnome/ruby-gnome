/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdktimecoord.c -

  $Author: sakai $
  $Date: 2006/05/26 14:18:22 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "global.h"

#define _SELF(s) ((GdkTimeCoord*)RVAL2BOXED(s, GDK_TYPE_TIME_COORD))

/**********************************/
static GdkTimeCoord*
timecoord_copy(const GdkTimeCoord* val)
{
    GdkTimeCoord* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(GdkTimeCoord, 1);
    *new_val = *val;
    return new_val;
}

GType
gdk_timecoord_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("GdkTimeCoord",
                    (GBoxedCopyFunc)timecoord_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
timecoord_initialize(self, time, axes)
    VALUE self, time, axes;
{
    GdkTimeCoord coord;
    int i;
    coord.time = NUM2UINT(time);

    if (RARRAY_LEN(axes) > GDK_MAX_TIMECOORD_AXES){
        rb_raise(rb_eArgError, "axes: Out of range: %ld", RARRAY_LEN(axes));
    }

    for (i = 0; i < RARRAY_LEN(axes); i++){
        coord.axes[i] = NUM2DBL(RARRAY_PTR(axes)[i]);
    }

    G_INITIALIZE(self, &coord);

    return Qnil;
}

static VALUE
timecoord_time(self)
    VALUE self;
{
    return UINT2NUM(_SELF(self)->time);
}

static VALUE
timecoord_set_time(self, time)
    VALUE self, time;
{
    _SELF(self)->time = NUM2UINT(time);
    return self;
}

static VALUE
timecoord_axes(self)
    VALUE self;
{
    VALUE ary = rb_ary_new();
    int i;
    for (i = 0; i < GDK_MAX_TIMECOORD_AXES; i++){
        rb_ary_push(ary, rb_float_new(_SELF(self)->axes[i]));
    }
    return ary;
}

static VALUE
timecoord_set_axes(self, axes)
    VALUE self, axes;
{
    int i;
    GdkTimeCoord* coord = _SELF(self);

    if (RARRAY_LEN(axes) > GDK_MAX_TIMECOORD_AXES){
        rb_raise(rb_eArgError, "axes: Out of range: %ld", RARRAY_LEN(axes));
    }

    for (i = 0; i < RARRAY_LEN(axes); i++){
        coord->axes[i] = NUM2DBL(RARRAY_PTR(axes)[i]);
    }
    return self;
}

void
Init_gtk_gdk_timecoord()
{
    VALUE tc = G_DEF_CLASS(GDK_TYPE_TIME_COORD, "TimeCoord", mGdk);

    rb_define_method(tc, "initialize", timecoord_initialize, 2);
    rb_define_method(tc, "time", timecoord_time, 0);
    rb_define_method(tc, "set_time", timecoord_set_time, 1);
    rb_define_method(tc, "axes", timecoord_axes, 0);
    rb_define_method(tc, "set_axes", timecoord_set_axes, 1);

    G_DEF_SETTERS(tc);

    rb_define_const(tc, "MAX_AXES", INT2NUM(GDK_MAX_TIMECOORD_AXES));
}
