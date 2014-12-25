/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cBuildable
#define _SELF(self) (RVAL2GTKBUILDABLE(self))

static VALUE
rg_builder_name(VALUE self)
{
    return CSTR2RVAL(gtk_buildable_get_name(_SELF(self)));
}

static VALUE
rg_set_builder_name(VALUE self, VALUE name)
{
    gtk_buildable_set_name(_SELF(self), RVAL2CSTR(name));
    return self;
}

static VALUE
rg_add_child(int argc, VALUE *argv, VALUE self)
{
    VALUE builder, child, type;

    rb_scan_args(argc, argv, "21", &builder, &child, &type);
    gtk_buildable_add_child(_SELF(self), RVAL2GTKBUILDER(builder), RVAL2GOBJ(child),
                            RVAL2CSTR_ACCEPT_NIL(type));
    return self;
}

static VALUE
rg_set_buildable_property(VALUE self, VALUE rb_builder,
                                 VALUE rb_name, VALUE rb_value)
{
    GtkBuilder *builder;
    const gchar *name;
    GValue value = G_VALUE_INIT;

    builder = RVAL2GTKBUILDER(rb_builder);
    name = RVAL2CSTR(rb_name);
    rbgobj_initialize_gvalue(&value, rb_value);
    gtk_buildable_set_buildable_property(_SELF(self), builder, name, &value);

    return self;
}

static VALUE
rg_construct_child(VALUE self, VALUE builder, VALUE name)
{
    return GOBJ2RVAL(gtk_buildable_construct_child(_SELF(self),
                                                   RVAL2GTKBUILDER(builder),
                                                   RVAL2CSTR(name)));
}

static VALUE
rg_get_internal_child(VALUE self, VALUE builder, VALUE child_name)
{
    return GOBJ2RVAL(gtk_buildable_get_internal_child(_SELF(self),
                                                      RVAL2GTKBUILDER(builder),
                                                      RVAL2CSTR(child_name)));
}

void
Init_gtk_buildable(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_BUILDABLE, "Buildable", mGtk);

    RG_DEF_METHOD(builder_name, 0);
    RG_DEF_METHOD(set_builder_name, 1);
    RG_DEF_METHOD(add_child, -1);
    RG_DEF_METHOD(set_buildable_property, 3);
    RG_DEF_METHOD(construct_child, 2);
    RG_DEF_METHOD(get_internal_child, 2);
}
