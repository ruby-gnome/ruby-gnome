/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Masao Mutoh
 *  Copyright (C) 1998-2000 Yukihiro Matsumoto,
 *                          Daisuke Kanda,
 *                          Hiroshi Igarashi
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

#define RG_TARGET_NAMESPACE cColormap
#define _SELF(self) (GDK_COLORMAP(RVAL2GOBJ(self)))

static VALUE
rg_initialize(VALUE self, VALUE visual, VALUE allocate)
{
    GdkColormap *cmap  = gdk_colormap_new(GDK_VISUAL(RVAL2GOBJ(visual)),
                                          RVAL2CBOOL(allocate));
    G_INITIALIZE(self, cmap);
    return Qnil;
}

static VALUE
rg_s_system(G_GNUC_UNUSED VALUE self)
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
rg_alloc_color(VALUE self, VALUE color, VALUE writeable, VALUE best_match)
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
rg_free_color(VALUE self, VALUE color)
{
    gdk_colormap_free_colors(_SELF(self), RVAL2GDKCOLOR(color), 1);
    return self;
}

static VALUE
rg_query_color(VALUE self, VALUE pixel)
{
    GdkColor color;
    gdk_colormap_query_color(_SELF(self), NUM2ULONG(pixel), &color);
    return GDKCOLOR2RVAL(&color);
}

static VALUE
rg_visual(VALUE self)
{
    return GOBJ2RVAL(gdk_colormap_get_visual(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_colormap_get_screen(_SELF(self)));
}
#endif

static VALUE
rg_colors(VALUE self)
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
Init_gtk_gdk_colormap(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_COLORMAP, "Colormap", mGdk);

    RG_DEF_SMETHOD(system, 0);
    RG_DEF_METHOD(initialize, 2);
    RG_DEF_METHOD(alloc_color, 3);
    RG_DEF_METHOD(free_color, 1);
    RG_DEF_METHOD(query_color, 1);
    RG_DEF_METHOD(visual, 0);
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(screen, 0);
#endif
    RG_DEF_METHOD(colors, 0);
}
