/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdraw.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002-2005 Masao Mutoh
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include "rbpango.h"
#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif
#ifdef HAVE_RB_CAIRO_H
#include <rb_cairo.h>
#endif

#define _SELF(s) GDK_DRAWABLE(RVAL2GOBJ(s))

static VALUE
gdkdraw_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self));
}

static VALUE
gdkdraw_set_colormap(VALUE self, VALUE colormap)
{
    VALUE old_colormap;

    old_colormap = GOBJ2RVAL(gdk_drawable_get_colormap(_SELF(self)));
    G_CHILD_REMOVE(self, old_colormap);

    G_CHILD_ADD(self, colormap);
    gdk_drawable_set_colormap(_SELF(self), GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
gdkdraw_get_colormap(VALUE self)
{
    VALUE rb_colormap;

    rb_colormap = GOBJ2RVAL(gdk_drawable_get_colormap(_SELF(self)));
    G_CHILD_ADD(self, rb_colormap);
    return rb_colormap;
}

static VALUE
gdkdraw_get_depth(self)
    VALUE self;
{
    return INT2NUM(gdk_drawable_get_depth(_SELF(self)));
}

static VALUE
gdkdraw_get_size(self)
    VALUE self;
{
    gint width, height;
    gdk_drawable_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
gdkdraw_get_clip_region(self)
    VALUE self;
{
    return BOXED2RVAL(gdk_drawable_get_clip_region(_SELF(self)), GDK_TYPE_REGION);
}

static VALUE
gdkdraw_get_visible_region(self)
    VALUE self;
{
    return BOXED2RVAL(gdk_drawable_get_visible_region(_SELF(self)), GDK_TYPE_REGION);
}


static VALUE
gdkdraw_draw_point(self, gc, x, y)
    VALUE self, gc, x, y;
{
    gdk_draw_point(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                   NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkdraw_draw_points(self, gc, pnts)
    VALUE self, gc, pnts;
{
    GdkPoint *points;
    int i;

    Check_Type(pnts, T_ARRAY);
    points = ALLOCA_N(GdkPoint,RARRAY_LEN(pnts));
    for (i = 0; i < RARRAY_LEN(pnts); i++) {
        Check_Type(RARRAY_PTR(pnts)[i], T_ARRAY);
        if (RARRAY_LEN(RARRAY_PTR(pnts)[i]) < 2) {
            rb_raise(rb_eArgError, "point %d should be array of size 2", i);
        }
        points[i].x = NUM2INT(RARRAY_PTR(RARRAY_PTR(pnts)[i])[0]);
        points[i].y = NUM2INT(RARRAY_PTR(RARRAY_PTR(pnts)[i])[1]);
    }
    gdk_draw_points(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                    points,
                    RARRAY_LEN(pnts));
    return self;
}

static VALUE
gdkdraw_draw_line(self, gc, x1, y1, x2, y2)
    VALUE self, gc, x1, y1, x2, y2;
{
    gdk_draw_line(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                  NUM2INT(x1), NUM2INT(y1),
                  NUM2INT(x2), NUM2INT(y2));
    return self;
}

static VALUE
gdkdraw_draw_lines(self, gc, pnts)
    VALUE self, gc, pnts;
{
    GdkPoint *points;
    int i;

    Check_Type(pnts, T_ARRAY);
    points = ALLOCA_N(GdkPoint,RARRAY_LEN(pnts));
    for (i = 0; i < RARRAY_LEN(pnts); i++) {
        Check_Type(RARRAY_PTR(pnts)[i], T_ARRAY);
        if (RARRAY_LEN(RARRAY_PTR(pnts)[i]) < 2) {
            rb_raise(rb_eArgError, "point %d should be array of size 2", i);
        }
        points[i].x = NUM2INT(RARRAY_PTR(RARRAY_PTR(pnts)[i])[0]);
        points[i].y = NUM2INT(RARRAY_PTR(RARRAY_PTR(pnts)[i])[1]);
    }
    gdk_draw_lines(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                   points,
                   RARRAY_LEN(pnts));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkdraw_draw_pixbuf(self, gc, pixbuf, src_x, src_y, dest_x, dest_y, 
                    width, height, dither, x_dither, y_dither)
    VALUE self, gc, pixbuf, src_x, src_y, dest_x, dest_y; 
    VALUE width, height, dither, x_dither, y_dither;
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

static VALUE
gdkdraw_draw_segs(self, gc, segs)
    VALUE self, gc, segs;
{
    GdkSegment *segments;
    int i;

    Check_Type(segs, T_ARRAY);
    segments = ALLOCA_N(GdkSegment,RARRAY_LEN(segs));
    for (i = 0; i < RARRAY_LEN(segs); i++) {
        Check_Type(RARRAY_PTR(segs)[i], T_ARRAY);
        if (RARRAY_LEN(RARRAY_PTR(segs)[i]) < 4) {
            rb_raise(rb_eArgError, "segment %d should be array of size 4", i);
        }
        segments[i].x1 = NUM2INT(RARRAY_PTR(RARRAY_PTR(segs)[i])[0]);
        segments[i].y1 = NUM2INT(RARRAY_PTR(RARRAY_PTR(segs)[i])[1]);
        segments[i].x2 = NUM2INT(RARRAY_PTR(RARRAY_PTR(segs)[i])[2]);
        segments[i].y2 = NUM2INT(RARRAY_PTR(RARRAY_PTR(segs)[i])[3]);
    }
    gdk_draw_segments(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                      segments, RARRAY_LEN(segs));
    return self;
}

static VALUE
gdkdraw_draw_rect(self, gc, filled, x, y, w, h)
    VALUE self, gc, filled, x, y, w, h;
{
    gdk_draw_rectangle(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                       RVAL2CBOOL(filled),
                       NUM2INT(x), NUM2INT(y),
                       NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkdraw_draw_arc(self, gc, filled, x, y, w, h, a1, a2)
    VALUE self, gc, filled, x, y, w, h, a1, a2;
{
    gdk_draw_arc(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                 RVAL2CBOOL(filled),
                 NUM2INT(x), NUM2INT(y),
                 NUM2INT(w), NUM2INT(h),
                 NUM2INT(a1), NUM2INT(a2));
    return self;
}

static VALUE
gdkdraw_draw_poly(self, gc, filled, pnts)
    VALUE self, gc, filled, pnts;
{
    GdkPoint *points;
    int i;

    Check_Type(pnts, T_ARRAY);
    points = ALLOCA_N(GdkPoint,RARRAY_LEN(pnts));
    for (i = 0; i < RARRAY_LEN(pnts); i++) {
        Check_Type(RARRAY_PTR(pnts)[i], T_ARRAY);
        if (RARRAY_LEN(RARRAY_PTR(pnts)[i]) < 2) {
            rb_raise(rb_eArgError, "point %d should be array of size 2", i);
        }
        points[i].x = NUM2INT(RARRAY_PTR(RARRAY_PTR(pnts)[i])[0]);
        points[i].y = NUM2INT(RARRAY_PTR(RARRAY_PTR(pnts)[i])[1]);
    }
    gdk_draw_polygon(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                     RVAL2CBOOL(filled),
                     points,
                     RARRAY_LEN(pnts));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
/*
  trapezoids = [[y1, x11, x21, y2, x12, x22], ...]
 */
static VALUE
gdkdraw_draw_trapezoids(self, gc, trapezoids)
    VALUE self, gc, trapezoids;
{
    GdkTrapezoid *gtrapezoids;
    gint i, len;

    Check_Type(trapezoids, T_ARRAY);

    len = RARRAY_LEN(trapezoids);

    gtrapezoids = ALLOCA_N(GdkTrapezoid, len);
    for (i = 0; i < len; i++) {
        Check_Type(RARRAY_PTR(trapezoids)[i], T_ARRAY);
        if (RARRAY_LEN(RARRAY_PTR(trapezoids)[i]) < 6) {
            rb_raise(rb_eArgError, "trapezoids %d should be array of size 6", i);
        }
        gtrapezoids[i].y1 = NUM2DBL(RARRAY_PTR(RARRAY_PTR(trapezoids)[i])[0]);
        gtrapezoids[i].x11 = NUM2DBL(RARRAY_PTR(RARRAY_PTR(trapezoids)[i])[1]);
        gtrapezoids[i].x21 = NUM2DBL(RARRAY_PTR(RARRAY_PTR(trapezoids)[i])[2]);
        gtrapezoids[i].y2 = NUM2DBL(RARRAY_PTR(RARRAY_PTR(trapezoids)[i])[3]);
        gtrapezoids[i].x12 = NUM2DBL(RARRAY_PTR(RARRAY_PTR(trapezoids)[i])[4]);
        gtrapezoids[i].x22 = NUM2DBL(RARRAY_PTR(RARRAY_PTR(trapezoids)[i])[5]);
    }
    gdk_draw_trapezoids(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), gtrapezoids, len);
    return self; 
}
#endif

static VALUE
gdkdraw_glyphs(self, gc, font, x, y, glyphs)
    VALUE self, gc, font, x, y, glyphs;
{
    gdk_draw_glyphs(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), PANGO_FONT(RVAL2GOBJ(font)),
                    NUM2INT(x), NUM2INT(y), 
                    (PangoGlyphString*)(RVAL2BOXED(glyphs, PANGO_TYPE_GLYPH_STRING)));
    return self;
}

#if GTK_CHECK_VERSION(2,6,0)
static VALUE
gdkdraw_glyphs_transformed(self, gc, matrix, font, x, y, glyphs)
    VALUE self, gc, matrix, font, x, y, glyphs;
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
gdkdraw_layout_line(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
gdkdraw_layout(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
gdkdraw_draw_drawable(self, gc, src, xsrc, ysrc, xdst, ydst, w, h)
    VALUE self, gc, src, xsrc, ysrc, xdst, ydst, w, h;
{
    gdk_draw_drawable(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), _SELF(src),
                      NUM2INT(xsrc), NUM2INT(ysrc),
                      NUM2INT(xdst), NUM2INT(ydst),
                      NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkdraw_draw_image(self, gc, image, xsrc, ysrc, xdst, ydst, w, h)
    VALUE self, gc, image, xsrc, ysrc, xdst, ydst, w, h;
{
    gdk_draw_image(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
                   GDK_IMAGE(RVAL2GOBJ(image)),
                   NUM2INT(xsrc), NUM2INT(ysrc),
                   NUM2INT(xdst), NUM2INT(ydst),
                   NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkdraw_get_image(self, x, y, w, h)
    VALUE self, x, y, w, h;
{
    return GOBJ2RVAL(gdk_drawable_get_image(_SELF(self), 
                                            NUM2INT(x), NUM2INT(y), 
                                            NUM2INT(w), NUM2INT(h)));
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
gdkdraw_copy_to_image(self, image, xsrc, ysrc, xdst, ydst, w, h)
    VALUE self, image, xsrc, ysrc, xdst, ydst, w, h;
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
gdkdraw_get_xid(self)
    VALUE self;
{
    return ULONG2NUM(GDK_DRAWABLE_XID(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkdraw_get_display(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_drawable_get_display(_SELF(self)));
}

static VALUE
gdkdraw_get_screen(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_drawable_get_screen(_SELF(self)));
}
#endif

#if GTK_CHECK_VERSION(2,8,0)
#  ifdef HAVE_RB_CAIRO_H
static VALUE
gdkdraw_cairo_create(self)
    VALUE self;
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
Init_gtk_gdk_draw()
{
    VALUE gdkDrawable = G_DEF_CLASS(GDK_TYPE_DRAWABLE, "Drawable", mGdk);

    rb_define_method(gdkDrawable, "visual", gdkdraw_get_visual, 0);
    rb_define_method(gdkDrawable, "set_colormap", gdkdraw_set_colormap, 1);
    rb_define_method(gdkDrawable, "colormap", gdkdraw_get_colormap, 0);
    rb_define_method(gdkDrawable, "depth", gdkdraw_get_depth, 0);
    rb_define_method(gdkDrawable, "size", gdkdraw_get_size, 0);
    rb_define_method(gdkDrawable, "clip_region", gdkdraw_get_clip_region, 0);
    rb_define_method(gdkDrawable, "visible_region", gdkdraw_get_visible_region, 0);
    rb_define_method(gdkDrawable, "draw_point", gdkdraw_draw_point, 3);
    rb_define_method(gdkDrawable, "draw_points", gdkdraw_draw_points, 2);
    rb_define_method(gdkDrawable, "draw_line", gdkdraw_draw_line, 5);
    rb_define_method(gdkDrawable, "draw_lines", gdkdraw_draw_lines, 2);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkDrawable, "draw_pixbuf", gdkdraw_draw_pixbuf, 11);
#endif
    rb_define_method(gdkDrawable, "draw_segments", gdkdraw_draw_segs, 2);
    rb_define_method(gdkDrawable, "draw_rectangle", gdkdraw_draw_rect, 6);
    rb_define_method(gdkDrawable, "draw_arc", gdkdraw_draw_arc, 8);
    rb_define_method(gdkDrawable, "draw_polygon", gdkdraw_draw_poly, 3);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gdkDrawable, "draw_trapezoids", gdkdraw_draw_trapezoids, 2);
#endif
    rb_define_method(gdkDrawable, "draw_glyphs", gdkdraw_glyphs, 5);
#if GTK_CHECK_VERSION(2,6,0)
    rb_define_method(gdkDrawable, "draw_glyphs_transformed", gdkdraw_glyphs_transformed, 6);
#endif
    rb_define_method(gdkDrawable, "draw_layout_line", gdkdraw_layout_line, -1);
    rb_define_method(gdkDrawable, "draw_layout", gdkdraw_layout, -1);
    rb_define_method(gdkDrawable, "draw_drawable", gdkdraw_draw_drawable, 8);
    rb_define_method(gdkDrawable, "draw_image", gdkdraw_draw_image, 8);
    rb_define_method(gdkDrawable, "get_image", gdkdraw_get_image, 4);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gdkDrawable, "copy_to_image", gdkdraw_copy_to_image, 7);
#endif

#ifdef GDK_WINDOWING_X11
    rb_define_method(gdkDrawable, "xid", gdkdraw_get_xid, 0);
#endif
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkDrawable, "display", gdkdraw_get_display, 0);
    rb_define_method(gdkDrawable, "screen", gdkdraw_get_screen, 0);
#endif
    
#if GTK_CHECK_VERSION(2,8,0)
#  ifdef HAVE_RB_CAIRO_H
    rb_define_method(gdkDrawable, "create_cairo_context",
                     gdkdraw_cairo_create, 0);
#  endif
#endif
    
#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkDrawableImplX11", "DrawableImplX11", mGdk);
#elif defined(GDK_WINDOWING_WIN32)
    G_DEF_CLASS3("GdkDrawableImplWin32", "DrawableImplWin32", mGdk);
#elif defined(GDK_WINDOWING_FB)
    G_DEF_CLASS3("GdkDrawableFB", "DrawableFB", mGdk);
#endif
    
    G_DEF_SETTERS(gdkDrawable);
}

