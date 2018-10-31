/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015-2018  Ruby-GNOME2 Project Team
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

#include "rb-gtk3-private.h"

#include <rb_cairo.h>

#define RG_TARGET_NAMESPACE cWidget
#define _SELF(self) (RVAL2GTKWIDGET(self))

static void
class_init_func(gpointer g_class, gpointer class_data)
{
    rbgobj_class_init_func(g_class, class_data);
    rbgtk3_class_init_func(g_class, class_data);
}

static VALUE
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    rb_call_super(argc, argv);
    rbgtk3_initialize(self);
    return Qnil;
}

static VALUE
rg_s_type_register(int argc, VALUE *argv, VALUE klass)
{
    VALUE type_name;

    rb_scan_args(argc, argv, "01", &type_name);

    rbgobj_register_type(klass, type_name, class_init_func);

    {
        VALUE initialize_module;
        initialize_module = rb_define_module_under(klass, "WidgetHook");
        rbg_define_method(initialize_module,
                          "initialize", rg_initialize, -1);
        rb_include_module(klass, initialize_module);
    }

    return Qnil;
}

static void
rb_gtk3_widget_draw(RGClosureCallData *data)
{
    VALUE rb_widget;
    VALUE rb_cairo_context;
    VALUE rb_args;
    VALUE rb_stop_propagate;

    rb_widget = GVAL2RVAL(&(data->param_values[0]));
    rb_cairo_context = GVAL2RVAL(&(data->param_values[1]));
    rb_args = rb_ary_new_from_args(2, rb_widget, rb_cairo_context);
    if (!NIL_P(data->extra_args)) {
        rb_ary_concat(rb_args, data->extra_args);
    }
    rb_stop_propagate = rb_apply(data->callback,
                                 rb_intern("call"),
                                 rb_args);
    if (!RVAL2CBOOL(rb_funcall(rb_cairo_context, rb_intern("destroyed?"), 0))) {
        rb_funcall(rb_cairo_context, rb_intern("destroy"), 0);
    }
    g_value_set_boolean(data->return_value,
                        RVAL2CBOOL(rb_stop_propagate));
}

void
rbgtk3_widget_init(void)
{
    VALUE mGtk;
    VALUE RG_TARGET_NAMESPACE;

    mGtk = rb_const_get(rb_cObject, rb_intern("Gtk"));
    RG_TARGET_NAMESPACE = rb_const_get(mGtk, rb_intern("Widget"));

    RG_DEF_SMETHOD(type_register, -1);

    rbgobj_set_signal_call_func(RG_TARGET_NAMESPACE,
                                "draw",
                                rb_gtk3_widget_draw);
}
