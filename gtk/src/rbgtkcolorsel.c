/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkcolorsel.c -

  $Author: sakai $
  $Date: 2007/07/08 03:00:49 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(s) (GTK_COLOR_SELECTION(RVAL2GOBJ(self)))
#define RVAL2COLOR(c) ((GdkColor*)RVAL2BOXED(c, GDK_TYPE_COLOR))
#define COLOR2RVAL(c) (BOXED2RVAL(c, GDK_TYPE_COLOR))

static VALUE gColorSel;

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
colorsel_is_adjusting(self)
    VALUE self;
{
    return gtk_color_selection_is_adjusting(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
colorsel_s_palette_from_string(self, str)
    VALUE self, str;
{
    GdkColor* gcolors;
    gint i, n_colors;
    VALUE ary = Qnil;
    gboolean ret = gtk_color_selection_palette_from_string(RVAL2CSTR(str), 
                                                           &gcolors, &n_colors);
    
    if (ret) {
        ary = rb_ary_new();
        for (i = 0; i < n_colors; i++) {
            rb_ary_push(ary, COLOR2RVAL(&gcolors[i]));
        }
    }
                                                                                
    return ary;
}

static VALUE
colorsel_s_palette_to_string(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    GdkColor* gcolors;
    GdkColor* gcolor;
    VALUE colors;
    gint i, len;

    if (argc > 1) {
        rb_scan_args(argc, argv, "*", &colors);
    } else {
        rb_scan_args(argc, argv, "10", &colors);
    }

    len = RARRAY(colors)->len;
    gcolors = ALLOCA_N(GdkColor, len);

    for (i = 0; i < len; i++) {
        gcolor = (GdkColor*)RVAL2BOXED(RARRAY(colors)->ptr[i], GDK_TYPE_COLOR);
        gcolors[i] = *gcolor;
    }

    return CSTR2RVAL(gtk_color_selection_palette_to_string((const GdkColor*)gcolors, len));
}

#if GTK_CHECK_VERSION(2,2,0)
static void
screen_func(screen, colors, n_colors)
    GdkScreen* screen;
    const GdkColor* colors;
    gint n_colors;
{
    int i;
    VALUE func = rb_cvar_get(gColorSel, rb_intern("__palette_proc__"));
    VALUE ary = rb_ary_new();
    for (i = 0; i < n_colors; i++){
        ary = rb_ary_push(ary, BOXED2RVAL((GdkColor*)&colors[i], GDK_TYPE_COLOR));
    }
    if (! NIL_P(func))
        rb_funcall(func, id_call, 2, GOBJ2RVAL(screen), ary);
}

static VALUE
colorsel_s_set_change_palette_hook(self)
    VALUE self;
{
    VALUE func = rb_block_proc();

    rb_cvar_set(gColorSel, rb_intern("__palette_proc__"), func, 0);
    gtk_color_selection_set_change_palette_with_screen_hook(
        (GtkColorSelectionChangePaletteWithScreenFunc)screen_func);
    return self;
}
#endif

/* Don't implement them.
GtkColorSelectionChangePaletteFunc gtk_color_selection_set_change_palette_hook
                                            (GtkColorSelectionChangePaletteFunc func);
void        (*GtkColorSelectionChangePaletteWithScreenFunc)
                                            (GdkScreen *screen,
                                             const GdkColor *colors,
                                             gint n_colors);
*/

void 
Init_gtk_color_selection()
{
    gColorSel = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION, "ColorSelection", mGtk);

    rb_define_method(gColorSel, "initialize", colorsel_initialize, 0);
    rb_define_method(gColorSel, "previous_alpha", colorsel_get_previous_alpha, 0);
    rb_define_method(gColorSel, "set_previous_alpha", colorsel_set_previous_alpha, 1);
    rb_define_method(gColorSel, "set_previous_color", colorsel_set_previous_color, 1);
    rb_define_method(gColorSel, "previous_color", colorsel_get_previous_color, 0);
    rb_define_method(gColorSel, "adjusting?", colorsel_is_adjusting, 0);

    rb_define_singleton_method(gColorSel, "palette_to_string", colorsel_s_palette_to_string, -1);
    rb_define_singleton_method(gColorSel, "palette_from_string", colorsel_s_palette_from_string, 1);

#if GTK_CHECK_VERSION(2,2,0)
    rb_define_singleton_method(gColorSel, "set_change_palette_hook", colorsel_s_set_change_palette_hook, 0);
#endif

    G_DEF_SETTERS(gColorSel);
}
