/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgdkcolor.c -

  $Author: mutoh $
  $Date: 2002/05/19 12:39:06 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Gdk::Colormap
 */

VALUE gdkColormap;

static VALUE
gdkcmap_s_new(self, visual, allocate)
    VALUE self, visual, allocate;
{
    GdkColormap *cmap  = gdk_colormap_new(get_gdkvisual(visual),
                                          RTEST(allocate));
    return make_gdkcmap(cmap);
}

static VALUE
gdkcmap_s_get_system(self)
    VALUE self;
{
    return make_gdkcmap(gdk_colormap_get_system());
}
 
/* obsoleted */
static VALUE
gdkcmap_alloc(self, color)
    VALUE self, color;
{
    gboolean result;
    rb_warn("Gdk::Colormap.alloc is obsoleted. Use Gdk::Colormap.alloc_color.");
    result = gdk_color_alloc(get_gdkcmap(self), get_gdkcolor(color));
    return result?Qtrue:Qfalse;
}

static VALUE
gdkcmap_alloc_color(self, color, writeable, best_match)
    VALUE self, color, writeable, best_match;
{
    gboolean result;
    GdkColor *c = get_gdkcolor(color);
    result = gdk_colormap_alloc_color(get_gdkcmap(self), c,
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

    cmap = get_gdkcmap(self);
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

/*
 * Gdk::Color
 */

VALUE gdkColor;

static VALUE
gdkcolor_s_new(self, r, g, b)
    VALUE self;
    VALUE r, g, b;
{
    GdkColor c;
    c.pixel = 0;
    c.red = NUM2INT(r);
    c.green = NUM2INT(g);
    c.blue = NUM2INT(b);
    return make_gdkcolor(&c);
}

static VALUE
gdkcolor_s_parse(self, name)
    VALUE self;
    VALUE name;
{
    GdkColor c;
    if (! gdk_color_parse(STR2CSTR(name), &c)) {
        rb_raise(rb_eArgError, "can't parse color name `%s'", STR2CSTR(name));
    }
    return make_gdkcolor(&c);
}

static VALUE
gdkcolor_pixel(self)
    VALUE self;
{
    return INT2NUM(get_gdkcolor(self)->pixel);
}

static VALUE
gdkcolor_red(self)
    VALUE self;
{
    return INT2FIX(get_gdkcolor(self)->red);
}

static VALUE
gdkcolor_set_red(self, red)
    VALUE self;
    VALUE red;
{
    get_gdkcolor(self)->red = NUM2INT(red);
    return red;
}

static VALUE
gdkcolor_green(self)
    VALUE self;
{
    return INT2FIX(get_gdkcolor(self)->green);
}

static VALUE
gdkcolor_set_green(self, green)
    VALUE self;
    VALUE green;
{
    get_gdkcolor(self)->green = NUM2INT(green);
    return green;
}

static VALUE
gdkcolor_blue(self)
    VALUE self;
{
    return INT2FIX(get_gdkcolor(self)->blue);
}

static VALUE
gdkcolor_set_blue(self, blue)
    VALUE self;
    VALUE blue;
{
    get_gdkcolor(self)->blue = NUM2INT(blue);
    return blue;
}

static VALUE
gdkcolor_to_a(self)
    VALUE self;
{
    GdkColor *c = get_gdkcolor(self);
    return rb_ary_new3(3, INT2FIX(c->red), INT2FIX(c->green), INT2FIX(c->blue));
}

/*
 * Gdk::ColorContext
 */
VALUE gdkColorContext;

void
Init_gtk_gdk_color()
{
    /* Gdk::Colormap */
    gdkColormap = rb_define_class_under(mGdk, "Colormap", rb_cData);

    rb_define_singleton_method(gdkColormap, "new", gdkcmap_s_new, 2);
    rb_define_singleton_method(gdkColormap, "get_system",
                               gdkcmap_s_get_system, 0);
    rb_define_method(gdkColormap, "alloc", gdkcmap_alloc, 1);
    rb_define_method(gdkColormap, "alloc_color", gdkcmap_alloc_color, 3);
    rb_define_method(gdkColormap, "colors", gdkcmap_colors, 0);

    /* Gdk::Color */
    gdkColor = rb_define_class_under(mGdk, "Color", rb_cData);

    rb_define_singleton_method(gdkColor, "new", gdkcolor_s_new, 3);
    rb_define_singleton_method(gdkColor, "parse", gdkcolor_s_parse, 1);
    rb_define_method(gdkColor, "pixel", gdkcolor_pixel, 0);
    rb_define_method(gdkColor, "red", gdkcolor_red, 0);
    rb_define_method(gdkColor, "red=", gdkcolor_set_red, 1);
    rb_define_method(gdkColor, "green", gdkcolor_green, 0);
    rb_define_method(gdkColor, "green=", gdkcolor_set_green, 1);
    rb_define_method(gdkColor, "blue", gdkcolor_blue, 0);
    rb_define_method(gdkColor, "blue=", gdkcolor_set_blue, 1);
    rb_define_method(gdkColor, "to_a", gdkcolor_to_a, 0);

    /* Gdk::ColorContext */
    gdkColorContext = rb_define_class_under(mGdk, "ColorContext", rb_cData);
}


/*
 * Gdk::Visual
 */

VALUE gdkVisual;

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
        rb_ary_push(ary, make_gdkvisual((GdkVisual *)cur->data));
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
    return make_gdkvisual(gdk_visual_get_system());
}

static VALUE
gdkvisual_s_get_best(self)
    VALUE self;
{
    return make_gdkvisual(gdk_visual_get_best());
}

static VALUE
gdkvisual_s_get_best_with_depth(self, depth)
    VALUE self, depth;
{
    return make_gdkvisual(gdk_visual_get_best_with_depth(NUM2INT(depth)));
}

static VALUE
gdkvisual_s_get_best_with_type(self, type)
    VALUE self, type;
{
    return make_gdkvisual(gdk_visual_get_best_with_depth(
                                    (GdkVisualType)NUM2INT(type)));
}

static VALUE
gdkvisual_s_get_best_with_both(self, depth, type)
    VALUE self, depth, type;
{
    return make_gdkvisual(gdk_visual_get_best_with_both(
                                    NUM2INT(depth),
                                    (GdkVisualType)NUM2INT(type)));
}

static VALUE
gdkvisual_type(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->type);
}

static VALUE
gdkvisual_depth(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->depth);
}

static VALUE
gdkvisual_byte_order(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->byte_order);
}

static VALUE
gdkvisual_colormap_size(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->colormap_size);
}

static VALUE
gdkvisual_bits_per_rgb(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->bits_per_rgb);
}

static VALUE
gdkvisual_red_mask(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->red_mask);
}

static VALUE
gdkvisual_red_shift(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->red_shift);
}

static VALUE
gdkvisual_red_prec(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->red_prec);
}

static VALUE
gdkvisual_green_mask(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->green_mask);
}

static VALUE
gdkvisual_green_shift(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->green_shift);
}

static VALUE
gdkvisual_green_prec(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->green_prec);
}

static VALUE
gdkvisual_blue_mask(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->blue_mask);
}

static VALUE
gdkvisual_blue_shift(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->blue_shift);
}

static VALUE
gdkvisual_blue_prec(self)
    VALUE self;
{
    return INT2FIX(get_gdkvisual(self)->blue_prec);
}

void
Init_gtk_gdk_visual()
{
    /* Gdk::Visual */
    gdkVisual = rb_define_class_under(mGdk, "Visual", rb_cData);

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

    rb_define_singleton_method(gdkVisual, "get_best_depth",
                               gdkvisual_s_get_best_depth, 0);
    rb_define_singleton_method(gdkVisual, "get_best_type",
                               gdkvisual_s_get_best_type, 0);

    rb_define_singleton_method(gdkVisual, "get_system",
                               gdkvisual_s_get_system, 0);
    rb_define_singleton_method(gdkVisual, "get_best",
                               gdkvisual_s_get_best, 0);
    rb_define_singleton_method(gdkVisual, "get_best_with_depth",
                               gdkvisual_s_get_best_with_depth, 1);
    rb_define_singleton_method(gdkVisual, "get_best_with_type",
                               gdkvisual_s_get_best_with_type, 1);
    rb_define_singleton_method(gdkVisual, "get_best_with_both",
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
