/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkwidget.c -

  $Author: mutoh $
  $Date: 2003/05/22 17:06:05 $

  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_WIDGET(RVAL2GOBJ(self)))

static VALUE
widget_get_flags(self)
    VALUE self;
{
    return INT2NUM(GTK_WIDGET_FLAGS(_SELF(self)));
}

static VALUE
widget_set_flags(self, flag)
    VALUE self;
{
    GTK_WIDGET_SET_FLAGS(_SELF(self), NUM2INT(flag));
    return self;
}

static VALUE
widget_unset_flags(self, flag)
    VALUE self;
{
    GTK_WIDGET_UNSET_FLAGS(_SELF(self), NUM2INT(flag));
    return self;
}


static VALUE
widget_unparent(self)
    VALUE self;
{
    gtk_widget_unparent(_SELF(self));
    return self;
}

static VALUE
widget_show(self)
    VALUE self;
{
    gtk_widget_show(_SELF(self));
    return self;
}

static VALUE
widget_show_now(self)
    VALUE self;
{
    gtk_widget_show_now(_SELF(self));
    return self;
}

static VALUE
widget_hide(self)
    VALUE self;
{
    gtk_widget_hide(_SELF(self));
    return self;
}

static VALUE
widget_show_all(self)
    VALUE self;
{
    gtk_widget_show_all(_SELF(self));
    return self;
}

static VALUE
widget_hide_all(self)
    VALUE self;
{
    gtk_widget_hide_all(_SELF(self));
    return self;
}

static VALUE
widget_map(self)
    VALUE self;
{
    gtk_widget_map(_SELF(self));
    return self;
}

static VALUE
widget_unmap(self)
    VALUE self;
{
    gtk_widget_unmap(_SELF(self));
    return self;
}

static VALUE
widget_realize(self)
    VALUE self;
{
    gtk_widget_realize(_SELF(self));
    return self;
}

static VALUE
widget_unrealize(self)
    VALUE self;
{
    gtk_widget_unrealize(_SELF(self));
    return self;
}

static VALUE
widget_queue_draw(self)
    VALUE self;
{
    gtk_widget_queue_draw(_SELF(self));
    return self;
}

static VALUE
widget_queue_resize(self)
    VALUE self;
{
    gtk_widget_queue_resize(_SELF(self));
    return self;
}

/* Note this method is not
   gtk_widget_get_size_request */
static VALUE
widget_size_request(self)
    VALUE self;
{
    GtkRequisition req;
    gtk_widget_size_request(_SELF(self), &req);
    return rb_ary_new3(2, INT2NUM(req.width), INT2NUM(req.height));
}

static VALUE
widget_get_child_requisition(self)
    VALUE self;
{
    GtkRequisition req;

    gtk_widget_get_child_requisition(_SELF(self), &req);
    return rb_ary_new3(2, INT2NUM(req.width), INT2NUM(req.height));
}

static VALUE
widget_size_allocate(self, alloc)
    VALUE self, alloc;
{
    gtk_widget_size_allocate(_SELF(self), (GtkAllocation*)RVAL2BOXED(alloc, GTK_TYPE_ALLOCATION));
    return self;
}

static VALUE
widget_add_accelerator(self, sig, accel, key, mod, flag)
	VALUE self, sig, accel, key, mod, flag;
{
    gtk_widget_add_accelerator(_SELF(self),
                               RVAL2CSTR(sig),
                               GTK_ACCEL_GROUP(RVAL2GOBJ(accel)),
                               NUM2INT(key),
                               NUM2INT(mod),
                               NUM2INT(flag));
    return self;
}

static VALUE
widget_remove_accelerator(self, accel, key, mod)
    VALUE self, accel, key, mod;
{
    return gtk_widget_remove_accelerator(_SELF(self),
                                         GTK_ACCEL_GROUP(RVAL2GOBJ(accel)),
                                         NUM2INT(key),
                                         NUM2INT(mod)) ? Qtrue : Qfalse;
}

static VALUE
widget_set_accel_path(self, accel_path, accel_group)
    VALUE self, accel_path, accel_group;
{
    gtk_widget_set_accel_path(_SELF(self), RVAL2CSTR(accel_path), 
                              GTK_ACCEL_GROUP(RVAL2GOBJ(accel_group)));
    return self;
}

static VALUE
widget_list_accel_closures(self)
    VALUE self;
{
    return GLIST2ARY2(gtk_widget_list_accel_closures(_SELF(self)), G_TYPE_CLOSURE);
}

static VALUE
widget_event(self, event)
    VALUE self, event;
{
    return gtk_widget_event(_SELF(self), RVAL2GEV(event)) ? Qtrue : Qfalse;
}

static VALUE
widget_activate(self)
    VALUE self;
{
    return gtk_widget_activate(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
widget_reparent(self, parent)
    VALUE self, parent;
{
    gtk_widget_reparent(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
widget_intersect(self, area)
    VALUE self, area;
{
    GdkRectangle intersection;
    gboolean ret = gtk_widget_intersect(_SELF(self),
                                        (GdkRectangle*)RVAL2BOXED(area, GDK_TYPE_RECTANGLE),
                                        &intersection);
    return ret ? BOXED2RVAL(&intersection, GDK_TYPE_RECTANGLE) : Qnil;
}

static VALUE
widget_is_focus(self)
    VALUE self;
{
    return gtk_widget_is_focus(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
widget_grab_focus(self)
    VALUE self;
{
    gtk_widget_grab_focus(_SELF(self));
    return self;
}

static VALUE
widget_grab_default(self)
    VALUE self;
{
    gtk_widget_grab_default(_SELF(self));
    return self;
}

static VALUE
widget_set_state(self, state)
    VALUE self, state;
{
    gtk_widget_set_state(_SELF(self), (GtkStateType)NUM2INT(state));
    return self;
}

static VALUE
widget_set_parent_window(self, parent_window)
    VALUE self, parent_window;
{
    gtk_widget_set_parent_window(_SELF(self), 
                                 GDK_WINDOW(RVAL2GOBJ(parent_window)));
    return self;
}

static VALUE
widget_get_parent_window(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_parent_window(_SELF(self)));
}

static VALUE
widget_add_events(self, events)
    VALUE self, events;
{
    gtk_widget_add_events(_SELF(self), NUM2INT(events));
    return self;
}

static VALUE
widget_get_toplevel(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_toplevel(_SELF(self)));
}

static VALUE
widget_get_ancestor(self, klass)
    VALUE self, klass;
{
    return GOBJ2RVAL(gtk_widget_get_ancestor(_SELF(self), CLASS2GTYPE(klass)));
}

static VALUE
widget_get_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_colormap(_SELF(self)));
}

static VALUE
widget_set_colormap(self, colormap)
    VALUE self, colormap;
{
    gtk_widget_set_colormap(_SELF(self), GDK_COLORMAP(RVAL2GOBJ(self)));
    return self;
}

static VALUE
widget_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_visual(_SELF(self)));
}

static VALUE
widget_get_pointer(self)
    VALUE self;
{
    int x, y;

    gtk_widget_get_pointer(_SELF(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

static VALUE
widget_is_ancestor(self, ancestor)
    VALUE self, ancestor;
{
    return (gtk_widget_is_ancestor(_SELF(self), _SELF(ancestor)) ? Qtrue : Qfalse);
}

static VALUE
widget_translate_coordinates(self, dest_widget, src_x, src_y)
    VALUE self, dest_widget, src_x, src_y;
{
    gint dest_x, dest_y;
    gboolean ret;
    VALUE result = Qnil;
    ret = gtk_widget_translate_coordinates(_SELF(self), _SELF(dest_widget),
                                           NUM2INT(src_x), NUM2INT(src_y),
                                           &dest_x, &dest_y);
    if (ret)
        result = rb_ary_new3(2, INT2FIX(dest_x), INT2FIX(dest_y));

    return result;
}

static VALUE
widget_hide_on_delete(self)
	VALUE self;
{
    return gtk_widget_hide_on_delete(_SELF(self)) ? Qtrue: Qfalse;
}

static VALUE
widget_ensure_style(self)
    VALUE self;
{
    gtk_widget_ensure_style(_SELF(self));
    return self;
}

static VALUE
widget_reset_rc_styles(self)
    VALUE self;
{
    gtk_widget_reset_rc_styles(_SELF(self));
    return self;
}

static VALUE
widget_s_push_colormap(self, cmap)
    VALUE self, cmap;
{
    gtk_widget_push_colormap(GDK_COLORMAP(RVAL2GOBJ(cmap)));
    return cmap;
}

static VALUE
widget_s_pop_colormap(self)
    VALUE self;
{
    gtk_widget_pop_colormap();
    return self;
}

static VALUE
widget_s_set_default_colormap(self, cmap)
    VALUE self, cmap;
{
    gtk_widget_set_default_colormap(GDK_COLORMAP(RVAL2GOBJ(cmap)));
    return cmap;
}

static VALUE
widget_s_get_default_style(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_default_style());
}

static VALUE
widget_s_get_default_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_default_colormap());
}

static VALUE
widget_s_get_default_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_default_visual());
}

static VALUE
widget_set_direction(self, dir)
    VALUE self, dir;
{
    gtk_widget_set_direction(_SELF(self), FIX2INT(dir));
    return self;
}

static VALUE
widget_get_direction(self)
    VALUE self;
{
    return INT2FIX(gtk_widget_get_direction(_SELF(self)));
}

static VALUE
widget_s_set_default_direction(self, dir)
    VALUE self;
{
    gtk_widget_set_default_direction(FIX2INT(dir));
    return self;
}

static VALUE
widget_s_get_default_direction(self)
    VALUE self;
{
    return INT2FIX(gtk_widget_get_default_direction());
}

static VALUE
widget_shape_combine_mask(self, shape_mask, offset_x, offset_y)
    VALUE self, shape_mask, offset_x, offset_y;
{
    gtk_widget_shape_combine_mask(_SELF(self),
                                  GDK_BITMAP(RVAL2GOBJ(shape_mask)),
                                  NUM2INT(offset_x),
                                  NUM2INT(offset_y));
    return self;
}

static VALUE
widget_path(self)
    VALUE self;
{
    guint path_length;
    gchar *path;
    gchar *path_reversed;
    VALUE str_path;
    VALUE str_path_reversed;

    gtk_widget_path(_SELF(self), &path_length, &path, &path_reversed);
    str_path = CSTR2RVAL(path);
    str_path_reversed = CSTR2RVAL(path_reversed);
    g_free(path);
    g_free(path_reversed);
    return rb_ary_new3(2, str_path, str_path_reversed);
}

static VALUE
widget_class_path(self)
    VALUE self;
{
    guint path_length;
    gchar *path;
    gchar *path_reversed;
    VALUE str_path;
    VALUE str_path_reversed;

    gtk_widget_class_path(_SELF(self), &path_length, &path, &path_reversed);
    str_path = CSTR2RVAL(path);
    str_path_reversed = CSTR2RVAL(path_reversed);
    g_free(path);
    g_free(path_reversed);
    return rb_ary_new3(2, str_path, str_path_reversed);
}

static VALUE
widget_get_composite_name(self)
    VALUE self;
{
    return CSTR2RVAL(gtk_widget_get_composite_name(_SELF(self)));
}

static VALUE
widget_modify_style(self, style)
    VALUE self, style;
{
    gtk_widget_modify_style(_SELF(self),
                            GTK_RC_STYLE(RVAL2GOBJ(style)));
    return self;
}

static VALUE
widget_get_modifier_style(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_modifier_style(_SELF(self)));
}

static VALUE
widget_modify_fg(self, state, color)
    VALUE self, state, color;
{
    gtk_widget_modify_fg(_SELF(self), FIX2INT(state), 
                         (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
    return self;
}

static VALUE
widget_modify_bg(self, state, color)
    VALUE self, state, color;
{
    gtk_widget_modify_bg(_SELF(self), FIX2INT(state), 
                         (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
    return self;
}

static VALUE
widget_modify_text(self, state, color)
    VALUE self, state, color;
{
    gtk_widget_modify_text(_SELF(self), FIX2INT(state), 
                         (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
    return self;
}

static VALUE
widget_modify_base(self, state, color)
    VALUE self, state, color;
{
    gtk_widget_modify_base(_SELF(self), FIX2INT(state), 
                         (GdkColor*)RVAL2BOXED(color, GDK_TYPE_COLOR));
    return self;
}

static VALUE
widget_modify_font(self, font_desc)
    VALUE self, font_desc;
{
    gtk_widget_modify_font(_SELF(self), 
                           (PangoFontDescription*)RVAL2BOXED(font_desc, PANGO_TYPE_FONT_DESCRIPTION));
    return self;
}

static VALUE
widget_create_pango_context(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_create_pango_context(_SELF(self)));
}

static VALUE
widget_get_pango_context(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_pango_context(_SELF(self)));
}

static VALUE
widget_create_pango_layout(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
    VALUE text;
    rb_scan_args(argc, argv, "01", &text);
    return GOBJ2RVAL(gtk_widget_create_pango_layout(_SELF(self), 
                                                    NIL_P(text) ? NULL : RVAL2CSTR(text)));
}

static VALUE
widget_render_icon(self, stock_id, size, detail)
    VALUE self, stock_id, size, detail;
{
    return GOBJ2RVAL(gtk_widget_render_icon(_SELF(self),  rb_id2name(SYM2ID(stock_id)),
                                            FIX2INT(size), RVAL2CSTR(detail)));
}

static VALUE
widget_s_pop_composite_child(self)
    VALUE self;
{
    gtk_widget_pop_composite_child();
    return self;
}

static VALUE
widget_s_push_composite_child(self)
    VALUE self;
{
    gtk_widget_push_composite_child();
    return self;
}

static VALUE
widget_queue_draw_area(self, x, y, width, height)
    VALUE self, x, y, width, height;
{
    gtk_widget_queue_draw_area(_SELF(self), NUM2INT(x), NUM2INT(y),
                               NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
widget_reset_shapes(self)
    VALUE self;
{
    gtk_widget_reset_shapes(_SELF(self));
    return self;
}

static VALUE
widget_set_double_buffered(self, double_buffered)
    VALUE self, double_buffered;
{
    gtk_widget_set_double_buffered(_SELF(self), RTEST(double_buffered));
    return self;
}

static VALUE
widget_set_redraw_on_allocate(self, redraw_on_allocate)
    VALUE self, redraw_on_allocate;
{
    gtk_widget_set_redraw_on_allocate(_SELF(self), RTEST(redraw_on_allocate));
    return self;
}

static VALUE
widget_set_composite_name(self, name)
    VALUE self, name;
{
    gtk_widget_set_composite_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
widget_set_scroll_adjustments(self, h, v)
    VALUE self, h, v;
{
    gboolean res = gtk_widget_set_scroll_adjustments(_SELF(self),
                                                     GTK_ADJUSTMENT(_SELF(h)),
                                                     GTK_ADJUSTMENT(_SELF(v)));
    return res ? Qtrue : Qfalse;
}

static VALUE
widget_mnemonic_activate(self, group_cycling)
    VALUE self, group_cycling;
{
    return gtk_widget_mnemonic_activate(_SELF(self), RTEST(group_cycling)) ? Qtrue : Qfalse;
}

/* Can we implement this?
static VALUE
widget_class_install_style_property(self, pspec)
    VALUE self, pspec;
{
    gtk_widget_class_install_style_property((GtkWidgetClass*)G_OBJECT_GET_CLASS(_SELF(self)), 
                                            RVAL2GOBJ(pspec));
    return self;
}

void        gtk_widget_class_install_style_property_parser
                                            (GtkWidgetClass *klass,
                                             GParamSpec *pspec,
                                             GtkRcPropertyParser parser);
GParamSpec* gtk_widget_class_find_style_property
                                            (GtkWidgetClass *klass,
                                             const gchar *property_name);
Is this exist in GTK+-2.2?
static VALUE
widget_style_properties(self)
    VALUE self;
{
    GtkWidgetClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    gint n_properties;
    GParamSpec** props;
    VALUE ary;
    int i;

    props = gtk_widget_class_list_style_properties(oclass, &n_properties);

    ary = rb_ary_new2(n_properties);
    for (i = 0; i < n_properties; i++)
        rb_ary_store(ary, i, rb_str_new2(props[i]->name));

    g_type_class_unref(oclass);
    return ary;
}
*/

static VALUE
widget_region_intersect(self, region)
    VALUE self, region;
{
    return BOXED2RVAL(gtk_widget_region_intersect(_SELF(self), 
                                                  (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION)), 
                      GDK_TYPE_REGION);
}

static VALUE
widget_send_expose(self, event)
    VALUE self, event;
{
    return INT2NUM(gtk_widget_send_expose(_SELF(self), RVAL2GEV(event)));
}

/* They are needless method for ruby.
void        gtk_widget_style_get            (GtkWidget *widget,
                                             const gchar *first_property_name,
                                             ...);
void        gtk_widget_style_get_valist     (GtkWidget *widget,
                                             const gchar *first_property_name,
                                             va_list var_args);
*/

static VALUE
widget_style_get_property(self, property_name)
    VALUE self, property_name;
{
    GValue value = {0, };
    gtk_widget_style_get_property(_SELF(self), RVAL2CSTR(property_name),
                                  &value);
    return G_VALUE_TYPE(&value) != G_TYPE_INVALID ? GVAL2RVAL(&value) : Qnil;
}

static VALUE
widget_get_accessible(self)
    VALUE self;
{
    return GOBJ2RVAL(_SELF(self));
}

static VALUE
widget_child_focus(self, direction)
    VALUE self, direction;
{
    return gtk_widget_child_focus(_SELF(self), NUM2INT(direction)) ? Qtrue : Qfalse;
}

static VALUE
widget_child_notify(self, child_property)
    VALUE self, child_property;
{
    gtk_widget_child_notify(_SELF(self), RVAL2CSTR(child_property));
    return self;
}

static VALUE
widget_freeze_child_notify(self)
    VALUE self;
{
    gtk_widget_freeze_child_notify(_SELF(self));
    return self;
}

static VALUE
widget_get_child_visible(self)
    VALUE self;
{
    return gtk_widget_get_child_visible(_SELF(self)) ? Qtrue : Qfalse;
}

static VALUE
widget_get_settings(self)
    VALUE self;
{
    return GOBJ2RVAL(gtk_widget_get_settings(_SELF(self)));
}

/*
  Note this method is not gtk_widget_size_request()
*/
static VALUE
widget_get_size_request(self)
    VALUE self;
{
    gint width, height;
    gtk_widget_get_size_request(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
widget_set_child_visible(self, is_visible)
    VALUE self, is_visible;
{
    gtk_widget_set_child_visible(_SELF(self), RTEST(is_visible));
    return self;
}

static VALUE
widget_set_size_request(self, width, height)
    VALUE self, width, height;
{
    gtk_widget_set_size_request(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
widget_thaw_child_notify(self)
    VALUE self;
{
    gtk_widget_thaw_child_notify(_SELF(self));
    return self;
}

static VALUE
widget_window(self)
    VALUE self;
{
    GdkWindow* window = _SELF(self)->window;
    return window ? GOBJ2RVAL(window) : Qnil;
}

#define DEFINE_IS_WIDGET(STATE) \
static VALUE \
widget_ ## STATE (self) \
    VALUE self; \
{ \
    return( GTK_WIDGET_ ## STATE (_SELF(self))? Qtrue: Qfalse ); \
}
DEFINE_IS_WIDGET(TOPLEVEL);
DEFINE_IS_WIDGET(NO_WINDOW);
DEFINE_IS_WIDGET(REALIZED);
DEFINE_IS_WIDGET(MAPPED);
/*DEFINE_IS_WIDGET(VISIBLE); VISIBLE is defined as property*/
DEFINE_IS_WIDGET(DRAWABLE);
DEFINE_IS_WIDGET(PARENT_SENSITIVE);
DEFINE_IS_WIDGET(IS_SENSITIVE);
DEFINE_IS_WIDGET(HAS_GRAB);
DEFINE_IS_WIDGET(RC_STYLE);
DEFINE_IS_WIDGET(DOUBLE_BUFFERED);

static VALUE
widget_get_allocation(self)
    VALUE self;
{
    return BOXED2RVAL(&(_SELF(self)->allocation), GTK_TYPE_ALLOCATION);
}

static VALUE
widget_set_allocation(self, x,y,w,h)
    VALUE self, x,y,w,h;
{
    GtkAllocation *a = &(_SELF(self)->allocation);
    a->x      = NUM2INT(x);
    a->y      = NUM2INT(y);
    a->width  = NUM2INT(w);
    a->height = NUM2INT(h);
    return self;
}

static VALUE
widget_get_requisition(self)
    VALUE self;
{
    GtkRequisition req = _SELF(self)->requisition;
    return rb_ary_new3(2, INT2NUM(req.width), INT2NUM(req.height));
}

static VALUE
widget_state(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->state);
}

static VALUE
widget_saved_state(self)
    VALUE self;
{
    return INT2FIX(_SELF(self)->saved_state);
}

static VALUE
widget_signal_size_request(num, values)
    guint num;
    const GValue* values;
{
    GtkRequisition* req = (GtkRequisition*)g_value_get_boxed(&values[1]);
    return rb_ary_new3(2, GVAL2RVAL(&values[0]), 
                       rb_ary_new3(2, INT2NUM(req->width), INT2NUM(req->height)));
}
static VALUE
widget_signal_size_allocate(num, values)
    guint num;
    const GValue* values;
{
    GtkAllocation* alloc = (GtkAllocation*)g_value_get_boxed(&values[1]);
    return rb_ary_new3(2, GVAL2RVAL(&values[0]), BOXED2RVAL(alloc, GTK_TYPE_ALLOCATION));
}
void 
Init_gtk_widget()
{
    VALUE gWidget = G_DEF_CLASS(GTK_TYPE_WIDGET, "Widget", mGtk);

    /*
     * instance methods
     */
    rb_define_method(gWidget, "flags", widget_get_flags, 0);
    rb_define_method(gWidget, "set_flags", widget_set_flags, 1);
    rb_define_method(gWidget, "unset_flags", widget_unset_flags, 1);
    rb_define_method(gWidget, "unparent", widget_unparent, 0);
    rb_define_method(gWidget, "show", widget_show, 0);
    rb_define_method(gWidget, "show_now", widget_show_now, 0);
    rb_define_method(gWidget, "hide", widget_hide, 0);
    rb_define_method(gWidget, "show_all", widget_show_all, 0);
    rb_define_method(gWidget, "hide_all", widget_hide_all, 0);
    rb_define_method(gWidget, "map", widget_map, 0);
    rb_define_method(gWidget, "unmap", widget_unmap, 0);
    rb_define_method(gWidget, "realize", widget_realize, 0);
    rb_define_method(gWidget, "unrealize", widget_unrealize, 0);
    rb_define_method(gWidget, "queue_draw", widget_queue_draw, 0);
    rb_define_method(gWidget, "queue_resize", widget_queue_resize, 0);
    rb_define_method(gWidget, "size_request", widget_size_request, 0);
    rb_define_method(gWidget, "child_requisition", widget_get_child_requisition, 0);
    rb_define_method(gWidget, "size_allocate", widget_size_allocate, 1);
    rb_define_method(gWidget, "add_accelerator", widget_add_accelerator, 5);
    rb_define_method(gWidget, "remove_accelerator", widget_remove_accelerator, 3);
    rb_define_method(gWidget, "set_accel_path", widget_set_accel_path, 2);
    rb_define_method(gWidget, "list_accel_closures", widget_list_accel_closures, 0);
    rb_define_method(gWidget, "event", widget_event, 1);
    rb_define_method(gWidget, "activate", widget_activate, 0);
    rb_define_method(gWidget, "reparent", widget_reparent, 1);
/*
    rb_define_method(gWidget, "style_properties", widget_style_properties, 0);
*/
    rb_define_method(gWidget, "intersect", widget_intersect, 1);
    rb_define_method(gWidget, "focus?", widget_is_focus, 0);
    rb_define_method(gWidget, "grab_focus", widget_grab_focus, 0);
    rb_define_method(gWidget, "grab_default", widget_grab_default, 0);
    rb_define_method(gWidget, "set_state", widget_set_state, 1);
    rb_define_method(gWidget, "set_parent_window", widget_set_parent_window, 1);
    rb_define_method(gWidget, "parent_window", widget_get_parent_window, 0);
    rb_define_method(gWidget, "add_events", widget_add_events, 1);
    rb_define_method(gWidget, "toplevel", widget_get_toplevel, 0);
    rb_define_method(gWidget, "get_ancestor", widget_get_ancestor, 1);
    rb_define_method(gWidget, "colormap", widget_get_colormap, 0);
    rb_define_method(gWidget, "set_colormap", widget_set_colormap, 1);
    rb_define_method(gWidget, "visual", widget_get_visual, 0);
    rb_define_method(gWidget, "pointer", widget_get_pointer, 0);
    rb_define_method(gWidget, "ancestor?", widget_is_ancestor, 1);
    rb_define_method(gWidget, "translate_coordinates", widget_translate_coordinates, 3);
    rb_define_method(gWidget, "hide_on_delete", widget_hide_on_delete, 0);
    rb_define_method(gWidget, "ensure_style", widget_ensure_style, 0);
    rb_define_method(gWidget, "reset_rc_styles", widget_reset_rc_styles, 0);
    rb_define_method(gWidget, "set_direction", widget_set_direction, 1);
    rb_define_method(gWidget, "direction", widget_get_direction, 0);
    rb_define_method(gWidget, "shape_combine_mask", widget_shape_combine_mask, 3);
    rb_define_method(gWidget, "path", widget_path, 0);
    rb_define_method(gWidget, "class_path", widget_class_path, 0);
    rb_define_method(gWidget, "composite_name", widget_get_composite_name, 0);
    rb_define_method(gWidget, "modify_style", widget_modify_style, 1);
    rb_define_method(gWidget, "modifier_style", widget_get_modifier_style, 0);
    rb_define_method(gWidget, "modify_fg", widget_modify_fg, 2);
    rb_define_method(gWidget, "modify_bg", widget_modify_bg, 2);
    rb_define_method(gWidget, "modify_text", widget_modify_text, 2);
    rb_define_method(gWidget, "modify_base", widget_modify_base, 2);
    rb_define_method(gWidget, "modify_font", widget_modify_font, 1);
    rb_define_method(gWidget, "create_pango_context", widget_create_pango_context, 0);
    rb_define_method(gWidget, "pango_context", widget_get_pango_context, 0);
    rb_define_method(gWidget, "create_pango_layout", widget_create_pango_layout, -1);
    rb_define_method(gWidget, "render_icon", widget_render_icon, 3);
    rb_define_method(gWidget, "queue_draw_area", widget_queue_draw_area, 4);
    rb_define_method(gWidget, "reset_shapes", widget_reset_shapes, 0);
    rb_define_method(gWidget, "set_double_buffered", widget_set_double_buffered, 1);
    rb_define_method(gWidget, "set_redraw_on_allocate", widget_set_redraw_on_allocate, 1);
    rb_define_method(gWidget, "set_composite_name", widget_set_composite_name, 1);
    rb_define_method(gWidget, "set_scroll_adjustment", widget_set_scroll_adjustments, 2);
    rb_define_method(gWidget, "mnemonic_activate", widget_mnemonic_activate, 1);
    rb_define_method(gWidget, "region_intersect", widget_region_intersect, 1);
    rb_define_method(gWidget, "send_expose", widget_send_expose, 1);
    rb_define_method(gWidget, "style_get_property", widget_style_get_property, 1);
    rb_define_method(gWidget, "accessible", widget_get_accessible, 0);
    rb_define_method(gWidget, "child_focus", widget_child_focus, 1);
    rb_define_method(gWidget, "child_notify", widget_child_notify, 1);
    rb_define_method(gWidget, "freeze_child_notify", widget_freeze_child_notify, 0);
    rb_define_method(gWidget, "child_visible?", widget_get_child_visible, 0);
    rb_define_method(gWidget, "settings", widget_get_settings, 0);
    rb_define_method(gWidget, "set_child_visible", widget_set_child_visible, 1);
    rb_define_method(gWidget, "get_size_request", widget_get_size_request, 0);
    rb_define_method(gWidget, "set_size_request", widget_set_size_request, 2);
    rb_define_method(gWidget, "thaw_child_notify", widget_thaw_child_notify, 0);
    rb_define_method(gWidget, "window", widget_window, 0);
    rb_define_method(gWidget, "allocation", widget_get_allocation, 0);
    rb_define_method(gWidget, "set_allocation", widget_set_allocation, 4);
    rb_define_method(gWidget, "requisition", widget_get_requisition, 0);
    rb_define_method(gWidget, "state", widget_state, 0);
    rb_define_method(gWidget, "saved_state", widget_saved_state, 0);

    rb_define_method(gWidget, "toplevel?",  widget_TOPLEVEL, 0);
    rb_define_method(gWidget, "no_window?", widget_NO_WINDOW, 0);
    rb_define_method(gWidget, "realized?",  widget_REALIZED, 0);
    rb_define_method(gWidget, "mapped?",    widget_MAPPED, 0); 
    rb_define_method(gWidget, "drawable?",  widget_DRAWABLE, 0);
    rb_define_method(gWidget, "parent_sensitive?", widget_PARENT_SENSITIVE, 0);
    /* This method's name avoid to sensitive? of a property variables. */
    rb_define_method(gWidget, "sensitive_with_parent?",   widget_IS_SENSITIVE, 0);
    rb_define_method(gWidget, "has_grab?",    widget_HAS_GRAB, 0);
    rb_define_method(gWidget, "rc_style?",    widget_RC_STYLE, 0);
    rb_define_method(gWidget, "double_buffered?",  widget_DOUBLE_BUFFERED, 0);

    /*
     * singleton methods
     */
    rb_define_singleton_method(gWidget, "push_colormap", widget_s_push_colormap, 1);
    rb_define_singleton_method(gWidget, "pop_colormap", widget_s_pop_colormap, 0);
    rb_define_singleton_method(gWidget, "set_default_colormap", widget_s_set_default_colormap, 1);
    rb_define_singleton_method(gWidget, "default_style", widget_s_get_default_style, 0);
    rb_define_singleton_method(gWidget, "default_colormap", widget_s_get_default_colormap, 0);
    rb_define_singleton_method(gWidget, "default_visual", widget_s_get_default_visual, 0);
    rb_define_singleton_method(gWidget, "set_default_direction",widget_s_set_default_direction, 1);
    rb_define_singleton_method(gWidget, "default_direction",widget_s_get_default_direction, 0);
    rb_define_singleton_method(gWidget, "pop_composite_child",widget_s_pop_composite_child, 0);
    rb_define_singleton_method(gWidget, "push_composite_child",widget_s_push_composite_child, 0);

    G_DEF_SETTERS(gWidget);

    /*
     * constants
     */
    /* GtkWidgetFlags */
    rb_define_const(gWidget, "TOPLEVEL", INT2NUM(GTK_TOPLEVEL));
    rb_define_const(gWidget, "NO_WINDOW", INT2NUM(GTK_NO_WINDOW));
    rb_define_const(gWidget, "REALIZED", INT2NUM(GTK_REALIZED));
    rb_define_const(gWidget, "MAPPED", INT2NUM(GTK_MAPPED));
    rb_define_const(gWidget, "VISIBLE", INT2NUM(GTK_VISIBLE));
    rb_define_const(gWidget, "SENSITIVE", INT2NUM(GTK_SENSITIVE));
    rb_define_const(gWidget, "PARENT_SENSITIVE", INT2NUM(GTK_PARENT_SENSITIVE));
    rb_define_const(gWidget, "CAN_FOCUS", INT2NUM(GTK_CAN_FOCUS));
    rb_define_const(gWidget, "HAS_FOCUS", INT2NUM(GTK_HAS_FOCUS));
    rb_define_const(gWidget, "CAN_DEFAULT", INT2NUM(GTK_CAN_DEFAULT));
    rb_define_const(gWidget, "HAS_DEFAULT", INT2NUM(GTK_HAS_DEFAULT));
    rb_define_const(gWidget, "HAS_GRAB", INT2NUM(GTK_HAS_GRAB));
    rb_define_const(gWidget, "RC_STYLE", INT2NUM(GTK_RC_STYLE));
    rb_define_const(gWidget, "COMPOSITE_CHILD", INT2NUM(GTK_COMPOSITE_CHILD));
    rb_define_const(gWidget, "APP_PAINTABLE", INT2NUM(GTK_APP_PAINTABLE));
    rb_define_const(gWidget, "RECEIVES_DEFAULT", INT2NUM(GTK_RECEIVES_DEFAULT));
    rb_define_const(gWidget, "DOUBLE_BUFFERED", INT2NUM(GTK_DOUBLE_BUFFERED));

    /* GtkWidgetHelpType */
    rb_define_const(gWidget, "HELP_TOOLTIP", INT2FIX(GTK_WIDGET_HELP_TOOLTIP));
    rb_define_const(gWidget, "HELP_WHATS_THIS", INT2FIX(GTK_WIDGET_HELP_WHATS_THIS));

    /* GtkTextDirection */
    rb_define_const(gWidget, "TEXT_DIR_NONE", INT2FIX(GTK_TEXT_DIR_NONE));
    rb_define_const(gWidget, "TEXT_DIR_LTR", INT2FIX(GTK_TEXT_DIR_LTR));
    rb_define_const(gWidget, "TEXT_DIR_RTL", INT2FIX(GTK_TEXT_DIR_RTL));

    /* Special signals */
    G_DEF_SIGNAL_FUNC(gWidget, "size-request", widget_signal_size_request);
    G_DEF_SIGNAL_FUNC(gWidget, "size-allocate", widget_signal_size_allocate);
    
}
