/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkdraw.c -

  $Author: mutoh $
  $Date: 2002/09/14 15:43:40 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"


#define _SELF(s) GDK_DRAWABLE(RVAL2GOBJ(s))

static VALUE
gdkdraw_draw_point(self, gc, x, y)
    VALUE self, gc, x, y;
{
    gdk_draw_point(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
				   NUM2INT(x), NUM2INT(y));
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

#ifndef GTK_DISABLE_DEPRECATED
static VALUE
gdkdraw_draw_text(self, font, gc, x, y, str)
    VALUE self, font, gc, x, y, str;
{
    Check_Type(str, T_STRING);
    gdk_draw_text(_SELF(self), (GdkFont*)RVAL2BOXED(font), 
                  GDK_GC(RVAL2GOBJ(gc)),
				  NUM2INT(x), NUM2INT(y),
				  RSTRING(str)->ptr, RSTRING(str)->len);
    return self;
}

static VALUE
gdkdraw_draw_pmap(self, gc, src, xsrc, ysrc, xdst, ydst, w, h)
    VALUE self, gc, src, xsrc, ysrc, xdst, ydst, w, h;
{
    gdk_draw_pixmap(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
					_SELF(src),
					NUM2INT(xsrc), NUM2INT(ysrc),
					NUM2INT(xdst), NUM2INT(ydst),
					NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkdraw_draw_bmap(self, gc, src, xsrc, ysrc, xdst, ydst, w, h)
    VALUE self, gc, src, xsrc, ysrc, xdst, ydst, w, h;
{
    /* why there's no gdk_draw_bitmap()?? */
    gdk_draw_pixmap(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
					_SELF(src),
					NUM2INT(xsrc), NUM2INT(ysrc),
					NUM2INT(xdst), NUM2INT(ydst),
					NUM2INT(w), NUM2INT(h));
    return self;
}
#endif

static VALUE
gdkdraw_draw_rgb_image(self, gc, x, y, w, h, dither, buf, rowstride)
    VALUE self, gc, x, y, w, h, dither, buf, rowstride;
{
    gdk_draw_rgb_image(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
					   NUM2INT(x), NUM2INT(y),
					   NUM2INT(w), NUM2INT(h),
					   NUM2INT(dither),
					   RVAL2CSTR(buf),
					   NUM2INT(rowstride));
    return self;
}

static VALUE
gdkdraw_draw_rgb_image_dithalign(self, gc, x, y, w, h, dither, buf, rowstride, 
								 xdith, ydith)
    VALUE self, gc, x, y, w, h, dither, buf, rowstride, xdith, ydith;
{
    gdk_draw_rgb_image_dithalign(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
								 NUM2INT(x), NUM2INT(y),
								 NUM2INT(w), NUM2INT(h),
								 NUM2INT(dither),
								 RVAL2CSTR(buf),
								 NUM2INT(rowstride),
								 NUM2INT(xdith), NUM2INT(ydith));
    return self;
}

static VALUE
gdkdraw_draw_gray_image(self, gc, x, y, w, h, dither, buf, rowstride)
    VALUE self, gc, x, y, w, h, dither, buf, rowstride;
{
    gdk_draw_gray_image(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
						NUM2INT(x), NUM2INT(y),
						NUM2INT(w), NUM2INT(h),
						NUM2INT(dither),
						RVAL2CSTR(buf),
						NUM2INT(rowstride));
    return self;
}

static VALUE
gdkdraw_draw_rgb_32_image(self, gc, x, y, w, h, dither, buf, rowstride)
    VALUE self, gc, x, y, w, h, dither, buf, rowstride;
{
    gdk_draw_rgb_32_image(_SELF(self), GDK_GC(RVAL2GOBJ(gc)),
						  NUM2INT(x), NUM2INT(y),
						  NUM2INT(w), NUM2INT(h),
						  NUM2INT(dither),
						  RVAL2CSTR(buf),
						  NUM2INT(rowstride));
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
gdkdraw_draw_pnts(self, gc, pnts)
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
gdkdraw_get_size(self)
    VALUE self;
{
    gint width, height;
    gdk_drawable_get_size(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
gdkdraw_get_depth(self)
    VALUE self;
{
    return INT2NUM(gdk_drawable_get_depth(_SELF(self)));
}

void
Init_gtk_gdk_draw()
{
    VALUE gdkDrawable = G_DEF_CLASS(GDK_TYPE_DRAWABLE, "Drawable", mGdk);

    /* instance methods */
    rb_define_method(gdkDrawable, "draw_point", gdkdraw_draw_point, 3);
    rb_define_method(gdkDrawable, "draw_line", gdkdraw_draw_line, 5);
    rb_define_method(gdkDrawable, "draw_rectangle", gdkdraw_draw_rect, 6);
    rb_define_method(gdkDrawable, "draw_arc", gdkdraw_draw_arc, 8);
    rb_define_method(gdkDrawable, "draw_polygon", gdkdraw_draw_poly, 3);
#ifndef GTK_DISABLE_DEPRECATED
    rb_define_method(gdkDrawable, "draw_string", gdkdraw_draw_text, 5);
    rb_define_method(gdkDrawable, "draw_text", gdkdraw_draw_text, 5);
    rb_define_method(gdkDrawable, "draw_pixmap", gdkdraw_draw_pmap, 8);
    rb_define_method(gdkDrawable, "draw_bitmap", gdkdraw_draw_bmap, 8);
#endif
    rb_define_method(gdkDrawable, "draw_rgb_image", gdkdraw_draw_rgb_image, 8);
    rb_define_method(gdkDrawable, "draw_rgb_image_dithalign", gdkdraw_draw_rgb_image_dithalign, 10);
    rb_define_method(gdkDrawable, "draw_gray_image", gdkdraw_draw_gray_image, 8);
    rb_define_method(gdkDrawable, "draw_rgb_32_image", gdkdraw_draw_rgb_32_image, 8);
    rb_define_method(gdkDrawable, "draw_image", gdkdraw_draw_image, 8);
    rb_define_method(gdkDrawable, "draw_points", gdkdraw_draw_pnts, 2);
    rb_define_method(gdkDrawable, "draw_segments", gdkdraw_draw_segs, 2);
    rb_define_method(gdkDrawable, "draw_lines", gdkdraw_draw_lines, 2);
    rb_define_method(gdkDrawable, "size", gdkdraw_get_size, 0);
    rb_define_method(gdkDrawable, "depth", gdkdraw_get_depth, 0);
}
