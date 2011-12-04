/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2006 Ruby-GNOME2 Project Team
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

#include "rbgtk3private.h"

#define RG_TARGET_NAMESPACE cWidget
#define _SELF(self) (GTK_WIDGET(RVAL2GOBJ(self)))

static VALUE style_prop_func_table;

static VALUE
rg_unparent(VALUE self)
{
    gtk_widget_unparent(_SELF(self));
    return self;
}

static VALUE
rg_show(VALUE self)
{
    gtk_widget_show(_SELF(self));
    return self;
}

static VALUE
rg_show_now(VALUE self)
{
    gtk_widget_show_now(_SELF(self));
    return self;
}

static VALUE
rg_hide(VALUE self)
{
    gtk_widget_hide(_SELF(self));
    return self;
}

static VALUE
rg_show_all(VALUE self)
{
    gtk_widget_show_all(_SELF(self));
    return self;
}

static VALUE
rg_map(VALUE self)
{
    gtk_widget_map(_SELF(self));
    return self;
}

static VALUE
rg_unmap(VALUE self)
{
    gtk_widget_unmap(_SELF(self));
    return self;
}

static VALUE
rg_realize(VALUE self)
{
    gtk_widget_realize(_SELF(self));
    return self;
}

static VALUE
rg_unrealize(VALUE self)
{
    gtk_widget_unrealize(_SELF(self));
    return self;
}

static VALUE
rg_queue_draw(VALUE self)
{
    gtk_widget_queue_draw(_SELF(self));
    return self;
}

static VALUE
rg_queue_resize(VALUE self)
{
    gtk_widget_queue_resize(_SELF(self));
    return self;
}

static VALUE
rg_queue_resize_no_redraw(VALUE self)
{
    gtk_widget_queue_resize_no_redraw(_SELF(self));
    return self;
}

static VALUE
rg_size_allocate(VALUE self, VALUE alloc)
{
    gtk_widget_size_allocate(_SELF(self), (GtkAllocation*)RVAL2BOXED(alloc, GTK_TYPE_ALLOCATION));
    return self;
}

static VALUE
rg_add_accelerator(VALUE self, VALUE sig, VALUE accel, VALUE key, VALUE mod, VALUE flag)
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
rg_remove_accelerator(VALUE self, VALUE accel, VALUE key, VALUE mod)
{
    return CBOOL2RVAL(gtk_widget_remove_accelerator(_SELF(self),
                                                    GTK_ACCEL_GROUP(RVAL2GOBJ(accel)),
                                                    NUM2INT(key),
                                                    RVAL2GFLAGS(mod, GDK_TYPE_MODIFIER_TYPE)));
}

static VALUE
rg_set_accel_path(VALUE self, VALUE accel_path, VALUE accel_group)
{
    gtk_widget_set_accel_path(_SELF(self), RVAL2CSTR(accel_path), 
                              GTK_ACCEL_GROUP(RVAL2GOBJ(accel_group)));
    return self;
}

static VALUE
rg_accel_closures(VALUE self)
{
    return GLIST2ARY2F(gtk_widget_list_accel_closures(_SELF(self)), G_TYPE_CLOSURE);
}

static VALUE
rg_can_activate_accel_p(VALUE self, VALUE signal_id)
{
    return CBOOL2RVAL(gtk_widget_can_activate_accel(_SELF(self), NUM2UINT(signal_id)));
}

static VALUE
rg_event(VALUE self, VALUE event)
{
    return CBOOL2RVAL(gtk_widget_event(_SELF(self), RVAL2GEV(event)));
}

static VALUE
rg_activate(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_activate(_SELF(self)));
}

static VALUE
rg_reparent(VALUE self, VALUE parent)
{
    gtk_widget_reparent(_SELF(self), _SELF(parent));
    return self;
}

static VALUE
rg_intersect(VALUE self, VALUE area)
{
    GdkRectangle intersection;
    gboolean ret = gtk_widget_intersect(_SELF(self),
                                        (GdkRectangle*)RVAL2BOXED(area, GDK_TYPE_RECTANGLE),
                                        &intersection);
    return ret ? BOXED2RVAL(&intersection, GDK_TYPE_RECTANGLE) : Qnil;
}

static VALUE
rg_grab_default(VALUE self)
{
    gtk_widget_grab_default(_SELF(self));
    return self;
}

static VALUE
rg_set_parent_window(VALUE self, VALUE parent_window)
{
    gtk_widget_set_parent_window(_SELF(self), 
                                 GDK_WINDOW(RVAL2GOBJ(parent_window)));
    return self;
}

static VALUE
rg_parent_window(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_parent_window(_SELF(self)));
}

static VALUE
rg_add_events(VALUE self, VALUE events)
{
    gtk_widget_add_events(_SELF(self), NUM2INT(events));
    return self;
}

static VALUE
rg_toplevel(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_toplevel(_SELF(self)));
}

static VALUE
rg_get_ancestor(VALUE self, VALUE klass)
{
    return GOBJ2RVAL(gtk_widget_get_ancestor(_SELF(self), CLASS2GTYPE(klass)));
}

static VALUE
rg_visual(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_visual(_SELF(self)));
}

static VALUE
rg_pointer(VALUE self)
{
    int x, y;

    gtk_widget_get_pointer(_SELF(self), &x, &y);
    return rb_assoc_new(INT2FIX(x), INT2FIX(y));
}

static VALUE
rg_ancestor_p(VALUE self, VALUE ancestor)
{
    return CBOOL2RVAL(gtk_widget_is_ancestor(_SELF(self), _SELF(ancestor)));
}

static VALUE
rg_translate_coordinates(VALUE self, VALUE dest_widget, VALUE src_x, VALUE src_y)
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
rg_hide_on_delete(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_hide_on_delete(_SELF(self)));
}

static VALUE
rg_set_direction(VALUE self, VALUE dir)
{
    gtk_widget_set_direction(_SELF(self), RVAL2GENUM(dir, GTK_TYPE_TEXT_DIRECTION));
    return self;
}

static VALUE
rg_direction(VALUE self)
{
    return GENUM2RVAL(gtk_widget_get_direction(_SELF(self)), GTK_TYPE_TEXT_DIRECTION);
}

static VALUE
rg_s_set_default_direction(VALUE self, VALUE dir)
{
    gtk_widget_set_default_direction(RVAL2GENUM(dir, GTK_TYPE_TEXT_DIRECTION));
    return self;
}

static VALUE
rg_s_default_direction(G_GNUC_UNUSED VALUE self)
{
    return GENUM2RVAL(gtk_widget_get_default_direction(), GTK_TYPE_TEXT_DIRECTION);
}

static VALUE
rg_composite_name(VALUE self)
{
    return CSTR2RVAL(gtk_widget_get_composite_name(_SELF(self)));
}

static VALUE
rg_create_pango_context(VALUE self)
{
    return GOBJ2RVALU(gtk_widget_create_pango_context(_SELF(self)));
}

static VALUE
rg_pango_context(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_pango_context(_SELF(self)));
}

static VALUE
rg_create_pango_layout(int argc, VALUE *argv, VALUE self)
{
    VALUE text;
    rb_scan_args(argc, argv, "01", &text);
    return GOBJ2RVALU(gtk_widget_create_pango_layout(_SELF(self), RVAL2CSTR_ACCEPT_NIL(text)));
}

static VALUE
rg_s_pop_composite_child(VALUE self)
{
    gtk_widget_pop_composite_child();
    return self;
}

static VALUE
rg_s_push_composite_child(VALUE self)
{
    gtk_widget_push_composite_child();
    return self;
}

static VALUE
rg_queue_draw_area(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    gtk_widget_queue_draw_area(_SELF(self), NUM2INT(x), NUM2INT(y),
                               NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_set_redraw_on_allocate(VALUE self, VALUE redraw_on_allocate)
{
    gtk_widget_set_redraw_on_allocate(_SELF(self), RVAL2CBOOL(redraw_on_allocate));
    return self;
}

static VALUE
rg_set_composite_name(VALUE self, VALUE name)
{
    gtk_widget_set_composite_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
rg_mnemonic_activate(VALUE self, VALUE group_cycling)
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
rg_s_install_style_property(VALUE self, VALUE spec)
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

static VALUE
rg_s_style_property(VALUE self, VALUE property_name)
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
rg_s_style_properties(int argc, VALUE *argv, VALUE self)
{
    GtkWidgetClass* oclass = g_type_class_ref(CLASS2GTYPE(self));
    guint n_properties;
    GParamSpec** props;
    VALUE inherited_too;
    VALUE ary;
    guint i;

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

static VALUE
rg_region_intersect(VALUE self, VALUE region)
{
    return CRREGION2RVAL(gtk_widget_region_intersect(_SELF(self), 
                                                     RVAL2CRREGION(region)));
}

static VALUE
rg_send_expose(VALUE self, VALUE event)
{
    return INT2NUM(gtk_widget_send_expose(_SELF(self), RVAL2GEV(event)));
}

static VALUE
rg_style_get_property(VALUE self, VALUE prop_name)
{
    GParamSpec* pspec = NULL;
    const char* name;

    if (SYMBOL_P(prop_name)) {
        name = rb_id2name(SYM2ID(prop_name));
    } else {
        name = RVAL2CSTR(prop_name);
    }
    pspec = gtk_widget_class_find_style_property((GtkWidgetClass*)g_type_class_ref(RVAL2GTYPE(self)), name);
    if (!pspec)
        rb_raise(rb_eval_string("GLib::NoPropertyError"), "No such property: %s", name);
    else {
        // FIXME: use rb_ensure to call g_value_unset()
        GValue gval = G_VALUE_INIT;
        VALUE ret;
        g_value_init(&gval, G_PARAM_SPEC_VALUE_TYPE(pspec));
        gtk_widget_style_get_property(GTK_WIDGET(RVAL2GOBJ(self)), name, &gval);
        ret = GVAL2RVAL(&gval);
        g_value_unset(&gval);
        return ret;
    }
}

static VALUE
rg_accessible(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_accessible(_SELF(self)));
}

static VALUE
rg_child_focus(VALUE self, VALUE direction)
{
    return CBOOL2RVAL(gtk_widget_child_focus(_SELF(self), RVAL2GENUM(direction, GTK_TYPE_DIRECTION_TYPE)));
}

static VALUE
rg_error_bell(VALUE self)
{
    gtk_widget_error_bell(_SELF(self));
    return self;
}

static VALUE
rg_keynav_failed(VALUE self, VALUE direction)
{
    return CBOOL2RVAL(gtk_widget_keynav_failed(_SELF(self), RVAL2GENUM(direction, GTK_TYPE_DIRECTION_TYPE)));
}

static VALUE
rg_child_notify(VALUE self, VALUE child_property)
{
    gtk_widget_child_notify(_SELF(self), RVAL2CSTR(child_property));
    return self;
}

static VALUE
rg_freeze_child_notify(VALUE self)
{
    gtk_widget_freeze_child_notify(_SELF(self));
    return self;
}

static VALUE
rg_child_visible_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_child_visible(_SELF(self)));
}

static VALUE
rg_settings(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_settings(_SELF(self)));
}

static VALUE
rg_get_clipboard(VALUE self, VALUE selection)
{
    return CLIPBOARD2RVAL(gtk_widget_get_clipboard(_SELF(self), RVAL2ATOM(selection)));
}

static VALUE
rg_display(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_display(_SELF(self)));
}

static VALUE
rg_root_window(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_root_window(_SELF(self)));
}

static VALUE
rg_screen(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_screen(_SELF(self)));
}

static VALUE
rg_has_screen_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_has_screen(_SELF(self)));
}

static VALUE
rg_get_size_request(VALUE self)
{
    gint width, height;
    gtk_widget_get_size_request(_SELF(self), &width, &height);
    return rb_ary_new3(2, INT2NUM(width), INT2NUM(height));
}

static VALUE
rg_set_child_visible(VALUE self, VALUE is_visible)
{
    gtk_widget_set_child_visible(_SELF(self), RVAL2CBOOL(is_visible));
    return self;
}

static VALUE
rg_set_size_request(VALUE self, VALUE width, VALUE height)
{
    gtk_widget_set_size_request(_SELF(self), NUM2INT(width), NUM2INT(height));
    return self;
}

static VALUE
rg_thaw_child_notify(VALUE self)
{
    gtk_widget_thaw_child_notify(_SELF(self));
    return self;
}

static VALUE
rg_mnemonic_labels(VALUE self)
{
    return GLIST2ARYF(gtk_widget_list_mnemonic_labels(_SELF(self)));
}

static VALUE
rg_add_mnemonic_label(VALUE self, VALUE label)
{
    gtk_widget_add_mnemonic_label(_SELF(self), GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}

static VALUE
rg_remove_mnemonic_label(VALUE self, VALUE label)
{
    gtk_widget_remove_mnemonic_label(_SELF(self), GTK_WIDGET(RVAL2GOBJ(label)));
    return self;
}

static VALUE
rg_set_tooltip_window(VALUE self, VALUE custom_window)
{
    gtk_widget_set_tooltip_window(_SELF(self), GTK_WINDOW(RVAL2GOBJ(custom_window)));
    return self;
}

static VALUE
rg_tooltip_window(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_tooltip_window(_SELF(self)));
}

static VALUE
rg_trigger_tooltip_query(VALUE self)
{
    gtk_widget_trigger_tooltip_query(_SELF(self));
    return self;
}

static VALUE
rg_composited_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_is_composited(_SELF(self)));
}

static VALUE
rg_set_window(VALUE self, VALUE window)
{
    gtk_widget_set_window(_SELF(self), GDK_WINDOW(RVAL2GOBJ(window)));
    return self;
}

static VALUE
rg_toplevel_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_is_toplevel(_SELF(self)));
}

static VALUE
rg_realized_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_realized(_SELF(self)));
}

static VALUE
rg_mapped_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_mapped(_SELF(self)));
}

static VALUE
rg_drawable_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_is_drawable(_SELF(self)));
}

static VALUE
rg_sensitive_with_parent_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_sensitive(_SELF(self)));
}

static VALUE
rg_has_grab_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_has_grab(_SELF(self)));
}

static VALUE
rg_allocation(VALUE self)
{
    GtkAllocation alloc;
    gtk_widget_get_allocation(_SELF(self), &alloc);
    return BOXED2RVAL(&alloc, GTK_TYPE_ALLOCATION);
}

static VALUE
rg_set_allocation(VALUE self, VALUE alloc)
{
    gtk_widget_set_allocation(_SELF(self), (GtkAllocation*)RVAL2BOXED(alloc, GTK_TYPE_ALLOCATION));
    return self;
}

static VALUE
rg_destroy(VALUE self)
{
    gtk_widget_destroy(_SELF(self));
    return Qnil;
}

static VALUE
rg_bindings_activate(VALUE self, VALUE keyval, VALUE modifiers)
{
     return CBOOL2RVAL(gtk_bindings_activate(RVAL2GOBJ(self),
                                             NUM2UINT(keyval),
                                             NUM2UINT(modifiers)));
}

static VALUE
rg_has_window_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_has_window(_SELF(self)));
}

static VALUE
rg_set_has_window(VALUE self, VALUE has_window)
{
    gtk_widget_set_has_window(_SELF(self), RVAL2CBOOL(has_window));
    return self;
}

static VALUE
rg_s_binding_set(VALUE self)
{
    GType gtype;
    gpointer gclass;
    GtkBindingSet* binding_set;

    Check_Type(self, T_CLASS);

    gtype = CLASS2GTYPE(self);

    if (!G_TYPE_IS_CLASSED(gtype)) {
        rb_raise(rb_eTypeError, "%s is not a classed GType",
                 rb_class2name(self));
    }
    gclass = g_type_class_ref(gtype);
    if (!gclass) {
        rb_raise(rb_eRuntimeError, "couldn't get class reference");
    }
    if (!G_IS_OBJECT_CLASS(gclass)) {
        g_type_class_unref(gclass);
        rb_raise(rb_eTypeError, "%s is not a GObject class",
                 rb_class2name(self));
    }

    binding_set = gtk_binding_set_by_class(gclass);
    if (!binding_set) {
        g_type_class_unref(gclass);
        rb_raise(rb_eRuntimeError, "couldn't get BindingSet from %s",
                 rb_class2name(self));
    }
    g_type_class_unref(gclass);

    return BOXED2RVAL(binding_set, GTK_TYPE_BINDING_SET);
}

static VALUE
widget_signal_size_request(G_GNUC_UNUSED guint num, const GValue *values)
{
    GtkRequisition* req = (GtkRequisition*)g_value_get_boxed(&values[1]);
    return rb_ary_new3(2, GVAL2RVAL(&values[0]), 
                       rb_ary_new3(2, INT2NUM(req->width), INT2NUM(req->height)));
}
static VALUE
widget_signal_size_allocate(G_GNUC_UNUSED guint num, const GValue *values)
{
    GtkAllocation* alloc = (GtkAllocation*)g_value_get_boxed(&values[1]);
    return rb_ary_new3(2, GVAL2RVAL(&values[0]), BOXED2RVAL(alloc, GTK_TYPE_ALLOCATION));
}

static VALUE
rg_add_device_events(VALUE self, VALUE device, VALUE events)
{
    gtk_widget_add_device_events(_SELF(self),
                                 GDK_DEVICE(RVAL2GOBJ(device)),
                                 RVAL2GENUM(events, GDK_TYPE_EVENT_MASK));

    return self;
}

static VALUE
rg_compute_expand(VALUE self, VALUE orientation)
{
    gboolean result;

    result = gtk_widget_compute_expand(_SELF(self),
                                       RVAL2GENUM(orientation, GTK_TYPE_ORIENTATION));

    return CBOOL2RVAL(result);
}

static VALUE
rg_device_is_shadowed_p(VALUE self, VALUE device)
{
    return CBOOL2RVAL(gtk_widget_device_is_shadowed(_SELF(self),
                                                    GDK_DEVICE(RVAL2GOBJ(device))));
}

static VALUE
rg_draw(VALUE self, VALUE cr)
{
    gtk_widget_draw(_SELF(self), RVAL2CRCONTEXT(cr));

    return self;
}

static VALUE
rg_allocated_height(VALUE self)
{
    return INT2NUM(gtk_widget_get_allocated_height(_SELF(self)));
}

static VALUE
rg_allocated_width(VALUE self)
{
    return INT2NUM(gtk_widget_get_allocated_width(_SELF(self)));
}

/* TODO: device_enabled_p? */
static VALUE
rg_get_device_enabled_p(VALUE self, VALUE device)
{
    return CBOOL2RVAL(gtk_widget_get_device_enabled(_SELF(self),
                                                    GDK_DEVICE(RVAL2GOBJ(device))));
}

static VALUE
rg_get_device_events(VALUE self, VALUE device)
{
    GdkEventMask mask;

    mask = gtk_widget_get_device_events(_SELF(self), GDK_DEVICE(RVAL2GOBJ(device)));

    return GENUM2RVAL(mask, GDK_TYPE_EVENT_MASK);
}

static VALUE
rg_preferred_height(VALUE self)
{
    gint minimum_height, natural_height;

    gtk_widget_get_preferred_height(_SELF(self), &minimum_height, &natural_height);

    return rb_ary_new3(2, INT2NUM(minimum_height), INT2NUM(natural_height));
}

static VALUE
rg_get_preferred_height_for_width(VALUE self, VALUE width)
{
    gint minimum_height, natural_height;

    gtk_widget_get_preferred_height_for_width(_SELF(self), NUM2INT(width),
                                              &minimum_height, &natural_height);

    return rb_ary_new3(2, INT2NUM(minimum_height), INT2NUM(natural_height));
}

static VALUE
rg_preferred_size(VALUE self)
{
    GtkRequisition minimum_size, natural_size;

    gtk_widget_get_preferred_size(_SELF(self), &minimum_size, &natural_size);

    return rb_ary_new3(2,
                       rb_ary_new3(2, minimum_size.width, minimum_size.height),
                       rb_ary_new3(2, natural_size.width, natural_size.height));
}

static VALUE
rg_preferred_width(VALUE self)
{
    gint minimum_width, natural_width;

    gtk_widget_get_preferred_width(_SELF(self), &minimum_width, &natural_width);

    return rb_ary_new3(2, INT2NUM(minimum_width), INT2NUM(natural_width));
}

static VALUE
rg_get_preferred_width_for_height(VALUE self, VALUE height)
{
    gint minimum_width, natural_width;

    gtk_widget_get_preferred_width_for_height(_SELF(self), NUM2INT(height),
                                              &minimum_width, &natural_width);

    return rb_ary_new3(2, INT2NUM(minimum_width), INT2NUM(natural_width));
}

static VALUE
rg_request_mode(VALUE self)
{
    return GENUM2RVAL(gtk_widget_get_request_mode(_SELF(self)), GTK_TYPE_SIZE_REQUEST_MODE);
}

static VALUE
rg_state_flags(VALUE self)
{
    return GFLAGS2RVAL(gtk_widget_get_state_flags(_SELF(self)), GTK_TYPE_STATE_FLAGS);
}

static VALUE
rg_style_context(VALUE self)
{
    return GOBJ2RVAL(gtk_widget_get_style_context(_SELF(self)));
}

static VALUE
rg_support_multidevice_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_get_support_multidevice(_SELF(self)));
}

#if GTK_CHECK_VERSION(3, 2, 0)
static VALUE
rg_has_visible_focus_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_has_visible_focus(_SELF(self)));
}
#endif

static VALUE
rg_in_destruction_p(VALUE self)
{
    return CBOOL2RVAL(gtk_widget_in_destruction(_SELF(self)));
}

static VALUE
rg_input_shape_combine_region(VALUE self, VALUE region)
{
    gtk_widget_input_shape_combine_region(_SELF(self),
                                          NIL_P(region) ? NULL : RVAL2CRREGION(region));

    return self;
}

static VALUE
rg_override_background_color(VALUE self, VALUE state, VALUE color)
{
    gtk_widget_override_background_color(_SELF(self),
                                         RVAL2GFLAGS(state, GTK_TYPE_STATE_FLAGS),
                                         NIL_P(color) ? NULL : RVAL2GDKRGBA(color));

    return self;
}

static VALUE
rg_override_color(VALUE self, VALUE state, VALUE color)
{
    gtk_widget_override_color(_SELF(self),
                              RVAL2GFLAGS(state, GTK_TYPE_STATE_FLAGS),
                              NIL_P(color) ? NULL : RVAL2GDKRGBA(color));

    return self;
}

static VALUE
rg_override_cursor(VALUE self, VALUE cursor, VALUE secondary_cursor)
{
    gtk_widget_override_cursor(_SELF(self),
                               NIL_P(cursor) ? NULL : RVAL2GDKRGBA(cursor),
                               NIL_P(secondary_cursor) ? NULL : RVAL2GDKRGBA(secondary_cursor));

    return self;
}

static VALUE
rg_override_font(VALUE self, VALUE font_desc)
{
    gtk_widget_override_font(_SELF(self),
                             NIL_P(font_desc) ? NULL : RVAL2BOXED(font_desc, PANGO_TYPE_FONT_DESCRIPTION));

    return self;
}

static VALUE
rg_override_symbolic_color(VALUE self, VALUE name, VALUE color)
{
    gtk_widget_override_symbolic_color(_SELF(self),
                                       RVAL2CSTR(name),
                                       NIL_P(color) ? NULL : RVAL2GDKRGBA(color));

    return self;
}

static VALUE
rg_queue_compute_expand(VALUE self)
{
    gtk_widget_queue_compute_expand(_SELF(self));

    return self;
}

static VALUE
rg_queue_draw_region(VALUE self, VALUE region)
{
    gtk_widget_queue_draw_region(_SELF(self), RVAL2CRREGION(region));

    return self;
}

static VALUE
rg_render_icon_pixbuf(VALUE self, VALUE stock_id, VALUE size)
{
    GdkPixbuf *pixbuf;

    pixbuf = gtk_widget_render_icon_pixbuf(_SELF(self),
                                           RVAL2CSTR(stock_id),
                                           RVAL2GENUM(size, GTK_TYPE_ICON_SIZE));

    return GOBJ2RVAL(pixbuf);
}

static VALUE
rg_reset_style(VALUE self)
{
    gtk_widget_reset_style(_SELF(self));

    return self;
}

static VALUE
rg_send_focus_change(VALUE self, VALUE event)
{
    gboolean result;

    result = gtk_widget_send_focus_change(_SELF(self), RVAL2BOXED(event, GDK_TYPE_EVENT));

    return CBOOL2RVAL(result);
}

static VALUE
rg_set_device_enabled(VALUE self, VALUE device, VALUE enabled)
{
    gtk_widget_set_device_enabled(_SELF(self),
                                  GDK_DEVICE(RVAL2GOBJ(device)),
                                  RVAL2CBOOL(enabled));

    return self;
}

static VALUE
rg_set_device_events(VALUE self, VALUE device, VALUE events)
{
    gtk_widget_set_device_events(_SELF(self),
                                 GDK_DEVICE(RVAL2GOBJ(device)),
                                 RVAL2GENUM(events, GDK_TYPE_EVENT_MASK));

    return self;
}

static VALUE
rg_set_mapped(VALUE self, VALUE mapped)
{
    gtk_widget_set_mapped(_SELF(self), RVAL2CBOOL(mapped));

    return self;
}

static VALUE
rg_set_realized(VALUE self, VALUE realized)
{
    gtk_widget_set_realized(_SELF(self), RVAL2CBOOL(realized));

    return self;
}

static VALUE
rg_set_state_flags(VALUE self, VALUE flags, VALUE clear)
{
    gtk_widget_set_state_flags(_SELF(self),
                               RVAL2GFLAGS(flags, GTK_TYPE_STATE_FLAGS),
                               RVAL2CBOOL(clear));

    return self;
}

static VALUE
rg_set_support_multidevice(VALUE self, VALUE support_multidevice)
{
    gtk_widget_set_support_multidevice(_SELF(self), RVAL2CBOOL(support_multidevice));

    return self;
}

static VALUE
rg_set_visual(VALUE self, VALUE visual)
{
    gtk_widget_set_visual(_SELF(self), GDK_VISUAL(RVAL2GOBJ(visual)));

    return self;
}

static VALUE
rg_shape_combine_region(VALUE self, VALUE region)
{
    gtk_widget_shape_combine_region(_SELF(self),
                                    NIL_P(region) ? NULL : RVAL2CRREGION(region));

    return self;
}

static VALUE
rg_unset_state_flags(VALUE self, VALUE flags)
{
    gtk_widget_unset_state_flags(_SELF(self),
                                 RVAL2GFLAGS(flags, GTK_TYPE_STATE_FLAGS));

    return self;
}

void 
Init_gtk_widget(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_WIDGET, "Widget", mGtk);

    rb_global_variable(&style_prop_func_table);
    style_prop_func_table = rb_hash_new();

    RG_DEF_METHOD(unparent, 0);
    RG_DEF_METHOD(show, 0);
    RG_DEF_METHOD(show_now, 0);
    RG_DEF_METHOD(hide, 0);
    RG_DEF_METHOD(show_all, 0);
    RG_DEF_METHOD(map, 0);
    RG_DEF_METHOD(unmap, 0);
    RG_DEF_METHOD(realize, 0);
    RG_DEF_METHOD(unrealize, 0);
    RG_DEF_METHOD(queue_draw, 0);
    RG_DEF_METHOD(queue_resize, 0);
    RG_DEF_METHOD(queue_resize_no_redraw, 0);
    RG_DEF_METHOD(size_allocate, 1);
    RG_DEF_METHOD(add_accelerator, 5);
    RG_DEF_METHOD(remove_accelerator, 3);
    RG_DEF_METHOD(set_accel_path, 2);
    RG_DEF_METHOD(accel_closures, 0);
    RG_DEF_METHOD_P(can_activate_accel, 1);
    RG_DEF_METHOD(event, 1);
    RG_DEF_METHOD(activate, 0);
    RG_DEF_METHOD(reparent, 1);
    RG_DEF_SMETHOD(install_style_property, 1);
    RG_DEF_SMETHOD(style_property, 1);
    RG_DEF_SMETHOD(style_properties, -1);
    RG_DEF_METHOD(intersect, 1);
    RG_DEF_METHOD(grab_default, 0);
    RG_DEF_METHOD(set_parent_window, 1);
    RG_DEF_METHOD(parent_window, 0);
    RG_DEF_METHOD(add_events, 1);
    RG_DEF_METHOD(toplevel, 0);
    RG_DEF_METHOD(get_ancestor, 1);
    RG_DEF_METHOD(visual, 0);
    RG_DEF_METHOD(pointer, 0);
    RG_DEF_METHOD_P(ancestor, 1);
    RG_DEF_METHOD(translate_coordinates, 3);
    RG_DEF_METHOD(hide_on_delete, 0);
    RG_DEF_METHOD(set_direction, 1);
    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD(composite_name, 0);
    RG_DEF_METHOD(create_pango_context, 0);
    RG_DEF_METHOD(pango_context, 0);
    RG_DEF_METHOD(create_pango_layout, -1);
    RG_DEF_METHOD(queue_draw_area, 4);
    RG_DEF_METHOD(set_redraw_on_allocate, 1);
    RG_DEF_METHOD(set_composite_name, 1);
    RG_DEF_METHOD(mnemonic_activate, 1);
    RG_DEF_METHOD(region_intersect, 1);
    RG_DEF_METHOD(send_expose, 1);
    RG_DEF_METHOD(style_get_property, 1);
    RG_DEF_METHOD(accessible, 0);
    RG_DEF_METHOD(child_focus, 1);
    RG_DEF_METHOD(error_bell, 0);
    RG_DEF_METHOD(keynav_failed, 0);
    RG_DEF_METHOD(child_notify, 1);
    RG_DEF_METHOD(freeze_child_notify, 0);
    RG_DEF_METHOD_P(child_visible, 0);
    RG_DEF_METHOD(settings, 0);
    RG_DEF_METHOD(get_clipboard, 1);
    RG_DEF_METHOD(display, 0);
    RG_DEF_METHOD(root_window, 0);
    RG_DEF_METHOD(screen, 0);
    RG_DEF_METHOD_P(has_screen, 0);
    RG_DEF_METHOD(set_child_visible, 1);
    RG_DEF_METHOD(get_size_request, 0);
    RG_DEF_METHOD(set_size_request, 2);
    RG_DEF_METHOD(thaw_child_notify, 0);
    RG_DEF_METHOD(mnemonic_labels, 0);
    RG_DEF_METHOD(add_mnemonic_label, 1);
    RG_DEF_METHOD(remove_mnemonic_label, 1);
    RG_DEF_METHOD(set_tooltip_window, 1);
    RG_DEF_METHOD(tooltip_window, 0);
    RG_DEF_METHOD(trigger_tooltip_query, 0);
    RG_DEF_METHOD_P(composited, 0);
    RG_DEF_METHOD(set_window, 1);
    RG_DEF_METHOD(allocation, 0);
    RG_DEF_METHOD(set_allocation, 1);
    RG_DEF_METHOD(destroy, 0);
    RG_DEF_METHOD(bindings_activate, 2);

    RG_DEF_METHOD_P(toplevel, 0);
    RG_DEF_METHOD_P(realized, 0);
    RG_DEF_METHOD_P(mapped, 0);
    RG_DEF_METHOD_P(drawable, 0);
    RG_DEF_METHOD_P(sensitive_with_parent, 0);
    RG_DEF_METHOD_P(has_grab, 0);

    RG_DEF_SMETHOD(set_default_direction, 1);
    RG_DEF_SMETHOD(default_direction, 0);
    RG_DEF_SMETHOD(pop_composite_child, 0);
    RG_DEF_SMETHOD(push_composite_child, 0);
    RG_DEF_SMETHOD(binding_set, 0);
    RG_DEF_METHOD_P(has_window, 0);
    RG_DEF_METHOD(set_has_window, 1);

    RG_DEF_METHOD(add_device_events, 2);
    RG_DEF_METHOD(compute_expand, 1);
    RG_DEF_METHOD_P(device_is_shadowed, 1);
    RG_DEF_METHOD(draw, 1);
    RG_DEF_METHOD(allocated_height, 0);
    RG_DEF_METHOD(allocated_width, 0);
    RG_DEF_METHOD_P(get_device_enabled, 1);
    RG_DEF_METHOD(get_device_events, 1);
    RG_DEF_METHOD(preferred_height, 0);
    RG_DEF_METHOD(get_preferred_height_for_width, 1);
    RG_DEF_METHOD(preferred_size, 0);
    RG_DEF_METHOD(preferred_width, 0);
    RG_DEF_METHOD(get_preferred_width_for_height, 1);
    RG_DEF_METHOD(request_mode, 0);
    RG_DEF_METHOD(state_flags, 0);
    RG_DEF_METHOD(style_context, 0);
    RG_DEF_METHOD_P(support_multidevice, 0);
#if GTK_CHECK_VERSION(3, 2, 0)
    RG_DEF_METHOD_P(has_visible_focus, 0);
#endif
    RG_DEF_METHOD_P(in_destruction, 0);
    RG_DEF_METHOD(input_shape_combine_region, 1);
    RG_DEF_METHOD(override_background_color, 2);
    RG_DEF_METHOD(override_color, 2);
    RG_DEF_METHOD(override_cursor, 2);
    RG_DEF_METHOD(override_font, 1);
    RG_DEF_METHOD(override_symbolic_color, -1);
    RG_DEF_METHOD(queue_compute_expand, 0);
    RG_DEF_METHOD(queue_draw_region, 1);
    RG_DEF_METHOD(render_icon_pixbuf, 2);
    RG_DEF_METHOD(reset_style, 0);
    RG_DEF_METHOD(send_focus_change, 1);
    RG_DEF_METHOD(set_device_enabled, 2);
    RG_DEF_METHOD(set_device_events, 2);
    RG_DEF_METHOD(set_mapped, 1);
    RG_DEF_METHOD(set_realized, 1);
    RG_DEF_METHOD(set_state_flags, 2);
    RG_DEF_METHOD(set_support_multidevice, 1);
    RG_DEF_METHOD(set_visual, 1);
    RG_DEF_METHOD(shape_combine_region, 1);
    RG_DEF_METHOD(unset_state_flags, 1);
    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    G_DEF_CLASS(GTK_TYPE_WIDGET_HELP_TYPE, "HelpType", RG_TARGET_NAMESPACE);

    G_DEF_CLASS(GTK_TYPE_TEXT_DIRECTION, "TextDirection", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GTK_TYPE_ALIGN, "Align", RG_TARGET_NAMESPACE);

    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "size-request", (GValToRValSignalFunc)widget_signal_size_request);
    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "size-allocate", (GValToRValSignalFunc)widget_signal_size_allocate);

}
