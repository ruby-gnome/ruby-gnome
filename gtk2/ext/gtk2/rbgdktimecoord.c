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
timecoord_initialize(VALUE self, VALUE rbtime, VALUE rbaxes)
{
    guint32 time = NUM2UINT(rbtime);
    long n;
    gdouble *axes = RVAL2GDOUBLES(rbaxes, n);
    GdkTimeCoord *coord;

    if (n > GDK_MAX_TIMECOORD_AXES) {
        g_free(axes);

        rb_raise(rb_eArgError,
                 "axes out of range: %ld (0..%d)",
                 n, GDK_MAX_TIMECOORD_AXES);
    }

    coord = g_new(GdkTimeCoord, 1);
    coord->time = time;
    MEMCPY(coord->axes, axes, gdouble, n);

    g_free(axes);

    G_INITIALIZE(self, coord);

    return Qnil;
}

static VALUE
timecoord_time(VALUE self)
{
    return UINT2NUM(_SELF(self)->time);
}

static VALUE
timecoord_set_time(VALUE self, VALUE time)
{
    _SELF(self)->time = NUM2UINT(time);
    return self;
}

static VALUE
timecoord_axes(VALUE self)
{
    VALUE ary = rb_ary_new();
    int i;
    for (i = 0; i < GDK_MAX_TIMECOORD_AXES; i++){
        rb_ary_push(ary, rb_float_new(_SELF(self)->axes[i]));
    }
    return ary;
}

static VALUE
timecoord_set_axes(VALUE self, VALUE rbaxes)
{
    GdkTimeCoord *coord = _SELF(self);
    VALUE axes = rb_ary_to_ary(rbaxes);
    long i;
    long n = RARRAY_LEN(axes);

    if (n < 0 || n > GDK_MAX_TIMECOORD_AXES)
        rb_raise(rb_eArgError,
                 "axes out of range: %ld (0..%d)",
                 n, GDK_MAX_TIMECOORD_AXES);

    for (i = 0; i < n; i++)
        coord->axes[i] = NUM2DBL(RARRAY_PTR(axes)[i]);

    return self;
}

void
Init_gtk_gdk_timecoord(void)
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
