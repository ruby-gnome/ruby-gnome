/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkvisual.c -

  $Author: mutoh $
  $Date: 2002/07/31 17:23:54 $

  Copyright (C) 2002 Masao Mutoh

  This file is devided from rbgdkcolor.c.
  rbgdkcolor.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GDK_VISUAL(RVAL2GOBJ(self)))

static VALUE
gdkvisual_s_query_depths(self)
    VALUE self;
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
gdkvisual_s_query_visual_types(self)
    VALUE self;
{
    GdkVisualType *visual_types;
    gint count;
    VALUE ary;
    int i;

    gdk_query_visual_types(&visual_types, &count);
    ary = rb_ary_new2(count);
    for (i = 0; i < count; i++) {
        rb_ary_push(ary, INT2NUM((visual_types)[i]));
    }
    return ary;
}

static VALUE
gdkvisual_s_list_visuals(self)
    VALUE self;
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
gdkvisual_s_get_best_depth(self)
    VALUE self;
{
    return INT2NUM(gdk_visual_get_best_depth());
}

static VALUE
gdkvisual_s_get_best_type(self)
    VALUE self;
{
    return INT2NUM(gdk_visual_get_best_type());
}

static VALUE
gdkvisual_s_get_system(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_visual_get_system());
}

static VALUE
gdkvisual_s_get_best(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_visual_get_best());
}

static VALUE
gdkvisual_s_get_best_with_depth(self, depth)
    VALUE self, depth;
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(NUM2INT(depth)));
}

static VALUE
gdkvisual_s_get_best_with_type(self, type)
    VALUE self, type;
{
    return GOBJ2RVAL(gdk_visual_get_best_with_depth(
                                    (GdkVisualType)NUM2INT(type)));
}

static VALUE
gdkvisual_s_get_best_with_both(self, depth, type)
    VALUE self, depth, type;
{
    return GOBJ2RVAL(gdk_visual_get_best_with_both(
                                    NUM2INT(depth),
                                    (GdkVisualType)NUM2INT(type)));
}

static VALUE
gdkvisual_type(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->type);
}

static VALUE
gdkvisual_depth(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->depth);
}

static VALUE
gdkvisual_byte_order(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->byte_order);
}

static VALUE
gdkvisual_colormap_size(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->colormap_size);
}

static VALUE
gdkvisual_bits_per_rgb(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->bits_per_rgb);
}

static VALUE
gdkvisual_red_mask(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->red_mask);
}

static VALUE
gdkvisual_red_shift(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->red_shift);
}

static VALUE
gdkvisual_red_prec(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->red_prec);
}

static VALUE
gdkvisual_green_mask(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->green_mask);
}

static VALUE
gdkvisual_green_shift(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->green_shift);
}

static VALUE
gdkvisual_green_prec(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->green_prec);
}

static VALUE
gdkvisual_blue_mask(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->blue_mask);
}

static VALUE
gdkvisual_blue_shift(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->blue_shift);
}

static VALUE
gdkvisual_blue_prec(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->blue_prec);
}

void
Init_gtk_gdk_visual()
{
    VALUE gdkVisual = G_DEF_CLASS(GDK_TYPE_VISUAL, "Visual", mGdk);

    /* GdkVisualType */
    rb_define_const(gdkVisual, "STATIC_GRAY", INT2FIX(GDK_VISUAL_STATIC_GRAY));
    rb_define_const(gdkVisual, "GRAYSCALE", INT2FIX(GDK_VISUAL_GRAYSCALE));
    rb_define_const(gdkVisual, "STATIC_COLOR", INT2FIX(GDK_VISUAL_STATIC_COLOR));
    rb_define_const(gdkVisual, "PSEUDO_COLOR", INT2FIX(GDK_VISUAL_PSEUDO_COLOR));
    rb_define_const(gdkVisual, "TRUE_COLOR", INT2FIX(GDK_VISUAL_TRUE_COLOR));
    rb_define_const(gdkVisual, "DIRECT_COLOR", INT2FIX(GDK_VISUAL_DIRECT_COLOR));

    /* class methods */
    rb_define_singleton_method(gdkVisual, "query_depths",
                               gdkvisual_s_query_depths, 0);
    rb_define_singleton_method(gdkVisual, "query_visual_types",
                               gdkvisual_s_query_visual_types, 0);
    rb_define_singleton_method(gdkVisual, "list_visuals",
                               gdkvisual_s_list_visuals, 0);

    rb_define_singleton_method(gdkVisual, "best_depth",
                               gdkvisual_s_get_best_depth, 0);
    rb_define_singleton_method(gdkVisual, "best_type",
                               gdkvisual_s_get_best_type, 0);

    rb_define_singleton_method(gdkVisual, "system",
                               gdkvisual_s_get_system, 0);
    rb_define_singleton_method(gdkVisual, "best",
                               gdkvisual_s_get_best, 0);
    rb_define_singleton_method(gdkVisual, "best_with_depth",
                               gdkvisual_s_get_best_with_depth, 1);
    rb_define_singleton_method(gdkVisual, "best_with_type",
                               gdkvisual_s_get_best_with_type, 1);
    rb_define_singleton_method(gdkVisual, "best_with_both",
                               gdkvisual_s_get_best_with_both, 2);

    /* instance methods */
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
}
