/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkwidget.c -

  $Author: ggc $
  $Date: 2007/07/13 16:07:32 $

  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(self) (GTK_WIDGET(RVAL2GOBJ(self)))

static VALUE style_prop_func_table;

static VALUE
widget_get_flags(VALUE self)
{
    return GFLAGS2RVAL(GTK_WIDGET_FLAGS(_SELF(self)), GTK_TYPE_WIDGET_FLAGS);
}

static VALUE
widget_set_flags(VALUE self, VALUE flag)
{
    GTK_WIDGET_SET_FLAGS(_SELF(self), RVAL2GFLAGS(flag, GTK_TYPE_WIDGET_FLAGS));
    return self;
}

static VALUE
widget_unset_flags(VALUE self, VALUE flag)
{
    GTK_WIDGET_UNSET_FLAGS(_SELF(self), RVAL2GFLAGS(flag, GTK_TYPE_WIDGET_FLAGS));
    return self;
}


static VALUE
widget_unparent(VALUE self)
{
    gtk_widget_unparent(_SELF(self));
    return self;
}

static VALUE
widget_show(VALUE self)
{
    gtk_widget_show(_SELF(self));
    return self;
}

static VALUE
widget_show_now(VALUE self)
{
    gtk_widget_show_now(_SELF(self));
    return self;
}

static VALUE
widget_hide(VALUE self)
{
    gtk_widget_hide(_SELF(self));
    return self;
}

static VALUE
widget_show_all(VALUE self)
{
    gtk_widget_show_all(_SELF(self));
    return self;
}

static VALUE
widget_hide_all(VALUE self)
{
    gtk_widget_hide_all(_SELF(self));
    return self;
}

static VALUE
widget_map(VALUE self)
{
    gtk_widget_map(_SELF(self));
    return self;
}

static VALUE
widget_unmap(VALUE self)
{
    gtk_widget_unmap(_SELF(self));
    return self;
}

static VALUE
widget_realize(VALUE self)
{
    gtk_widget_realize(_SELF(self));
    return self;
}

static VALUE
widget_unrealize(VALUE self)
{
    gtk_widget_unrealize(_SELF(self));
    return self;
}

static VALUE
widget_queue_draw(VALUE self)
{
    gtk_widget_queue_draw(_SELF(self));
    return self;
}

static VALUE
widget_queue_resize(VALUE self)
{
    gtk_widget_queue_resize(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
widget_queue_resize_no_redraw(VALUE self)
{
    gtk_widget_queue_resize_no_redraw(_SELF(self));
    return self;
}
#endif

/* Note this method is not
   gtk_widget_get_size_request */
static VALUE
widget_size_request(VALUE self)
{
    GtkRequisition req;
    gtk_widget_size_request(_SELF(self), &req);
    return rb_ary_new3(2, INT2NUM(req.width), INT2NUM(req.height));
}

static VALUE
widget_get_child_requisition(VALUE self)
{
    GtkRequisition req;

    gtk_widget_get_child_requisition(_SELF(self), &req);
    return rb_ary_new3(2, INT2NUM(req.width), INT2NUM(req.height));
}

static VALUE
widget_size_allocate(VALUE self, VALUE alloc)
{
    gtk_widget_size_allocate(_SELF(self), (GtkAllocation*)RVAL2BOXED(alloc, GTK_TYPE_ALLOCATION));
    return self;
}

static VALUE
widget_add_accelerator(VALUE self, VALUE sig, VALUE accel, VALUE key, VALUE mod, VALUE flag)
{
    gtk_widget_add_accelerator(_SELF(self),
                               RVAL2CSTR(sig),
                               GTK_ACCEL_GROUP(RVAL2GOBJ(accel)),
                               NUM2INT(key),
                               RVAL2GFLAGS(mod, GDK_TYPE_MODIFIER_TYPE),
                               RVAL2GFLAGS(flag, GTK_TYPE_ACCEL_FLAGS));
    return self;
}

static VALUE
widget_remove_accelerator(VALUE self, VALUE accel, VALUE key, VALUE mod)
{
    return CBOOL2RVAL(gtk_widget_remove_accelerator(_SELF(self),
                                                    GTK_ACCEL_GROUP(RVAL2GOBJ(accel)),
                                                    NUM2INT(key),
                                                    RVAL2GFLAGS(mod, GDK_TYPE_MODIFIER_TYPE)));
}

static VALUE
widget_set_accel_path(VALUE self, VALUE accel_path, VALUE accel_group)
{
    gtk_widget_set_accel_path(_SELF(self), RVAL2CSTR(accel_path), 
                              GTK_ACCEL_GROUP(RVAL2GOBJ(accel_group)));
    return self;
}

static VALUE
widget_list_accel_closures(VALUE self)
{
    return GLIST2ARY2F(gtk_widget_list_accel_closures(_SELF(self)), G_TYPE_CLOSURE);
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
widget_can_activate_accel(VALUE self, VALUE signal_id)
{
    return CBOOL2RVAL(gtk_widget_can_activate_accel(_SELF(self), NUM2UINT(signal_id)));
}
#endif

static VALUE
widget_event(VALUE self, VALUE event)
{
    return CBOOL2RVAL(gtk_widget_event(_SELF(self), RVAL2GEV(event)));
}

static VALUE
widget_activate(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_activate(_SELF(self)));
}

static VALUE
widget_reparent(VALUE self, VALUE parent)
{
    gtk_widget_reparent(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
widget_intersect(VALUE self, VALUE area)
{
    GdkRectangle intersection;
    gboolean ret = gtk_widget_intersect(_SELF(self),
                                        (GdkRectangle*)RVAL2BOXED(area, GDK_TYPE_RECTANGLE),
                                        &intersection);
    return ret ? BOXED2RVAL(&intersection, GDK_TYPE_RECTANGLE) : Qnil;
}

static VALUE
widget_is_focus(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_is_focus(_SELF(self)));
}

static VALUE
widget_grab_focus(VALUE self)
{
    gtk_widget_grab_focus(_SELF(self));
    return self;
}

static VALUE
widget_grab_default(VALUE self)
{
    gtk_widget_grab_default(_SELF(self));
    return self;
}

static VALUE
widget_set_state(VALUE self, VALUE state)
{
    gtk_widget_set_state(_SELF(self), RVAL2GTKSTATETYPE(state));
    return self;
}

static VALUE
widget_set_parent_window(VALUE self, VALUE parent_window)
{
    gtk_widget_set_parent_window(_SELF(self), 
                                 GDK_WINDOW(RVAL2GOBJ(parent_window)));
    return self;
}

static VALUE
widget_get_parent_window(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_parent_window(_SELF(self)));
}

static VALUE
widget_add_events(VALUE self, VALUE events)
{
    gtk_widget_add_events(_SELF(self), NUM2INT(events));
    return self;
}

static VALUE
widget_get_toplevel(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_toplevel(_SELF(self)));
}

static VALUE
widget_get_ancestor(VALUE self, VALUE klass)
{
    return GOBJ2RVAL(gtk_widget_get_ancestor(_SELF(self), CLASS2GTYPE(klass)));
}

static VALUE
widget_get_colormap(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_colormap(_SELF(self)));
}

static VALUE
widget_set_colormap(VALUE self, VALUE colormap)
{
    gtk_widget_set_colormap(_SELF(self), GDK_COLORMAP(RVAL2GOBJ(colormap)));
    return self;
}

static VALUE
widget_get_visual(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_visual(_SELF(self)));
}

static VALUE
widget_get_pointer(VALUE self)
{
    int x, y;

    gtk_widget_get_pointer(_SELF(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

static VALUE
widget_is_ancestor(VALUE self, VALUE ancestor)
{
    return CBOOL2RVAL(gtk_widget_is_ancestor(_SELF(self), _SELF(ancestor)));
}

static VALUE
widget_translate_coordinates(VALUE self, VALUE dest_widget, VALUE src_x, VALUE src_y)
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
widget_hide_on_delete(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_hide_on_delete(_SELF(self)));
}

static VALUE
widget_ensure_style(VALUE self)
{
    gtk_widget_ensure_style(_SELF(self));
    return self;
}

static VALUE
widget_reset_rc_styles(VALUE self)
{
    gtk_widget_reset_rc_styles(_SELF(self));
    return self;
}

static VALUE
widget_s_push_colormap(VALUE self, VALUE cmap)
{
    gtk_widget_push_colormap(GDK_COLORMAP(RVAL2GOBJ(cmap)));
    return cmap;
}

static VALUE
widget_s_pop_colormap(VALUE self)
{
    gtk_widget_pop_colormap();
    return self;
}

static VALUE
widget_s_set_default_colormap(VALUE self, VALUE cmap)
{
    gtk_widget_set_default_colormap(GDK_COLORMAP(RVAL2GOBJ(cmap)));
    return cmap;
}

static VALUE
widget_s_get_default_style(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_default_style());
}

static VALUE
widget_s_get_default_colormap(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_default_colormap());
}

static VALUE
widget_s_get_default_visual(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_default_visual());
}

static VALUE
widget_set_direction(VALUE self, VALUE dir)
{
    gtk_widget_set_direction(_SELF(self), RVAL2GENUM(dir, GTK_TYPE_TEXT_DIRECTION));
    return self;
}

static VALUE
widget_get_direction(VALUE self)
{
    return GENUM2RVAL(gtk_widget_get_direction(_SELF(self)), GTK_TYPE_TEXT_DIRECTION);
}

static VALUE
widget_s_set_default_direction(VALUE self, VALUE dir)
{
    gtk_widget_set_default_direction(RVAL2GENUM(dir, GTK_TYPE_TEXT_DIRECTION));
    return self;
}

static VALUE
widget_s_get_default_direction(VALUE self)
{
    return GENUM2RVAL(gtk_widget_get_default_direction(), GTK_TYPE_TEXT_DIRECTION);
}

static VALUE
widget_shape_combine_mask(VALUE self, VALUE shape_mask, VALUE offset_x, VALUE offset_y)
{
    gtk_widget_shape_combine_mask(_SELF(self),
                                  GDK_BITMAP(RVAL2GOBJ(shape_mask)),
                                  NUM2INT(offset_x),
                                  NUM2INT(offset_y));
    return self;
}

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
widget_input_shape_combine_mask(VALUE self, VALUE shape_mask, VALUE offset_x, VALUE offset_y)
{
    gtk_widget_input_shape_combine_mask(_SELF(self),
                                        GDK_BITMAP(RVAL2GOBJ(shape_mask)),
                                        NUM2INT(offset_x),
                                        NUM2INT(offset_y));
    return self;
}
#endif

static VALUE
widget_path(VALUE self)
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
widget_class_path(VALUE self)
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
widget_get_composite_name(VALUE self)
{
    return CSTR2RVAL(gtk_widget_get_composite_name(_SELF(self)));
}

static VALUE
widget_modify_style(VALUE self, VALUE style)
{
    gtk_widget_modify_style(_SELF(self),
                            GTK_RC_STYLE(RVAL2GOBJ(style)));
    return self;
}

static VALUE
widget_get_modifier_style(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_modifier_style(_SELF(self)));
}

static VALUE
widget_modify_fg(VALUE self, VALUE state, VALUE color)
{
    gtk_widget_modify_fg(_SELF(self), RVAL2GTKSTATETYPE(state),
                         RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
widget_modify_bg(VALUE self, VALUE state, VALUE color)
{
    gtk_widget_modify_bg(_SELF(self), RVAL2GTKSTATETYPE(state),
                         RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
widget_modify_text(VALUE self, VALUE state, VALUE color)
{
    gtk_widget_modify_text(_SELF(self), RVAL2GTKSTATETYPE(state),
                         RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
widget_modify_base(VALUE self, VALUE state, VALUE color)
{
    gtk_widget_modify_base(_SELF(self), RVAL2GTKSTATETYPE(state),
                         RVAL2GDKCOLOR(color));
    return self;
}

static VALUE
widget_modify_font(VALUE self, VALUE font_desc)
{
    gtk_widget_modify_font(_SELF(self), 
                           (PangoFontDescription*)RVAL2BOXED(font_desc, PANGO_TYPE_FONT_DESCRIPTION));
    return self;
}

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
widget_modify_cursor(VALUE self, VALUE primary, VALUE seconday)
{
    gtk_widget_modify_cursor(_SELF(self), 
                             RVAL2BOXED(primary, GDK_TYPE_COLOR),
                             RVAL2BOXED(seconday, GDK_TYPE_COLOR));
    return self;
}
#endif

static VALUE
widget_create_pango_context(VALUE self)
{
    return GOBJ2RVALU(gtk_widget_create_pango_context(_SELF(self)));
}

static VALUE
widget_get_pango_context(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_pango_context(_SELF(self)));
}

static VALUE
widget_create_pango_layout(int argc, VALUE *argv, VALUE self)
{
    VALUE text;
    rb_scan_args(argc, argv, "01", &text);
    return GOBJ2RVALU(gtk_widget_create_pango_layout(_SELF(self), RVAL2CSTR_ACCEPT_NIL(text)));
}

static VALUE
widget_render_icon(int argc, VALUE *argv, VALUE self)
{
    VALUE stock_id, size, detail;
    rb_scan_args(argc, argv, "21", &stock_id, &size, &detail);
    return GOBJ2RVALU(gtk_widget_render_icon(_SELF(self), rb_id2name(SYM2ID(stock_id)),
                                             RVAL2GENUM(size, GTK_TYPE_ICON_SIZE),
                                             RVAL2CSTR_ACCEPT_NIL(detail)));
}

static VALUE
widget_s_pop_composite_child(VALUE self)
{
    gtk_widget_pop_composite_child();
    return self;
}

static VALUE
widget_s_push_composite_child(VALUE self)
{
    gtk_widget_push_composite_child();
    return self;
}

static VALUE
widget_queue_draw_area(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_widget_queue_draw_area(_SELF(self), NUM2INT(x), NUM2INT(y),
                               NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
widget_reset_shapes(VALUE self)
{
    gtk_widget_reset_shapes(_SELF(self));
    return self;
}

static VALUE
widget_set_double_buffered(VALUE self, VALUE double_buffered)
{
    gtk_widget_set_double_buffered(_SELF(self), RVAL2CBOOL(double_buffered));
    return self;
}

static VALUE
widget_set_redraw_on_allocate(VALUE self, VALUE redraw_on_allocate)
{
    gtk_widget_set_redraw_on_allocate(_SELF(self), RVAL2CBOOL(redraw_on_allocate));
    return self;
}

static VALUE
widget_set_composite_name(VALUE self, VALUE name)
{
    gtk_widget_set_composite_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
widget_set_scroll_adjustments(VALUE self, VALUE h, VALUE v)
{
    return CBOOL2RVAL(gtk_widget_set_scroll_adjustments(_SELF(self),
                                                        GTK_ADJUSTMENT(_SELF(h)),
                                                        GTK_ADJUSTMENT(_SELF(v))));
}

static VALUE
widget_mnemonic_activate(VALUE self, VALUE group_cycling)
{
    return CBOOL2RVAL(gtk_widget_mnemonic_activate(_SELF(self), RVAL2CBOOL(group_cycling)));
}

/*
 * Gtk::Widget.install_style_property(...) do |spec, str|
 *   #parse str
 *   new_val = ....   # parse value from string.
 *   new_val          # return new_val or nil if you can't convert the value.
 * end
 */
static gboolean
rc_property_parser(const GParamSpec *pspec, const GString *rc_string, GValue *property_value)
{
    VALUE spec = GOBJ2RVAL((gpointer)pspec);
    VALUE func = rb_hash_aref(style_prop_func_table, spec);
    VALUE ret = rb_funcall(func, id_call, 2, spec, CSTR2RVAL(rc_string->str));
    if (NIL_P(ret)) {
        return FALSE;
    } else if (RVAL2CBOOL(ret)){
        rbgobj_rvalue_to_gvalue(ret, property_value);
        return TRUE;
    } else {
        rb_raise(rb_eArgError, "Gtk::Widget#install_style_property() block should return new value or nil");
    }
}

static VALUE
widget_s_install_style_property(VALUE self, VALUE spec)
{
    const RGObjClassInfo* cinfo = rbgobj_lookup_class(self);
    GtkWidgetClass* gclass;
    GParamSpec* pspec = G_PARAM_SPEC(RVAL2GOBJ(spec));

    if (cinfo->klass != self)
        rb_raise(rb_eTypeError, "%s isn't registered class",
                 rb_class2name(self));

    gclass = (GtkWidgetClass *)g_type_class_ref(cinfo->gtype);
    if (rb_block_given_p()){
        VALUE func = rb_block_proc();
        rb_hash_aset(style_prop_func_table, spec, func);
        gtk_widget_class_install_style_property_parser(gclass, pspec, 
                                                       (GtkRcPropertyParser)rc_property_parser);
    } else {
        gtk_widget_class_install_style_property(gclass, pspec); 
    }
    return self;
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
widget_s_find_style_property(VALUE self, VALUE property_name)
{
    GtkWidgetClass* oclass;
    const char* name;
    GParamSpec* prop;
    VALUE result;

    if (SYMBOL_P(property_name)) {
        name = rb_id2name(SYM2ID(property_name));
    } else {
        name = RVAL2CSTR(property_name);
    }

    oclass = (GtkWidgetClass*)g_type_class_ref(CLASS2GTYPE(self));

    prop = gtk_widget_class_find_style_property(oclass, name);
    if (!prop){
        g_type_class_unref(oclass);
        rb_raise(rb_eval_string("GLib::NoPropertyError"), "no such property: %s", name);
    }

    result = GOBJ2RVAL(prop);
    g_type_class_unref(oclass);
    return result;
}

static VALUE
widget_s_style_properties(int argc, VALUE *argv, VALUE self)
{
    GtkWidgetClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    guint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary;
    int i;

    if (rb_scan_args(argc, argv, "01", &inherited_too) == 0)
        inherited_too = Qtrue;

    props = gtk_widget_class_list_style_properties(oclass, &n_properties);

    ary = rb_ary_new();
    for (i = 0; i < n_properties; i++){
        if (RVAL2CBOOL(inherited_too)
            || GTYPE2CLASS(props[i]->owner_type) == self)
            rb_ary_push(ary, CSTR2RVAL(props[i]->name));
    }

    g_free(props);
    g_type_class_unref(oclass);
    return ary;
}
#endif

static VALUE
widget_region_intersect(VALUE self, VALUE region)
{
    return BOXED2RVAL(gtk_widget_region_intersect(_SELF(self), 
                                                  (GdkRegion*)RVAL2BOXED(region, GDK_TYPE_REGION)), 
                      GDK_TYPE_REGION);
}

static VALUE
widget_send_expose(VALUE self, VALUE event)
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
widget_style_get_property(VALUE self, VALUE prop_name)
{
    GParamSpec* pspec = NULL;
    const char* name;

    if (SYMBOL_P(prop_name)) {
        name = rb_id2name(SYM2ID(prop_name));
    } else {
        name = RVAL2CSTR(prop_name);
    }
#if GTK_CHECK_VERSION(2,2,0)
    pspec = gtk_widget_class_find_style_property((GtkWidgetClass*)g_type_class_ref(RVAL2GTYPE(self)), name);
#endif
    if (!pspec)
        rb_raise(rb_eval_string("GLib::NoPropertyError"), "No such property: %s", name);
    else {
        // FIXME: use rb_ensure to call g_value_unset()
        GValue gval = {0,};
        VALUE ret;
        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));
        gtk_widget_style_get_property(GTK_WIDGET(RVAL2GOBJ(self)), name, &gval);
        ret = GVAL2RVAL(&gval);
        g_value_unset(&gval);
        return ret;
    }
}

static VALUE
widget_get_accessible(VALUE self)
{
    return GOBJ2RVAL(_SELF(self));
}

static VALUE
widget_child_focus(VALUE self, VALUE direction)
{
    return CBOOL2RVAL(gtk_widget_child_focus(_SELF(self), RVAL2GENUM(direction, GTK_TYPE_DIRECTION_TYPE)));
}

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
widget_error_bell(VALUE self)
{
    gtk_widget_error_bell(_SELF(self));
    return self;
}

static VALUE
widget_keynav_failed(VALUE self, VALUE direction)
{
    return CBOOL2RVAL(gtk_widget_keynav_failed(_SELF(self), RVAL2GENUM(direction, GTK_TYPE_DIRECTION_TYPE)));
}
#endif

static VALUE
widget_child_notify(VALUE self, VALUE child_property)
{
    gtk_widget_child_notify(_SELF(self), RVAL2CSTR(child_property));
    return self;
}

static VALUE
widget_freeze_child_notify(VALUE self)
{
    gtk_widget_freeze_child_notify(_SELF(self));
    return self;
}

static VALUE
widget_get_child_visible(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_child_visible(_SELF(self)));
}

static VALUE
widget_get_settings(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_settings(_SELF(self)));
}

#if GTK_CHECK_VERSION(2,2,0)
static VALUE
widget_get_clipboard(VALUE self, VALUE selection)
{
    return CLIPBOARD2RVAL(gtk_widget_get_clipboard(_SELF(self), RVAL2ATOM(selection)));
}

static VALUE
widget_get_display(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_display(_SELF(self)));
}

static VALUE
widget_get_root_window(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_root_window(_SELF(self)));
}

static VALUE
widget_get_screen(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_screen(_SELF(self)));
}

static VALUE
widget_has_screen(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_has_screen(_SELF(self)));
}
#endif

/*
  Note this method is not gtk_widget_size_request()
*/
static VALUE
widget_get_size_request(VALUE self)
{
    gint width, height;
    gtk_widget_get_size_request(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
widget_set_child_visible(VALUE self, VALUE is_visible)
{
    gtk_widget_set_child_visible(_SELF(self), RVAL2CBOOL(is_visible));
    return self;
}

static VALUE
widget_set_size_request(VALUE self, VALUE width, VALUE height)
{
    gtk_widget_set_size_request(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
widget_thaw_child_notify(VALUE self)
{
    gtk_widget_thaw_child_notify(_SELF(self));
    return self;
}

#if GTK_CHECK_VERSION(2,4,0)
static VALUE
widget_set_no_show_all(VALUE self, VALUE no_show_all)
{
    gtk_widget_set_no_show_all(_SELF(self), RVAL2CBOOL(no_show_all));
    return self;
}

static VALUE
widget_get_no_show_all(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_no_show_all(_SELF(self)));
}

static VALUE
widget_list_mnemonic_labels(VALUE self)
{
    return GLIST2ARYF(gtk_widget_list_mnemonic_labels(_SELF(self)));
}

static VALUE
widget_add_mnemonic_label(VALUE self, VALUE label)
{
    gtk_widget_add_mnemonic_label(_SELF(self), GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}

static VALUE
widget_remove_mnemonic_label(VALUE self, VALUE label)
{
    gtk_widget_remove_mnemonic_label(_SELF(self), GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}
#endif

#if GTK_CHECK_VERSION(2,12,0)
static VALUE
widget_set_tooltip_window(VALUE self, VALUE custom_window)
{
    gtk_widget_set_tooltip_window(_SELF(self), GTK_WINDOW(RVAL2GOBJ(custom_window)));
    return self;
}

static VALUE
widget_get_tooltip_window(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_tooltip_window(_SELF(self)));
}

static VALUE
widget_trigger_tooltip_query(VALUE self)
{
    gtk_widget_trigger_tooltip_query(_SELF(self));
    return self;
}

#endif

#if GTK_CHECK_VERSION(2,10,0)
static VALUE
widget_get_action(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_action(_SELF(self)));
}

static VALUE
widget_is_composited(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_is_composited(_SELF(self)));
}
#endif

static VALUE
widget_window(VALUE self)
{
    return GOBJ2RVAL(_SELF(self)->window);
}

static VALUE
widget_set_window(VALUE self, VALUE window)
{
    _SELF(self)->window = RVAL2GOBJ(window);
    return self;
}

#define DEFINE_IS_WIDGET(STATE) \
static VALUE \
widget_ ## STATE (VALUE self) \
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
widget_get_allocation(VALUE self)
{
    return BOXED2RVAL(&(_SELF(self)->allocation), GTK_TYPE_ALLOCATION);
}

static VALUE
widget_set_allocation(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
    GtkAllocation *a = &(_SELF(self)->allocation);
    a->x      = NUM2INT(x);
    a->y      = NUM2INT(y);
    a->width  = NUM2INT(w);
    a->height = NUM2INT(h);
    return self;
}

static VALUE
widget_get_requisition(VALUE self)
{
    GtkRequisition req = _SELF(self)->requisition;
    return rb_ary_new3(2, INT2NUM(req.width), INT2NUM(req.height));
}

static VALUE
widget_set_requisition(VALUE self, VALUE w, VALUE h)
{
    GtkRequisition *r = &(_SELF(self)->requisition);
    r->width  = NUM2INT(w);
    r->height = NUM2INT(h);
    return self;    
}

static VALUE
widget_state(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->state, GTK_TYPE_STATE_TYPE);
}

static VALUE
widget_saved_state(VALUE self)
{
    return GENUM2RVAL(_SELF(self)->saved_state, GTK_TYPE_STATE_TYPE);
}

static VALUE
widget_signal_size_request(guint num, const GValue *values)
{
    GtkRequisition* req = (GtkRequisition*)g_value_get_boxed(&values[1]);
    return rb_ary_new3(2, GVAL2RVAL(&values[0]), 
                       rb_ary_new3(2, INT2NUM(req->width), INT2NUM(req->height)));
}
static VALUE
widget_signal_size_allocate(guint num, const GValue *values)
{
    GtkAllocation* alloc = (GtkAllocation*)g_value_get_boxed(&values[1]);
    return rb_ary_new3(2, GVAL2RVAL(&values[0]), BOXED2RVAL(alloc, GTK_TYPE_ALLOCATION));
}

void 
Init_gtk_widget()
{
    VALUE gWidget = G_DEF_CLASS(GTK_TYPE_WIDGET, "Widget", mGtk);

    rb_global_variable(&style_prop_func_table);
    style_prop_func_table = rb_hash_new();

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
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gWidget, "queue_resize_no_redraw", widget_queue_resize_no_redraw, 0);
#endif
    rb_define_method(gWidget, "size_request", widget_size_request, 0);
    rb_define_method(gWidget, "child_requisition", widget_get_child_requisition, 0);
    rb_define_method(gWidget, "size_allocate", widget_size_allocate, 1);
    rb_define_method(gWidget, "add_accelerator", widget_add_accelerator, 5);
    rb_define_method(gWidget, "remove_accelerator", widget_remove_accelerator, 3);
    rb_define_method(gWidget, "set_accel_path", widget_set_accel_path, 2);
    rb_define_method(gWidget, "accel_closures", widget_list_accel_closures, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gWidget, "can_activate_accel?", widget_can_activate_accel, 1);
#endif
    rb_define_method(gWidget, "event", widget_event, 1);
    rb_define_method(gWidget, "activate", widget_activate, 0);
    rb_define_method(gWidget, "reparent", widget_reparent, 1);
    rb_define_singleton_method(gWidget, "install_style_property", widget_s_install_style_property, 1);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_singleton_method(gWidget, "style_property", widget_s_find_style_property, 1);
    rb_define_singleton_method(gWidget, "style_properties", widget_s_style_properties, -1);
#endif
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
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gWidget, "input_shape_combine_mask", widget_input_shape_combine_mask, 3);
#endif
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
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gWidget, "modify_cursor", widget_modify_cursor, 2);
#endif
    rb_define_method(gWidget, "create_pango_context", widget_create_pango_context, 0);
    rb_define_method(gWidget, "pango_context", widget_get_pango_context, 0);
    rb_define_method(gWidget, "create_pango_layout", widget_create_pango_layout, -1);
    rb_define_method(gWidget, "render_icon", widget_render_icon, -1);
    rb_define_method(gWidget, "queue_draw_area", widget_queue_draw_area, 4);
    rb_define_method(gWidget, "reset_shapes", widget_reset_shapes, 0);
    rb_define_method(gWidget, "set_double_buffered", widget_set_double_buffered, 1);
    rb_define_method(gWidget, "set_redraw_on_allocate", widget_set_redraw_on_allocate, 1);
    rb_define_method(gWidget, "set_composite_name", widget_set_composite_name, 1);
    rb_define_method(gWidget, "set_scroll_adjustments", widget_set_scroll_adjustments, 2);
    /* for backward compatibility. */
    rb_define_alias(gWidget, "set_scroll_adjustment", "set_scroll_adjustments");
    rb_define_method(gWidget, "mnemonic_activate", widget_mnemonic_activate, 1);
    rb_define_method(gWidget, "region_intersect", widget_region_intersect, 1);
    rb_define_method(gWidget, "send_expose", widget_send_expose, 1);
    rb_define_method(gWidget, "style_get_property", widget_style_get_property, 1);
    rb_define_method(gWidget, "accessible", widget_get_accessible, 0);
    rb_define_method(gWidget, "child_focus", widget_child_focus, 1);
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gWidget, "error_bell", widget_error_bell, 0);
    rb_define_method(gWidget, "keynav_failed", widget_keynav_failed, 0);
#endif
    rb_define_method(gWidget, "child_notify", widget_child_notify, 1);
    rb_define_method(gWidget, "freeze_child_notify", widget_freeze_child_notify, 0);
    rb_define_method(gWidget, "child_visible?", widget_get_child_visible, 0);
    rb_define_method(gWidget, "settings", widget_get_settings, 0);
#if GTK_CHECK_VERSION(2,2,0)
    rb_define_method(gWidget, "get_clipboard", widget_get_clipboard, 1);
    rb_define_method(gWidget, "display", widget_get_display, 0);
    rb_define_method(gWidget, "root_window", widget_get_root_window, 0);
    rb_define_method(gWidget, "screen", widget_get_screen, 0);
    rb_define_method(gWidget, "has_screen?", widget_has_screen, 0);
#endif
    rb_define_method(gWidget, "set_child_visible", widget_set_child_visible, 1);
    rb_define_method(gWidget, "get_size_request", widget_get_size_request, 0);
    rb_define_method(gWidget, "set_size_request", widget_set_size_request, 2);
    rb_define_method(gWidget, "thaw_child_notify", widget_thaw_child_notify, 0);
#if GTK_CHECK_VERSION(2,4,0)
    rb_define_method(gWidget, "set_no_show_all", widget_set_no_show_all, 1);
    rb_define_method(gWidget, "no_show_all?", widget_get_no_show_all, 0);
    rb_define_method(gWidget, "mnemonic_labels", widget_list_mnemonic_labels, 0);
    rb_define_method(gWidget, "add_mnemonic_label", widget_add_mnemonic_label, 1);
    rb_define_method(gWidget, "remove_mnemonic_label", widget_remove_mnemonic_label, 1);
#endif
#if GTK_CHECK_VERSION(2,12,0)
    rb_define_method(gWidget, "set_tooltip_window", widget_set_tooltip_window, 1);
    rb_define_method(gWidget, "tooltip_window", widget_get_tooltip_window, 0);
    rb_define_method(gWidget, "trigger_tooltip_query", widget_trigger_tooltip_query, 0);
#endif
#if GTK_CHECK_VERSION(2,10,0)
    rb_define_method(gWidget, "action", widget_get_action, 0);
    rb_define_method(gWidget, "composited?", widget_is_composited, 0);
#endif
    rb_define_method(gWidget, "window", widget_window, 0);
    rb_define_method(gWidget, "set_window", widget_set_window, 1);
    rb_define_method(gWidget, "allocation", widget_get_allocation, 0);
    rb_define_method(gWidget, "set_allocation", widget_set_allocation, 4);
    rb_define_method(gWidget, "requisition", widget_get_requisition, 0);
    rb_define_method(gWidget, "set_requisition", widget_set_requisition, 2);
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
    G_DEF_CLASS(GTK_TYPE_WIDGET_FLAGS, "Flags", gWidget);
    G_DEF_CONSTANTS(gWidget, GTK_TYPE_WIDGET_FLAGS, "GTK_");

    /* GtkWidgetHelpType */
    G_DEF_CLASS(GTK_TYPE_WIDGET_HELP_TYPE, "HelpType", gWidget);
    G_DEF_CONSTANTS(gWidget, GTK_TYPE_WIDGET_HELP_TYPE, "GTK_WIDGET_");

    /* GtkTextDirection */
    G_DEF_CLASS(GTK_TYPE_TEXT_DIRECTION, "TextDirection", gWidget);
    G_DEF_CONSTANTS(gWidget, GTK_TYPE_TEXT_DIRECTION, "GTK_");

    /* Special signals */
    G_DEF_SIGNAL_FUNC(gWidget, "size-request", (GValToRValSignalFunc)widget_signal_size_request);
    G_DEF_SIGNAL_FUNC(gWidget, "size-allocate", (GValToRValSignalFunc)widget_signal_size_allocate);
    
}
