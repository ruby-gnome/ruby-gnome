/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkpangorenderer.c -

  $Author: mutoh $
  $Date: 2005/01/30 11:24:36 $

  Copyright (C) 2005 Masao Mutoh
************************************************/

#include "global.h"

#if GTK_CHECK_VERSION(2,6,0)

#define _SELF(s) (GDK_PANGO_RENDERER(RVAL2GOBJ(s)))

static VALUE
prenderer_initialize(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
prenderer_s_get_default(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
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
prenderer_set_drawable(self, drawable)
    VALUE self, drawable;
{
    gdk_pango_renderer_set_drawable(_SELF(self), 
                                    GDK_DRAWABLE(RVAL2GOBJ(drawable)));
    return self;
}

static VALUE
prenderer_set_gc(self, gc)
    VALUE self, gc;
{
    gdk_pango_renderer_set_gc(_SELF(self), 
                              NIL_P(gc) ? NULL : GDK_GC(RVAL2GOBJ(gc)));
    return self;
}

static VALUE
prenderer_set_stipple(self, part, stipple)
    VALUE self, part, stipple;
{
    gdk_pango_renderer_set_stipple(_SELF(self), RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                                   NIL_P(stipple) ? NULL : GDK_BITMAP(RVAL2GOBJ(stipple)));
    return self;
}

static VALUE
prenderer_set_override_color(self, part, color)
    VALUE self, part, color;
{
    gdk_pango_renderer_set_override_color(_SELF(self), 
                                          RVAL2GENUM(part, PANGO_TYPE_RENDER_PART),
                                          NIL_P(color) ? NULL : (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
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

    G_DEF_SETTERS(renderer);
#endif
}


