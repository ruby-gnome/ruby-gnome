/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkstyle.c -

  $Author: mutoh $
  $Date: 2003/05/23 18:47:57 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(w) GTK_STYLE(RVAL2GOBJ(w))
#define RVAL2REC(r) ((GdkRectangle*)RVAL2BOXED(r, GDK_TYPE_RECTANGLE))
#define RVAL2ICONSOURCE(s) ((GtkIconSource*)RVAL2BOXED(s, GTK_TYPE_ICON_SOURCE))

static VALUE
style_initialize(self)
    VALUE self;
{
    G_INITIALIZE(self, gtk_style_new());
    return Qnil;
}

static VALUE
style_attached(self)
    VALUE self;
{
    return (GTK_STYLE_ATTACHED(_SELF(self)) ? Qtrue : Qfalse);
}

static VALUE
style_copy(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_style_copy(_SELF(self)));
}

static VALUE
style_attach(self, win)
    VALUE self, win;
{
    return GOBJ2RVAL(gtk_style_attach(_SELF(self),
                                      GDK_WINDOW(RVAL2GOBJ(win))));
}

static VALUE
style_detach(self)
    VALUE self;
{
    gtk_style_detach(_SELF(self));
    return self;
}

static VALUE
style_set_background(self, win, state_type)
    VALUE self, win, state_type;
{
    gtk_style_set_background(_SELF(self), GDK_WINDOW(RVAL2GOBJ(win)),
                             (GtkStateType)NUM2INT(state_type));
    return self;
}

static VALUE
style_apply_default_background(self, gdkwindow, set_bg, state_type, area,
                               x, y, width, height)
    VALUE self, gdkwindow, set_bg, state_type, area, x, y, width, height;
{
    gtk_style_apply_default_background(_SELF(self), 
                                       GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                                       RTEST(set_bg), FIX2INT(state_type),
                                       RVAL2REC(area), 
                                       NUM2INT(x), NUM2INT(y), 
                                       NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_lookup_icon_set(self, stock_id)
    VALUE self, stock_id;
{
    return BOXED2RVAL(gtk_style_lookup_icon_set(_SELF(self), RVAL2CSTR(stock_id)),
                      GTK_TYPE_ICON_SET);
}

static VALUE
style_render_icon(self, source, direction, state, size, widget, detail)
    VALUE self, source, direction, state, size, widget, detail;
{
    return GOBJ2RVAL(gtk_style_render_icon(_SELF(self), RVAL2ICONSOURCE(source),
                                           FIX2INT(direction), FIX2INT(state),
                                           FIX2INT(size), 
                                           GTK_WIDGET(RVAL2GOBJ(widget)),
                                           RVAL2CSTR(detail)));
}

static VALUE
style_paint_arrow(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                  arrow_type, fill, x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    arrow_type, fill, x, y, width, height;
{
    gtk_paint_arrow(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    FIX2INT(arrow_type), RTEST(fill), NUM2INT(x), NUM2INT(y),
                    NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_box(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_box(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_box_gap(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                  x, y, width, height, gap_side, gap_x, gap_width)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height, gap_side, gap_x, gap_width;
{
    gtk_paint_box_gap(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                      FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                      GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                      NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                      FIX2INT(gap_side), NUM2INT(gap_x), NUM2INT(gap_width));
    return self;
}

static VALUE
style_paint_check(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                  x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_check(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_diamond(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                  x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_diamond(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_extension(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                      x, y, width, height, gap_side)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height, gap_side;
{
    gtk_paint_extension(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                        FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                        GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                        NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height), 
                        FIX2INT(gap_side));
    return self;
}

static VALUE
style_paint_flat_box(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                     x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_flat_box(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_focus(self, gdkwindow, state_type, area, widget, detail,
                  x, y, width, height)
    VALUE self, gdkwindow, state_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_focus(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_handle(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                  x, y, width, height, orientation)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height, orientation;
{
    gtk_paint_handle(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                     FIX2INT(orientation));
    return self;
}

static VALUE
style_paint_hline(self, gdkwindow, state_type, area, widget, detail, x1, x2, y)
    VALUE self, gdkwindow, state_type, area, widget, detail, x1, x2, y;
{
    gtk_paint_hline(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x1), NUM2INT(x2), NUM2INT(y));
    return self;
}

static VALUE
style_paint_option(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                   x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_option(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_polygon(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                    points, fill)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    points, fill;
{
    int i;
    GdkPoint* gpoints = g_new (GdkPoint, RARRAY(points)->len);

    for (i = 0; i < RARRAY(points)->len; i++){
        gpoints[i].x = RARRAY(RARRAY(points)->ptr[i])->ptr[0];
        gpoints[i].y = RARRAY(RARRAY(points)->ptr[i])->ptr[1];
    }
    gtk_paint_polygon(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                      GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail), gpoints,
                      RARRAY(points)->len, RTEST(fill));
    return self;
}

static VALUE
style_paint_shadow(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                   x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_shadow(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_shadow_gap(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                  x, y, width, height, gap_side, gap_x, gap_width)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height, gap_side, gap_x, gap_width;
{
    gtk_paint_shadow_gap(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                         FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                         GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                         NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                         FIX2INT(gap_side), NUM2INT(gap_x), NUM2INT(gap_width));
    return self;
}

static VALUE
style_paint_slider(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                   x, y, width, height, orientation)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height, orientation;
{
    gtk_paint_slider(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                     FIX2INT(orientation));
    return self;
}

static VALUE
style_paint_tab(self, gdkwindow, state_type, shadow_type, area, widget, detail,
                x, y, width, height)
    VALUE self, gdkwindow, state_type, shadow_type, area, widget, detail,
    x, y, width, height;
{
    gtk_paint_tab(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                  FIX2INT(state_type), FIX2INT(shadow_type), RVAL2REC(area),
                  GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                  NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
style_paint_vline(self, gdkwindow, state_type, area, widget, detail, y1, y2, x)
    VALUE self, gdkwindow, state_type, area, widget, detail, y1, y2, x;
{
    gtk_paint_vline(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    FIX2INT(state_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(y1), NUM2INT(y2), NUM2INT(x));
    return self;
}

static VALUE
style_paint_expander(self, gdkwindow, state_type, area, widget, detail,
                     x, y, expander_style)
    VALUE self, gdkwindow, state_type, area, widget, detail,
    x, y, expander_style;
{
    gtk_paint_expander(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                       FIX2INT(state_type), RVAL2REC(area),
                       GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                       NUM2INT(x), NUM2INT(y), FIX2INT(expander_style));
    return self;
}

static VALUE
style_paint_layout(self, gdkwindow, state_type, use_text, area, 
                   widget, detail, x, y, layout)
    VALUE self, gdkwindow, state_type, use_text, area, widget, detail,
    x, y, layout;
{
    gtk_paint_layout(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     FIX2INT(state_type), RTEST(use_text), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), PANGO_LAYOUT(RVAL2GOBJ(self)));
    return self;
}

static VALUE
style_paint_resize_grip(self, gdkwindow, state_type, area, widget, detail,
                        edge, x, y, width, height)
    VALUE self, gdkwindow, state_type, area, widget, detail, edge, x, y, 
    width, height;
{
    gtk_paint_resize_grip(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                          FIX2INT(state_type), RVAL2REC(area),
                          GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                          FIX2INT(edge), NUM2INT(x), NUM2INT(y), 
                          NUM2INT(width), NUM2INT(height));
    return self;
}

#define DEFINE_STYLE_COLOR(type) \
static VALUE \
style_set_ ## type(self, idx, r, g, b) \
    VALUE self, idx, r, g, b; \
{ \
  GdkColor *color; \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  color =  &(_SELF(self) -> type [i]); \
  color->red   = NUM2INT(r); \
  color->green = NUM2INT(g); \
  color->blue  = NUM2INT(b); \
  return self; \
} \
static VALUE \
style_ ## type(self, idx) \
    VALUE self, idx; \
{ \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  return BOXED2RVAL(&_SELF(self)-> type [i], GDK_TYPE_COLOR); \
} \

DEFINE_STYLE_COLOR(fg);
DEFINE_STYLE_COLOR(bg);
DEFINE_STYLE_COLOR(light);
DEFINE_STYLE_COLOR(dark);
DEFINE_STYLE_COLOR(mid);
DEFINE_STYLE_COLOR(text);
DEFINE_STYLE_COLOR(base);
DEFINE_STYLE_COLOR(text_aa);

static VALUE
style_black(self)
    VALUE self;
{
    return BOXED2RVAL(&_SELF(self)->black, GDK_TYPE_COLOR);
}

static VALUE
style_set_black(self, r, g, b)
    VALUE self, r, g, b;
{
   GdkColor* color =  &(_SELF(self)->black);
   color->red   = NUM2INT(r); 
   color->green = NUM2INT(g); 
   color->blue  = NUM2INT(b); 
   
   return self;
}

static VALUE
style_white(self)
    VALUE self;
{
    return BOXED2RVAL(&_SELF(self)->white, GDK_TYPE_COLOR);
}

static VALUE
style_set_white(self, r, g, b)
    VALUE self, r, g, b;
{
    GdkColor* color =  &(_SELF(self)->black);
    color->red   = NUM2INT(r); 
    color->green = NUM2INT(g); 
    color->blue  = NUM2INT(b); 
    return self;
}

static VALUE
style_font_desc(self)
    VALUE self;
{
    PangoFontDescription* desc = _SELF(self)->font_desc;
    return desc ? BOXED2RVAL(desc, PANGO_TYPE_FONT_DESCRIPTION) : Qnil;
}

static VALUE
style_set_font_desc(self, font_desc)
    VALUE self, font_desc;
{
    _SELF(self)->font_desc = (PangoFontDescription*)RVAL2BOXED(font_desc, PANGO_TYPE_FONT_DESCRIPTION);
    return self;
}

static VALUE
style_xthickness(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->xthickness);
}

static VALUE
style_set_xthickness(self, xthickness)
    VALUE self, xthickness;
{
    _SELF(self)->xthickness = NUM2INT(xthickness);
    return self;
}

static VALUE
style_ythickness(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->ythickness);
}

static VALUE
style_set_ythickness(self, ythickness)
    VALUE self, ythickness;
{
    _SELF(self)->ythickness = NUM2INT(ythickness);
    return self;
}

#define DEFINE_STYLE_GC(type) \
static VALUE \
style_set_ ## type(self, idx, gc) \
    VALUE self, idx, gc; \
{ \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  _SELF(self)-> type [i] = GDK_GC(RVAL2GOBJ(gc));\
  return self;\
} \
static VALUE \
style_ ## type(self, idx) \
    VALUE self, idx; \
{ \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  return GOBJ2RVAL(&_SELF(self)-> type [i]); \
} \

DEFINE_STYLE_GC(fg_gc);
DEFINE_STYLE_GC(bg_gc);
DEFINE_STYLE_GC(light_gc);
DEFINE_STYLE_GC(dark_gc);
DEFINE_STYLE_GC(mid_gc);
DEFINE_STYLE_GC(text_gc);
DEFINE_STYLE_GC(base_gc);
DEFINE_STYLE_GC(text_aa_gc);

static VALUE
style_black_gc(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->black_gc);
}

static VALUE
style_set_black_gc(self, gc)
    VALUE self, gc;
{
    _SELF(self)->black_gc = GDK_GC(RVAL2GOBJ(gc));
    return self;
}

static VALUE
style_white_gc(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->white_gc);
}

static VALUE
style_set_white_gc(self, gc)
    VALUE self, gc;
{
    _SELF(self)->white_gc = GDK_GC(RVAL2GOBJ(gc));
    return self;
}

static VALUE
style_bg_pixmap(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->bg_pixmap[i]);
}

static VALUE
style_set_bg_pixmap(self, idx, bg_pixmap)
    VALUE self, idx, bg_pixmap;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    _SELF(self)->bg_pixmap[i] = RVAL2GOBJ(bg_pixmap);
    return self;
}

void 
Init_gtk_style()
{
    VALUE gStyle = G_DEF_CLASS(GTK_TYPE_STYLE, "Style", mGtk);

    rb_define_method(gStyle, "initialize", style_initialize, 0);
    rb_define_method(gStyle, "attached?", style_attached, 0);
    rb_define_method(gStyle, "copy", style_copy, 0);
    rb_define_method(gStyle, "clone", style_copy, 0);
    rb_define_method(gStyle, "dup", style_copy, 0);
    rb_define_method(gStyle, "attach", style_attach, 1);
    rb_define_method(gStyle, "detach", style_detach, 0);
    rb_define_method(gStyle, "set_background", style_set_background, 2);
    rb_define_method(gStyle, "apply_default_background", style_apply_default_background, 8);
    rb_define_method(gStyle, "lookup_icon_set", style_lookup_icon_set, 1);
    rb_define_method(gStyle, "render_icon", style_render_icon, 6);
    rb_define_method(gStyle, "paint_arrow", style_paint_arrow, 12);
    rb_define_method(gStyle, "paint_box", style_paint_box, 10);
    rb_define_method(gStyle, "paint_box_gap", style_paint_box_gap, 13);
    rb_define_method(gStyle, "paint_check", style_paint_check, 10);
    rb_define_method(gStyle, "paint_diamond", style_paint_diamond, 10);
    rb_define_method(gStyle, "paint_extension", style_paint_extension, 10);
    rb_define_method(gStyle, "paint_flat_box", style_paint_flat_box, 10);
    rb_define_method(gStyle, "paint_focus", style_paint_focus, 9);
    rb_define_method(gStyle, "paint_handle", style_paint_handle, 11);
    rb_define_method(gStyle, "paint_hline", style_paint_hline, 8);
    rb_define_method(gStyle, "paint_option", style_paint_option, 10);
    rb_define_method(gStyle, "paint_polygon", style_paint_polygon, 8);
    rb_define_method(gStyle, "paint_shadow", style_paint_shadow, 10);
    rb_define_method(gStyle, "paint_shadow_gap", style_paint_shadow_gap, 13);
    rb_define_method(gStyle, "paint_slider", style_paint_slider, 11);
    rb_define_method(gStyle, "paint_tab", style_paint_tab, 10);
    rb_define_method(gStyle, "paint_vline", style_paint_vline, 9);
    rb_define_method(gStyle, "paint_expander", style_paint_expander, 8);
    rb_define_method(gStyle, "paint_layout", style_paint_layout, 9);
    rb_define_method(gStyle, "paint_resize_grip", style_paint_resize_grip, 10);
    
    rb_define_method(gStyle, "fg", style_fg, 1);
    rb_define_method(gStyle, "bg", style_bg, 1);
    rb_define_method(gStyle, "light", style_light, 1);
    rb_define_method(gStyle, "dark", style_dark, 1);
    rb_define_method(gStyle, "mid", style_mid, 1);
    rb_define_method(gStyle, "text", style_text, 1);
    rb_define_method(gStyle, "base", style_base, 1);
    rb_define_method(gStyle, "text_aa", style_text_aa, 1);
    rb_define_method(gStyle, "set_fg", style_set_fg, 4);
    rb_define_method(gStyle, "set_bg", style_set_bg, 4);
    rb_define_method(gStyle, "set_light", style_set_light, 4);
    rb_define_method(gStyle, "set_dark", style_set_dark, 4);
    rb_define_method(gStyle, "set_mid", style_set_mid, 4);
    rb_define_method(gStyle, "set_text", style_set_text, 4);
    rb_define_method(gStyle, "set_base", style_set_base, 4);
    rb_define_method(gStyle, "set_text_aa", style_set_text_aa, 4);

    rb_define_method(gStyle, "black", style_black, 0);
    rb_define_method(gStyle, "set_black", style_set_black, 3);
    rb_define_method(gStyle, "white", style_white, 0);
    rb_define_method(gStyle, "set_white", style_set_white, 3);
    rb_define_method(gStyle, "font_desc", style_font_desc, 0);
    rb_define_method(gStyle, "set_font_desc", style_set_font_desc, 1);
    rb_define_method(gStyle, "xthickness", style_xthickness, 0);
    rb_define_method(gStyle, "ythickness", style_ythickness, 0);
    rb_define_method(gStyle, "set_xthickness", style_set_xthickness, 1);
    rb_define_method(gStyle, "set_ythickness", style_set_ythickness, 1);

    rb_define_method(gStyle, "fg_gc", style_fg_gc, 1);
    rb_define_method(gStyle, "bg_gc", style_bg_gc, 1);
    rb_define_method(gStyle, "light_gc", style_light_gc, 1);
    rb_define_method(gStyle, "dark_gc", style_dark_gc, 1);
    rb_define_method(gStyle, "mid_gc", style_mid_gc, 1);
    rb_define_method(gStyle, "text_gc", style_text_gc, 1);
    rb_define_method(gStyle, "base_gc", style_base_gc, 1);
    rb_define_method(gStyle, "text_aa_gc", style_text_aa_gc, 1);
    rb_define_method(gStyle, "black_gc", style_black_gc, 0);
    rb_define_method(gStyle, "white_gc", style_white_gc, 0);
    rb_define_method(gStyle, "bg_pixmap", style_bg_pixmap, 1);
    rb_define_method(gStyle, "set_fg_gc", style_set_fg_gc, 2);
    rb_define_method(gStyle, "set_bg_gc", style_set_bg_gc, 2);
    rb_define_method(gStyle, "set_light_gc", style_set_light_gc, 2);
    rb_define_method(gStyle, "set_dark_gc", style_set_dark_gc, 2);
    rb_define_method(gStyle, "set_mid_gc", style_set_mid_gc, 2);
    rb_define_method(gStyle, "set_text_gc", style_set_text_gc, 2);
    rb_define_method(gStyle, "set_base_gc", style_set_base_gc, 2);
    rb_define_method(gStyle, "set_text_aa_gc", style_set_text_aa_gc, 2);
    rb_define_method(gStyle, "set_black_gc", style_set_black_gc, 1);
    rb_define_method(gStyle, "set_white_gc", style_set_white_gc, 1);
    rb_define_method(gStyle, "set_bg_pixmap", style_set_bg_pixmap, 2);

    G_DEF_SETTERS(gStyle);
}
