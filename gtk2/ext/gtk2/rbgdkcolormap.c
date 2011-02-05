/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkcolormap.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:31 $

  Copyright (C) 2002,2003 Masao Mutoh

  This file is devided from rbgdkcolor.c.
  rbgdkcolor.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GDK_COLORMAP(RVAL2GOBJ(self)))

static VALUE
gdkcmap_initialize(VALUE self, VALUE visual, VALUE allocate)
{
    GdkColormap *cmap  = gdk_colormap_new(GDK_VISUAL(RVAL2GOBJ(visual)),
                                          RVAL2CBOOL(allocate));
    G_INITIALIZE(self, cmap);
    return Qnil;
}

static VALUE
gdkcmap_s_get_system(VALUE self)
{
    return GOBJ2RVAL(gdk_colormap_get_system());
}

/* Don't implement this. Because API is ugly
   (especially treating of "success"). 
   Use Gdk::Colormap#alloc_color instead.
gint        gdk_colormap_alloc_colors       (GdkColormap *colormap,
                                             GdkColor *colors,
                                             gint ncolors,
                                             gboolean writeable,
                                             gboolean best_match,
                                             gboolean *success);
*/

static VALUE
gdkcmap_alloc_color(VALUE self, VALUE color, VALUE writeable, VALUE best_match)
{
    gboolean result;
    GdkColor *c = RVAL2GDKCOLOR(color);
    result = gdk_colormap_alloc_color(_SELF(self), c,
                                      RVAL2CBOOL(writeable), RVAL2CBOOL(best_match));
    return result ? INT2NUM(c->pixel) : Qnil;
}

/* Don't implement Gdk::Colormap#free_colors.
   Because it should be pair with Gdk::Colormap#alloc_colors */
static VALUE
gdkcmap_free_color(VALUE self, VALUE color)
{
    gdk_colormap_free_colors(_SELF(self), RVAL2GDKCOLOR(color), 1);
    return self;
}

static VALUE
gdkcmap_query_color(VALUE self, VALUE pixel)
{
    GdkColor color;
    gdk_colormap_query_color(_SELF(self), NUM2ULONG(pixel), &color);
    return GDKCOLOR2RVAL(&color);
}

static VALUE
gdkcmap_get_visual(VALUE self)
{
    return GOBJ2RVAL(gdk_colormap_get_visual(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkcmap_get_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_colormap_get_screen(_SELF(self)));
}
#endif

static VALUE
gdkcmap_colors(VALUE self)
{
    GdkColormap *cmap;
    GdkColor *colors;
    GdkVisual *visual;
    VALUE ary;
    int i;

    cmap = _SELF(self);
    colors = cmap->colors;

    visual = gdk_colormap_get_visual(cmap);

    if (visual->type == GDK_VISUAL_GRAYSCALE ||
          visual->type == GDK_VISUAL_PSEUDO_COLOR) {
        ary = rb_ary_new2(cmap->size);
        for (i = 0; i < cmap->size; i++) {
            rb_ary_push(ary, BOXED2RVAL(colors, GDK_TYPE_COLOR));
            colors++;
        }
        return ary;
    } else {
        return Qnil;
    }
}

void
Init_gtk_gdk_colormap()
{
    VALUE gdkColormap = G_DEF_CLASS(GDK_TYPE_COLORMAP, "Colormap", mGdk);

    rb_define_singleton_method(gdkColormap, "system",
                               gdkcmap_s_get_system, 0);
    rb_define_method(gdkColormap, "initialize", gdkcmap_initialize, 2);
    rb_define_method(gdkColormap, "alloc_color", gdkcmap_alloc_color, 3);
    rb_define_method(gdkColormap, "free_color", gdkcmap_free_color, 1);
    rb_define_method(gdkColormap, "query_color", gdkcmap_query_color, 1);
    rb_define_method(gdkColormap, "visual", gdkcmap_get_visual, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkColormap, "screen", gdkcmap_get_screen, 0);
#endif
    rb_define_method(gdkColormap, "colors", gdkcmap_colors, 0);
}


