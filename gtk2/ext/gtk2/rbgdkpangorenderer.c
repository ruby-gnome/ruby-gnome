/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkpangorenderer.c -

  $Author: mutoh $
  $Date: 2005/02/12 16:03:46 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

#define _SELF(s) (GDK_PANGO_RENDERER(RVAL2GOBJ(s)))

static VALUE
prenderer_initialize(int argc, VALUE *argv, VALUE self)
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
prenderer_s_get_default(int argc, VALUE *argv, VALUE self)
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
prenderer_s_default(VALUE self)
{
    GdkScreen* gscreen = gdk_screen_get_default();
    return GOBJ2RVAL(gdk_pango_renderer_get_default(gscreen));
}

static VALUE
prenderer_set_drawable(VALUE self, VALUE drawable)
{
    gdk_pango_renderer_set_drawable(_SELF(self), 
                                    GDK_DRAWABLE(RVAL2GOBJ(drawable)));
    return self;
}

static VALUE
prenderer_set_gc(VALUE self, VALUE gc)
{
    gdk_pango_renderer_set_gc(_SELF(self), 
                              NIL_P(gc) ? NULL : GDK_GC(RVAL2GOBJ(gc)));
    return self;
}

static VALUE
prenderer_set_stipple(VALUE self, VALUE part, VALUE stipple)
{
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    gdk_pango_renderer_set_stipple(_SELF(self), RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                                   NIL_P(stipple) ? NULL : GDK_BITMAP(RVAL2GOBJ(stipple)));
#else
      rb_warning("Gdk::PangoRender#set_tipple is not supported (Require pango-1.8.1 or later");
#endif
    return self;
}

static VALUE
prenderer_set_override_color(VALUE self, VALUE part, VALUE color)
{
#if HAVE_PANGO_RENDER_PART_GET_TYPE
    gdk_pango_renderer_set_override_color(_SELF(self), 
                                          RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                                          RVAL2GDKCOLOR(color));
#else
      rb_warning("Gdk::PangoRender#set_override_color is not supported (Require pango-1.8.1 or later");
#endif
    return self;
}
#endif

void
Init_gtk_gdk_pangorenderer()
{
#if GTK_CHECK_VERSION(2,6,0)
    VALUE renderer = G_DEF_CLASS(GDK_TYPE_PANGO_RENDERER, "PangoRenderer", mGdk);

    rb_define_method(renderer, "initialize", prenderer_initialize, -1);
    rb_define_method(renderer, "set_drawable", prenderer_set_drawable, 1);
    rb_define_method(renderer, "set_gc", prenderer_set_gc, 1);
    rb_define_method(renderer, "set_stipple", prenderer_set_stipple, 2);
    rb_define_method(renderer, "set_override_color", prenderer_set_override_color, 2);

    rb_define_singleton_method(renderer, "get_default", prenderer_s_get_default, -1);
    rb_define_singleton_method(renderer, "default", prenderer_s_default, 0);

    G_DEF_SETTERS(renderer);
#endif
}


