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
#define _SELF(self) (RVAL2GTKWIDGET(self))

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
    gtk_widget_size_allocate(_SELF(self), RVAL2GTKALLOCATION(alloc));
    return self;
}

static VALUE
rg_add_accelerator(VALUE self, VALUE sig, VALUE accel, VALUE key, VALUE mod, VALUE flag)
{
    gtk_widget_add_accelerator(_SELF(self),
                               RVAL2CSTR(sig),
                               RVAL2GTKACCELGROUP(accel),
                               NUM2INT(key),
                               RVAL2GDKMODIFIERTYPE(mod),
                               RVAL2GTKACCELFLAGS(flag));
    return self;
}

static VALUE
rg_remove_accelerator(VALUE self, VALUE accel, VALUE key, VALUE mod)
{
    return CBOOL2RVAL(gtk_widget_remove_accelerator(_SELF(self),
                                                    RVAL2GTKACCELGROUP(accel),
                                                    NUM2INT(key),
                                                    RVAL2GDKMODIFIERTYPE(mod)));
}

static VALUE
rg_set_accel_path(VALUE self, VALUE accel_path, VALUE accel_group)
{
    gtk_widget_set_accel_path(_SELF(self), RVAL2CSTR(accel_path), 
                              RVAL2GTKACCELGROUP(accel_group));
    return self;
}

static VALUE
rg_accel_closures(VALUE self)
{
    return BOXEDGLIST2RVAL_FREE(gtk_widget_list_accel_closures(_SELF(self)),
                                G_TYPE_CLOSURE,
                                g_list_free, NULL);
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
                                        RVAL2GDKRECTANGLE(area),
                                        &intersection);
    return ret ? GDKRECTANGLE2RVAL(&intersection) : Qnil;
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
                                 RVAL2GDKWINDOW(parent_window));
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
    gtk_widget_set_direction(_SELF(self), RVAL2GTKTEXTDIRECTION(dir));
    return self;
}

static VALUE
rg_direction(VALUE self)
{
    return GTKTEXTDIRECTION2RVAL(gtk_widget_get_direction(_SELF(self)));
}

static VALUE
rg_s_set_default_direction(VALUE self, VALUE dir)
{
    gtk_widget_set_default_direction(RVAL2GTKTEXTDIRECTION(dir));
    return self;
}

static VALUE
rg_s_default_direction(G_GNUC_UNUSED VALUE self)
{
    return GTKTEXTDIRECTION2RVAL(gtk_widget_get_default_direction());
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
    GParamSpec* pspec = RVAL2GPARAMSPEC(spec);

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
        gtk_widget_style_get_property(RVAL2GTKWIDGET(self), name, &gval);
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
    return CBOOL2RVAL(gtk_widget_child_focus(_SELF(self), RVAL2GTKDIRECTIONTYPE(direction)));
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
    return CBOOL2RVAL(gtk_widget_keynav_failed(_SELF(self), RVAL2GTKDIRECTIONTYPE(direction)));
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
    return GOBJ2RVAL(gtk_widget_get_clipboard(_SELF(self), RVAL2ATOM(selection)));
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
    return GOBJGLIST2RVAL_FREE(gtk_widget_list_mnemonic_labels(_SELF(self)),
                               g_list_free, NULL);
}

static VALUE
rg_add_mnemonic_label(VALUE self, VALUE label)
{
    gtk_widget_add_mnemonic_label(_SELF(self), RVAL2GTKWIDGET(label));
    return self;
}

static VALUE
rg_remove_mnemonic_label(VALUE self, VALUE label)
{
    gtk_widget_remove_mnemonic_label(_SELF(self), RVAL2GTKWIDGET(label));
    return self;
}

static VALUE
rg_set_tooltip_window(VALUE self, VALUE custom_window)
{
    gtk_widget_set_tooltip_window(_SELF(self), RVAL2GTKWINDOW(custom_window));
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
    gtk_widget_set_window(_SELF(self), RVAL2GDKWINDOW(window));
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
    return GTKALLOCATION2RVAL(&alloc);
}

static VALUE
rg_set_allocation(VALUE self, VALUE alloc)
{
    gtk_widget_set_allocation(_SELF(self), RVAL2GTKALLOCATION(alloc));
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

    return GTKBINDINGSET2RVAL(binding_set);
}

static VALUE
rg_path(VALUE self)
{
    return GTKWIDGETPATH2RVAL(gtk_widget_get_path(_SELF(self)));
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
    return rb_ary_new3(2, GVAL2RVAL(&values[0]), GTKALLOCATION2RVAL(alloc));
}

static VALUE
rg_add_device_events(VALUE self, VALUE device, VALUE events)
{
    gtk_widget_add_device_events(_SELF(self),
                                 RVAL2GDKDEVICE(device),
                                 RVAL2GDKEVENTMASK(events));

    return self;
}

static VALUE
rg_compute_expand(VALUE self, VALUE orientation)
{
    gboolean result;

    result = gtk_widget_compute_expand(_SELF(self),
                                       RVAL2GTKORIENTATION(orientation));

    return CBOOL2RVAL(result);
}

static VALUE
rg_device_is_shadowed_p(VALUE self, VALUE device)
{
    return CBOOL2RVAL(gtk_widget_device_is_shadowed(_SELF(self),
                                                    RVAL2GDKDEVICE(device)));
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
                                                    RVAL2GDKDEVICE(device)));
}

static VALUE
rg_get_device_events(VALUE self, VALUE device)
{
    GdkEventMask mask;

    mask = gtk_widget_get_device_events(_SELF(self), RVAL2GDKDEVICE(device));

    return GDKEVENTMASK2RVAL(mask);
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
    return GTKSIZEREQUESTMODE2RVAL(gtk_widget_get_request_mode(_SELF(self)));
}

static VALUE
rg_state_flags(VALUE self)
{
    return GTKSTATEFLAGS2RVAL(gtk_widget_get_state_flags(_SELF(self)));
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
                                         RVAL2GTKSTATEFLAGS(state),
                                         NIL_P(color) ? NULL : RVAL2GDKRGBA(color));

    return self;
}

static VALUE
rg_override_color(VALUE self, VALUE state, VALUE color)
{
    gtk_widget_override_color(_SELF(self),
                              RVAL2GTKSTATEFLAGS(state),
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
                             NIL_P(font_desc) ? NULL : RVAL2PANGOFONTDESCRIPTION(font_desc));

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
    VALUE buffer;
    GdkPixbuf *pixbuf;

    pixbuf = gtk_widget_render_icon_pixbuf(_SELF(self),
                                           RVAL2GLIBID(stock_id, buffer),
                                           RVAL2GTKICONSIZE(size));

    return GOBJ2RVAL_UNREF(pixbuf);
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

    result = gtk_widget_send_focus_change(_SELF(self), RVAL2GDKEVENT(event));

    return CBOOL2RVAL(result);
}

static VALUE
rg_set_device_enabled(VALUE self, VALUE device, VALUE enabled)
{
    gtk_widget_set_device_enabled(_SELF(self),
                                  RVAL2GDKDEVICE(device),
                                  RVAL2CBOOL(enabled));

    return self;
}

static VALUE
rg_set_device_events(VALUE self, VALUE device, VALUE events)
{
    gtk_widget_set_device_events(_SELF(self),
                                 RVAL2GDKDEVICE(device),
                                 RVAL2GDKEVENTMASK(events));

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
                               RVAL2GTKSTATEFLAGS(flags),
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
    gtk_widget_set_visual(_SELF(self), RVAL2GDKVISUAL(visual));

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
                                 RVAL2GTKSTATEFLAGS(flags));

    return self;
}

static VALUE
rg_drag_dest_set(VALUE self, VALUE flags, VALUE targets, VALUE actions)
{
    GtkDestDefaults flg = RVAL2GTKDESTDEFAULTS(flags);
    GdkDragAction acts = RVAL2GDKDRAGACTION(actions);
    long n;
    GtkTargetEntry *entries = RVAL2GTKTARGETENTRIES_ACCEPT_NIL(targets, &n);

    gtk_drag_dest_set(_SELF(self), flg, entries, n, acts);

    g_free(entries);

    return self;
}

static VALUE
rg_drag_dest_set_proxy(VALUE self, VALUE proxy_window, VALUE protocol, VALUE use_coordinates)
{
    gtk_drag_dest_set_proxy(_SELF(self), 
                            RVAL2GDKWINDOW(proxy_window),
                            RVAL2GDKDRAGPROTOCOL(protocol), 
                            RVAL2CBOOL(use_coordinates)); 
    return self;
}

static VALUE
rg_drag_dest_unset(VALUE self)
{
    gtk_drag_dest_unset(_SELF(self));
    return self;
}

static VALUE
rg_drag_dest_find_target(int argc, VALUE *argv, VALUE self)
{
    VALUE context, target_list;
    GdkAtom ret;
    rb_scan_args(argc, argv, "11", &context, &target_list);

    ret = gtk_drag_dest_find_target(_SELF(self),
                                    RVAL2GDKDRAGCONTEXT(context),
                                    NIL_P(target_list) ? NULL : RVAL2GTKTARGETLIST(target_list));

    return GDKATOM2RVAL(ret);
}

static VALUE
rg_drag_dest_get_target_list(VALUE self)
{
    GtkTargetList* list = gtk_drag_dest_get_target_list(_SELF(self));
    return GTKTARGETLIST2RVAL(list);
}

static VALUE
rg_drag_dest_set_target_list(VALUE self, VALUE target_list)
{
    gtk_drag_dest_set_target_list(_SELF(self), 
                                  NIL_P(target_list) ? NULL : RVAL2GTKTARGETLIST(target_list));

    return self;
}

static VALUE
rg_drag_dest_add_text_targets(VALUE self)
{
    gtk_drag_dest_add_text_targets(_SELF(self));
    return self;
}

static VALUE
rg_drag_dest_add_image_targets(VALUE self)
{
    gtk_drag_dest_add_image_targets(_SELF(self));
    return self;
}

static VALUE
rg_drag_dest_add_uri_targets(VALUE self)
{
    gtk_drag_dest_add_uri_targets(_SELF(self));
    return self;
}

static VALUE
rg_drag_dest_set_track_motion(VALUE self, VALUE track_motion)
{
    gtk_drag_dest_set_track_motion(_SELF(self), RVAL2CBOOL(track_motion));
    return self;
}

static VALUE
rg_drag_dest_get_track_motion(VALUE self)
{
    return CBOOL2RVAL(gtk_drag_dest_get_track_motion(_SELF(self)));
}

static VALUE
rg_drag_get_data(VALUE self, VALUE context, VALUE target, VALUE time)
{
    gtk_drag_get_data(_SELF(self),
                      RVAL2GDKDRAGCONTEXT(context),
                      RVAL2ATOM(target),
                      NUM2UINT(time));
    return self;
}

static VALUE
rg_drag_highlight(VALUE self)
{
    gtk_drag_highlight(_SELF(self));
    return self;
}

static VALUE
rg_drag_unhighlight(VALUE self)
{
    gtk_drag_unhighlight(_SELF(self));
    return self;
}

static VALUE
rg_drag_begin(VALUE self, VALUE target_list, VALUE actions, VALUE button, VALUE event)
{
    return GOBJ2RVAL(gtk_drag_begin(_SELF(self),
                                    RVAL2GTKTARGETLIST(target_list),
                                    RVAL2GDKDRAGACTION(actions),
                                    NUM2INT(button),
                                    RVAL2GEV(event)));
}

static VALUE
rg_drag_threshold_p(VALUE self, VALUE start_x, VALUE start_y, VALUE current_x, VALUE current_y)
{
    return CBOOL2RVAL(gtk_drag_check_threshold(_SELF(self), 
                                               NUM2INT(start_x), NUM2INT(start_y),
                                               NUM2INT(current_x), NUM2INT(current_y)));
}

static VALUE
rg_drag_source_set(VALUE self, VALUE start_button_mask, VALUE targets, VALUE actions)
{
    GdkModifierType mask = RVAL2GDKMODIFIERTYPE(start_button_mask);
    GdkDragAction acts = RVAL2GDKDRAGACTION(actions);
    long n;
    GtkTargetEntry *entries = RVAL2GTKTARGETENTRIES(targets, &n);

    gtk_drag_source_set(_SELF(self), mask, entries, n, acts);

    g_free(entries);

    return self;
}

static VALUE
rg_drag_source_set_icon(VALUE self, VALUE icon)
{
    if (TYPE(icon) == T_HASH) {
        VALUE stock_id, icon_name, gicon, pixbuf, buffer;
        rbg_scan_options(icon,
                         "stock_id", &stock_id,
                         "icon_name", &icon_name,
                         "gicon", &gicon,
                         "pixbuf", &pixbuf,
                         NULL);

        if (!NIL_P(stock_id))
            gtk_drag_source_set_icon_stock(_SELF(self), RVAL2GLIBID(stock_id, buffer));
        else if (!NIL_P(icon_name))
            gtk_drag_source_set_icon_name(_SELF(self), RVAL2CSTR(icon_name));
#if GTK_CHECK_VERSION(3, 2, 0)
        else if (!NIL_P(gicon))
            gtk_drag_source_set_icon_gicon(_SELF(self), RVAL2GICON(gicon));
#endif
        else if (!NIL_P(pixbuf))
            gtk_drag_source_set_icon_pixbuf(_SELF(self), RVAL2GDKPIXBUF(pixbuf));
        else
            rb_raise(rb_eArgError, "Invalid arguments.");
    } else {
        GType gtype = RVAL2GTYPE(icon);

        if (gtype == GDK_TYPE_PIXBUF)
            gtk_drag_source_set_icon_pixbuf(_SELF(self), RVAL2GDKPIXBUF(icon));
#if GTK_CHECK_VERSION(3, 2, 0)
        else if (g_type_is_a(gtype, G_TYPE_ICON))
            gtk_drag_source_set_icon_gicon(_SELF(self), RVAL2GICON(icon));
#endif
        else
            rb_raise(rb_eArgError, "Invalid arguments.");
    }

    return self;
}

static VALUE
rg_drag_source_unset(VALUE self)
{
    gtk_drag_source_unset(_SELF(self));
    return self;
}

static VALUE
rg_drag_source_set_target_list(VALUE self, VALUE target_list)
{
    gtk_drag_source_set_target_list(_SELF(self),
                                    NIL_P(target_list) ? NULL : RVAL2GTKTARGETLIST(target_list));
    return self;
}

static VALUE
rg_drag_source_get_target_list(VALUE self)
{
    return GTKTARGETLIST2RVAL(gtk_drag_source_get_target_list(_SELF(self)));
}

static VALUE
rg_drag_source_add_text_targets(VALUE self)
{
    gtk_drag_source_add_text_targets(_SELF(self));
    return self;
}
static VALUE
rg_drag_source_add_image_targets(VALUE self)
{
    gtk_drag_source_add_image_targets(_SELF(self));
    return self;
}
static VALUE
rg_drag_source_add_uri_targets(VALUE self)
{
    gtk_drag_source_add_uri_targets(_SELF(self));
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

    RG_DEF_METHOD(path, 0);

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

    RG_DEF_METHOD(drag_dest_set, 3);
    RG_DEF_METHOD(drag_dest_set_proxy, 3);
    RG_DEF_METHOD(drag_dest_unset, 0);
    RG_DEF_METHOD(drag_dest_find_target, -1);
    RG_DEF_METHOD(drag_dest_get_target_list, 0);
    RG_DEF_METHOD(drag_dest_set_target_list, 1);
    RG_DEF_METHOD(drag_dest_add_text_targets, 0);
    RG_DEF_METHOD(drag_dest_add_image_targets, 0);
    RG_DEF_METHOD(drag_dest_add_uri_targets, 0);
    RG_DEF_METHOD(drag_dest_set_track_motion, 1);
    RG_DEF_METHOD(drag_dest_get_track_motion, 0);
    RG_DEF_METHOD(drag_get_data, 3);
    RG_DEF_METHOD(drag_highlight, 0);
    RG_DEF_METHOD(drag_unhighlight, 0);
    RG_DEF_METHOD(drag_begin, 4);
    RG_DEF_METHOD_P(drag_threshold, 4);
    RG_DEF_METHOD(drag_source_set, 3);
    RG_DEF_METHOD(drag_source_set_icon, 1);
    RG_DEF_METHOD(drag_source_unset, 0);
    RG_DEF_METHOD(drag_source_set_target_list, 1);
    RG_DEF_METHOD(drag_source_get_target_list, 0);
    RG_DEF_METHOD(drag_source_add_text_targets, 0);
    RG_DEF_METHOD(drag_source_add_image_targets, 0);
    RG_DEF_METHOD(drag_source_add_uri_targets, 0);

    G_DEF_CLASS(GTK_TYPE_WIDGET_HELP_TYPE, "HelpType", RG_TARGET_NAMESPACE);

    G_DEF_CLASS(GTK_TYPE_TEXT_DIRECTION, "TextDirection", RG_TARGET_NAMESPACE);
    G_DEF_CLASS(GTK_TYPE_ALIGN, "Align", RG_TARGET_NAMESPACE);

    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "size-request", (GValToRValSignalFunc)widget_signal_size_request);
    G_DEF_SIGNAL_FUNC(RG_TARGET_NAMESPACE, "size-allocate", (GValToRValSignalFunc)widget_signal_size_allocate);

}
