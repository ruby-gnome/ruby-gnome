/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkalignment.c -

  $Author: mutoh $
  $Date: 2002/06/24 15:15:59 $

  Copyright (C) 2001 Neil Conway
************************************************/

#include "global.h"

VALUE gdkGC;
VALUE gdkGCValues;

#define _SELF(s) GDK_GC(RVAL2GOBJ(s))

static VALUE
gdkgc_s_new(self, win)
	VALUE self, win;
{
	return GOBJ2RVAL(gdk_gc_new(GDK_DRAWABLE(RVAL2GOBJ(win))));
}

static VALUE
gdkgc_copy(self, copy)
	VALUE self, copy;
{
	gdk_gc_copy(_SELF(self), _SELF(copy));
	return copy;
}

static VALUE
gdkgc_destroy(self)
	VALUE self;
{
	gdk_gc_destroy(_SELF(self));
	DATA_PTR(self) = 0;
	return Qnil;
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
	return func;
}

static VALUE
gdkgc_set_foreground(self, color)
	VALUE self, color;
{
	gdk_gc_set_foreground(_SELF(self), get_gdkcolor(color));
	return self;
}

static VALUE
gdkgc_set_background(self, color)
	VALUE self, color;
{
	gdk_gc_set_background(_SELF(self), get_gdkcolor(color));
	return self;
}

static VALUE
gdkgc_set_clip_mask(self, mask)
	VALUE self, mask;
{
	gdk_gc_set_clip_mask(_SELF(self), GDK_BITMAP(RVAL2GOBJ(mask)));
	return mask;
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
	gdk_gc_set_clip_rectangle(_SELF(self), get_gdkrectangle(rectangle));
	return rectangle;
}

static VALUE
gdkgc_set_clip_region(self, region)
	VALUE self, region;
{
	gdk_gc_set_clip_region(_SELF(self), get_gdkregion(region));
	return region;
}

static VALUE
gdkgc_set_dashes(self, dash_offset, dash_list)
	VALUE self, dash_offset, dash_list;
{
	gchar *buf;
	int   i;

	if (rb_obj_is_instance_of(dash_list, rb_cString)) {
		gdk_gc_set_dashes(_SELF(self), NUM2INT(dash_offset),
						  RSTRING(dash_list)->ptr, RSTRING(dash_list)->len);
		rb_warn("Gdk::GC.set_dashes(dash_offset, dash_list): dash_list with String is obsoleted. Use dash_list with Array.");
	} else if (rb_obj_is_instance_of(dash_list, rb_cArray)) {
		buf = ALLOCA_N(gchar, RARRAY(dash_list)->len);
		for (i = 0; i < RARRAY(dash_list)->len; i++) {
			Check_Type(RARRAY(dash_list)->ptr[i], T_FIXNUM);
			buf[i] = (gchar)NUM2CHR(RARRAY(dash_list)->ptr[i]);
		}
		gdk_gc_set_dashes(_SELF(self), NUM2INT(dash_offset),
						  buf, RSTRING(dash_list)->len);
	} else {
		Check_Type(dash_list, T_ARRAY);
	}

	return self;
}

static VALUE
gdkgc_set_exposures(self, exposures)
	VALUE self, exposures;
{
	gdk_gc_set_exposures(_SELF(self), NUM2INT(exposures));
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
gdkgc_set_font(self, font)
	VALUE self, font;
{
	gdk_gc_set_font(_SELF(self), get_gdkfont(font));
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
    static RGObjClassInfo cinfo;

	gdkGC = rb_define_class_under(mGdk, "GC", rb_cData);
    cinfo.klass = gdkGC;
    cinfo.gtype = GDK_TYPE_GC;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

	rb_define_singleton_method(gdkGC, "new", gdkgc_s_new, 1);
	rb_define_method(gdkGC, "copy", gdkgc_copy, 1);
	rb_define_method(gdkGC, "destroy", gdkgc_destroy, 0);
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
	rb_define_method(gdkGC, "set_font", gdkgc_set_font, 1);
	rb_define_method(gdkGC, "set_line_attributes", gdkgc_set_line_attributes, 4);
	rb_define_method(gdkGC, "set_stipple", gdkgc_set_stipple, 1);
	rb_define_method(gdkGC, "set_subwindow", gdkgc_set_subwindow, 1);
	rb_define_method(gdkGC, "set_tile", gdkgc_set_tile, 1);
	rb_define_method(gdkGC, "set_ts_origin", gdkgc_set_ts_origin, 2);

	/*
	 * Gdk::GCValues
	 */
	gdkGCValues = rb_define_class_under(mGdk, "GCValues", rb_cData);
}
