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

#define RG_TARGET_NAMESPACE cContainer
#define _SELF(self) (RVAL2GTKCONTAINER(self))

static GQuark q_ruby_setter;
static GQuark q_ruby_getter;

static void rb_gtk3_container_mark(gpointer object);

static void
rb_gtk3_container_mark_callback(GtkWidget *widget,
                                G_GNUC_UNUSED gpointer data)
{
    if (rbgobj_gc_mark_instance(widget))
        return;

    if (GTK_IS_CONTAINER(widget))
        rb_gtk3_container_mark(widget);
}

static void
rb_gtk3_container_mark(gpointer object)
{
    gtk_container_forall(GTK_CONTAINER(object),
                         rb_gtk3_container_mark_callback,
                         NULL);
}

/* FIXME: use rb_protect */
static void
get_child_prop_func(GtkContainer *container,
                    GtkWidget *child,
                    G_GNUC_UNUSED guint property_id,
                    GValue *value,
                    GParamSpec *pspec)
{
    ID ruby_getter = (ID)g_param_spec_get_qdata(pspec, q_ruby_getter);
    if (!ruby_getter) {
        gchar *name = g_strconcat("get_", g_param_spec_get_name(pspec), NULL);
        gchar *p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_getter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_getter, (gpointer)ruby_getter);
        g_free(name);
    }

    {
        VALUE ret = rb_funcall(GOBJ2RVAL(container), ruby_getter, 1, GOBJ2RVAL(child));
        rbgobj_rvalue_to_gvalue(ret, value);
    }
}

/* FIXME: use rb_protect */
static void
set_child_prop_func(GtkContainer *container,
                    GtkWidget *child,
                    G_GNUC_UNUSED guint property_id,
                    const GValue *value,
                    GParamSpec *pspec)
{
    ID ruby_setter = (ID)g_param_spec_get_qdata(pspec, q_ruby_setter);
    if (!ruby_setter) {
        gchar *name = g_strconcat("set_", g_param_spec_get_name(pspec), NULL);
        gchar *p;
        for (p = name; *p; p++) {
          if (*p == '-')
            *p = '_';
        }
        ruby_setter = rb_intern(name);
        g_param_spec_set_qdata(pspec, q_ruby_setter, (gpointer)ruby_setter);
        g_free(name);
    }

    rb_funcall(GOBJ2RVAL(container), ruby_setter,
               2,
               GOBJ2RVAL(child), GVAL2RVAL(value));
}

static void
class_init_func(gpointer g_class, gpointer class_data)
{
    GtkContainerClass *g_container_class = GTK_CONTAINER_CLASS(g_class);

    rbgobj_class_init_func(g_class, class_data);

    g_container_class->set_child_property = set_child_prop_func;
    g_container_class->get_child_property = get_child_prop_func;

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
        initialize_module = rb_define_module_under(klass, "ContainerHook");
        rbg_define_method(initialize_module,
                          "initialize", rg_initialize, -1);
        rb_include_module(klass, initialize_module);
    }

    return Qnil;
}

void
rbgtk3_container_init(void)
{
    VALUE mGtk;
    VALUE RG_TARGET_NAMESPACE;

    q_ruby_getter = g_quark_from_static_string("__ruby_getter");
    q_ruby_setter = g_quark_from_static_string("__ruby_setter");

    mGtk = rb_const_get(rb_cObject, rb_intern("Gtk"));
    RG_TARGET_NAMESPACE = rb_const_get(mGtk, rb_intern("Container"));

    rbgobj_register_mark_func(GTK_TYPE_CONTAINER, rb_gtk3_container_mark);

    RG_DEF_SMETHOD(type_register, -1);
}
