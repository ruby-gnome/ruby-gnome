/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cColorSelection
#define _SELF(s) (GTK_COLOR_SELECTION(RVAL2GOBJ(self)))

static VALUE RG_TARGET_NAMESPACE;

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_color_selection_new());
    return Qnil;
}

static VALUE
rg_previous_alpha(VALUE self)
{
    return INT2NUM(gtk_color_selection_get_previous_alpha(_SELF(self)));
}

static VALUE
rg_set_previous_alpha(VALUE self, VALUE alpha)
{
    gtk_color_selection_set_previous_alpha(_SELF(self), NUM2INT(alpha));
    return self;
}

static VALUE
rg_set_previous_color(VALUE self, VALUE color)
{
    gtk_color_selection_set_previous_color(_SELF(self), RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
rg_previous_color(VALUE self)
{
    GdkColor color;
    gtk_color_selection_get_previous_color(_SELF(self), &color);
    return GDKCOLOR2RVAL(&color);
}

static VALUE
rg_adjusting_p(VALUE self)
{
    return CBOOL2RVAL(gtk_color_selection_is_adjusting(_SELF(self)));
}

static VALUE
rg_s_palette_from_string(G_GNUC_UNUSED VALUE self, VALUE str)
{
    GdkColor* gcolors;
    gint i, n_colors;
    VALUE ary = Qnil;
    gboolean ret = gtk_color_selection_palette_from_string(RVAL2CSTR(str), 
                                                           &gcolors, &n_colors);

    if (ret) {
        ary = rb_ary_new();
        for (i = 0; i < n_colors; i++) {
            rb_ary_push(ary, GDKCOLOR2RVAL(&gcolors[i]));
        }
    }

    return ary;
}

struct rbgdk_rval2gdkcolors_args {
    VALUE ary;
    long n;
    GdkColor *result;
};

static VALUE
rbgdk_rval2gdkcolors_body(VALUE value)
{
    long i;
    struct rbgdk_rval2gdkcolors_args *args = (struct rbgdk_rval2gdkcolors_args *)value;

    for (i = 0; i < args->n; i++)
        args->result[i] = *RVAL2GDKCOLOR(RARRAY_PTR(args->ary)[i]);

    return Qnil;
}

static G_GNUC_NORETURN VALUE
rbgdk_rval2gdkcolors_rescue(VALUE value)
{
    g_free(((struct rbgdk_rval2gdkcolors_args *)value)->result);

    rb_exc_raise(rb_errinfo());
}

static GdkColor *
rbgdk_rval2gdkcolors(VALUE value, long *n)
{
    struct rbgdk_rval2gdkcolors_args args;

    args.ary = rb_ary_to_ary(value);
    args.n = RARRAY_LEN(args.ary);
    args.result = g_new(GdkColor, args.n + 1);

    rb_rescue(rbgdk_rval2gdkcolors_body, (VALUE)&args,
              rbgdk_rval2gdkcolors_rescue, (VALUE)&args);

    if (n != NULL)
        *n = args.n;

    return args.result;
}

#define RVAL2GDKCOLORS(value, n) rbgdk_rval2gdkcolors(value, n)

static VALUE
rg_s_palette_to_string(int argc, VALUE *argv, G_GNUC_UNUSED VALUE self)
{
    VALUE rbcolors;
    long n;
    GdkColor *colors;
    gchar *palette;

    if (argc > 1)
        rb_scan_args(argc, argv, "*", &rbcolors);
    else
        rb_scan_args(argc, argv, "10", &rbcolors);

    colors = RVAL2GDKCOLORS(rbcolors, &n);

    palette = gtk_color_selection_palette_to_string(colors, n);

    g_free(colors);

    return CSTR2RVAL_FREE(palette);
}

#if GTK_CHECK_VERSION(2,2,0)
static void
screen_func(GdkScreen *screen, const GdkColor *colors, gint n_colors)
{
    int i;
    VALUE func = rb_cvar_get(RG_TARGET_NAMESPACE, rb_intern("__palette_proc__"));
    VALUE ary = rb_ary_new();
    for (i = 0; i < n_colors; i++){
        ary = rb_ary_push(ary, GDKCOLOR2RVAL((GdkColor *)&colors[i]));
    }
    if (! NIL_P(func))
        rb_funcall(func, id_call, 2, GOBJ2RVAL(screen), ary);
}

static VALUE
rg_s_set_change_palette_hook(VALUE self)
{
    VALUE func = rb_block_proc();

    rb_cv_set(RG_TARGET_NAMESPACE, "__palette_proc__", func);
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
Init_gtk_color_selection(VALUE mGtk)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_COLOR_SELECTION, "ColorSelection", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(previous_alpha, 0);
    RG_DEF_METHOD(set_previous_alpha, 1);
    RG_DEF_METHOD(set_previous_color, 1);
    RG_DEF_METHOD(previous_color, 0);
    RG_DEF_METHOD_P(adjusting, 0);

    RG_DEF_SMETHOD(palette_to_string, -1);
    RG_DEF_SMETHOD(palette_from_string, 1);

#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_SMETHOD(set_change_palette_hook, 0);
#endif

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
