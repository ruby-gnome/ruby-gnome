/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *  Copyright (C) 2001  Neil Conway
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

#define RG_TARGET_NAMESPACE cGC
#define _SELF(s) (GDK_GC(RVAL2GOBJ(s)))

static VALUE RG_TARGET_NAMESPACE;
static VALUE gdkDrawable;

static VALUE
gdkgc_initialize(VALUE self, VALUE win)
{
    G_INITIALIZE(self, gdk_gc_new(GDK_DRAWABLE(RVAL2GOBJ(win))));
    return Qnil;
}

/* we don't need them.
GdkGC*      gdk_gc_new_with_values          (GdkDrawable *drawable,
                                             GdkGCValues *values,
                                             GdkGCValuesMask values_mask);
void        gdk_gc_set_values               (GdkGC *gc,
                                             GdkGCValues *values,
                                             GdkGCValuesMask values_mask);
void        gdk_gc_get_values               (GdkGC *gc,
                                             GdkGCValues *values);
*/

static VALUE
gdkgc_set_foreground(VALUE self, VALUE color)
{
    gdk_gc_set_foreground(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
gdkgc_set_background(VALUE self, VALUE color)
{
    gdk_gc_set_background(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
gdkgc_set_rgb_fg_color(VALUE self, VALUE color)
{
    gdk_gc_set_rgb_fg_color(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
gdkgc_set_rgb_bg_color(VALUE self, VALUE color)
{
    gdk_gc_set_rgb_bg_color(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
gdkgc_set_function(VALUE self, VALUE func)
{
    GdkFunction f;
    f = (GdkFunction)(RVAL2GENUM(func, GDK_TYPE_FUNCTION));
    if (f > GDK_SET)
        rb_raise(rb_eArgError, "function out of range");
  
    gdk_gc_set_function(_SELF(self), f);
    return self;
}

static VALUE
gdkgc_set_fill(VALUE self, VALUE fill)
{
    gdk_gc_set_fill(_SELF(self), RVAL2GENUM(fill, GDK_TYPE_FILL));
    return self;
}

static VALUE
gdkgc_set_tile(VALUE self, VALUE tile)
{
    gdk_gc_set_tile(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(tile))); 
    return self;
}

static VALUE
gdkgc_set_stipple(VALUE self, VALUE stipple)
{
    gdk_gc_set_stipple(_SELF(self), GDK_PIXMAP(RVAL2GOBJ(stipple))); 
    return self;
}

static VALUE
gdkgc_set_ts_origin(VALUE self, VALUE x, VALUE y)
{
    gdk_gc_set_ts_origin(_SELF(self), NUM2INT(x), NUM2INT(y)); 
    return self;
}

static VALUE
gdkgc_set_clip_origin(VALUE self, VALUE x, VALUE y)
{
    gdk_gc_set_clip_origin(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

static VALUE
gdkgc_set_clip_mask(VALUE self, VALUE mask)
{
    gdk_gc_set_clip_mask(_SELF(self), GDK_BITMAP(RVAL2GOBJ(mask)));
    return self;
}

static VALUE
gdkgc_set_clip_rectangle(VALUE self, VALUE rectangle)
{
    gdk_gc_set_clip_rectangle(_SELF(self), 
                              (GdkRectangle*)RVAL2BOXED(rectangle, GDK_TYPE_RECTANGLE));
    return self;
}

static VALUE
gdkgc_set_clip_region(VALUE self, VALUE region)
{
    gdk_gc_set_clip_region(_SELF(self), 
                           (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION));
    return self;
}

static VALUE
gdkgc_set_subwindow(VALUE self, VALUE mode)
{
    gdk_gc_set_subwindow(_SELF(self), RVAL2GENUM(mode, GDK_TYPE_SUBWINDOW_MODE));
    return self;
}

static VALUE
gdkgc_set_exposures(VALUE self, VALUE exposures)
{
    gdk_gc_set_exposures(_SELF(self), RVAL2CBOOL(exposures));
    return self;
}

static VALUE
gdkgc_set_line_attributes(VALUE self, VALUE line_width, VALUE line_style, VALUE cap_style, VALUE join_style)
{
    gdk_gc_set_line_attributes(_SELF(self), NUM2INT(line_width),
                               RVAL2GENUM(line_style, GDK_TYPE_LINE_STYLE), 
                               RVAL2GENUM(cap_style, GDK_TYPE_CAP_STYLE),
                               RVAL2GENUM(join_style, GDK_TYPE_JOIN_STYLE));
    return self;
}

static VALUE
gdkgc_set_dashes(VALUE self, VALUE rbdash_offset, VALUE rbdash_list)
{
    GdkGC *gc = _SELF(self);
    gint dash_offset = NUM2INT(rbdash_offset);
    long n;
    gint8 *dash_list = RVAL2GINT8S(rbdash_list, n);

    gdk_gc_set_dashes(gc, dash_offset, dash_list, n);

    g_free(dash_list);

    return self;
}

static VALUE
gdkgc_copy(VALUE self, VALUE dst)
{
    if (RVAL2CBOOL(rb_obj_is_kind_of(dst, gdkDrawable))) {
        VALUE args[1];
        args[0] = dst;
        dst = rb_class_new_instance(1, args, RG_TARGET_NAMESPACE);
    }
    gdk_gc_copy(_SELF(dst), _SELF(self));
    return dst;
}

static VALUE
gdkgc_set_colormap(VALUE self, VALUE colormap)
{
    gdk_gc_set_colormap(_SELF(self), GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
gdkgc_get_foreground(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return BOXED2RVAL(&val.foreground, GDK_TYPE_COLOR);
}

static VALUE
gdkgc_get_background(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return BOXED2RVAL(&val.background, GDK_TYPE_COLOR);
}

static VALUE
gdkgc_get_function(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return GENUM2RVAL(val.function, GDK_TYPE_FUNCTION);
}

static VALUE
gdkgc_get_fill(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return GENUM2RVAL(val.fill, GDK_TYPE_FILL);
}

static VALUE
gdkgc_get_tile(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return GOBJ2RVAL(val.tile);
}

static VALUE
gdkgc_get_stipple(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return GOBJ2RVAL(val.stipple);
}    

static VALUE
gdkgc_get_ts_origin(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return rb_ary_new3(2, INT2NUM(val.ts_x_origin), 
                       INT2NUM(val.ts_y_origin));
}
 
static VALUE
gdkgc_get_clip_origin(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return rb_ary_new3(2, INT2NUM(val.clip_x_origin), 
                       INT2NUM(val.clip_y_origin));
}

static VALUE
gdkgc_get_clip_mask(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return GOBJ2RVAL(val.clip_mask);
}

static VALUE
gdkgc_get_subwindow(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return GENUM2RVAL(val.subwindow_mode, GDK_TYPE_SUBWINDOW_MODE);
}

static VALUE
gdkgc_get_exposures(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return CBOOL2RVAL(val.graphics_exposures);
}

static VALUE
gdkgc_get_line_attributes(VALUE self)
{
    GdkGCValues val;
    gdk_gc_get_values(_SELF(self), &val);

    return rb_ary_new3(4, INT2NUM(val.line_width),
                       GENUM2RVAL(val.line_style, GDK_TYPE_LINE_STYLE),
                       GENUM2RVAL(val.cap_style, GDK_TYPE_CAP_STYLE),
                       GENUM2RVAL(val.join_style, GDK_TYPE_JOIN_STYLE));
}

static VALUE
gdkgc_get_colormap(VALUE self)
{
    return GOBJ2RVAL(gdk_gc_get_colormap(_SELF(self)));
}

static VALUE
gdkgc_offset(VALUE self, VALUE x, VALUE y)
{
    gdk_gc_offset(_SELF(self), NUM2INT(x), NUM2INT(y));
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkgc_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_gc_get_screen(_SELF(self)));
}
#endif

void
Init_gtk_gdk_gc(void)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_GC, "GC", mGdk);
    gdkDrawable = rb_const_get(mGdk, rb_intern("Drawable"));
        
    rbgobj_add_abstract_but_create_instance_class(GDK_TYPE_GC);

    rb_define_method(RG_TARGET_NAMESPACE, "initialize", gdkgc_initialize, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_foreground", gdkgc_set_foreground, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_background", gdkgc_set_background, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_rgb_fg_color", gdkgc_set_rgb_fg_color, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_rgb_bg_color", gdkgc_set_rgb_bg_color, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_function", gdkgc_set_function, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_fill", gdkgc_set_fill, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_tile", gdkgc_set_tile, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_stipple", gdkgc_set_stipple, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_ts_origin", gdkgc_set_ts_origin, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_clip_origin", gdkgc_set_clip_origin, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_clip_mask", gdkgc_set_clip_mask, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_clip_rectangle", gdkgc_set_clip_rectangle, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_clip_region", gdkgc_set_clip_region, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_subwindow_mode", gdkgc_set_subwindow, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_exposures", gdkgc_set_exposures, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_line_attributes", gdkgc_set_line_attributes, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_dashes", gdkgc_set_dashes, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "copy", gdkgc_copy, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_colormap", gdkgc_set_colormap, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "foreground", gdkgc_get_foreground, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "background", gdkgc_get_background, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "function", gdkgc_get_function, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "fill", gdkgc_get_fill, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "tile", gdkgc_get_tile, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "stipple", gdkgc_get_stipple, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "ts_origin", gdkgc_get_ts_origin, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "clip_origin", gdkgc_get_clip_origin, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "clip_mask", gdkgc_get_clip_mask, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "subwindow_mode", gdkgc_get_subwindow, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "exposures?", gdkgc_get_exposures, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "line_attributes", gdkgc_get_line_attributes, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "colormap", gdkgc_get_colormap, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "offset", gdkgc_offset, 2);

#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(RG_TARGET_NAMESPACE, "screen", gdkgc_screen, 0);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    /* GdkGCValuesMask */
/* Don't need them.
    G_DEF_CLASS(GDK_TYPE_GC_VALUES_MASK, "ValuesMask", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_GC_VALUES_MASK, "GDK_GC_");
*/

    /* GdkFunction */
    G_DEF_CLASS(GDK_TYPE_FUNCTION, "Function", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_FUNCTION, "GDK_");

    /* GdkFill */
    G_DEF_CLASS(GDK_TYPE_FILL, "Fill", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_FILL, "GDK_");

    /* GdkSubwindowMode */
    G_DEF_CLASS(GDK_TYPE_SUBWINDOW_MODE, "SubWindowMode", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_SUBWINDOW_MODE, "GDK_");

    /* GdkLineStyle */
    G_DEF_CLASS(GDK_TYPE_LINE_STYLE, "LineStyle", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_LINE_STYLE, "GDK_");

    /* GdkCapStyle */
    G_DEF_CLASS(GDK_TYPE_CAP_STYLE, "CapStyle", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_CAP_STYLE, "GDK_");

    /* GdkJoinStyle */
    G_DEF_CLASS(GDK_TYPE_JOIN_STYLE, "JoinStyle", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_JOIN_STYLE, "GDK_");

#ifdef GDK_WINDOWING_X11
    G_DEF_CLASS3("GdkGCX11", "GCX11", mGdk);
#elif defined(GDK_WINDOWING_WIN32)
    G_DEF_CLASS3("GdkGCWin32", "GCWin32", mGdk);
#elif defined(GDK_WINDOWING_FB)
    G_DEF_CLASS3("GdkGCFB", "GCFB", mGdk);
#endif
}
