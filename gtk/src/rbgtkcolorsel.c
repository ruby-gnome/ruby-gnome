/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcolorsel.c -

  $Author: mutoh $
  $Date: 2002/10/23 18:02:13 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_COLOR_SELECTION(RVAL2GOBJ(self)))
#define RVAL2COLOR(c) ((GdkColor*)RVAL2BOXED(c, GDK_TYPE_COLOR))
#define COLOR2RVAL(c) (BOXED2RVAL(c, GDK_TYPE_COLOR))

static VALUE
colorsel_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_color_selection_new());
    return Qnil;
}

static VALUE
colorsel_get_previous_alpha(self)
    VALUE self;
{
    return INT2NUM(gtk_color_selection_get_previous_alpha(_SELF(self)));
}

static VALUE
colorsel_set_previous_alpha(self, alpha)
    VALUE self, alpha;
{
    gtk_color_selection_set_previous_alpha(_SELF(self), NUM2INT(alpha));
    return self;
}

static VALUE
colorsel_set_previous_color(self, color)
    VALUE self, color;
{
    gtk_color_selection_set_previous_color(_SELF(self), RVAL2COLOR(color));
    return self;
}

static VALUE
colorsel_get_previous_color(self)
    VALUE self;
{
    GdkColor color;
    gtk_color_selection_get_previous_color(_SELF(self), &color);
    return COLOR2RVAL(&color);
}

static VALUE
colorsel_get_is_adjusting(self)
    VALUE self;
{
    return gtk_color_selection_is_adjusting(_SELF(self)) ? Qtrue : Qfalse;
}

/*
gboolean    gtk_color_selection_palette_from_string
                                            (const gchar *str,
                                             GdkColor **colors,
                                             gint *n_colors);
gchar*      gtk_color_selection_palette_to_string
                                            (const GdkColor *colors,
                                             gint n_colors);
GtkColorSelectionChangePaletteFunc gtk_color_selection_set_change_palette_hook
                                            (GtkColorSelectionChangePaletteFunc func);
void        (*GtkColorSelectionChangePaletteFunc)
                                            (const GdkColor *colors,
                                             gint n_colors);
GtkColorSelectionChangePaletteWithScreenFunc gtk_color_selection_set_change_palette_with_screen_hook
                                            (GtkColorSelectionChangePaletteWithScreenFunc func);
void        (*GtkColorSelectionChangePaletteWithScreenFunc)
                                            (GdkScreen *screen,
                                             const GdkColor *colors,
                                             gint n_colors);
*/

void 
Init_gtk_color_selection()
{
    VALUE gColorSel = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION, "ColorSelection", mGtk);

    rb_define_method(gColorSel, "initialize", colorsel_initialize, 0);
    rb_define_method(gColorSel, "previous_alpha", colorsel_get_previous_alpha, 0);
    rb_define_method(gColorSel, "set_previous_alpha", colorsel_set_previous_alpha, 1);
    rb_define_method(gColorSel, "set_previous_color", colorsel_set_previous_color, 1);
    rb_define_method(gColorSel, "previous_color", colorsel_get_previous_color, 0);
    rb_define_method(gColorSel, "adjusting?", colorsel_get_is_adjusting, 0);

    G_DEF_SETTERS(gColorSel);
}
