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

#define RG_TARGET_NAMESPACE cStyleProperties
#define _SELF(self) (RVAL2GTKSTYLEPROPERTIES(self))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_style_properties_new());

    return Qnil;
}

static VALUE
rg_clear(VALUE self)
{
    gtk_style_properties_clear(_SELF(self));

    return self;
}

static VALUE
rg_get_property(VALUE self, VALUE property, VALUE state)
{
    GValue value = G_VALUE_INIT;
    gboolean exist;
    VALUE ret = Qnil;

    exist = gtk_style_properties_get_property(_SELF(self),
                                               RVAL2CSTR(property),
                                               RVAL2GTKSTATEFLAGS(state),
                                               &value);
    if (exist){
        ret = GVAL2RVAL(&value);
        g_value_unset(&value);
    }

    return ret;
}

static VALUE
rg_lookup_color(VALUE self, VALUE name)
{
    return GOBJ2RVAL(gtk_style_properties_lookup_color(_SELF(self), RVAL2CSTR(name)));
}

static VALUE
rg_map_color(VALUE self, VALUE name, VALUE color)
{
    gtk_style_properties_map_color(_SELF(self),
                                   RVAL2CSTR(name),
                                   RVAL2GTKSYMBOLICCOLOR(color));

    return self;
}

static VALUE
rg_merge(VALUE self, VALUE other, VALUE replace)
{
    gtk_style_properties_merge(_SELF(self), _SELF(other), RVAL2CBOOL(replace));

    return self;
}

static VALUE
rg_set_property(VALUE self, VALUE property, VALUE state, VALUE value)
{
    gtk_style_properties_set_property(_SELF(self),
                                      RVAL2CSTR(property),
                                      RVAL2GTKSTATEFLAGS(state),
                                      RVAL2GVALUE(value));

    return self;
}

static VALUE
rg_unset_property(VALUE self, VALUE property, VALUE state)
{
    gtk_style_properties_unset_property(_SELF(self),
                                        RVAL2CSTR(property),
                                        RVAL2GTKSTATEFLAGS(state));

    return self;
}

void
Init_gtk_styleproperties(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_STYLE_PROPERTIES, "StyleProperties", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(clear, 0);
    RG_DEF_METHOD(get_property, 2);
    RG_DEF_METHOD(lookup_color, 1);
    RG_DEF_METHOD(map_color, 2);
    RG_DEF_METHOD(merge, 2);
    RG_DEF_METHOD(set_property, 3);
    RG_DEF_METHOD(unset_property, 2);
}
