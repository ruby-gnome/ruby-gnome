/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkalignment.c -

  $Author: mutoh $
  $Date: 2002/10/15 15:41:59 $

  Copyright (C) 2001 Neil Conway
************************************************/

#include "global.h"

#define _SELF(s) ((GdkGC*)RVAL2GOBJ(s))

static VALUE
gdkgc_initialize(self, win)
    VALUE self, win;
{
    G_INITIALIZE(self, gdk_gc_new(GDK_DRAWABLE(RVAL2GOBJ(win))));
    return Qnil;
}

static VALUE
gdkgc_copy(self)
	VALUE self;
{
    GdkGC copy;
	gdk_gc_copy(&copy, _SELF(self));
	return GOBJ2RVAL(&copy);
}

static VALUE
gdkgc_set_function(self, func)
	VALUE self, func;
{
	GdkFunction f;
	f = (GdkFunction) NUM2INT(func);
	if (f > GDK_SET)
		rb_raise(rb_eArgError, "function out of range");
  
	gdk_gc_set_function(_SELF(self), f);
	return self;
}

static VALUE
gdkgc_set_foreground(self, color)
	VALUE self, color;
{
	gdk_gc_set_foreground(_SELF(self), 
                          (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
	return self;
}

static VALUE
gdkgc_set_background(self, color)
	VALUE self, color;
{
	gdk_gc_set_background(_SELF(self), 
                          (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
	return self;
}

static VALUE
gdkgc_set_clip_mask(self, mask)
	VALUE self, mask;
{
	gdk_gc_set_clip_mask(_SELF(self), GDK_BITMAP(RVAL2GOBJ(mask)));
	return self;
}

static VALUE
gdkgc_set_clip_origin(self, x, y)
	VALUE self, x, y;
{
	gdk_gc_set_clip_origin(_SELF(self), NUM2INT(x), NUM2INT(y));
	return self;
}

static VALUE
gdkgc_set_clip_rectangle(self, rectangle)
	VALUE self, rectangle;
{
	gdk_gc_set_clip_rectangle(_SELF(self), 
                              (GdkRectangle*)RVAL2BOXED(rectangle, GDK_TYPE_COLOR));
	return self;
}

static VALUE
gdkgc_set_clip_region(self, region)
	VALUE self, region;
{
	gdk_gc_set_clip_region(_SELF(self), 
                           (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION));
	return self;
}

static VALUE
gdkgc_set_dashes(self, dash_offset, dash_list)
	VALUE self, dash_offset, dash_list;
{
	gchar *buf;
	int   i;

    Check_Type(dash_list, T_ARRAY);

    buf = ALLOCA_N(gchar, RARRAY(dash_list)->len);
    for (i = 0; i < RARRAY(dash_list)->len; i++) {
        Check_Type(RARRAY(dash_list)->ptr[i], T_FIXNUM);
        buf[i] = (gchar)NUM2CHR(RARRAY(dash_list)->ptr[i]);
    }
    gdk_gc_set_dashes(_SELF(self), NUM2INT(dash_offset),
                      buf, RSTRING(dash_list)->len);

	return self;
}

static VALUE
gdkgc_set_exposures(self, exposures)
	VALUE self, exposures;
{
	gdk_gc_set_exposures(_SELF(self), RTEST(exposures));
	return self;
}

static VALUE
gdkgc_set_fill(self, fill)
	VALUE self, fill;
{
	gdk_gc_set_fill(_SELF(self), NUM2INT(fill));
	return self;
}

static VALUE
gdkgc_set_line_attributes(self, line_width, line_style, cap_style, join_style)
	VALUE self, line_width, line_style, cap_style, join_style;
{
	gdk_gc_set_line_attributes(_SELF(self), NUM2INT(line_width),
							   NUM2INT(line_style), NUM2INT(cap_style),
							   NUM2INT(join_style));
	return self;
}

static VALUE
gdkgc_set_stipple(self, stipple)
	VALUE self, stipple;
{
	gdk_gc_set_stipple(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(stipple))); 
	return self;
}

static VALUE
gdkgc_set_subwindow(self, mode)
	VALUE self, mode;
{
	gdk_gc_set_subwindow(_SELF(self), NUM2INT(mode));
	return self;
}

static VALUE
gdkgc_set_tile(self, tile)
	VALUE self, tile;
{
	gdk_gc_set_tile(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(tile))); 
	return self;
}

static VALUE
gdkgc_set_ts_origin(self, x, y)
	VALUE self, x, y;
{
	gdk_gc_set_ts_origin(_SELF(self), NUM2INT(x), NUM2INT(y)); 
	return self;
}

void
Init_gtk_gdk_gc()
{
    VALUE gdkGC = G_DEF_CLASS(GDK_TYPE_GC, "GC", mGdk);

	rb_define_method(gdkGC, "initialize", gdkgc_initialize, 1);
	rb_define_method(gdkGC, "copy", gdkgc_copy, 0);
	rb_define_method(gdkGC, "set_function", gdkgc_set_function, 1);
	rb_define_method(gdkGC, "set_foreground", gdkgc_set_foreground, 1);
	rb_define_method(gdkGC, "set_background", gdkgc_set_background, 1);
	rb_define_method(gdkGC, "set_clip_mask", gdkgc_set_clip_mask, 1);
	rb_define_method(gdkGC, "set_clip_origin", gdkgc_set_clip_origin, 2);
	rb_define_method(gdkGC, "set_clip_rectangle", gdkgc_set_clip_rectangle, 1);
	rb_define_method(gdkGC, "set_clip_region", gdkgc_set_clip_region, 1);
	rb_define_method(gdkGC, "set_dashes", gdkgc_set_dashes, 2);
	rb_define_method(gdkGC, "set_exposures", gdkgc_set_exposures, 1);
	rb_define_method(gdkGC, "set_fill", gdkgc_set_fill, 1);
	rb_define_method(gdkGC, "set_line_attributes", gdkgc_set_line_attributes, 4);
	rb_define_method(gdkGC, "set_stipple", gdkgc_set_stipple, 1);
	rb_define_method(gdkGC, "set_subwindow", gdkgc_set_subwindow, 1);
	rb_define_method(gdkGC, "set_tile", gdkgc_set_tile, 1);
	rb_define_method(gdkGC, "set_ts_origin", gdkgc_set_ts_origin, 2);

    G_DEF_SETTERS(gdkGC);

    /* GdkGCValuesMask */
    rb_define_const(gdkGC, "FOREGROUND", INT2FIX(GDK_GC_FOREGROUND));
    rb_define_const(gdkGC, "BACKGROUND", INT2FIX(GDK_GC_BACKGROUND));
    rb_define_const(gdkGC, "FONT", INT2FIX(GDK_GC_FONT));
    rb_define_const(gdkGC, "FUNCTION", INT2FIX(GDK_GC_FUNCTION));
    rb_define_const(gdkGC, "FILL", INT2FIX(GDK_GC_FILL));
    rb_define_const(gdkGC, "TILE", INT2FIX(GDK_GC_TILE));
    rb_define_const(gdkGC, "STIPPLE", INT2FIX(GDK_GC_STIPPLE));
    rb_define_const(gdkGC, "CLIP_MASK", INT2FIX(GDK_GC_CLIP_MASK));
    rb_define_const(gdkGC, "SUBWINDOW", INT2FIX(GDK_GC_SUBWINDOW));
    rb_define_const(gdkGC, "TS_X_ORIGIN", INT2FIX(GDK_GC_TS_X_ORIGIN));
    rb_define_const(gdkGC, "TS_Y_ORIGIN", INT2FIX(GDK_GC_TS_Y_ORIGIN));
    rb_define_const(gdkGC, "CLIP_X_ORIGIN", INT2FIX(GDK_GC_CLIP_X_ORIGIN));
    rb_define_const(gdkGC, "CLIP_Y_ORIGIN", INT2FIX(GDK_GC_CLIP_Y_ORIGIN));
    rb_define_const(gdkGC, "EXPOSURES", INT2FIX(GDK_GC_EXPOSURES));
    rb_define_const(gdkGC, "LINE_WIDTH", INT2FIX(GDK_GC_LINE_WIDTH));
    rb_define_const(gdkGC, "LINE_STYLE", INT2FIX(GDK_GC_LINE_STYLE));
    rb_define_const(gdkGC, "CAP_STYLE", INT2FIX(GDK_GC_CAP_STYLE));
    rb_define_const(gdkGC, "JOIN_STYLE", INT2FIX(GDK_GC_JOIN_STYLE));

    /* GdkFunction */
    rb_define_const(gdkGC, "COPY", INT2FIX(GDK_COPY));
    rb_define_const(gdkGC, "INVERT", INT2FIX(GDK_INVERT));
    rb_define_const(gdkGC, "XOR", INT2FIX(GDK_XOR));
    rb_define_const(gdkGC, "CLEAR", INT2FIX(GDK_CLEAR));
    rb_define_const(gdkGC, "AND", INT2FIX(GDK_AND));
    rb_define_const(gdkGC, "AND_REVERSE", INT2FIX(GDK_AND_REVERSE));
    rb_define_const(gdkGC, "AND_INVERT", INT2FIX(GDK_AND_INVERT));
    rb_define_const(gdkGC, "NOOP", INT2FIX(GDK_NOOP));
    rb_define_const(gdkGC, "OR", INT2FIX(GDK_OR));
    rb_define_const(gdkGC, "EQUIV", INT2FIX(GDK_EQUIV));
    rb_define_const(gdkGC, "OR_REVERSE", INT2FIX(GDK_OR_REVERSE));
    rb_define_const(gdkGC, "COPY_INVERT", INT2FIX(GDK_COPY_INVERT));
    rb_define_const(gdkGC, "OR_INVERT", INT2FIX(GDK_OR_INVERT));
    rb_define_const(gdkGC, "NAND", INT2FIX(GDK_NAND));
    rb_define_const(gdkGC, "NOR", INT2FIX(GDK_NOR));
    rb_define_const(gdkGC, "SET", INT2FIX(GDK_SET));

    /* GdkFill */
    rb_define_const(gdkGC, "SOLID", INT2FIX(GDK_SOLID));
    rb_define_const(gdkGC, "TILED", INT2FIX(GDK_TILED));
    rb_define_const(gdkGC, "STIPPLED", INT2FIX(GDK_STIPPLED));
    rb_define_const(gdkGC, "OPAQUE_STIPPLED", INT2FIX(GDK_OPAQUE_STIPPLED));

    /* GdkSubwindowMode */
    rb_define_const(gdkGC, "CLIP_BY_CHILDREN", INT2FIX(GDK_CLIP_BY_CHILDREN));
    rb_define_const(gdkGC, "INCLUDE_INFERIORS", INT2FIX(GDK_INCLUDE_INFERIORS));

    /* GdkLineStyle */
    rb_define_const(gdkGC, "LINE_SOLID", INT2FIX(GDK_LINE_SOLID));
    rb_define_const(gdkGC, "LINE_ON_OFF_DASH", INT2FIX(GDK_LINE_ON_OFF_DASH));
    rb_define_const(gdkGC, "LINE_DOUBLE_DASH", INT2FIX(GDK_LINE_DOUBLE_DASH));

    /* GdkCapStyle */
    rb_define_const(gdkGC, "CAP_NOT_LAST", INT2FIX(GDK_CAP_NOT_LAST));
    rb_define_const(gdkGC, "CAP_BUTT", INT2FIX(GDK_CAP_BUTT));
    rb_define_const(gdkGC, "CAP_ROUND", INT2FIX(GDK_CAP_ROUND));
    rb_define_const(gdkGC, "CAP_PROJECTING", INT2FIX(GDK_CAP_PROJECTING));

    /* GdkJoinStyle */
    rb_define_const(gdkGC, "JOIN_MITER", INT2FIX(GDK_JOIN_MITER));
    rb_define_const(gdkGC, "JOIN_ROUND", INT2FIX(GDK_JOIN_ROUND));
    rb_define_const(gdkGC, "JOIN_BEVEL", INT2FIX(GDK_JOIN_BEVEL));

}
