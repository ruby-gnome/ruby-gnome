/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkwindow.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:11 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Gdk::Window
 */

VALUE gdkWindow;

static VALUE
gdkwin_get_size(self)
    VALUE self;
{
    int width, height;
    gdk_window_get_size(get_gdkwindow(self), &width, &height);
    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

static VALUE
gdkwin_get_pointer(self)
    VALUE self;
{
    int x, y;
    GdkModifierType state;
    gdk_window_get_pointer(get_gdkwindow(self), &x, &y, &state);
    return rb_ary_new3(3, INT2FIX(x), INT2FIX(y), INT2FIX((int)state));

}

static VALUE
gdkwin_pointer_grab(self, owner_events, event_mask, confine_to, cursor, time)
    VALUE self, owner_events, event_mask, confine_to, cursor, time;
{
    gdk_pointer_grab(get_gdkwindow(self),
		     RTEST(owner_events),
		     NUM2INT(event_mask),
		     get_gdkwindow(confine_to),
		     get_gdkcursor(cursor),
		     NUM2INT(time));
    return self;
}

static VALUE
gdkwin_pointer_ungrab(self, time)
    VALUE self, time;
{
    gdk_pointer_ungrab(NUM2INT(time));
    return self;
}

static VALUE
gdkwin_pointer_is_grabbed(self)
    VALUE self;
{
    return (gdk_pointer_is_grabbed() ? Qtrue : Qfalse);
}

static VALUE
gdkwin_keyboard_grab(self, owner_events, time)
    VALUE self, owner_events, time;
{
    gdk_keyboard_grab(get_gdkwindow(self),
		     RTEST(owner_events),
		     NUM2INT(time));
    return self;
}

static VALUE
gdkwin_keyboard_ungrab(self, time)
    VALUE self, time;
{
    gdk_keyboard_ungrab(NUM2INT(time));
    return self;
}

static VALUE
gdkwin_foreign_new(self, anid)
    VALUE self, anid;
{
    GdkWindow *window;
    window = gdk_window_foreign_new(NUM2INT(anid));
    return make_gdkwindow(window);
}

static VALUE
gdkwin_root_window(self)
    VALUE self;
{
    return INT2NUM(GDK_ROOT_WINDOW());
}

static VALUE
gdkwin_clear(self)
    VALUE self;
{
    gdk_window_clear(get_gdkwindow(self));
    return self;
}

static VALUE
gdkwin_clear_area(self, x,y,w,h)
    VALUE self,x,y,w,h;
{
    gdk_window_clear_area(get_gdkwindow(self),
			  NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_clear_area_e(self, x,y,w,h)
    VALUE self,x,y,w,h;
{
    gdk_window_clear_area_e(get_gdkwindow(self),
			    NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_set_background(self, color)
    VALUE self, color;
{
    gdk_window_set_background(get_gdkwindow(self), get_gdkcolor(color));
    return self;
}

static VALUE
gdkwin_set_back_pixmap(self, pixmap, parent_relative)
    VALUE self, pixmap, parent_relative;
{
    gdk_window_set_back_pixmap(get_gdkwindow(self), get_gdkpixmap(pixmap),
			       RTEST(parent_relative));
    return self;
}

static VALUE
gdkwin_move(self, x,y)
    VALUE self, x,y;
{
    gdk_window_move(get_gdkwindow(self),
		    NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkwin_raise(self)
    VALUE self;
{
    gdk_window_raise(get_gdkwindow(self));
    return self;
}

static VALUE
gdkwin_lower(self)
    VALUE self;
{
    gdk_window_lower(get_gdkwindow(self));
    return self;
}

static VALUE
gdkwin_resize(self, w,h)
    VALUE self, w,h;
{
    gdk_window_resize(get_gdkwindow(self),
		      NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_move_resize(self, x,y,w,h)
    VALUE self, x,y,w,h;
{
    gdk_window_move_resize(get_gdkwindow(self),
			   NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
gdkwin_set_cursor(self, cursor)
    VALUE self, cursor;
{
    gdk_window_set_cursor(get_gdkwindow(self), get_gdkcursor(cursor));
    return self;
}

static VALUE
gdkwin_set_icon(self, icon, pixmap, mask)
    VALUE self, icon, pixmap, mask;
{
    gdk_window_set_icon(get_gdkwindow(self), get_gdkwindow(icon),
			get_gdkpixmap(pixmap), get_gdkbitmap(mask));
    return self;
}

static VALUE
gdkwin_set_icon_name(self, name)
    VALUE self, name;
{
    gdk_window_set_icon_name(get_gdkwindow(self), STR2CSTR(name));
    return self;
}

static VALUE
gdkwin_set_decorations(self, decor)
    VALUE self, decor;
{
    gdk_window_set_decorations(get_gdkwindow(self), NUM2INT(decor));
    return self;
}

static VALUE
gdkwin_set_functions(self, func)
    VALUE self, func;
{
    gdk_window_set_functions(get_gdkwindow(self), NUM2INT(func));
    return self;
}

static VALUE
gdkwin_set_override_redirect(self, override_redirect)
    VALUE self, override_redirect;
{
    gdk_window_set_override_redirect(get_gdkwindow(self),
				     RTEST(override_redirect));
    return self;
}

static VALUE
gdkwin_get_root_origin(self)
    VALUE self;
{
    int x, y;
    gdk_window_get_root_origin(get_gdkwindow(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

static VALUE
gdkwin_get_events(self)
    VALUE self;
{
    return INT2NUM(gdk_window_get_events(get_gdkwindow(self)));
}

static VALUE
gdkwin_set_events(self, mask)
    VALUE self, mask;
{
    gdk_window_set_events(get_gdkwindow(self), NUM2INT(mask));
    return self;
}

static VALUE
gdkwin_reparent(self, new_parent, x, y)
    VALUE self, new_parent, x, y;
{
    gdk_window_reparent(get_gdkwindow(self), get_gdkwindow(new_parent),
			NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkwin_get_geometry(self)
    VALUE self;
{
    gint x, y, w, h, d;
    gdk_window_get_geometry(get_gdkwindow(self), &x, &y, &w, &h, &d);
    return rb_ary_new3(5, INT2NUM(x), INT2NUM(y),
		       INT2NUM(w), INT2NUM(h), INT2NUM(d));
}

static VALUE
gdkwin_show(self)
    VALUE self;
{
    gdk_window_show(get_gdkwindow(self));
    return Qnil;
}

static VALUE
gdkwin_hide(self)
    VALUE self;
{
    gdk_window_hide(get_gdkwindow(self));
    return Qnil;
}

VALUE gdkWindowAttr;

void
Init_gtk_gdk_window()
{
    /* Gdk::Window */

    gdkWindow = rb_define_class_under(mGdk, "Window", gdkDrawable);

    rb_define_method(gdkWindow, "get_size", gdkwin_get_size, 0);
    rb_define_method(gdkWindow, "get_pointer", gdkwin_get_pointer, 0);
    rb_define_method(gdkWindow, "pointer_grab", gdkwin_pointer_grab, 5);
    rb_define_method(gdkWindow, "pointer_ungrab", gdkwin_pointer_ungrab, 1);
    rb_define_method(gdkWindow, "pointer_is_grabbed?", gdkwin_pointer_is_grabbed, 0);
    rb_define_method(gdkWindow, "keyboard_grab", gdkwin_keyboard_grab, 2);
    rb_define_method(gdkWindow, "keyboard_ungrab", gdkwin_keyboard_ungrab, 1);
    rb_define_singleton_method(gdkWindow, "foreign_new", gdkwin_foreign_new, 1);
    rb_define_singleton_method(gdkWindow, "root_window", gdkwin_root_window, 0);
    rb_define_method(gdkWindow, "clear", gdkwin_clear, 0);
    rb_define_method(gdkWindow, "clear_area", gdkwin_clear_area, 4);
    rb_define_method(gdkWindow, "clear_area_e", gdkwin_clear_area_e, 4);
    rb_define_method(gdkWindow, "set_background", gdkwin_set_background, 1);
    rb_define_method(gdkWindow, "set_back_pixmap", gdkwin_set_back_pixmap, 2);
    rb_define_method(gdkWindow, "move", gdkwin_move, 2);
    rb_define_method(gdkWindow, "raise", gdkwin_raise, 0);
    rb_define_method(gdkWindow, "lower", gdkwin_lower, 0);
    rb_define_method(gdkWindow, "resize", gdkwin_resize, 2);
    rb_define_method(gdkWindow, "move_resize", gdkwin_move_resize, 4);
    rb_define_method(gdkWindow, "set_cursor", gdkwin_set_cursor, 1);
    rb_define_method(gdkWindow, "set_icon", gdkwin_set_icon, 3);
    rb_define_method(gdkWindow, "set_icon_name", gdkwin_set_icon_name, 1);
    rb_define_method(gdkWindow, "set_decorations", gdkwin_set_decorations, 1);
    rb_define_method(gdkWindow, "set_functions", gdkwin_set_functions, 1);
    rb_define_method(gdkWindow, "set_override_redirect", gdkwin_set_override_redirect, 1);
    rb_define_method(gdkWindow, "get_root_origin", gdkwin_get_root_origin, 0);
    rb_define_method(gdkWindow, "get_events", gdkwin_get_events, 0);
    rb_define_method(gdkWindow, "set_events", gdkwin_set_events, 1);
    rb_define_method(gdkWindow, "reparent", gdkwin_reparent, 3);
    rb_define_method(gdkWindow, "show", gdkwin_show, 0);
    rb_define_method(gdkWindow, "hide", gdkwin_hide, 0);
    rb_define_method(gdkWindow, "get_geometry", gdkwin_get_geometry, 0);

    /* GdkWindowHints */
    rb_define_const(gdkWindow, "HINT_POS", INT2FIX(GDK_HINT_POS));
    rb_define_const(gdkWindow, "HINT_MIN_SIZE", INT2FIX(GDK_HINT_MIN_SIZE));
    rb_define_const(gdkWindow, "HINT_MAX_SIZE", INT2FIX(GDK_HINT_MAX_SIZE));
    rb_define_const(gdkWindow, "HINT_BASE_SIZE", INT2FIX(GDK_HINT_BASE_SIZE));
    rb_define_const(gdkWindow, "HINT_ASPECT", INT2FIX(GDK_HINT_ASPECT));
    rb_define_const(gdkWindow, "HINT_RESIZE_INC", INT2FIX(GDK_HINT_RESIZE_INC));

    /* Gdk::WindowAttr */

    gdkWindowAttr = rb_define_class_under(mGdk, "WindowAttr", rb_cData);
}

/*
 * Gdk::Geometry
 */

VALUE gdkGeometry;

GdkGeometry *
rbgdk_geometry_get(geo)
    VALUE geo;
{
    GdkGeometry *ret;

    Data_Get_Struct(geo, GdkGeometry, ret);
    return ret;
}

VALUE
rbgdk_geometry_make(geo)
    GdkGeometry *geo;
{
    VALUE obj;

    obj = Data_Wrap_Struct(gdkGeometry, 0, g_free, geo);

    return obj;
}

static VALUE
geo_s_new(self)
    VALUE self;
{
    VALUE obj;
    GdkGeometry *geo;

    geo = g_new0(GdkGeometry, 1);
    obj = rbgdk_geometry_make(geo);

    return obj;
}

static VALUE
geo_min_width(self)
    VALUE self;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->min_width);
}

static VALUE
geo_min_height(self)
    VALUE self;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->min_height);
}

static VALUE
geo_max_width(self)
    VALUE self;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->max_width);
}

static VALUE
geo_max_height(self)
    VALUE self;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->max_height);
}

static VALUE
geo_base_width(self)
    VALUE self;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->base_width);
}

static VALUE
geo_base_height(self)
    VALUE self;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->base_height);
}

static VALUE
geo_width_inc(self)
    VALUE self;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->width_inc);
}

static VALUE
geo_height_inc(self)
    VALUE self;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    return INT2NUM(geo->height_inc);
}

static VALUE
geo_min_aspect(self)
    VALUE self;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    return rb_float_new(geo->min_aspect);
}

static VALUE
geo_max_aspect(self)
    VALUE self;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    return rb_float_new(geo->max_aspect);
}

static VALUE
geo_set(self, min_width, min_height, max_width,	max_height,
	base_width, base_height, width_inc, height_inc,	min_aspect, max_aspect)
    VALUE self,
    min_width, min_height, max_width, max_height,
    base_width, base_height, width_inc, height_inc,
    min_aspect, max_aspect;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    geo->min_width = NUM2INT(min_width);
    geo->min_height = NUM2INT(min_height);
    geo->max_width = NUM2INT(max_width);
    geo->max_height = NUM2INT(max_height);
    geo->base_width = NUM2INT(base_width);
    geo->base_height = NUM2INT(base_height);
    geo->width_inc = NUM2INT(width_inc);
    geo->height_inc = NUM2INT(height_inc);
    geo->min_aspect = NUM2DBL(min_aspect);
    geo->max_aspect = NUM2DBL(max_aspect);
    return self;
}

static VALUE
geo_set_min_width(self, min_width)
    VALUE self, min_width;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    geo->min_width = NUM2INT(min_width);
    return self;
}

static VALUE
geo_set_min_height(self, min_height)
    VALUE self, min_height;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    geo->min_height = NUM2INT(min_height);
    return self;
}

static VALUE
geo_set_max_width(self, max_width)
    VALUE self, max_width;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    geo->max_width = NUM2INT(max_width);
    return self;
}

static VALUE
geo_set_max_height(self, max_height)
    VALUE self, max_height;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    geo->max_height = NUM2INT(max_height);
    return self;
}

static VALUE
geo_set_base_width(self, base_width)
    VALUE self, base_width;
{
    GdkGeometry *geo;
    
    geo = rbgdk_geometry_get(self);
    geo->base_width = NUM2INT(base_width);
    return self;
}

static VALUE
geo_set_base_height(self, base_height)
    VALUE self, base_height;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    geo->base_height = NUM2INT(base_height);
    return self;
}

static VALUE
geo_set_width_inc(self, width_inc)
    VALUE self, width_inc;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    geo->width_inc = NUM2INT(width_inc);
    return self;
}

static VALUE
geo_set_height_inc(self, height_inc)
    VALUE self, height_inc;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    geo->height_inc = NUM2INT(height_inc);
    return self;
}

static VALUE
geo_set_min_aspect(self, min_aspect)
    VALUE self, min_aspect;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    geo->min_aspect = NUM2DBL(min_aspect);
    return self;
}

static VALUE
geo_set_max_aspect(self, max_aspect)
    VALUE self, max_aspect;
{
    GdkGeometry *geo;

    geo = rbgdk_geometry_get(self);
    geo->max_aspect = NUM2DBL(max_aspect);
    return self;
}

void Init_gtk_gdk_geometry()
{
    gdkGeometry = rb_define_class_under(mGdk, "Geometry", rb_cData);

    rb_define_singleton_method(gdkGeometry, "new", geo_s_new, 0);

    rb_define_method(gdkGeometry, "min_width", geo_min_width, 0);
    rb_define_method(gdkGeometry, "min_height", geo_min_height, 0);
    rb_define_method(gdkGeometry, "max_width", geo_max_width, 0);
    rb_define_method(gdkGeometry, "max_height", geo_max_height, 0);
    rb_define_method(gdkGeometry, "base_width", geo_base_width, 0);
    rb_define_method(gdkGeometry, "base_height", geo_base_height, 0);
    rb_define_method(gdkGeometry, "width_inc", geo_width_inc, 0);
    rb_define_method(gdkGeometry, "height_inc", geo_height_inc, 0);
    rb_define_method(gdkGeometry, "min_aspect", geo_min_aspect, 0);
    rb_define_method(gdkGeometry, "max_aspect", geo_max_aspect, 0);

    /* once ruby gets parameterized arguments feature, setters should
     * be changed. 2000-040-26 yashi */
    rb_define_method(gdkGeometry, "set", geo_set, 10);

    rb_define_method(gdkGeometry, "set_min_width", geo_set_min_width, 1);
    rb_define_method(gdkGeometry, "set_min_height", geo_set_min_height, 1);
    rb_define_method(gdkGeometry, "set_max_width", geo_set_max_width, 1);
    rb_define_method(gdkGeometry, "set_max_height", geo_set_max_height, 1);
    rb_define_method(gdkGeometry, "set_base_width", geo_set_base_width, 1);
    rb_define_method(gdkGeometry, "set_base_height", geo_set_base_height, 1);
    rb_define_method(gdkGeometry, "set_width_inc", geo_set_width_inc, 1);
    rb_define_method(gdkGeometry, "set_height_inc", geo_set_height_inc, 1);
    rb_define_method(gdkGeometry, "set_min_aspect", geo_set_min_aspect, 1);
    rb_define_method(gdkGeometry, "set_max_aspect", geo_set_max_aspect, 1);

}
