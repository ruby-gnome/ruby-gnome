/* -*- c-file-style: "ruby" -*- */
/************************************************

  rbgtkstyle.c -

  $Author: mutoh $
  $Date: 2002/05/19 15:48:28 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

/*
 * Style
 */
static VALUE
style_s_new(klass)
    VALUE klass;
{
    return make_gstyle(gtk_style_new());
}

static VALUE
style_copy(self)
    VALUE self;
{
    return make_gstyle(gtk_style_copy(get_gstyle(self)));
}

static VALUE
style_attach(self, win)
    VALUE self, win;
{
    return make_gstyle(gtk_style_attach(get_gstyle(self),
                                        get_gdkwindow(win)));
}

static VALUE
style_detach(self)
    VALUE self;
{
    gtk_style_detach(get_gstyle(self));
    return self;
}

static VALUE
style_set_background(self, win, state_type)
    VALUE self, win, state_type;
{
    gtk_style_set_background(get_gstyle(self), get_gdkwindow(win),
                             (GtkStateType)NUM2INT(state_type));
    return self;
}

static VALUE
style_fg(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkcolor(&get_gstyle(self)->fg[i]);
}

static VALUE
style_bg(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkcolor(&get_gstyle(self)->bg[i]);
}

static VALUE
style_light(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkcolor(&get_gstyle(self)->light[i]);
}

static VALUE
style_dark(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkcolor(&get_gstyle(self)->dark[i]);
}

static VALUE
style_mid(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkcolor(&get_gstyle(self)->mid[i]);
}

static VALUE
style_text(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkcolor(&get_gstyle(self)->text[i]);
}

static VALUE
style_base(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkcolor(&get_gstyle(self)->base[i]);
}

#define DEFINE_STYLE_SET_COLOR(func, type) \
static VALUE \
func(self, idx, r, g, b) \
    VALUE self, idx, r, g, b; \
{ \
  GtkStyle *style; \
  GdkColor *color; \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  style = get_gstyle(self); \
  if (style->fg_gc[0] != NULL) rb_raise(rb_eArgError, "you must not change widget style."); \
  color =  &(style-> type [i]); \
  color->red   = NUM2INT(r); \
  color->green = NUM2INT(g); \
  color->blue  = NUM2INT(b); \
  return(make_gdkcolor(color)); \
} \

DEFINE_STYLE_SET_COLOR(style_set_fg, fg)
    DEFINE_STYLE_SET_COLOR(style_set_bg, bg)
    DEFINE_STYLE_SET_COLOR(style_set_light, light)
    DEFINE_STYLE_SET_COLOR(style_set_dark, dark)
    DEFINE_STYLE_SET_COLOR(style_set_mid, mid)
    DEFINE_STYLE_SET_COLOR(style_set_text, text)
    DEFINE_STYLE_SET_COLOR(style_set_base, base)

static VALUE
style_black(self)
    VALUE self;
{
    return make_gdkcolor(&get_gstyle(self)->black);
}

static VALUE
style_white(self)
    VALUE self;
{
    return make_gdkcolor(&get_gstyle(self)->white);
}

static VALUE
style_font(self)
    VALUE self;
{
    return make_gdkfont(get_gstyle(self)->font);
}

static VALUE
style_set_font(self, f)
    VALUE self, f;
{
    GtkStyle *style = get_gstyle(self);
    GdkFont *font = get_gdkfont(f);

    if (font == NULL)
        rb_raise(rb_eArgError, "Invalid or unknown font.");
    if (style->fg_gc[0] != NULL)
        rb_raise(rb_eArgError, "you must not change widget style.");
    if (style->font != NULL)
        gdk_font_unref(style->font);

    gdk_font_ref(font);
    style->font = font;

    return self;
}

static VALUE
style_fg_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkgc(get_gstyle(self)->fg_gc[i]);
}

static VALUE
style_bg_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkgc(get_gstyle(self)->bg_gc[i]);
}

static VALUE
style_light_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkgc(get_gstyle(self)->light_gc[i]);
}

static VALUE
style_dark_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkgc(get_gstyle(self)->dark_gc[i]);
}

static VALUE
style_mid_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkgc(get_gstyle(self)->mid_gc[i]);
}

static VALUE
style_text_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkgc(get_gstyle(self)->text_gc[i]);
}

static VALUE
style_base_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkgc(get_gstyle(self)->base_gc[i]);
}

static VALUE
style_black_gc(self)
    VALUE self;
{
    return make_gdkgc(get_gstyle(self)->black_gc);
}

static VALUE
style_white_gc(self)
    VALUE self;
{
    return make_gdkgc(get_gstyle(self)->white_gc);
}

static VALUE
style_bg_pixmap(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return make_gdkpixmap(get_gstyle(self)->bg_pixmap[i]);
}


static VALUE
style_draw_hline(self, win, state_type, x1, x2, y)
    VALUE self, win, state_type, x1, x2, y;
{
    gtk_draw_hline(get_gstyle(self), get_gdkwindow(win),
                   (GtkStateType)NUM2INT(state_type),
                   NUM2INT(x1), NUM2INT(x2), NUM2INT(y));
    return self;
}

static VALUE
style_draw_vline(self, win, state_type, y1, y2, x)
    VALUE self, win, state_type, y1, y2, x;
{
    gtk_draw_vline(get_gstyle(self), get_gdkwindow(win),
                   (GtkStateType)NUM2INT(state_type),
                   NUM2INT(y1), NUM2INT(y2), NUM2INT(x));
    return self;
}

static VALUE
style_draw_shadow(self,win,state_type,shadow_type,x,y,w,h)
    VALUE self, win, state_type, shadow_type, x, y, w, h;
{
    gtk_draw_shadow(get_gstyle(self), get_gdkwindow(win),
                    (GtkStateType)NUM2INT(state_type),
                    (GtkShadowType)NUM2INT(shadow_type),
                    NUM2INT(x), NUM2INT(y), 
                    NUM2INT(w), NUM2INT(h));
    return self;
}

static VALUE
style_draw_polygon(self,win,state_type,shadow_type,pnts,fill)
    VALUE self, win, state_type, shadow_type, pnts, fill;
{
    GdkPoint *points;
    int i;

    Check_Type(pnts, T_ARRAY);
    points = ALLOCA_N(GdkPoint,RARRAY(pnts)->len);
    for (i=0; i<RARRAY(pnts)->len; i++) {
        Check_Type(RARRAY(pnts)->ptr[i], T_ARRAY);

        if (RARRAY(RARRAY(pnts)->ptr[i])->len < 2)
            rb_raise(rb_eArgError, "point %d should be array of size 2", i);

        points[i].x = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[0]);
        points[i].y = NUM2INT(RARRAY(RARRAY(pnts)->ptr[i])->ptr[1]);
    }

    gtk_draw_polygon(get_gstyle(self), get_gdkwindow(win),
                     (GtkStateType)NUM2INT(state_type),
                     (GtkShadowType)NUM2INT(shadow_type),
                     points, RARRAY(pnts)->len,
                     RTEST(fill));
    return self;
}

#if 0
static VALUE
style_draw_arrow(self,)     /* 9 */
{
    gtk_draw_polygon(get_gstyle(self), get_gdkwindow(win),
             (GtkStateType)NUM2INT(state_type),
             (GtkShadowType)NUM2INT(shadow_type),
             points, RARRAY(pnts)->len,
             RTEST(fill));
    return self;
}

static VALUE
style_draw_diamond(self,)   /* 7 */
{
}

static VALUE
style_draw_oval(self,)      /* 7 */
{
}

static VALUE
style_draw_string(self,)    /* 5 */
{
}
#endif

void Init_gtk_style()
{
    gStyle = rb_define_class_under(mGtk, "Style", rb_cData);

    rb_define_singleton_method(gStyle, "new", style_s_new, 0);
    rb_define_method(gStyle, "copy", style_copy, 0);
    rb_define_method(gStyle, "clone", style_copy, 0);
    rb_define_method(gStyle, "dup", style_copy, 0);
    rb_define_method(gStyle, "attach", style_attach, 1);
    rb_define_method(gStyle, "detach", style_detach, 0);
    rb_define_method(gStyle, "set_background", style_set_background, 2);
    rb_define_method(gStyle, "fg", style_fg, 1);
    rb_define_method(gStyle, "bg", style_bg, 1);
    rb_define_method(gStyle, "light", style_light, 1);
    rb_define_method(gStyle, "dark", style_dark, 1);
    rb_define_method(gStyle, "mid", style_mid, 1);
    rb_define_method(gStyle, "text", style_text, 1);
    rb_define_method(gStyle, "base", style_base, 1);
    rb_define_method(gStyle, "set_fg", style_set_fg, 4);
    rb_define_method(gStyle, "set_bg", style_set_bg, 4);
    rb_define_method(gStyle, "set_light", style_set_light, 4);
    rb_define_method(gStyle, "set_dark", style_set_dark, 4);
    rb_define_method(gStyle, "set_mid", style_set_mid, 4);
    rb_define_method(gStyle, "set_text", style_set_text, 4);
    rb_define_method(gStyle, "set_base", style_set_base, 4);

    rb_define_method(gStyle, "black", style_black, 0);
    rb_define_method(gStyle, "white", style_white, 0);
    rb_define_method(gStyle, "font", style_font, 0);
    rb_define_method(gStyle, "set_font", style_set_font, 1);
    rb_define_method(gStyle, "fg_gc", style_fg_gc, 1);
    rb_define_method(gStyle, "bg_gc", style_bg_gc, 1);
    rb_define_method(gStyle, "light_gc", style_light_gc, 1);
    rb_define_method(gStyle, "dark_gc", style_dark_gc, 1);
    rb_define_method(gStyle, "mid_gc", style_mid_gc, 1);
    rb_define_method(gStyle, "text_gc", style_text_gc, 1);
    rb_define_method(gStyle, "base_gc", style_base_gc, 1);
    rb_define_method(gStyle, "black_gc", style_black_gc, 0);
    rb_define_method(gStyle, "white_gc", style_white_gc, 0);
    rb_define_method(gStyle, "bg_pixmap", style_bg_pixmap, 1);
    rb_define_method(gStyle, "draw_hline", style_draw_hline, 5);
    rb_define_method(gStyle, "draw_vline", style_draw_vline, 5);
    rb_define_method(gStyle, "draw_shadow", style_draw_shadow, 7);
    rb_define_method(gStyle, "draw_polygon", style_draw_polygon, 6);
#if 0
    rb_define_method(gStyle, "draw_arrow", style_draw_arrow, 9);
    rb_define_method(gStyle, "draw_diamond", style_draw_diamond, 7);
    rb_define_method(gStyle, "draw_oval", style_draw_oval, 7);
    rb_define_method(gStyle, "draw_string", style_draw_string, 5);
#endif
}
