/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkim.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:29:32 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#ifdef USE_XIM

VALUE mGdkIM;
VALUE gdkIC;
VALUE gdkICAttr;

static VALUE
gdkim_m_begin(self, ic, window)
     VALUE self, ic, window;
{
    gdk_im_begin(get_gdkic(ic), get_gdkwindow(window));
    return Qnil;
}

static VALUE
gdkim_m_end(self)
     VALUE self;
{
    gdk_im_end();
    return Qnil;
}

static VALUE
gdkim_m_ready(self)
     VALUE self;
{
    return gdk_im_ready() ? Qtrue : Qfalse;
}

static VALUE
gdkim_m_decide_style(self, style)
     VALUE self, style;
{
	return INT2FIX(gdk_im_decide_style(NUM2INT(style)));
}

static VALUE
gdkim_m_set_best_style(self, style)
     VALUE self, style;
{
    return INT2FIX(gdk_im_set_best_style(NUM2INT(style)));
}


/*
 * Gdk::IC
 */
VALUE
make_gdkic(ic)
    GdkIC *ic;
{
    return Data_Wrap_Struct(gdkIC, 0, 0, ic);
}

static VALUE
gdkic_s_new(klass, attr, mask)
     VALUE klass, attr, mask;
{
    GdkIC *ic = gdk_ic_new(get_gdkicattr(attr), NUM2INT(mask));
    return make_gdkic(ic);
}

static VALUE
gdkic_get_events(self)
     VALUE self;
{
    gint mask = gdk_ic_get_events(get_gdkic(self));
    return INT2FIX(mask);
}

static VALUE
gdkic_get_style(self)
     VALUE self;
{
    gint st = gdk_ic_get_style(get_gdkic(self));
    return INT2FIX(st);
}

static VALUE
gdkic_get_attr(self, mask)
     VALUE self, mask;
{
    GdkICAttr *attr = gdk_ic_attr_new();

    gdk_ic_get_attr(get_gdkic(self), attr, NUM2INT(mask));
    return make_gdkicattr(attr);
}

static VALUE
gdkic_set_attr(self, attr, mask)
     VALUE self, attr, mask;
{
    gint error = gdk_ic_set_attr(get_gdkic(self),
				 get_gdkicattr(attr),
				 NUM2INT(mask));
    return INT2FIX(error);
}


/*
 * Gdk::ICAttr
 */
VALUE
make_gdkicattr(attr)
    GdkICAttr *attr;
{
    return Data_Wrap_Struct(gdkICAttr, 0, gdk_ic_attr_destroy, attr);
}

static VALUE
gdkicattr_s_new(klass)
     VALUE klass;
{
    GdkICAttr *attr = gdk_ic_attr_new();
    return make_gdkicattr(attr);
}

static VALUE
gdkicattr_style(self)
     VALUE self;
{
    return INT2NUM(get_gdkicattr(self)->style);
}

static VALUE
gdkicattr_set_style(self, style)
     VALUE self, style;
{
    get_gdkicattr(self)->style = NUM2INT(style);
    return self;
}

static VALUE
gdkicattr_client_window(self)
     VALUE self;
{
    GdkWindow *win = get_gdkicattr(self)->client_window;
    return make_gdkwindow(win);
}

static VALUE
gdkicattr_set_client_window(self, win)
     VALUE self, win;
{
    get_gdkicattr(self)->client_window = get_gdkwindow(win);
    return self;
}

static VALUE
gdkicattr_focus_window(self)
     VALUE self;
{
    GdkWindow *win = get_gdkicattr(self)->focus_window;
    return make_gdkwindow(win);
}

static VALUE
gdkicattr_set_focus_window(self, win)
     VALUE self, win;
{
    get_gdkicattr(self)->focus_window = get_gdkwindow(win);
    return self;
}

static VALUE
gdkicattr_spot_location(self)
     VALUE self;
{
    GdkPoint *point = &get_gdkicattr(self)->spot_location;
    return Data_Wrap_Struct(gdkPoint, 0, 0, point);
}

static VALUE
gdkicattr_set_spot_location(self, point)
     VALUE self, point;
{
    get_gdkicattr(self)->spot_location = *get_gdkpoint(point);
    return self;
}

static VALUE
gdkicattr_preedit_fontset(self)
     VALUE self;
{
    GdkFont *font = get_gdkicattr(self)->preedit_fontset;
    return make_gdkfont(font);
}

static VALUE
gdkicattr_set_preedit_fontset(self, font)
     VALUE self, font;
{
    get_gdkicattr(self)->preedit_fontset = get_gdkfont(font);
    return self;
}

static VALUE
gdkicattr_preedit_area(self)
     VALUE self;
{
    GdkRectangle *rect = &get_gdkicattr(self)->preedit_area;
    return Data_Wrap_Struct(gdkRectangle, 0, 0, rect);
}

static VALUE
gdkicattr_set_preedit_area(self, rect)
     VALUE self, rect;
{
    get_gdkicattr(self)->preedit_area = *get_gdkrectangle(rect);
    return self;
}

static VALUE
gdkicattr_preedit_area_needed(self)
     VALUE self;
{
    GdkRectangle *rect = &get_gdkicattr(self)->preedit_area_needed;
    return Data_Wrap_Struct(gdkRectangle, 0, 0, rect);
}

static VALUE
gdkicattr_preedit_cmap(self)
     VALUE self;
{
    GdkColormap *cmap = get_gdkicattr(self)->preedit_colormap;
    return make_gdkcmap(cmap);
}

static VALUE
gdkicattr_set_preedit_cmap(self, cmap)
     VALUE self, cmap;
{
    get_gdkicattr(self)->preedit_colormap = get_gdkcmap(cmap);
    return self;
}


void
Init_gtk_gdk_im()
{
    mGdkIM = rb_define_module_under(mGdk, "IM");

    rb_define_module_function(mGdkIM, "begin", gdkim_m_begin, 2);
    rb_define_module_function(mGdkIM, "end", gdkim_m_end, 0);
    rb_define_module_function(mGdkIM, "ready?", gdkim_m_ready, 0);
    rb_define_module_function(mGdkIM, "decide_style", gdkim_m_decide_style, 1);
    rb_define_module_function(mGdkIM, "set_best_style", gdkim_m_set_best_style, 1);

    rb_define_const(mGdkIM, "PREEDIT_AREA", INT2FIX(GDK_IM_PREEDIT_AREA));
    rb_define_const(mGdkIM, "PREEDIT_CALLBACKS", INT2FIX(GDK_IM_PREEDIT_CALLBACKS));
    rb_define_const(mGdkIM, "PREEDIT_POSITION", INT2FIX(GDK_IM_PREEDIT_POSITION));
    rb_define_const(mGdkIM, "PREEDIT_NOTHING", INT2FIX(GDK_IM_PREEDIT_NOTHING));
    rb_define_const(mGdkIM, "PREEDIT_NONE", INT2FIX(GDK_IM_PREEDIT_NONE));
    rb_define_const(mGdkIM, "PREEDIT_MASK", INT2FIX(GDK_IM_PREEDIT_MASK));
    rb_define_const(mGdkIM, "STATUS_AREA", INT2FIX(GDK_IM_STATUS_AREA));
    rb_define_const(mGdkIM, "STATUS_CALLBACKS", INT2FIX(GDK_IM_STATUS_CALLBACKS));
    rb_define_const(mGdkIM, "STATUS_NOTHING", INT2FIX(GDK_IM_STATUS_NOTHING));
    rb_define_const(mGdkIM, "STATUS_NONE", INT2FIX(GDK_IM_STATUS_NONE));
    rb_define_const(mGdkIM, "STATUS_MASK", INT2FIX(GDK_IM_STATUS_MASK));


    gdkIC = rb_define_class_under(mGdk, "IC", rb_cData);

    rb_define_singleton_method(gdkIC, "new", gdkic_s_new, 2);
    rb_define_method(gdkIC, "get_events", gdkic_get_events, 0);
    rb_define_method(gdkIC, "get_style", gdkic_get_style, 0);
    rb_define_method(gdkIC, "get_attr", gdkic_get_attr, 1);
    rb_define_method(gdkIC, "set_attr", gdkic_set_attr, 2);

    rb_define_const(gdkIC, "STYLE", INT2FIX(GDK_IC_STYLE));
    rb_define_const(gdkIC, "CLIENT_WINDOW", INT2FIX(GDK_IC_CLIENT_WINDOW));
    rb_define_const(gdkIC, "FOCUS_WINDOW", INT2FIX(GDK_IC_FOCUS_WINDOW));
    rb_define_const(gdkIC, "FILTER_EVENTS", INT2FIX(GDK_IC_FILTER_EVENTS));
    rb_define_const(gdkIC, "SPOT_LOCATION", INT2FIX(GDK_IC_SPOT_LOCATION));
    rb_define_const(gdkIC, "LINE_SPACING", INT2FIX(GDK_IC_LINE_SPACING));
    rb_define_const(gdkIC, "CURSOR", INT2FIX(GDK_IC_CURSOR));
    rb_define_const(gdkIC, "PREEDIT_FONTSET", INT2FIX(GDK_IC_PREEDIT_FONTSET));
    rb_define_const(gdkIC, "PREEDIT_AREA", INT2FIX(GDK_IC_PREEDIT_AREA));
    rb_define_const(gdkIC, "PREEDIT_AREA_NEEDED", INT2FIX(GDK_IC_PREEDIT_AREA_NEEDED));
    rb_define_const(gdkIC, "PREEDIT_FOREGROUND", INT2FIX(GDK_IC_PREEDIT_FOREGROUND));
    rb_define_const(gdkIC, "PREEDIT_BACKGROUND", INT2FIX(GDK_IC_PREEDIT_BACKGROUND));
    rb_define_const(gdkIC, "PREEDIT_PIXMAP", INT2FIX(GDK_IC_PREEDIT_PIXMAP));
    rb_define_const(gdkIC, "PREEDIT_COLORMAP", INT2FIX(GDK_IC_PREEDIT_COLORMAP));

    rb_define_const(gdkIC, "STATUS_FONTSET", INT2FIX(GDK_IC_STATUS_FONTSET));
    rb_define_const(gdkIC, "STATUS_AREA", INT2FIX(GDK_IC_STATUS_AREA));
    rb_define_const(gdkIC, "STATUS_AREA_NEEDED", INT2FIX(GDK_IC_STATUS_AREA_NEEDED));
    rb_define_const(gdkIC, "STATUS_FOREGROUND", INT2FIX(GDK_IC_STATUS_FOREGROUND));
    rb_define_const(gdkIC, "STATUS_BACKGROUND", INT2FIX(GDK_IC_STATUS_BACKGROUND));
    rb_define_const(gdkIC, "STATUS_PIXMAP", INT2FIX(GDK_IC_STATUS_PIXMAP));
    rb_define_const(gdkIC, "STATUS_COLORMAP", INT2FIX(GDK_IC_STATUS_COLORMAP));
    rb_define_const(gdkIC, "ALL_REQ", INT2FIX(GDK_IC_ALL_REQ));
    rb_define_const(gdkIC, "PREEDIT_AREA_REQ", INT2FIX(GDK_IC_PREEDIT_AREA_REQ));
    rb_define_const(gdkIC, "PREEDIT_POSITION_REQ", INT2FIX(GDK_IC_PREEDIT_POSITION_REQ));
    rb_define_const(gdkIC, "STATUS_AREA_REQ", INT2FIX(GDK_IC_STATUS_AREA_REQ));


    gdkICAttr = rb_define_class_under(mGdk, "ICAttr", rb_cData);

    rb_define_singleton_method(gdkICAttr, "new", gdkicattr_s_new, 0);
    rb_define_method(gdkICAttr, "style", gdkicattr_style, 0);
    rb_define_method(gdkICAttr, "style=", gdkicattr_set_style, 1);
    rb_define_method(gdkICAttr, "client_window", gdkicattr_client_window, 0);
    rb_define_method(gdkICAttr, "client_window=", gdkicattr_set_client_window, 1);
    rb_define_method(gdkICAttr, "focus_window", gdkicattr_focus_window, 0);
    rb_define_method(gdkICAttr, "focus_window=", gdkicattr_set_focus_window, 1);
    rb_define_method(gdkICAttr, "spot_location", gdkicattr_spot_location, 0);
    rb_define_method(gdkICAttr, "spot_location=", gdkicattr_set_spot_location, 1);
    rb_define_method(gdkICAttr, "preedit_fontset", gdkicattr_preedit_fontset, 0);
    rb_define_method(gdkICAttr, "preedit_fontset=", gdkicattr_set_preedit_fontset, 1);
    rb_define_method(gdkICAttr, "preedit_area", gdkicattr_preedit_area, 0);
    rb_define_method(gdkICAttr, "preedit_area=", gdkicattr_set_preedit_area, 1);
    rb_define_method(gdkICAttr, "preedit_area_needed", gdkicattr_preedit_area_needed, 0);
    rb_define_method(gdkICAttr, "preedit_colormap", gdkicattr_preedit_cmap, 0);
    rb_define_method(gdkICAttr, "preedit_colormap=", gdkicattr_set_preedit_cmap, 1);
}
#endif /* USE_XIM */
