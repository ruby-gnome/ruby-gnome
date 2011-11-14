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

#define RG_TARGET_NAMESPACE cStyle
#define _SELF(w) GTK_STYLE(RVAL2GOBJ(w))
#define RVAL2REC(r) (NIL_P(r) ? NULL : (GdkRectangle*)RVAL2BOXED(r, GDK_TYPE_RECTANGLE))
#define RVAL2ICONSOURCE(s) ((GtkIconSource*)RVAL2BOXED(s, GTK_TYPE_ICON_SOURCE))
#define RVAL2STATE(s) (RVAL2GENUM(s, GTK_TYPE_STATE_TYPE))
#define RVAL2SHADOW(s) (RVAL2GENUM(s, GTK_TYPE_SHADOW_TYPE))
#define RVAL2ARROW(s) (RVAL2GENUM(s, GTK_TYPE_ARROW_TYPE))
#define RVAL2GAP(s) (RVAL2GENUM(s, GTK_TYPE_POSITION_TYPE))
#define RVAL2ORI(s) (RVAL2GENUM(s, GTK_TYPE_ORIENTATION))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_style_new());
    return Qnil;
}

static VALUE
rg_attached_p(VALUE self)
{
    return CBOOL2RVAL(GTK_STYLE_ATTACHED(_SELF(self)));
}

static VALUE
rg_copy(VALUE self)
{
    return GOBJ2RVAL(gtk_style_copy(_SELF(self)));
}

static VALUE
rg_attach(VALUE self, VALUE win)
{
    return GOBJ2RVAL(gtk_style_attach(_SELF(self),
                                      GDK_WINDOW(RVAL2GOBJ(win))));
}

static VALUE
rg_detach(VALUE self)
{
    gtk_style_detach(_SELF(self));
    return self;
}

static VALUE
rg_set_background(VALUE self, VALUE win, VALUE state_type)
{
    gtk_style_set_background(_SELF(self), GDK_WINDOW(RVAL2GOBJ(win)),
                             RVAL2STATE(state_type));
    return self;
}

static VALUE
rg_apply_default_background(VALUE self, VALUE gdkwindow, VALUE set_bg, VALUE state_type, VALUE area, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_style_apply_default_background(_SELF(self), 
                                       GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                                       RVAL2CBOOL(set_bg), RVAL2STATE(state_type),
                                       RVAL2REC(area), 
                                       NUM2INT(x), NUM2INT(y), 
                                       NUM2INT(width), NUM2INT(height));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
rg_lookup_color(VALUE self, VALUE color_name)
{
    GdkColor color;
    if (gtk_style_lookup_color(_SELF(self), RVAL2CSTR(color_name), &color)){
        return GDKCOLOR2RVAL(&color);
    } else {
        return Qnil;
    }
}
#endif

static VALUE
rg_lookup_icon_set(VALUE self, VALUE stock_id)
{
    const gchar *id;
    GtkIconSet *ret;
    if (TYPE(stock_id) == T_STRING) {
        id = RVAL2CSTR(stock_id);
    } else {
        id = rb_id2name(SYM2ID(stock_id));
    }

    ret = gtk_style_lookup_icon_set(_SELF(self), id);
    return BOXED2RVAL(ret, GTK_TYPE_ICON_SET);
}

static VALUE
rg_render_icon(VALUE self, VALUE source, VALUE direction, VALUE state, VALUE size, VALUE widget, VALUE detail)
{
    return GOBJ2RVAL(gtk_style_render_icon(_SELF(self), RVAL2ICONSOURCE(source),
                                           RVAL2GENUM(direction, GTK_TYPE_TEXT_DIRECTION), 
                                           RVAL2STATE(state),
                                           RVAL2GENUM(size, GTK_TYPE_ICON_SIZE), 
                                           GTK_WIDGET(RVAL2GOBJ(widget)),
                                           RVAL2CSTR(detail)));
}

static VALUE
rg_paint_arrow(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE arrow_type, VALUE fill, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_arrow(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    RVAL2ARROW(arrow_type), RVAL2CBOOL(fill), NUM2INT(x), NUM2INT(y),
                    NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_box(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_box(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_box_gap(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height, VALUE gap_side, VALUE gap_x, VALUE gap_width)
{
    gtk_paint_box_gap(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                      RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                      GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                      NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                      RVAL2GAP(gap_side), NUM2INT(gap_x), NUM2INT(gap_width));
    return self;
}

static VALUE
rg_paint_check(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_check(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_diamond(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_diamond(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_extension(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height, VALUE gap_side)
{
    gtk_paint_extension(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                        RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                        GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                        NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height), 
                        RVAL2GAP(gap_side));
    return self;
}

static VALUE
rg_paint_flat_box(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_flat_box(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_focus(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_focus(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_handle(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height, VALUE orientation)
{
    gtk_paint_handle(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                     RVAL2ORI(orientation));
    return self;
}

static VALUE
rg_paint_hline(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE area, VALUE widget, VALUE detail, VALUE x1, VALUE x2, VALUE y)
{
    gtk_paint_hline(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), RVAL2CSTR(detail),
                    NUM2INT(x1), NUM2INT(x2), NUM2INT(y));
    return self;
}

static VALUE
rg_paint_option(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_option(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), 
                     NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_polygon(VALUE self, VALUE rbwindow, VALUE rbstate_type,
                    VALUE rbshadow_type, VALUE rbarea, VALUE rbwidget,
                    VALUE rbdetail, VALUE rbpoints, VALUE rbfill)
{
    GtkStyle *style = _SELF(self);
    GdkWindow *window = GDK_WINDOW(RVAL2GOBJ(rbwindow));
    GtkStateType state_type = RVAL2STATE(rbstate_type);
    GtkShadowType shadow_type = RVAL2SHADOW(rbshadow_type);
    GdkRectangle *area = RVAL2REC(rbarea);
    GtkWidget *widget = GTK_WIDGET(RVAL2GOBJ(rbwidget));
    const gchar *detail = RVAL2CSTR_ACCEPT_NIL(rbdetail);
    gboolean fill = RVAL2CBOOL(rbfill);
    long n;
    GdkPoint *points = RVAL2GDKPOINTS(rbpoints, &n);

    gtk_paint_polygon(style, window, state_type, shadow_type, area, widget, detail, points, n, fill);

    g_free(points);

    return self;
}

static VALUE
rg_paint_shadow(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_shadow(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), 
                     NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_shadow_gap(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height, VALUE gap_side, VALUE gap_x, VALUE gap_width)
{
    gtk_paint_shadow_gap(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                         RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                         GTK_WIDGET(RVAL2GOBJ(widget)), 
                         NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                         NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                         RVAL2GAP(gap_side), NUM2INT(gap_x), NUM2INT(gap_width));
    return self;
}

static VALUE
rg_paint_slider(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height, VALUE orientation)
{
    gtk_paint_slider(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), 
                     NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height),
                     RVAL2ORI(orientation));
    return self;
}

static VALUE
rg_paint_tab(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE shadow_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_tab(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                  RVAL2STATE(state_type), RVAL2SHADOW(shadow_type), RVAL2REC(area),
                  GTK_WIDGET(RVAL2GOBJ(widget)), 
                  NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                  NUM2INT(x), NUM2INT(y), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_paint_vline(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE area, VALUE widget, VALUE detail, VALUE y1, VALUE y2, VALUE x)
{
    gtk_paint_vline(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                    RVAL2STATE(state_type), RVAL2REC(area),
                    GTK_WIDGET(RVAL2GOBJ(widget)), 
                    NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                    NUM2INT(y1), NUM2INT(y2), NUM2INT(x));
    return self;
}

static VALUE
rg_paint_expander(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE expander_style)
{
    gtk_paint_expander(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                       RVAL2STATE(state_type), RVAL2REC(area),
                       GTK_WIDGET(RVAL2GOBJ(widget)), 
                       NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                       NUM2INT(x), NUM2INT(y), RVAL2GENUM(expander_style, GTK_TYPE_EXPANDER_STYLE));
    return self;
}

static VALUE
rg_paint_layout(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE use_text, VALUE area, VALUE widget, VALUE detail, VALUE x, VALUE y, VALUE layout)
{
    gtk_paint_layout(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                     RVAL2STATE(state_type), RVAL2CBOOL(use_text), RVAL2REC(area),
                     GTK_WIDGET(RVAL2GOBJ(widget)), 
                     NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                     NUM2INT(x), NUM2INT(y), PANGO_LAYOUT(RVAL2GOBJ(layout)));
    return self;
}

static VALUE
rg_paint_resize_grip(VALUE self, VALUE gdkwindow, VALUE state_type, VALUE area, VALUE widget, VALUE detail, VALUE edge, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_paint_resize_grip(_SELF(self), GDK_WINDOW(RVAL2GOBJ(gdkwindow)),
                          RVAL2STATE(state_type), RVAL2REC(area),
                          GTK_WIDGET(RVAL2GOBJ(widget)), 
                          NIL_P(detail) ? NULL : RVAL2CSTR(detail),
                          RVAL2GENUM(edge, GDK_TYPE_WINDOW_EDGE), 
                          NUM2INT(x), NUM2INT(y), 
                          NUM2INT(width), NUM2INT(height));
    return self;
}
#if GTK_CHECK_VERSION(2,4,0)
static VALUE
rg_s_draw_insertion_cursor(VALUE self, VALUE widget, VALUE drawable, VALUE area, VALUE location, VALUE is_primary, VALUE direction, VALUE draw_arrow)
{
    gtk_draw_insertion_cursor(GTK_WIDGET(RVAL2GOBJ(widget)),
                              GDK_WINDOW(RVAL2GOBJ(drawable)),
                              RVAL2REC(area), RVAL2REC(location),
                              RVAL2CBOOL(is_primary), 
                              RVAL2GENUM(direction, GTK_TYPE_TEXT_DIRECTION),
                              RVAL2CBOOL(draw_arrow));
    return self;
}
#endif

#define DEFINE_STYLE_COLOR(type) \
static VALUE \
style_set_ ## type(VALUE self, VALUE idx, VALUE r, VALUE g, VALUE b) \
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
style_ ## type(VALUE self, VALUE idx) \
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
rg_black(VALUE self)
{
    return BOXED2RVAL(&_SELF(self)->black, GDK_TYPE_COLOR);
}

static VALUE
rg_set_black(VALUE self, VALUE r, VALUE g, VALUE b)
{
   GdkColor* color =  &(_SELF(self)->black);
   color->red   = NUM2INT(r); 
   color->green = NUM2INT(g); 
   color->blue  = NUM2INT(b); 

   return self;
}

static VALUE
rg_white(VALUE self)
{
    return BOXED2RVAL(&_SELF(self)->white, GDK_TYPE_COLOR);
}

static VALUE
rg_set_white(VALUE self, VALUE r, VALUE g, VALUE b)
{
    GdkColor* color =  &(_SELF(self)->white);
    color->red   = NUM2INT(r); 
    color->green = NUM2INT(g); 
    color->blue  = NUM2INT(b); 
    return self;
}

static VALUE
rg_font_desc(VALUE self)
{
    PangoFontDescription* desc = _SELF(self)->font_desc;
    return BOXED2RVAL(desc, PANGO_TYPE_FONT_DESCRIPTION);
}

static VALUE
rg_set_font_desc(VALUE self, VALUE font_desc)
{
    GtkStyle *style = _SELF(self);
    if (style->font_desc)
        pango_font_description_free(style->font_desc);
    style->font_desc = pango_font_description_copy((PangoFontDescription*)RVAL2BOXED(font_desc, PANGO_TYPE_FONT_DESCRIPTION));
    return self;
}

static VALUE
rg_xthickness(VALUE self)
{
    return INT2NUM(_SELF(self)->xthickness);
}

static VALUE
rg_set_xthickness(VALUE self, VALUE xthickness)
{
    _SELF(self)->xthickness = NUM2INT(xthickness);
    return self;
}

static VALUE
rg_ythickness(VALUE self)
{
    return INT2NUM(_SELF(self)->ythickness);
}

static VALUE
rg_set_ythickness(VALUE self, VALUE ythickness)
{
    _SELF(self)->ythickness = NUM2INT(ythickness);
    return self;
}

#define DEFINE_STYLE_GC(type) \
static VALUE \
style_set_ ## type(VALUE self, VALUE idx, VALUE gc) \
{ \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  _SELF(self)-> type [i] = GDK_GC(RVAL2GOBJ(gc));\
  return self;\
} \
static VALUE \
style_ ## type(VALUE self, VALUE idx) \
{ \
  int i = NUM2INT(idx); \
 \
  if (i < 0 || 5 < i) rb_raise(rb_eArgError, "state out of range"); \
  return GOBJ2RVAL(_SELF(self)-> type [i]); \
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
rg_black_gc(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->black_gc);
}

static VALUE
rg_set_black_gc(VALUE self, VALUE gc)
{
    _SELF(self)->black_gc = GDK_GC(RVAL2GOBJ(gc));
    return self;
}

static VALUE
rg_white_gc(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->white_gc);
}

static VALUE
rg_set_white_gc(VALUE self, VALUE gc)
{
    _SELF(self)->white_gc = GDK_GC(RVAL2GOBJ(gc));
    return self;
}

static VALUE
rg_bg_pixmap(VALUE self, VALUE idx)
{
    int i = NUM2INT(idx);

    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    return GOBJ2RVAL(_SELF(self)->bg_pixmap[i]);
}

static VALUE
rg_set_bg_pixmap(VALUE self, VALUE idx, VALUE bg_pixmap)
{
    int i = NUM2INT(idx);
    GdkPixmap* pixmap = RVAL2GOBJ(bg_pixmap);
    g_object_ref(G_OBJECT(pixmap));
    if (i < 0 || 5 < i)
        rb_raise(rb_eArgError, "state out of range");
    _SELF(self)->bg_pixmap[i] = pixmap;
    return self;
}

void 
Init_gtk_style(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_STYLE, "Style", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD_P(attached, 0);
    RG_DEF_METHOD(copy, 0);
    RG_DEF_ALIAS("clone", "copy");
    RG_DEF_ALIAS("dup", "copy");
    RG_DEF_METHOD(attach, 1);
    RG_DEF_METHOD(detach, 0);
    RG_DEF_METHOD(set_background, 2);
    RG_DEF_METHOD(apply_default_background, 8);
#if GTK_CHECK_VERSION(2,10,0)
    RG_DEF_METHOD(lookup_color, 1);
#endif
    RG_DEF_METHOD(lookup_icon_set, 1);
    RG_DEF_METHOD(render_icon, 6);
    RG_DEF_METHOD(paint_arrow, 12);
    RG_DEF_METHOD(paint_box, 10);
    RG_DEF_METHOD(paint_box_gap, 13);
    RG_DEF_METHOD(paint_check, 10);
    RG_DEF_METHOD(paint_diamond, 10);
    RG_DEF_METHOD(paint_extension, 10);
    RG_DEF_METHOD(paint_flat_box, 10);
    RG_DEF_METHOD(paint_focus, 9);
    RG_DEF_METHOD(paint_handle, 11);
    RG_DEF_METHOD(paint_hline, 8);
    RG_DEF_METHOD(paint_option, 10);
    RG_DEF_METHOD(paint_polygon, 8);
    RG_DEF_METHOD(paint_shadow, 10);
    RG_DEF_METHOD(paint_shadow_gap, 13);
    RG_DEF_METHOD(paint_slider, 11);
    RG_DEF_METHOD(paint_tab, 10);
    RG_DEF_METHOD(paint_vline, 9);
    RG_DEF_METHOD(paint_expander, 8);
    RG_DEF_METHOD(paint_layout, 9);
    RG_DEF_METHOD(paint_resize_grip, 10);
#if GTK_CHECK_VERSION(2,4,0)
    RG_DEF_SMETHOD(draw_insertion_cursor, 7);
#endif
    rb_define_method(RG_TARGET_NAMESPACE, "fg", style_fg, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "bg", style_bg, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "light", style_light, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "dark", style_dark, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "mid", style_mid, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "text", style_text, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "base", style_base, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "text_aa", style_text_aa, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_fg", style_set_fg, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_bg", style_set_bg, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_light", style_set_light, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_dark", style_set_dark, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_mid", style_set_mid, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_text", style_set_text, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_base", style_set_base, 4);
    rb_define_method(RG_TARGET_NAMESPACE, "set_text_aa", style_set_text_aa, 4);

    RG_DEF_METHOD(black, 0);
    RG_DEF_METHOD(set_black, 3);
    RG_DEF_METHOD(white, 0);
    RG_DEF_METHOD(set_white, 3);
    RG_DEF_METHOD(font_desc, 0);
    RG_DEF_METHOD(set_font_desc, 1);
    RG_DEF_METHOD(xthickness, 0);
    RG_DEF_METHOD(ythickness, 0);
    RG_DEF_METHOD(set_xthickness, 1);
    RG_DEF_METHOD(set_ythickness, 1);

    rb_define_method(RG_TARGET_NAMESPACE, "fg_gc", style_fg_gc, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "bg_gc", style_bg_gc, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "light_gc", style_light_gc, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "dark_gc", style_dark_gc, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "mid_gc", style_mid_gc, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "text_gc", style_text_gc, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "base_gc", style_base_gc, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "text_aa_gc", style_text_aa_gc, 1);
    RG_DEF_METHOD(black_gc, 0);
    RG_DEF_METHOD(white_gc, 0);
    RG_DEF_METHOD(bg_pixmap, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_fg_gc", style_set_fg_gc, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_bg_gc", style_set_bg_gc, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_light_gc", style_set_light_gc, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_dark_gc", style_set_dark_gc, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_mid_gc", style_set_mid_gc, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_text_gc", style_set_text_gc, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_base_gc", style_set_base_gc, 2);
    rb_define_method(RG_TARGET_NAMESPACE, "set_text_aa_gc", style_set_text_aa_gc, 2);
    RG_DEF_METHOD(set_black_gc, 1);
    RG_DEF_METHOD(set_white_gc, 1);
    RG_DEF_METHOD(set_bg_pixmap, 2);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
