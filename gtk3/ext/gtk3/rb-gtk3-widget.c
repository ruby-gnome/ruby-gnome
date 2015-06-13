/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2015  Ruby-GNOME2 Project Team
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

void
rb_gtk3_widget_init(void)
{
    VALUE mGtk;
    VALUE RG_TARGET_NAMESPACE;

    mGtk = rb_const_get(rb_cObject, rb_intern("Gtk"));
    RG_TARGET_NAMESPACE = rb_const_get(mGtk, rb_intern("Widget"));

    RG_DEF_SMETHOD(type_register, -1);
}
