/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2005 Masao Mutoh
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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
#include "rbpango.h"
#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif
#ifdef HAVE_RB_CAIRO_H
#include <rb_cairo.h>
#endif

#define RG_TARGET_NAMESPACE cDrawable
#define _SELF(s) GDK_DRAWABLE(RVAL2GOBJ(s))

static VALUE
rg_visual(VALUE self)
{
    return GOBJ2RVAL(_SELF(self));
}

static VALUE
rg_set_colormap(VALUE self, VALUE colormap)
{
    VALUE old_colormap;

    old_colormap = GOBJ2RVAL(gdk_drawable_get_colormap(_SELF(self)));
    G_CHILD_REMOVE(self, old_colormap);

    G_CHILD_ADD(self, colormap);
    gdk_drawable_set_colormap(_SELF(self), GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
rg_colormap(VALUE self)
{
    VALUE rb_colormap;

    rb_colormap = GOBJ2RVAL(gdk_drawable_get_colormap(_SELF(self)));
    G_CHILD_ADD(self, rb_colormap);
    return rb_colormap;
}

static VALUE
rg_depth(VALUE self)
{
    return INT2NUM(gdk_drawable_get_depth(_SELF(self)));
}

static VALUE
rg_size(VALUE self)
{
    gint width, height;
    gdk_drawable_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_clip_region(VALUE self)
{
    return BOXED2RVAL(gdk_drawable_get_clip_region(_SELF(self)), GDK_TYPE_REGION);
}

static VALUE
rg_visible_region(VALUE self)
{
    return BOXED2RVAL(gdk_drawable_get_visible_region(_SELF(self)), GDK_TYPE_REGION);
}

static VALUE
rg_draw_point(VALUE self, VALUE gc, VALUE x, VALUE y)
{
    gdk_draw_point(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                   NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
rg_draw_points(VALUE self, VALUE rbgc, VALUE rbpoints)
{
    GdkDrawable *drawable = _SELF(self);
    GdkGC *gc = GDK_GC(RVAL2GOBJ(rbgc));
    long n;
    GdkPoint *points = RVAL2GDKPOINTS(rbpoints, &n);

    gdk_draw_points(drawable, gc, points, n);

    g_free(points);

    return self;
}

static VALUE
rg_draw_line(VALUE self, VALUE gc, VALUE x1, VALUE y1, VALUE x2, VALUE y2)
{
    gdk_draw_line(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                  NUM2INT(x1), NUM2INT(y1),
                  NUM2INT(x2), NUM2INT(y2));
    return self;
}

static VALUE
rg_draw_lines(VALUE self, VALUE rbgc, VALUE rbpoints)
{
    GdkDrawable *drawable = _SELF(self);
    GdkGC *gc = GDK_GC(RVAL2GOBJ(rbgc));
    long n;
    GdkPoint *points = RVAL2GDKPOINTS(rbpoints, &n);

    gdk_draw_lines(drawable, gc, points, n);

    g_free(points);

    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_draw_pixbuf(VALUE self, VALUE gc, VALUE pixbuf, VALUE src_x, VALUE src_y, VALUE dest_x, VALUE dest_y, VALUE width, VALUE height, VALUE dither, VALUE x_dither, VALUE y_dither)
{
    gdk_draw_pixbuf(_SELF(self),
                    GDK_GC(RVAL2GOBJ(gc)),
                    GDK_PIXBUF(RVAL2GOBJ(pixbuf)),
                    NUM2INT(src_x), NUM2INT(src_y), 
                    NUM2INT(dest_x), NUM2INT(dest_y),
                    NUM2INT(width), NUM2INT(height),
                    RVAL2GENUM(dither, GDK_TYPE_RGB_DITHER),
                    NUM2INT(x_dither), NUM2INT(y_dither));
    return self;
}
#endif

struct rbgdk_rval2gdksegments_args {
    VALUE ary;
    long n;
    GdkSegment *result;
};

static VALUE
rbgdk_rval2gdksegments_body(VALUE value)
{
    long i;
    struct rbgdk_rval2gdksegments_args *args = (struct rbgdk_rval2gdksegments_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE segments = rb_ary_to_ary(RARRAY_PTR(args->ary)[i]);

        if (RARRAY_LEN(segments) != 2)
            rb_raise(rb_eArgError, "segment %ld should be array of size 4", i);

        args->result[i].x1 = NUM2INT(RARRAY_PTR(segments)[0]);
        args->result[i].y1 = NUM2INT(RARRAY_PTR(segments)[1]);
        args->result[i].x2 = NUM2INT(RARRAY_PTR(segments)[2]);
        args->result[i].y2 = NUM2INT(RARRAY_PTR(segments)[3]);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgdk_rval2gdksegments_rescue(VALUE value)
{
    g_free(((struct rbgdk_rval2gdksegments_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GdkSegment *
rbgdk_rval2gdksegments(VALUE value, long *n)
{
    struct rbgdk_rval2gdksegments_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GdkSegment, args.n + 1);

    rb_rescue(rbgdk_rval2gdksegments_body, (VALUE)&args,
              rbgdk_rval2gdksegments_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

#define RVAL2GDKSEGMENTS(value, n) rbgdk_rval2gdksegments(value, n)

static VALUE
rg_draw_segments(VALUE self, VALUE rbgc, VALUE rbsegments)
{
    GdkDrawable *drawable = _SELF(self);
    GdkGC *gc = GDK_GC(RVAL2GOBJ(rbgc));
    long n;
    GdkSegment *segments = RVAL2GDKSEGMENTS(rbsegments, &n);

    gdk_draw_segments(drawable, gc, segments, n);

    g_free(segments);

    return self;
}

static VALUE
rg_draw_rectangle(VALUE self, VALUE gc, VALUE filled, VALUE x, VALUE y, VALUE w, VALUE h)
{
    gdk_draw_rectangle(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                       RVAL2CBOOL(filled),
                       NUM2INT(x), NUM2INT(y),
                       NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_draw_arc(VALUE self, VALUE gc, VALUE filled, VALUE x, VALUE y, VALUE w, VALUE h, VALUE a1, VALUE a2)
{
    gdk_draw_arc(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                 RVAL2CBOOL(filled),
                 NUM2INT(x), NUM2INT(y),
                 NUM2INT(w), NUM2INT(h),
                 NUM2INT(a1), NUM2INT(a2));
    return self;
}

static VALUE
rg_draw_polygon(VALUE self, VALUE rbgc, VALUE rbfilled, VALUE rbpoints)
{
    GdkDrawable *drawable = _SELF(self);
    GdkGC *gc = GDK_GC(RVAL2GOBJ(rbgc));
    gboolean filled = RVAL2CBOOL(rbfilled);
    long n;
    GdkPoint *points = RVAL2GDKPOINTS(rbpoints, &n);

    gdk_draw_polygon(drawable, gc, filled, points, n);

    g_free(points);

    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
/*
  trapezoids = [[y1, x11, x21, y2, x12, x22], ...]
 */
struct rbgdk_rval2gdktrapezoids_args {
    VALUE ary;
    long n;
    GdkTrapezoid *result;
};

static VALUE
rbgdk_rval2gdktrapezoids_body(VALUE value)
{
    long i;
    struct rbgdk_rval2gdktrapezoids_args *args = (struct rbgdk_rval2gdktrapezoids_args *)value;

    for (i = 0; i < args->n; i++) {
        VALUE trapezoids = rb_ary_to_ary(RARRAY_PTR(args->ary)[i]);

        if (RARRAY_LEN(trapezoids) != 6)
            rb_raise(rb_eArgError, "trapezoid %ld should be array of size 6", i);

        args->result[i].y1 = NUM2DBL(RARRAY_PTR(trapezoids)[0]);
        args->result[i].x11 = NUM2DBL(RARRAY_PTR(trapezoids)[1]);
        args->result[i].x21 = NUM2DBL(RARRAY_PTR(trapezoids)[2]);
        args->result[i].y2 = NUM2DBL(RARRAY_PTR(trapezoids)[3]);
        args->result[i].x12 = NUM2DBL(RARRAY_PTR(trapezoids)[4]);
        args->result[i].x22 = NUM2DBL(RARRAY_PTR(trapezoids)[5]);
    }

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgdk_rval2gdktrapezoids_rescue(VALUE value)
{
    g_free(((struct rbgdk_rval2gdktrapezoids_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GdkTrapezoid *
rbgdk_rval2gdktrapezoids(VALUE value, long *n)
{
    struct rbgdk_rval2gdktrapezoids_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GdkTrapezoid, args.n + 1);

    rb_rescue(rbgdk_rval2gdktrapezoids_body, (VALUE)&args,
              rbgdk_rval2gdktrapezoids_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

#define RVAL2GDKTRAPEZOIDS(value, n) rbgdk_rval2gdktrapezoids(value, n)

static VALUE
rg_draw_trapezoids(VALUE self, VALUE rbgc, VALUE rbtrapezoids)
{
    GdkDrawable *drawable = _SELF(self);
    GdkGC *gc = GDK_GC(RVAL2GOBJ(rbgc));
    long n;
    GdkTrapezoid *trapezoids = RVAL2GDKTRAPEZOIDS(rbtrapezoids, &n);

    gdk_draw_trapezoids(drawable, gc, trapezoids, n);

    g_free(trapezoids);

    return self;
}
#endif

static VALUE
rg_draw_glyphs(VALUE self, VALUE gc, VALUE font, VALUE x, VALUE y, VALUE glyphs)
{
    gdk_draw_glyphs(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), PANGO_FONT(RVAL2GOBJ(font)),
                    NUM2INT(x), NUM2INT(y), 
                    (PangoGlyphString*)(RVAL2BOXED(glyphs, PANGO_TYPE_GLYPH_STRING)));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
rg_draw_glyphs_transformed(VALUE self, VALUE gc, VALUE matrix, VALUE font, VALUE x, VALUE y, VALUE glyphs)
{
    gdk_draw_glyphs_transformed(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), 
                                NIL_P(matrix) ? (PangoMatrix*)NULL : (PangoMatrix*)(RVAL2BOXED(matrix, PANGO_TYPE_MATRIX)),
                                PANGO_FONT(RVAL2GOBJ(font)),
                                NUM2INT(x), NUM2INT(y), 
                                (PangoGlyphString*)(RVAL2BOXED(glyphs, PANGO_TYPE_GLYPH_STRING)));
    return self;
}
#endif

static VALUE
rg_draw_layout_line(int argc, VALUE *argv, VALUE self)
{
    VALUE gc, x, y, line, fg, bg;

    rb_scan_args(argc, argv, "42", &gc, &x, &y, &line, &fg, &bg);

    gdk_draw_layout_line_with_colors(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                                     NUM2INT(x), NUM2INT(y),
                                     (PangoLayoutLine*)RVAL2BOXED(line, PANGO_TYPE_LAYOUT_LINE),
                                     RVAL2GDKCOLOR(fg),
                                     RVAL2GDKCOLOR(bg));

    return self;
}

static VALUE
rg_draw_layout(int argc, VALUE *argv, VALUE self)
{
    VALUE gc, x, y, layout, fg, bg;

    rb_scan_args(argc, argv, "42", &gc, &x, &y, &layout, &fg, &bg);

    gdk_draw_layout_with_colors(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                                NUM2INT(x), NUM2INT(y), PANGO_LAYOUT(RVAL2GOBJ(layout)),
                                RVAL2GDKCOLOR(fg),
                                RVAL2GDKCOLOR(bg));

    return self;
}

static VALUE
rg_draw_drawable(VALUE self, VALUE gc, VALUE src, VALUE xsrc, VALUE ysrc, VALUE xdst, VALUE ydst, VALUE w, VALUE h)
{
    gdk_draw_drawable(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), _SELF(src),
                      NUM2INT(xsrc), NUM2INT(ysrc),
                      NUM2INT(xdst), NUM2INT(ydst),
                      NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_draw_image(VALUE self, VALUE gc, VALUE image, VALUE xsrc, VALUE ysrc, VALUE xdst, VALUE ydst, VALUE w, VALUE h)
{
    gdk_draw_image(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                   GDK_IMAGE(RVAL2GOBJ(image)),
                   NUM2INT(xsrc), NUM2INT(ysrc),
                   NUM2INT(xdst), NUM2INT(ydst),
                   NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
rg_get_image(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
    return GOBJ2RVAL(gdk_drawable_get_image(_SELF(self), 
                                            NUM2INT(x), NUM2INT(y), 
                                            NUM2INT(w), NUM2INT(h)));
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_copy_to_image(VALUE self, VALUE image, VALUE xsrc, VALUE ysrc, VALUE xdst, VALUE ydst, VALUE w, VALUE h)
{
    return GOBJ2RVAL(gdk_drawable_copy_to_image(_SELF(self), 
                                                GDK_IMAGE(RVAL2GOBJ(image)),
                                                NUM2INT(xsrc), NUM2INT(ysrc),
                                                NUM2INT(xdst), NUM2INT(ydst),
                                                NUM2INT(w), NUM2INT(h)));
}
#endif

#ifdef GDK_WINDOWING_X11
static VALUE
rg_xid(VALUE self)
{
    return ULONG2NUM(GDK_DRAWABLE_XID(_SELF(self)));
}
#endif

#ifdef GDK_WINDOWING_WIN32
static VALUE
rg_handle(VALUE self)
{
    HGDIOBJ handle;
    handle = gdk_win32_drawable_get_handle(_SELF(self));
    return ULONG2NUM(GPOINTER_TO_UINT(handle));
}
#endif

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_display(VALUE self)
{
    return GOBJ2RVAL(gdk_drawable_get_display(_SELF(self)));
}

static VALUE
rg_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_drawable_get_screen(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
#  ifdef HAVE_RB_CAIRO_H
static VALUE
rg_create_cairo_context(VALUE self)
{
    VALUE rb_cr;
    cairo_t *cr;
    cr = gdk_cairo_create(_SELF(self));
    rb_cairo_check_status(cairo_status(cr));
    rb_cr = CRCONTEXT2RVAL(cr);
    cairo_destroy (cr);
    return rb_cr;
}
#  endif
#endif

void
Init_gtk_gdk_draw(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_DRAWABLE, "Drawable", mGdk);

    RG_DEF_METHOD(visual, 0);
    RG_DEF_METHOD(set_colormap, 1);
    RG_DEF_METHOD(colormap, 0);
    RG_DEF_METHOD(depth, 0);
    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(clip_region, 0);
    RG_DEF_METHOD(visible_region, 0);
    RG_DEF_METHOD(draw_point, 3);
    RG_DEF_METHOD(draw_points, 2);
    RG_DEF_METHOD(draw_line, 5);
    RG_DEF_METHOD(draw_lines, 2);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(draw_pixbuf, 11);
#endif
    RG_DEF_METHOD(draw_segments, 2);
    RG_DEF_METHOD(draw_rectangle, 6);
    RG_DEF_METHOD(draw_arc, 8);
    RG_DEF_METHOD(draw_polygon, 3);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(draw_trapezoids, 2);
#endif
    RG_DEF_METHOD(draw_glyphs, 5);
#if GTK_CHECK_VERSION(2,6,0)
    RG_DEF_METHOD(draw_glyphs_transformed, 6);
#endif
    RG_DEF_METHOD(draw_layout_line, -1);
    RG_DEF_METHOD(draw_layout, -1);
    RG_DEF_METHOD(draw_drawable, 8);
    RG_DEF_METHOD(draw_image, 8);
    RG_DEF_METHOD(get_image, 4);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_METHOD(copy_to_image, 7);
#endif

#ifdef GDK_WINDOWING_X11
    RG_DEF_METHOD(xid, 0);
#endif
#ifdef GDK_WINDOWING_WIN32
    RG_DEF_METHOD(handle, 0);
#endif
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(display, 0);
    RG_DEF_METHOD(screen, 0);
#endif

#if GTK_CHECK_VERSION(2,8,0)
#  ifdef HAVE_RB_CAIRO_H
    RG_DEF_METHOD(create_cairo_context, 0);
#  endif
#endif

#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkDrawableImplX11", "DrawableImplX11", mGdk);
#elif defined(GDK_WINDOWING_WIN32)
    G_DEF_CLASS3("GdkDrawableImplWin32", "DrawableImplWin32", mGdk);
#elif defined(GDK_WINDOWING_FB)
    G_DEF_CLASS3("GdkDrawableFB", "DrawableFB", mGdk);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
