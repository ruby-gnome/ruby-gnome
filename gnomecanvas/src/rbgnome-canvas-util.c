/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgnome-canvas-util.c,v 1.9 2005/02/15 06:12:56 mutoh Exp $ */

/* rbgnome-canvas-util.c
 *
 * Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

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
    int i;
    GnomeCanvasPoints* points = (GnomeCanvasPoints *)g_value_get_boxed(from);
    VALUE ary;

    if (! points) return Qnil;

    ary = rb_ary_new2(points->num_points);

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

/*
void        gnome_canvas_render_svp         (GnomeCanvasBuf *buf,
                                             ArtSVP *svp,
                                             guint32 rgba);
void        gnome_canvas_update_svp         (GnomeCanvas *canvas,
                                             ArtSVP **p_svp,
                                             ArtSVP *new_svp);
void        gnome_canvas_update_svp_clip    (GnomeCanvas *canvas,
                                             ArtSVP **p_svp,
                                             ArtSVP *new_svp,
                                             ArtSVP *clip_svp);
*/
static VALUE
canvasitem_reset_bounds(self)
    VALUE self;
{
    gnome_canvas_item_reset_bounds(GNOME_CANVAS_ITEM(RVAL2GOBJ(self)));
    return self;
}

/*
void        gnome_canvas_item_update_svp    (GnomeCanvasItem *item,
                                             ArtSVP **p_svp,
                                             ArtSVP *new_svp);
void        gnome_canvas_item_update_svp_clip
                                            (GnomeCanvasItem *item,
                                             ArtSVP **p_svp,
                                             ArtSVP *new_svp,
                                             ArtSVP *clip_svp);
void        gnome_canvas_item_request_redraw_svp
                                            (GnomeCanvasItem *item,
                                             const ArtSVP *svp);
*/

static VALUE
canvasitem_update_bbox(self, x1, y1, x2, y2)
    VALUE self, x1, y1, x2, y2;
{
    gnome_canvas_update_bbox(GNOME_CANVAS_ITEM(RVAL2GOBJ(self)), 
                             NUM2INT(x1), NUM2INT(y1), 
                             NUM2INT(x2), NUM2INT(y2));
    return self;
}

/*
void        gnome_canvas_buf_ensure_buf     (GnomeCanvasBuf *buf);
*/

static VALUE
canvas_s_join_gdk_to_art(self, gdk_join)
    VALUE self, gdk_join;
{
    return INT2NUM(gnome_canvas_join_gdk_to_art(RVAL2GENUM(gdk_join, GDK_TYPE_JOIN_STYLE)));
}

static VALUE
canvas_s_cap_gdk_to_art(self, gdk_cap)
    VALUE self, gdk_cap;
{
    return INT2NUM(gnome_canvas_cap_gdk_to_art(RVAL2GENUM(gdk_cap, GDK_TYPE_CAP_STYLE)));
}

void
Init_gnome_canvas_util(mGnome)
    VALUE mGnome;
{
    VALUE gnoCanvas = GTYPE2CLASS(GNOME_TYPE_CANVAS);
    VALUE gnoCanvasItem = GTYPE2CLASS(GNOME_TYPE_CANVAS_ITEM);

    rbgobj_register_g2r_func(GNOME_TYPE_CANVAS_POINTS, cpoint_to_ruby);
    rbgobj_register_r2g_func(GNOME_TYPE_CANVAS_POINTS, cpoint_from_ruby);

    rb_define_singleton_method(gnoCanvas, "color", canvas_s_color, -1);
    rb_define_singleton_method(gnoCanvas, "get_miter_points", canvas_s_get_miter_points, 7);
    rb_define_singleton_method(gnoCanvas, "get_butt_points", canvas_s_get_butt_points, 6);
    rb_define_singleton_method(gnoCanvas, "polygon_to_point", canvas_s_polygon_to_point, 3);

    rb_define_method(gnoCanvasItem, "reset_bounds", canvasitem_reset_bounds, 0);
    rb_define_method(gnoCanvasItem, "update_bbox", canvasitem_update_bbox, 4);

    rb_define_singleton_method(gnoCanvas, "join_gdk_to_art", canvas_s_join_gdk_to_art, 1);
    rb_define_singleton_method(gnoCanvas, "cap_gdk_to_art", canvas_s_cap_gdk_to_art, 1);
}
