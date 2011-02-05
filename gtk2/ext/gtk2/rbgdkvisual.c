/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkvisual.c -

  $Author: mutoh $
  $Date: 2004/08/01 07:10:03 $

  Copyright (C) 2002-2004 Masao Mutoh

  This file is devided from rbgdkcolor.c.
  rbgdkcolor.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GDK_VISUAL(RVAL2GOBJ(self)))

static VALUE
gdkvisual_s_query_depths(VALUE self)
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
gdkvisual_s_query_visual_types(VALUE self)
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
gdkvisual_s_list_visuals(VALUE self)
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
gdkvisual_s_get_best_depth(VALUE self)
{
    return INT2NUM(gdk_visual_get_best_depth());
}

static VALUE
gdkvisual_s_get_best_type(VALUE self)
{
    return INT2NUM(gdk_visual_get_best_type());
}

static VALUE
gdkvisual_s_get_system(VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_system());
}

static VALUE
gdkvisual_s_get_best(VALUE self)
{
    return GOBJ2RVAL(gdk_visual_get_best());
}

static VALUE
gdkvisual_s_get_best_with_depth(VALUE self, VALUE depth)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(NUM2INT(depth)));
}

static VALUE
gdkvisual_s_get_best_with_type(VALUE self, VALUE type)
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(
                                    (GdkVisualType)GENUM2RVAL(type, GDK_TYPE_VISUAL_TYPE)));
}

static VALUE
gdkvisual_s_get_best_with_both(VALUE self, VALUE depth, VALUE type)
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
Init_gtk_gdk_visual()
{
    VALUE gdkVisual = G_DEF_CLASS(GDK_TYPE_VISUAL, "Visual", mGdk);

    /* class methods */
    rb_define_singleton_method(gdkVisual, "query_depths", gdkvisual_s_query_depths, 0);
    rb_define_singleton_method(gdkVisual, "query_visual_types", gdkvisual_s_query_visual_types, 0);
    rb_define_singleton_method(gdkVisual, "visuals", gdkvisual_s_list_visuals, 0);
    rb_define_singleton_method(gdkVisual, "best_depth", gdkvisual_s_get_best_depth, 0);
    rb_define_singleton_method(gdkVisual, "best_type", gdkvisual_s_get_best_type, 0);
    rb_define_singleton_method(gdkVisual, "system", gdkvisual_s_get_system, 0);
    rb_define_singleton_method(gdkVisual, "best", gdkvisual_s_get_best, 0);
    rb_define_singleton_method(gdkVisual, "best_with_depth", gdkvisual_s_get_best_with_depth, 1);
    rb_define_singleton_method(gdkVisual, "best_with_type", gdkvisual_s_get_best_with_type, 1);
    rb_define_singleton_method(gdkVisual, "best_with_both", gdkvisual_s_get_best_with_both, 2);

    /* instance methods */
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gdkVisual, "screen", gdkvisual_get_screen, 0);
#endif
    rb_define_method(gdkVisual, "visual_type", gdkvisual_type, 0);
    rb_define_method(gdkVisual, "depth", gdkvisual_depth, 0);
    rb_define_method(gdkVisual, "byte_order", gdkvisual_byte_order, 0);
    rb_define_method(gdkVisual, "colormap_size", gdkvisual_colormap_size, 0);
    rb_define_method(gdkVisual, "bits_per_rgb", gdkvisual_bits_per_rgb, 0);
    rb_define_method(gdkVisual, "red_mask", gdkvisual_red_mask, 0);
    rb_define_method(gdkVisual, "red_shift", gdkvisual_red_shift, 0);
    rb_define_method(gdkVisual, "red_prec", gdkvisual_red_prec, 0);
    rb_define_method(gdkVisual, "green_mask", gdkvisual_green_mask, 0);
    rb_define_method(gdkVisual, "green_shift", gdkvisual_green_shift, 0);
    rb_define_method(gdkVisual, "green_prec", gdkvisual_green_prec, 0);
    rb_define_method(gdkVisual, "blue_mask", gdkvisual_blue_mask, 0);
    rb_define_method(gdkVisual, "blue_shift", gdkvisual_blue_shift, 0);
    rb_define_method(gdkVisual, "blue_prec", gdkvisual_blue_prec, 0);

    /* GdkVisualType */
    G_DEF_CLASS(GDK_TYPE_VISUAL_TYPE, "Type", gdkVisual);
    G_DEF_CONSTANTS(gdkVisual, GDK_TYPE_VISUAL_TYPE, "GDK_VISUAL_");

    /* GdkByteOrder */
    G_DEF_CLASS(GDK_TYPE_BYTE_ORDER, "ByteOrder", gdkVisual);
    G_DEF_CONSTANTS(gdkVisual, GDK_TYPE_BYTE_ORDER, "GDK_");
}
