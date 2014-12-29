/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cWidgetPath
#define _SELF(self) (RVAL2GTKWIDGETPATH(self))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_widget_path_new());
    return Qnil;
}

static VALUE
rg_append_type(VALUE self, VALUE type)
{
    gint pos;

    pos = gtk_widget_path_append_type(_SELF(self), CLASS2GTYPE(type));

    return INT2NUM(pos);
}

static VALUE
rg_object_type(VALUE self)
{
    return GTYPE2CLASS(gtk_widget_path_get_object_type(_SELF(self)));
}

static VALUE
rg_has_parent_p(VALUE self, VALUE type)
{
    return CBOOL2RVAL(gtk_widget_path_has_parent(_SELF(self), CLASS2GTYPE(type)));
}

static VALUE
rg_type_p(VALUE self, VALUE type)
{
    return CBOOL2RVAL(gtk_widget_path_is_type(_SELF(self), CLASS2GTYPE(type)));
}

static VALUE
rg_iter_add_class(VALUE self, VALUE pos, VALUE name)
{
    gtk_widget_path_iter_add_class(_SELF(self), NUM2INT(pos), RVAL2CSTR(name));

    return self;
}

static VALUE
rg_iter_add_region(VALUE self, VALUE pos, VALUE name, VALUE flags)
{
    gtk_widget_path_iter_add_region(_SELF(self),
                                    NUM2INT(pos),
                                    RVAL2CSTR(name),
                                    RVAL2GTKREGIONFLAGS(flags));

    return self;
}

static VALUE
rg_iter_clear_classes(VALUE self, VALUE pos)
{
    gtk_widget_path_iter_clear_classes(_SELF(self), NUM2INT(pos));

    return self;
}

static VALUE
rg_iter_clear_regions(VALUE self, VALUE pos)
{
    gtk_widget_path_iter_clear_regions(_SELF(self), NUM2INT(pos));

    return self;
}

static VALUE
rg_iter_get_name(VALUE self, VALUE pos)
{
    return CSTR2RVAL(gtk_widget_path_iter_get_name(_SELF(self), NUM2INT(pos)));
}

static VALUE
rg_iter_get_object_type(VALUE self, VALUE pos)
{
    return GTYPE2CLASS(gtk_widget_path_iter_get_object_type(_SELF(self), NUM2INT(pos)));
}

static VALUE
rg_iter_has_class_p(VALUE self, VALUE pos, VALUE name)
{
    return CBOOL2RVAL(gtk_widget_path_iter_has_class(_SELF(self), NUM2INT(pos), RVAL2CSTR(name)));
}

static VALUE
rg_iter_has_name_p(VALUE self, VALUE pos, VALUE name)
{
    return CBOOL2RVAL(gtk_widget_path_iter_has_name(_SELF(self), NUM2INT(pos), RVAL2CSTR(name)));
}

static VALUE
rg_iter_has_region(VALUE self, VALUE pos, VALUE name)
{
    GtkRegionFlags flags;
    gboolean result;

    result = gtk_widget_path_iter_has_region(_SELF(self), NUM2INT(pos), RVAL2CSTR(name), &flags);

    return result ? GTKREGIONFLAGS2RVAL(flags) : Qnil;
}

static VALUE
rg_iter_list_classes(VALUE self, VALUE pos)
{
    return CSTRGSLIST2RVAL_FREE(gtk_widget_path_iter_list_classes(_SELF(self), NUM2INT(pos)),
                                g_slist_free, NULL);
}

static VALUE
rg_iter_list_regions(VALUE self, VALUE pos)
{
    return CSTRGSLIST2RVAL_FREE(gtk_widget_path_iter_list_regions(_SELF(self), NUM2INT(pos)),
                                g_slist_free, NULL);
}

static VALUE
rg_iter_remove_class(VALUE self, VALUE pos, VALUE name)
{
    gtk_widget_path_iter_remove_class(_SELF(self), NUM2INT(pos), RVAL2CSTR(name));

    return self;
}

static VALUE
rg_iter_remove_region(VALUE self, VALUE pos, VALUE name)
{
    gtk_widget_path_iter_remove_region(_SELF(self), NUM2INT(pos), RVAL2CSTR(name));

    return self;
}

static VALUE
rg_iter_set_name(VALUE self, VALUE pos, VALUE name)
{
    gtk_widget_path_iter_set_name(_SELF(self), NUM2INT(pos), RVAL2CSTR(name));

    return self;
}

static VALUE
rg_iter_set_object_type(VALUE self, VALUE pos, VALUE type)
{
    gtk_widget_path_iter_set_object_type(_SELF(self), NUM2INT(pos), CLASS2GTYPE(type));

    return self;
}

static VALUE
rg_length(VALUE self)
{
    return INT2NUM(gtk_widget_path_length(_SELF(self)));
}

static VALUE
rg_prepend_type(VALUE self, VALUE type)
{
    gtk_widget_path_prepend_type(_SELF(self), CLASS2GTYPE(type));

    return self;
}

#if GTK_CHECK_VERSION(3, 2, 0)
static VALUE
rg_to_s(VALUE self)
{
    return CSTR2RVAL(gtk_widget_path_to_string(_SELF(self)));
}
#endif

void
Init_gtk_widgetpath(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_WIDGET_PATH, "WidgetPath", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(append_type, 1);
    RG_DEF_METHOD_P(has_parent, 1);
    RG_DEF_METHOD(iter_add_class, 2);
    RG_DEF_METHOD(iter_add_region, 3);
    RG_DEF_METHOD(iter_clear_classes, 1);
    RG_DEF_METHOD(iter_clear_regions, 1);
    RG_DEF_METHOD(iter_get_name, 1);
    RG_DEF_METHOD(iter_get_object_type, 1);
    RG_DEF_METHOD_P(iter_has_class, 2);
    RG_DEF_METHOD_P(iter_has_name, 2);
    RG_DEF_METHOD(iter_has_region, 2);
    RG_DEF_METHOD(iter_list_classes, 1);
    RG_DEF_METHOD(iter_list_regions, 1);
    RG_DEF_METHOD(iter_remove_class, 2);
    RG_DEF_METHOD(iter_remove_region, 2);
    RG_DEF_METHOD(iter_set_name, 2);
    RG_DEF_METHOD(iter_set_object_type, 2);
    RG_DEF_METHOD(length, 0);
    RG_DEF_METHOD(object_type, 0);
    RG_DEF_METHOD(prepend_type, 1);
#if GTK_CHECK_VERSION(3, 2, 0)
    RG_DEF_METHOD(to_s, 0);
#endif
    RG_DEF_METHOD_P(type, 1);
}
