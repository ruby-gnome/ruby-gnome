/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003  Masao Mutoh
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

#define RG_TARGET_NAMESPACE cTimeCoord
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
rg_initialize(VALUE self, VALUE rbtime, VALUE rbaxes)
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
rg_time(VALUE self)
{
    return UINT2NUM(_SELF(self)->time);
}

static VALUE
rg_set_time(VALUE self, VALUE time)
{
    _SELF(self)->time = NUM2UINT(time);
    return self;
}

static VALUE
rg_axes(VALUE self)
{
    VALUE ary = rb_ary_new();
    int i;
    for (i = 0; i < GDK_MAX_TIMECOORD_AXES; i++){
        rb_ary_push(ary, rb_float_new(_SELF(self)->axes[i]));
    }
    return ary;
}

static VALUE
rg_set_axes(VALUE self, VALUE rbaxes)
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
Init_gtk_gdk_timecoord(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_TIME_COORD, "TimeCoord", mGdk);

    RG_DEF_METHOD(initialize, 2);
    RG_DEF_METHOD(time, 0);
    RG_DEF_METHOD(set_time, 1);
    RG_DEF_METHOD(axes, 0);
    RG_DEF_METHOD(set_axes, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    rb_define_const(RG_TARGET_NAMESPACE, "MAX_AXES", INT2NUM(GDK_MAX_TIMECOORD_AXES));
}
