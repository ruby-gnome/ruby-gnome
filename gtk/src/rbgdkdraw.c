/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdraw.c -

  $Author: mutoh $
  $Date: 2003/01/12 18:09:10 $

  Copyright (C) 2002,2003 Masao Mutoh
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"
#include "rbpango.h"

#define _SELF(s) GDK_DRAWABLE(RVAL2GOBJ(s))

static VALUE
gdkdraw_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self));
}

static VALUE
gdkdraw_set_colormap(self, colormap)
    VALUE self, colormap;
{
    gdk_drawable_set_colormap(_SELF(self), GDK_COLORMAP(RVAL2GOBJ(self)));
    return self;
}

static VALUE
gdkdraw_get_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_drawable_get_colormap(_SELF(self)));
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
    points = ALLOCA_N(GdkPoint,RARRAY(pnts)->len);
    for (i=0; i<RARRAY(pnts)->len; i++) {
		Check_Type(RARRAY(pnts)->ptr[i], T_ARRAY);
		if (RARRAY(RARRAY(pnts)->ptr[i])->len < 2) {
			rb_raise(rb_eArgError, "point %d should be array of size 2", i);
		}
		points[i].x = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[0]);
		points[i].y = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[1]);
    }
    gdk_draw_points(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
					points,
					RARRAY(pnts)->len);
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
    points = ALLOCA_N(GdkPoint,RARRAY(pnts)->len);
    for (i=0; i<RARRAY(pnts)->len; i++) {
		Check_Type(RARRAY(pnts)->ptr[i], T_ARRAY);
		if (RARRAY(RARRAY(pnts)->ptr[i])->len < 2) {
			rb_raise(rb_eArgError, "point %d should be array of size 2", i);
		}
		points[i].x = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[0]);
		points[i].y = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[1]);
    }
    gdk_draw_lines(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
				   points,
				   RARRAY(pnts)->len);
    return self;
}

static VALUE
gdkdraw_draw_segs(self, gc, segs)
    VALUE self, gc, segs;
{
    GdkSegment *segments;
    int i;

    Check_Type(segs, T_ARRAY);
    segments = ALLOCA_N(GdkSegment,RARRAY(segs)->len);
    for (i=0; i<RARRAY(segs)->len; i++) {
		Check_Type(RARRAY(segs)->ptr[i], T_ARRAY);
		if (RARRAY(RARRAY(segs)->ptr[i])->len < 4) {
			rb_raise(rb_eArgError, "segment %d should be array of size 4", i);
		}
		segments[i].x1 = NUM2INT(RARRAY(RARRAY(segs)->ptr[i])->ptr[0]);
		segments[i].y1 = NUM2INT(RARRAY(RARRAY(segs)->ptr[i])->ptr[1]);
		segments[i].x2 = NUM2INT(RARRAY(RARRAY(segs)->ptr[i])->ptr[2]);
		segments[i].y2 = NUM2INT(RARRAY(RARRAY(segs)->ptr[i])->ptr[3]);
    }
    gdk_draw_segments(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
					  segments, RARRAY(segs)->len);
    return self;
}

static VALUE
gdkdraw_draw_rect(self, gc, filled, x, y, w, h)
    VALUE self, gc, filled, x, y, w, h;
{
    gdk_draw_rectangle(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
					   RTEST(filled),
					   NUM2INT(x), NUM2INT(y),
					   NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkdraw_draw_arc(self, gc, filled, x, y, w, h, a1, a2)
    VALUE self, gc, filled, x, y, w, h, a1, a2;
{
    gdk_draw_arc(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
				 RTEST(filled),
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
    points = ALLOCA_N(GdkPoint,RARRAY(pnts)->len);
    for (i=0; i<RARRAY(pnts)->len; i++) {
		Check_Type(RARRAY(pnts)->ptr[i], T_ARRAY);
		if (RARRAY(RARRAY(pnts)->ptr[i])->len < 2) {
			rb_raise(rb_eArgError, "point %d should be array of size 2", i);
		}
		points[i].x = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[0]);
		points[i].y = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[1]);
    }
    gdk_draw_polygon(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
					 RTEST(filled),
					 points,
					 RARRAY(pnts)->len);
    return self;
}

static VALUE
gdkdraw_glyphs(self, gc, font, x, y, glyphs)
    VALUE self, gc, font, x, y, glyphs;
{
    gdk_draw_glyphs(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), PANGO_FONT(RVAL2GOBJ(font)),
                    NUM2INT(x), NUM2INT(y), 
                    (PangoGlyphString*)(RVAL2BOXED(glyphs, PANGO_TYPE_GLYPH_STRING)));
    return self;
}

static VALUE
gdkdraw_layout_line(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    VALUE gc, x, y, line, fg, bg;

    rb_scan_args(argc, argv, "42", &gc, &x, &y, &line, &fg, &bg);

    if (argc == 4){
        gdk_draw_layout_line(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), 
                             NUM2INT(x), NUM2INT(y), 
                             (PangoLayoutLine*)RVAL2BOXED(line, PANGO_TYPE_LAYOUT_LINE));
    } else if (argc == 6){
        gdk_draw_layout_line_with_colors(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), 
                                         NUM2INT(x), NUM2INT(y), 
                                         (PangoLayoutLine*)RVAL2BOXED(line, PANGO_TYPE_LAYOUT_LINE),
                                         (GdkColor*)RVAL2BOXED(fg, GDK_TYPE_COLOR),
                                         (GdkColor*)RVAL2BOXED(bg, GDK_TYPE_COLOR));
    } else {
        rb_raise(rb_eArgError, "Wrong arguments");
    }

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

    if (argc == 4){
        gdk_draw_layout(_SELF(self), GDK_GC(RVAL2GOBJ(gc)), 
                        NUM2INT(x), NUM2INT(y), PANGO_LAYOUT(RVAL2GOBJ(layout)));
    } else if (argc == 6){
        gdk_draw_layout_with_colors( _SELF(self), GDK_GC(RVAL2GOBJ(gc)), 
                                     NUM2INT(x), NUM2INT(y), PANGO_LAYOUT(RVAL2GOBJ(layout)),
                                     (GdkColor*)RVAL2BOXED(fg, GDK_TYPE_COLOR),
                                     (GdkColor*)RVAL2BOXED(bg, GDK_TYPE_COLOR));
    } else {
        rb_raise(rb_eArgError, "Wrong arguments");
    }

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
    rb_define_method(gdkDrawable, "draw_segments", gdkdraw_draw_segs, 2);
    rb_define_method(gdkDrawable, "draw_rectangle", gdkdraw_draw_rect, 6);
    rb_define_method(gdkDrawable, "draw_arc", gdkdraw_draw_arc, 8);
    rb_define_method(gdkDrawable, "draw_polygon", gdkdraw_draw_poly, 3);
    rb_define_method(gdkDrawable, "draw_glyphs", gdkdraw_glyphs, 5);
    rb_define_method(gdkDrawable, "draw_layout_line", gdkdraw_layout_line, -1);
    rb_define_method(gdkDrawable, "draw_layout", gdkdraw_layout, -1);
    rb_define_method(gdkDrawable, "draw_drawable", gdkdraw_draw_drawable, 8);
    rb_define_method(gdkDrawable, "draw_image", gdkdraw_draw_image, 8);
    rb_define_method(gdkDrawable, "get_image", gdkdraw_get_image, 4);
    
    G_DEF_SETTERS(gdkDrawable);
}
