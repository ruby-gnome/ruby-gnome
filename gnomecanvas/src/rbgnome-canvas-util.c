/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
#include "rbgnomecanvas.h"

static VALUE
cpoint_to_ruby(from)
    GValue *from;
{
    GnomeCanvasPoints *points = (GnomeCanvasPoints *)g_value_get_boxed(from);
    VALUE ary = rb_ary_new2(points->num_points);
    int i;

    for (i = 0;i < points->num_points;i++) {
        rb_ary_push(ary, rb_ary_new3(2,
                                     rb_float_new(points->coords[i * 2]),
                                     rb_float_new(points->coords[i * 2 + 1])));
    }
    return ary;
}

static void
cpoint_from_ruby(from, to)
    VALUE from;
    GValue *to;
{
    GnomeCanvasPoints *points;
    VALUE entry;
    double *coords;
    int i;

    Check_Type(from, T_ARRAY);
    coords = ALLOCA_N(double, RARRAY(from)->len * 2);
    for (i = 0;i < RARRAY(from)->len;i++) {
        entry = RARRAY(from)->ptr[i];
        Check_Type(entry, T_ARRAY);
        if (RARRAY(entry)->len != 2) {
            rb_raise(rb_eArgError, "wrong coordinate value %d entry(s) for 2", RARRAY(entry)->len);
        }
        coords[i * 2] = NUM2DBL(RARRAY(entry)->ptr[0]);
        coords[i * 2 + 1] = NUM2DBL(RARRAY(entry)->ptr[1]);
    }
    /* store all values to coords before allocate points. */
    points = gnome_canvas_points_new(RARRAY(from)->len);
    memcpy(points->coords, coords, sizeof(double) * RARRAY(from)->len * 2);
    g_value_set_boxed(to, points);
    gnome_canvas_points_free(points);
}

void
Init_gnome_canvas_util(mGnome)
    VALUE mGnome;
{
    rbgobj_register_g2r_func(GNOME_TYPE_CANVAS_POINTS, cpoint_to_ruby);
    rbgobj_register_r2g_func(GNOME_TYPE_CANVAS_POINTS, cpoint_from_ruby);
}
