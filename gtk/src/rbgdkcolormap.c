/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcolormap.c -

  $Author: mutoh $
  $Date: 2002/07/07 06:06:08 $

  Copyright (C) 2002 Masao Mutoh

  This file is devided from rbgdkcolor.c.
  rbgdkcolor.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GDK_COLORMAP(RVAL2GOBJ(self)))

VALUE gdkColormap;

static VALUE
gdkcmap_initialize(self, visual, allocate)
    VALUE self, visual, allocate;
{
    GdkColormap *cmap  = gdk_colormap_new(GDK_VISUAL(RVAL2GOBJ(visual)),
                                          RTEST(allocate));
    RBGTK_INITIALIZE(self, cmap);
    return Qnil;
}

static VALUE
gdkcmap_s_get_system(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_colormap_get_system());
}
 
static VALUE
gdkcmap_alloc_color(self, color, writeable, best_match)
    VALUE self, color, writeable, best_match;
{
    gboolean result;
    GdkColor *c = get_gdkcolor(color);
    result = gdk_colormap_alloc_color(_SELF(self), c,
                                      RTEST(writeable), RTEST(best_match));
    return result ? INT2NUM(c->pixel) : Qnil;
}

static VALUE
gdkcmap_colors(self)
    VALUE self;
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
            rb_ary_push(ary, make_gdkcolor(colors));
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
	static RGObjClassInfo cinfo;

    gdkColormap = rb_define_class_under(mGdk, "Colormap", rb_cData);
	cinfo.klass = gdkColormap;
    cinfo.gtype = GDK_TYPE_COLORMAP;
    cinfo.mark = 0;
    cinfo.free = 0;
    rbgtk_register_class(&cinfo);

    rb_define_singleton_method(gdkColormap, "system",
                               gdkcmap_s_get_system, 0);
    rb_define_method(gdkColormap, "initialize", gdkcmap_initialize, 2);
    rb_define_method(gdkColormap, "alloc_color", gdkcmap_alloc_color, 3);
    rb_define_method(gdkColormap, "colors", gdkcmap_colors, 0);
}


