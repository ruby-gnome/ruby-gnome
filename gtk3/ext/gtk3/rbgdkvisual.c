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
rg_s_query_depths(G_GNUC_UNUSED VALUE self)
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
rg_s_query_visual_types(G_GNUC_UNUSED VALUE self)
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
rg_s_visuals(G_GNUC_UNUSED VALUE self)
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
rg_s_best_depth(G_GNUC_UNUSED VALUE self)
{
    return INT2NUM(gdk_visual_get_best_depth());
}

static VALUE
rg_s_best_type(G_GNUC_UNUSED VALUE self)
{
    return INT2NUM(gdk_visual_get_best_type());
}

static VALUE
rg_s_system(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_system());
}

static VALUE
rg_s_best(G_GNUC_UNUSED VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_best());
}

static VALUE
rg_s_best_with_depth(G_GNUC_UNUSED VALUE self, VALUE depth)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(NUM2INT(depth)));
}

static VALUE
rg_s_best_with_type(G_GNUC_UNUSED VALUE self, VALUE type)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(
                                    (GdkVisualType)GENUM2RVAL(type, GDK_TYPE_VISUAL_TYPE)));
}

static VALUE
rg_s_best_with_both(G_GNUC_UNUSED VALUE self, VALUE depth, VALUE type)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_both(
                                    NUM2INT(depth),
                                    (GdkVisualType)RVAL2GENUM(type, GDK_TYPE_VISUAL_TYPE)));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
rg_screen(VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_screen(_SELF(self)));
}
#endif

/* Structure accessors */
static VALUE
rg_visual_type(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->type, GDK_TYPE_VISUAL_TYPE);
}

static VALUE
rg_depth(VALUE self)
{
    return INT2FIX(_SELF(self)->depth);
}

static VALUE
rg_byte_order(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->byte_order, GDK_TYPE_BYTE_ORDER);
}

static VALUE
rg_colormap_size(VALUE self)
{
    return INT2FIX(_SELF(self)->colormap_size);
}

static VALUE
rg_bits_per_rgb(VALUE self)
{
    return INT2FIX(_SELF(self)->bits_per_rgb);
}

static VALUE
rg_red_mask(VALUE self)
{
    return INT2FIX(_SELF(self)->red_mask);
}

static VALUE
rg_red_shift(VALUE self)
{
    return INT2FIX(_SELF(self)->red_shift);
}

static VALUE
rg_red_prec(VALUE self)
{
    return INT2FIX(_SELF(self)->red_prec);
}

static VALUE
rg_green_mask(VALUE self)
{
    return INT2FIX(_SELF(self)->green_mask);
}

static VALUE
rg_green_shift(VALUE self)
{
    return INT2FIX(_SELF(self)->green_shift);
}

static VALUE
rg_green_prec(VALUE self)
{
    return INT2FIX(_SELF(self)->green_prec);
}

static VALUE
rg_blue_mask(VALUE self)
{
    return INT2FIX(_SELF(self)->blue_mask);
}

static VALUE
rg_blue_shift(VALUE self)
{
    return INT2FIX(_SELF(self)->blue_shift);
}

static VALUE
rg_blue_prec(VALUE self)
{
    return INT2FIX(_SELF(self)->blue_prec);
}

void
Init_gtk_gdk_visual(VALUE mGdk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GDK_TYPE_VISUAL, "Visual", mGdk);

    /* class methods */
    RG_DEF_SMETHOD(query_depths, 0);
    RG_DEF_SMETHOD(query_visual_types, 0);
    RG_DEF_SMETHOD(visuals, 0);
    RG_DEF_SMETHOD(best_depth, 0);
    RG_DEF_SMETHOD(best_type, 0);
    RG_DEF_SMETHOD(system, 0);
    RG_DEF_SMETHOD(best, 0);
    RG_DEF_SMETHOD(best_with_depth, 1);
    RG_DEF_SMETHOD(best_with_type, 1);
    RG_DEF_SMETHOD(best_with_both, 2);

    /* instance methods */
#if GTK_CHECK_VERSION(2,2,0)
    RG_DEF_METHOD(screen, 0);
#endif
    RG_DEF_METHOD(visual_type, 0);
    RG_DEF_METHOD(depth, 0);
    RG_DEF_METHOD(byte_order, 0);
    RG_DEF_METHOD(colormap_size, 0);
    RG_DEF_METHOD(bits_per_rgb, 0);
    RG_DEF_METHOD(red_mask, 0);
    RG_DEF_METHOD(red_shift, 0);
    RG_DEF_METHOD(red_prec, 0);
    RG_DEF_METHOD(green_mask, 0);
    RG_DEF_METHOD(green_shift, 0);
    RG_DEF_METHOD(green_prec, 0);
    RG_DEF_METHOD(blue_mask, 0);
    RG_DEF_METHOD(blue_shift, 0);
    RG_DEF_METHOD(blue_prec, 0);

    /* GdkVisualType */
    G_DEF_CLASS(GDK_TYPE_VISUAL_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_VISUAL_TYPE, "GDK_VISUAL_");

    /* GdkByteOrder */
    G_DEF_CLASS(GDK_TYPE_BYTE_ORDER, "ByteOrder", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GDK_TYPE_BYTE_ORDER, "GDK_");
}
