/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
#include "rbgnomecanvas.h"

/* points's data structure is:
 *    [[x1, y1], [x2, y2], [x3, y3], ...]
 */
static void
set_coords_from_points_data(coords, points)
    double *coords;
    VALUE points;
{
    VALUE entry;
    int i;

    for (i = 0;i < RARRAY(points)->len;i++) {
        entry = RARRAY(points)->ptr[i];
        Check_Type(entry, T_ARRAY);
        if (RARRAY(entry)->len != 2) {
            rb_raise(rb_eArgError, "wrong coordinate value %d entry(s) for 2", RARRAY(entry)->len);
        }
        coords[i * 2] = NUM2DBL(RARRAY(entry)->ptr[0]);
        coords[i * 2 + 1] = NUM2DBL(RARRAY(entry)->ptr[1]);
    }
}

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
    double *coords;

    Check_Type(from, T_ARRAY);
    coords = ALLOCA_N(double, RARRAY(from)->len * 2);
    set_coords_from_points_data(coords, from);
    /* store all values to coords before allocate points. */
    points = gnome_canvas_points_new(RARRAY(from)->len);
    memcpy(points->coords, coords, sizeof(double) * RARRAY(from)->len * 2);
    g_value_set_boxed(to, points);
    gnome_canvas_points_free(points);
}

static VALUE
canvas_s_color(argc, argv, klass)
    int argc;
    VALUE *argv, klass;
{
    VALUE vr, vg, vb, va;
    int r, g, b, a;

    rb_scan_args(argc, argv, "31", &vr, &vg, &vb, &va);
    r = NUM2INT(vr);
    b = NUM2INT(vb);
    g = NUM2INT(vg);
    a = NIL_P(va) ? 0xff : NUM2INT(va);
    if ((r & ~0xff) || (b & ~0xff) || (g & ~0xff) || (a & ~0xff)) {
        rb_raise(rb_eArgError, "Out of range for color. (expect 0..255 for each value)");
    }
    return UINT2NUM(GNOME_CANVAS_COLOR_A(r, g, b, a));
}

static VALUE
canvas_s_get_miter_points(klass, x1, y1, x2, y2, x3, y3, width)
    VALUE klass, x1, y1, x2, y2, x3, y3, width;
{
    double mx1, my1, mx2, my2;
    gnome_canvas_get_miter_points(NUM2DBL(x1), NUM2DBL(y1),
                                  NUM2DBL(x2), NUM2DBL(y2),
                                  NUM2DBL(x3), NUM2DBL(y3),
                                  NUM2DBL(width),
                                  &mx1, &my1, &mx2, &my2);
    return rb_ary_new3(4,
                       rb_float_new(mx1), rb_float_new(my1),
                       rb_float_new(mx2), rb_float_new(my2));
}

static VALUE
canvas_s_get_butt_points(klass, x1, y1, x2, y2, width, project)
    VALUE klass, x1, y1, x2, y2, width, project;
{
    double bx1, by1, bx2, by2;
    gnome_canvas_get_butt_points(NUM2DBL(x1), NUM2DBL(y1),
                                 NUM2DBL(x2), NUM2DBL(y2),
                                 NUM2DBL(width), NUM2INT(project),
                                 &bx1, &by1, &bx2, &by2);
    return rb_ary_new3(4,
                       rb_float_new(bx1), rb_float_new(by1),
                       rb_float_new(bx2), rb_float_new(by2));
}


static VALUE
canvas_s_polygon_to_point(klass, points, x, y)
    VALUE klass, points, x, y;
{
    double *coords;
    double result;

    Check_Type(points, T_ARRAY);
    coords = ALLOCA_N(double, RARRAY(points)->len * 2);
    set_coords_from_points_data(coords, points);

    result = gnome_canvas_polygon_to_point(coords,
                                           RARRAY(points)->len,
                                           NUM2DBL(x),
                                           NUM2DBL(y));
    return rb_float_new(result);
}


void
Init_gnome_canvas_util(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvas = GTYPE2CLASS(GNOME_TYPE_CANVAS);

    rbgobj_register_g2r_func(GNOME_TYPE_CANVAS_POINTS, cpoint_to_ruby);
    rbgobj_register_r2g_func(GNOME_TYPE_CANVAS_POINTS, cpoint_from_ruby);

    rb_define_singleton_method(gnoCanvas, "color", canvas_s_color, -1);
    rb_define_singleton_method(gnoCanvas, "get_miter_points", canvas_s_get_miter_points, 7);
    rb_define_singleton_method(gnoCanvas, "get_butt_points", canvas_s_get_butt_points, 6);
    rb_define_singleton_method(gnoCanvas, "polygon_to_point", canvas_s_polygon_to_point, 3);
}
