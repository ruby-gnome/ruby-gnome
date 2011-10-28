/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Masao Mutoh
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

#define RG_TARGET_NAMESPACE cVisual
#define _SELF(self) (GDK_VISUAL(RVAL2GOBJ(self)))

static VALUE
gdkvisual_s_query_depths(G_GNUC_UNUSED VALUE self)
{
    gint *depth;
    gint count;
    VALUE ary;
    int i;

    gdk_query_depths(&depth, &count);
    ary = rb_ary_new2(count);
    for (i = 0; i < count; i++) {
        rb_ary_push(ary, INT2NUM((depth)[i]));
    }
    return ary;
}

static VALUE
gdkvisual_s_query_visual_types(G_GNUC_UNUSED VALUE self)
{
    GdkVisualType *visual_types;
    gint count;
    VALUE ary;
    int i;

    gdk_query_visual_types(&visual_types, &count);
    ary = rb_ary_new2(count);
    for (i = 0; i < count; i++) {
        rb_ary_push(ary, GENUM2RVAL((visual_types)[i], GDK_TYPE_VISUAL_TYPE));
    }
    return ary;
}

static VALUE
gdkvisual_s_list_visuals(G_GNUC_UNUSED VALUE self)
{
    GList *list = gdk_list_visuals(), *cur;
    VALUE ary = rb_ary_new();

    for (cur = list; cur != NULL; cur = cur->next) {
        rb_ary_push(ary, GOBJ2RVAL((GdkVisual *)cur->data));
    }
    g_list_free(list);
    return ary;
}

static VALUE
gdkvisual_s_get_best_depth(G_GNUC_UNUSED VALUE self)
{
    return INT2NUM(gdk_visual_get_best_depth());
}

static VALUE
gdkvisual_s_get_best_type(G_GNUC_UNUSED VALUE self)
{
    return INT2NUM(gdk_visual_get_best_type());
}

static VALUE
gdkvisual_s_get_system(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_system());
}

static VALUE
gdkvisual_s_get_best(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_best());
}

static VALUE
gdkvisual_s_get_best_with_depth(G_GNUC_UNUSED VALUE self, VALUE depth)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(NUM2INT(depth)));
}

static VALUE
gdkvisual_s_get_best_with_type(G_GNUC_UNUSED VALUE self, VALUE type)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(
                                    (GdkVisualType)GENUM2RVAL(type, GDK_TYPE_VISUAL_TYPE)));
}

static VALUE
gdkvisual_s_get_best_with_both(G_GNUC_UNUSED VALUE self, VALUE depth, VALUE type)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_both(
                                    NUM2INT(depth),
                                    (GdkVisualType)RVAL2GENUM(type, GDK_TYPE_VISUAL_TYPE)));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
gdkvisual_get_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_screen(_SELF(self)));
}
#endif

/* Structure accessors */
static VALUE
gdkvisual_type(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->type, GDK_TYPE_VISUAL_TYPE);
}

static VALUE
gdkvisual_depth(VALUE self)
{
    return INT2FIX(_SELF(self)->depth);
}

static VALUE
gdkvisual_byte_order(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->byte_order, GDK_TYPE_BYTE_ORDER);
}

static VALUE
gdkvisual_colormap_size(VALUE self)
{
    return INT2FIX(_SELF(self)->colormap_size);
}

static VALUE
gdkvisual_bits_per_rgb(VALUE self)
{
    return INT2FIX(_SELF(self)->bits_per_rgb);
}

static VALUE
gdkvisual_red_mask(VALUE self)
{
    return INT2FIX(_SELF(self)->red_mask);
}

static VALUE
gdkvisual_red_shift(VALUE self)
{
    return INT2FIX(_SELF(self)->red_shift);
}

static VALUE
gdkvisual_red_prec(VALUE self)
{
    return INT2FIX(_SELF(self)->red_prec);
}

static VALUE
gdkvisual_green_mask(VALUE self)
{
    return INT2FIX(_SELF(self)->green_mask);
}

static VALUE
gdkvisual_green_shift(VALUE self)
{
    return INT2FIX(_SELF(self)->green_shift);
}

static VALUE
gdkvisual_green_prec(VALUE self)
{
    return INT2FIX(_SELF(self)->green_prec);
}

static VALUE
gdkvisual_blue_mask(VALUE self)
{
    return INT2FIX(_SELF(self)->blue_mask);
}

static VALUE
gdkvisual_blue_shift(VALUE self)
{
    return INT2FIX(_SELF(self)->blue_shift);
}

static VALUE
gdkvisual_blue_prec(VALUE self)
{
    return INT2FIX(_SELF(self)->blue_prec);
}

void
Init_gtk_gdk_visual(void)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_VISUAL, "Visual", mGdk);

    /* class methods */
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "query_depths", gdkvisual_s_query_depths, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "query_visual_types", gdkvisual_s_query_visual_types, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "visuals", gdkvisual_s_list_visuals, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "best_depth", gdkvisual_s_get_best_depth, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "best_type", gdkvisual_s_get_best_type, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "system", gdkvisual_s_get_system, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "best", gdkvisual_s_get_best, 0);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "best_with_depth", gdkvisual_s_get_best_with_depth, 1);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "best_with_type", gdkvisual_s_get_best_with_type, 1);
    rb_define_singleton_method(RG_TARGET_NAMESPACE, "best_with_both", gdkvisual_s_get_best_with_both, 2);

    /* instance methods */
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(RG_TARGET_NAMESPACE, "screen", gdkvisual_get_screen, 0);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "visual_type", gdkvisual_type, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "depth", gdkvisual_depth, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "byte_order", gdkvisual_byte_order, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "colormap_size", gdkvisual_colormap_size, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "bits_per_rgb", gdkvisual_bits_per_rgb, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "red_mask", gdkvisual_red_mask, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "red_shift", gdkvisual_red_shift, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "red_prec", gdkvisual_red_prec, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "green_mask", gdkvisual_green_mask, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "green_shift", gdkvisual_green_shift, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "green_prec", gdkvisual_green_prec, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "blue_mask", gdkvisual_blue_mask, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "blue_shift", gdkvisual_blue_shift, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "blue_prec", gdkvisual_blue_prec, 0);

    /* GdkVisualType */
    G_DEF_CLASS(GDK_TYPE_VISUAL_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_VISUAL_TYPE, "GDK_VISUAL_");

    /* GdkByteOrder */
    G_DEF_CLASS(GDK_TYPE_BYTE_ORDER, "ByteOrder", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_BYTE_ORDER, "GDK_");
}
