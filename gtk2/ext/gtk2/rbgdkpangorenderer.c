/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2005  Masao Mutoh
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

#if GTK_CHECK_VERSION(2,6,0)

#define RG_TARGET_NAMESPACE cPangoRenderer
#define _SELF(s) (GDK_PANGO_RENDERER(RVAL2GOBJ(s)))

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE screen;
    GdkScreen* gscreen;

    rb_scan_args(argc, argv, "01", &screen);

    if (NIL_P(screen)){
        gscreen = gdk_screen_get_default();
    } else {
        gscreen = GDK_SCREEN(RVAL2GOBJ(screen));
    }

    G_INITIALIZE(self, gdk_pango_renderer_new(gscreen));

    return Qnil;
}

static VALUE
rg_s_get_default(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE screen;
    GdkScreen* gscreen;

    rb_scan_args(argc, argv, "01", &screen);

    if (NIL_P(screen)){
        gscreen = gdk_screen_get_default();
    } else {
        gscreen = GDK_SCREEN(RVAL2GOBJ(screen));
    }
    return GOBJ2RVAL(gdk_pango_renderer_get_default(gscreen));
}

static VALUE
rg_s_default(G_GNUC_UNUSED VALUE self)
{
    GdkScreen* gscreen = gdk_screen_get_default();
    return GOBJ2RVAL(gdk_pango_renderer_get_default(gscreen));
}

static VALUE
rg_set_drawable(VALUE self, VALUE drawable)
{
    gdk_pango_renderer_set_drawable(_SELF(self), 
                                    GDK_DRAWABLE(RVAL2GOBJ(drawable)));
    return self;
}

static VALUE
rg_set_gc(VALUE self, VALUE gc)
{
    gdk_pango_renderer_set_gc(_SELF(self), 
                              NIL_P(gc) ? NULL : GDK_GC(RVAL2GOBJ(gc)));
    return self;
}

#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
static VALUE
rg_set_stipple(VALUE self, VALUE part, VALUE stipple)
{
    gdk_pango_renderer_set_stipple(_SELF(self), RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                                   NIL_P(stipple) ? NULL : GDK_BITMAP(RVAL2GOBJ(stipple)));

    return self;
}
#else
static VALUE
rg_set_stipple(G_GNUC_UNUSED VALUE self,
               G_GNUC_UNUSED VALUE part,
               G_GNUC_UNUSED VALUE stipple)
{
    rb_warning("Gdk::PangoRender#set_tipple is not supported (Require pango-1.8.1 or later");

    return self;
}
#endif

#ifdef HAVE_PANGO_RENDER_PART_GET_TYPE
static VALUE
rg_set_override_color(VALUE self, VALUE part, VALUE color)
{
    gdk_pango_renderer_set_override_color(_SELF(self),
                                          RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                                          RVAL2GDKCOLOR(color));

    return self;
}
#else
static VALUE
rg_set_override_color(G_GNUC_UNUSED VALUE self,
                      G_GNUC_UNUSED VALUE part,
                      G_GNUC_UNUSED VALUE color)
{
    rb_warning("Gdk::PangoRender#set_override_color is not supported (Require pango-1.8.1 or later");

    return self;
}
#endif

#endif

void
Init_gtk_gdk_pangorenderer(VALUE mGdk)
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_PANGO_RENDERER, "PangoRenderer", mGdk);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD(set_drawable, 1);
    RG_DEF_METHOD(set_gc, 1);
    RG_DEF_METHOD(set_stipple, 2);
    RG_DEF_METHOD(set_override_color, 2);

    RG_DEF_SMETHOD(get_default, -1);
    RG_DEF_SMETHOD(default, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
#endif
}
