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

#define RG_TARGET_NAMESPACE cThemingEngine
#define _SELF(self) (RVAL2GTKTHEMINGENGINE(self))

static VALUE
rg_s_load(G_GNUC_UNUSED VALUE self, VALUE name)
{
    return GOBJ2RVAL(gtk_theming_engine_load(RVAL2CSTR(name)));
}

static VALUE
rg_get_background_color(VALUE self, VALUE state)
{
    GdkRGBA color;

    gtk_theming_engine_get_background_color(_SELF(self),
                                            RVAL2GTKSTATEFLAGS(state),
                                            &color);

    return GDKRGBA2RVAL(&color);
}

static VALUE
rg_get_border(VALUE self, VALUE state)
{
    GtkBorder border;

    gtk_theming_engine_get_border(_SELF(self),
                                  RVAL2GTKSTATEFLAGS(state),
                                  &border);

    return GTKBORDER2RVAL(&border);
}

static VALUE
rg_get_border_color(VALUE self, VALUE state)
{
    GdkRGBA color;

    gtk_theming_engine_get_border_color(_SELF(self),
                                        RVAL2GTKSTATEFLAGS(state),
                                        &color);

    return GDKRGBA2RVAL(&color);
}

static VALUE
rg_get_color(VALUE self, VALUE state)
{
    GdkRGBA color;

    gtk_theming_engine_get_color(_SELF(self),
                                 RVAL2GTKSTATEFLAGS(state),
                                 &color);

    return GDKRGBA2RVAL(&color);
}

static VALUE
rg_direction(VALUE self)
{
    GtkTextDirection direction;

    direction = gtk_theming_engine_get_direction(_SELF(self));

    return GTKTEXTDIRECTION2RVAL(direction);
}

static VALUE
rg_get_font(VALUE self, VALUE state)
{
    PangoFontDescription *desc;

    desc = gtk_theming_engine_get_font(_SELF(self),
                                       RVAL2GTKSTATEFLAGS(state));

    return PANGOFONTDESCRIPTION2RVAL(desc);
}

static VALUE
rg_junction_sides(VALUE self)
{
    GtkJunctionSides sides;

    sides = gtk_theming_engine_get_junction_sides(_SELF(self));

    return GTKJUNCTIONSIDES2RVAL(sides);
}

static VALUE
rg_get_margin(VALUE self, VALUE state)
{
    GtkBorder margin;

    gtk_theming_engine_get_margin(_SELF(self),
                                  RVAL2GTKSTATEFLAGS(state),
                                  &margin);

    return GTKBORDER2RVAL(&margin);
}

static VALUE
rg_get_padding(VALUE self, VALUE state)
{
    GtkBorder padding;

    gtk_theming_engine_get_padding(_SELF(self),
                                   RVAL2GTKSTATEFLAGS(state),
                                   &padding);

    return GTKBORDER2RVAL(&padding);
}

static VALUE
rg_path(VALUE self)
{
    return GTKWIDGETPATH2RVAL(gtk_theming_engine_get_path(_SELF(self)));
}

static VALUE
rg_get_property(VALUE self, VALUE property, VALUE state)
{
    GValue value = G_VALUE_INIT;
    VALUE ret = Qnil;

    gtk_theming_engine_get_property(_SELF(self),
                                    RVAL2CSTR(property),
                                    RVAL2GTKSTATEFLAGS(state),
                                    &value);
    if (G_VALUE_TYPE(&value) != G_TYPE_INVALID){
        ret = GVAL2RVAL(&value);
        g_value_unset(&value);
    } 

    return ret;
}

static VALUE
rg_screen(VALUE self)
{
    return GOBJ2RVAL(gtk_theming_engine_get_screen(_SELF(self)));
}

static VALUE
rg_state(VALUE self)
{
    GtkStateFlags state;

    state = gtk_theming_engine_get_state(_SELF(self));

    return GTKSTATEFLAGS2RVAL(state);
}

static VALUE
rg_get_style_property(VALUE self, VALUE property_name)
{
    GValue value = G_VALUE_INIT;
    VALUE ret = Qnil;

    gtk_theming_engine_get_style_property(_SELF(self),
                                          RVAL2CSTR(property_name),
                                          &value);
    if (G_VALUE_TYPE(&value) != G_TYPE_INVALID){
        ret = GVAL2RVAL(&value);
        g_value_unset(&value);
    } 

    return ret;
}

static VALUE
rg_has_class_p(VALUE self, VALUE style_class)
{
    return CBOOL2RVAL(gtk_theming_engine_has_class(_SELF(self), RVAL2CSTR(style_class)));
}

static VALUE
rg_has_region(VALUE self, VALUE style_region)
{
    GtkRegionFlags flags;
    gboolean result;

    result = gtk_theming_engine_has_region(_SELF(self), RVAL2CSTR(style_region), &flags);

    return result ? GTKREGIONFLAGS2RVAL(flags) : Qnil;
}

static VALUE
rg_lookup_color(VALUE self, VALUE color_name)
{
    GdkRGBA color;
    gboolean result;

    result = gtk_theming_engine_lookup_color(_SELF(self), RVAL2CSTR(color_name), &color);

    return result ? GDKRGBA2RVAL(&color) : Qnil;
}

static VALUE
rg_state_is_running(VALUE self, VALUE state)
{
    gdouble progress;
    gboolean result;

    result = gtk_theming_engine_state_is_running(_SELF(self),
                                                 RVAL2GTKSTATEFLAGS(state),
                                                 &progress);

    return result ? DBL2NUM(progress) : Qnil;
}

void
Init_gtk_themingengine(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_THEMING_ENGINE, "ThemingEngine", mGtk);

    RG_DEF_SMETHOD(load, 1);
    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD(get_background_color, 1);
    RG_DEF_METHOD(get_border, 1);
    RG_DEF_METHOD(get_border_color, 1);
    RG_DEF_METHOD(get_color, 1);
    RG_DEF_METHOD(get_font, 1);
    RG_DEF_METHOD(get_margin, 1);
    RG_DEF_METHOD(get_padding, 1);
    RG_DEF_METHOD(get_property, 2);
    RG_DEF_METHOD(get_style_property, 1);
    RG_DEF_METHOD_P(has_class, 1);
    RG_DEF_METHOD(has_region, 1);
    RG_DEF_METHOD(junction_sides, 0);
    RG_DEF_METHOD(lookup_color, 1);
    RG_DEF_METHOD(path, 0);
    RG_DEF_METHOD(screen, 0);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD(state_is_running, 1);
}
