/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkstyle.c -

  $Author: mutoh $
  $Date: 2002/10/26 16:04:04 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(w) GTK_STYLE(RVAL2GOBJ(w))

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

/*
void        gtk_style_apply_default_background
                                            (GtkStyle *style,
                                             GdkWindow *window,
                                             gboolean set_bg,
                                             GtkStateType state_type,
                                             GdkRectangle *area,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
GtkIconSet* gtk_style_lookup_icon_set       (GtkStyle *style,
                                             const gchar *stock_id);
GdkPixbuf*  gtk_style_render_icon           (GtkStyle *style,
                                             const GtkIconSource *source,
                                             GtkTextDirection direction,
                                             GtkStateType state,
                                             GtkIconSize size,
                                             GtkWidget *widget,
                                             const gchar *detail);
void        gtk_paint_arrow                 (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             GtkArrowType arrow_type,
                                             gboolean fill,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_box                   (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_box_gap               (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height,
                                             GtkPositionType gap_side,
                                             gint gap_x,
                                             gint gap_width);
void        gtk_paint_check                 (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_diamond               (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_extension             (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height,
                                             GtkPositionType gap_side);
void        gtk_paint_flat_box              (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_focus                 (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_handle                (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height,
                                             GtkOrientation orientation);
void        gtk_paint_hline                 (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x1,
                                             gint x2,
                                             gint y);
void        gtk_paint_option                (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_polygon               (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             GdkPoint *points,
                                             gint npoints,
                                             gboolean fill);
void        gtk_paint_shadow                (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_shadow_gap            (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height,
                                             GtkPositionType gap_side,
                                             gint gap_x,
                                             gint gap_width);
void        gtk_paint_slider                (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height,
                                             GtkOrientation orientation);
void        gtk_paint_tab                   (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GtkShadowType shadow_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);
void        gtk_paint_vline                 (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint y1,
                                             gint y2,
                                             gint x);
void        gtk_paint_expander              (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             GtkExpanderStyle expander_style);
void        gtk_paint_layout                (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             gboolean use_text,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             gint x,
                                             gint y,
                                             PangoLayout *layout);
void        gtk_paint_resize_grip           (GtkStyle *style,
                                             GdkWindow *window,
                                             GtkStateType state_type,
                                             GdkRectangle *area,
                                             GtkWidget *widget,
                                             const gchar *detail,
                                             GdkWindowEdge edge,
                                             gint x,
                                             gint y,
                                             gint width,
                                             gint height);

struct      GtkBorder;
GtkBorder*  gtk_border_copy                 (const GtkBorder *border);
void        gtk_border_free                 (GtkBorder *border);

struct      GtkRcProperty;
gboolean    (*GtkRcPropertyParser)          (const GParamSpec *pspec,
                                             const GString *rc_string,
                                             GValue *property_value);
*/


static VALUE
style_fg(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return BOXED2RVAL(&_SELF(self)->fg[i], GDK_TYPE_COLOR);
}

static VALUE
style_bg(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return BOXED2RVAL(&_SELF(self)->bg[i], GDK_TYPE_COLOR);
}

static VALUE
style_light(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return BOXED2RVAL(&_SELF(self)->light[i], GDK_TYPE_COLOR);
}

static VALUE
style_dark(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return BOXED2RVAL(&_SELF(self)->dark[i], GDK_TYPE_COLOR);
}

static VALUE
style_mid(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return BOXED2RVAL(&_SELF(self)->mid[i], GDK_TYPE_COLOR);
}

static VALUE
style_text(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return BOXED2RVAL(&_SELF(self)->text[i], GDK_TYPE_COLOR);
}

static VALUE
style_base(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return BOXED2RVAL(&_SELF(self)->base[i], GDK_TYPE_COLOR);
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
  style = _SELF(self); \
  if (style->fg_gc[0] != NULL) rb_raise(rb_eArgError, "you must not change widget style."); \
  color =  &(style-> type [i]); \
  color->red   = NUM2INT(r); \
  color->green = NUM2INT(g); \
  color->blue  = NUM2INT(b); \
  return(BOXED2RVAL(color, GDK_TYPE_COLOR)); \
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
    return BOXED2RVAL(&_SELF(self)->black, GDK_TYPE_COLOR);
}

static VALUE
style_white(self)
    VALUE self;
{
    return BOXED2RVAL(&_SELF(self)->white, GDK_TYPE_COLOR);
}

/*
PangoFontDescription *font_desc;
  gint xthickness;
  gint ythickness;
 */

static VALUE
style_fg_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->fg_gc[i]);
}

static VALUE
style_bg_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->bg_gc[i]);
}

static VALUE
style_light_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->light_gc[i]);
}

static VALUE
style_dark_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->dark_gc[i]);
}

static VALUE
style_mid_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->mid_gc[i]);
}

static VALUE
style_text_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->text_gc[i]);
}

static VALUE
style_base_gc(self, idx)
    VALUE self, idx;
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->base_gc[i]);
}

/*
GdkGC *text_aa_gc[5];
*/

static VALUE
style_black_gc(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->black_gc);
}

static VALUE
style_white_gc(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self)->white_gc);
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
}
