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

#define RG_TARGET_NAMESPACE cStyleContext
#define _SELF(self) (RVAL2GTKSTYLECONTEXT(self))

static VALUE
rg_initialize(VALUE self)
{
    G_INITIALIZE(self, gtk_style_context_new());

    return Qnil;
}

static VALUE
rg_add_class(VALUE self, VALUE class_name)
{
    gtk_style_context_add_class(_SELF(self), RVAL2CSTR(class_name));

    return self;
}

static VALUE
rg_add_provider(VALUE self, VALUE provider, VALUE priority)
{
    gtk_style_context_add_provider(_SELF(self),
                                   RVAL2GTKSTYLEPROVIDER(provider),
                                   NUM2UINT(priority));

    return self;
}

static VALUE
rg_add_region(VALUE self, VALUE region_name, VALUE flags)
{
    gtk_style_context_add_region(_SELF(self),
                                 RVAL2CSTR(region_name),
                                 RVAL2GTKREGIONFLAGS(flags));

    return self;
}

static VALUE
rg_get_background_color(VALUE self, VALUE state)
{
    GdkRGBA color;

    gtk_style_context_get_background_color(_SELF(self),
                                           RVAL2GTKSTATEFLAGS(state),
                                           &color);

    return GDKRGBA2RVAL(&color);
}

static VALUE
rg_get_border(VALUE self, VALUE state)
{
    GtkBorder border;

    gtk_style_context_get_border(_SELF(self),
                                 RVAL2GTKSTATEFLAGS(state),
                                 &border);

    return GTKBORDER2RVAL(&border);
}

static VALUE
rg_get_border_color(VALUE self, VALUE state)
{
    GdkRGBA color;

    gtk_style_context_get_border_color(_SELF(self),
                                       RVAL2GTKSTATEFLAGS(state),
                                       &color);

    return GDKRGBA2RVAL(&color);
}

static VALUE
rg_get_color(VALUE self, VALUE state)
{
    GdkRGBA color;

    gtk_style_context_get_color(_SELF(self),
                                RVAL2GTKSTATEFLAGS(state),
                                &color);

    return GDKRGBA2RVAL(&color);
}

static VALUE
rg_get_font(VALUE self, VALUE state)
{
    PangoFontDescription *desc;

    desc = gtk_style_context_get_font(_SELF(self),
                                      RVAL2GTKSTATEFLAGS(state));

    return PANGOFONTDESCRIPTION2RVAL(desc);
}

static VALUE
rg_junction_sides(VALUE self)
{
    GtkJunctionSides sides;

    sides = gtk_style_context_get_junction_sides(_SELF(self));

    return GTKJUNCTIONSIDES2RVAL(sides);
}

static VALUE
rg_get_margin(VALUE self, VALUE state)
{
    GtkBorder margin;

    gtk_style_context_get_margin(_SELF(self),
                                 RVAL2GTKSTATEFLAGS(state),
                                 &margin);

    return GTKBORDER2RVAL(&margin);
}

static VALUE
rg_get_padding(VALUE self, VALUE state)
{
    GtkBorder padding;

    gtk_style_context_get_padding(_SELF(self),
                                  RVAL2GTKSTATEFLAGS(state),
                                  &padding);

    return GTKBORDER2RVAL(&padding);
}

static VALUE
rg_path(VALUE self)
{
    return GTKWIDGETPATH2RVAL(gtk_style_context_get_path(_SELF(self)));
}

static VALUE
rg_get_property(VALUE self, VALUE property, VALUE state)
{
    GValue value = G_VALUE_INIT;
    VALUE ret = Qnil;

    gtk_style_context_get_property(_SELF(self),
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
rg_state(VALUE self)
{
    GtkStateFlags state;

    state = gtk_style_context_get_state(_SELF(self));

    return GTKSTATEFLAGS2RVAL(state);
}

static VALUE
rg_get_style_property(VALUE self, VALUE property_name)
{
    GValue value = G_VALUE_INIT;
    VALUE ret = Qnil;

    gtk_style_context_get_style_property(_SELF(self),
                                         RVAL2CSTR(property_name),
                                         &value);
    if (G_VALUE_TYPE(&value) != G_TYPE_INVALID){
        ret = GVAL2RVAL(&value);
        g_value_unset(&value);
    } 

    return ret;
}

static VALUE
rg_has_class_p(VALUE self, VALUE class_name)
{
    return CBOOL2RVAL(gtk_style_context_has_class(_SELF(self), RVAL2CSTR(class_name)));
}

static VALUE
rg_has_region(VALUE self, VALUE region_name)
{
    GtkRegionFlags flags;
    gboolean result;

    result = gtk_style_context_has_region(_SELF(self),
                                          RVAL2CSTR(region_name),
                                          &flags);

    return result ? GTKREGIONFLAGS2RVAL(flags) : Qnil;
}

static VALUE
rg_invalidate(VALUE self)
{
    gtk_style_context_invalidate(_SELF(self));

    return self;
}

static VALUE
rg_classes(VALUE self)
{
    return CSTRGLIST2RVAL_FREE(gtk_style_context_list_classes(_SELF(self)),
                               g_list_free, NULL);
}

static VALUE
rg_regions(VALUE self)
{
    return CSTRGLIST2RVAL_FREE(gtk_style_context_list_regions(_SELF(self)),
                               g_list_free, NULL);
}

static VALUE
rg_lookup_color(VALUE self, VALUE color_name)
{
    GdkRGBA color;
    gboolean result;

    result = gtk_style_context_lookup_color(_SELF(self), RVAL2CSTR(color_name), &color);

    return result ? GDKRGBA2RVAL(&color) : Qnil;
}

static VALUE
rg_lookup_icon_set(VALUE self, VALUE stock_id)
{
    VALUE buffer;
    return GTKICONSET2RVAL(gtk_style_context_lookup_icon_set(_SELF(self),
                                                             RVAL2GLIBID(stock_id, buffer)));
}

static VALUE
rg_pop_animatable_region(VALUE self)
{
    gtk_style_context_pop_animatable_region(_SELF(self));

    return self;
}

static VALUE
rg_push_animatable_region(VALUE self, VALUE region_id)
{
    gtk_style_context_push_animatable_region(_SELF(self), (gpointer)region_id);
    if (rb_block_given_p())
        rb_ensure(rb_yield, self, rg_pop_animatable_region, self);

    return self;
}

static VALUE
rg_remove_class(VALUE self, VALUE class_name)
{
    gtk_style_context_remove_class(_SELF(self), RVAL2CSTR(class_name));

    return self;
}

static VALUE
rg_remove_provider(VALUE self, VALUE provider)
{
    gtk_style_context_remove_provider(_SELF(self),
                                      RVAL2GTKSTYLEPROVIDER(provider));

    return self;
}

static VALUE
rg_remove_region(VALUE self, VALUE region_name)
{
    gtk_style_context_remove_region(_SELF(self), RVAL2CSTR(region_name));

    return self;
}

static VALUE
rg_restore(VALUE self)
{
    gtk_style_context_restore(_SELF(self));

    return self;
}

static VALUE
rg_save(VALUE self)
{
    gtk_style_context_save(_SELF(self));

    return self;
}

static VALUE
rg_set_background(VALUE self, VALUE window)
{
    gtk_style_context_set_background(_SELF(self), RVAL2GDKWINDOW(window));

    return self;
}

static VALUE
rg_set_junction_sides(VALUE self, VALUE sides)
{
    gtk_style_context_set_junction_sides(_SELF(self),
                                         RVAL2GTKJUNCTIONSIDES(sides));

    return self;
}

static VALUE
rg_set_path(VALUE self, VALUE path)
{
    gtk_style_context_set_path(_SELF(self), RVAL2GTKWIDGETPATH(path));

    return self;
}

static VALUE
rg_set_state(VALUE self, VALUE state)
{
    gtk_style_context_set_state(_SELF(self), RVAL2GTKSTATEFLAGS(state));

    return self;
}

static VALUE
rg_state_is_running(VALUE self, VALUE state)
{
    gdouble progress;
    gboolean result;

    result = gtk_style_context_state_is_running(_SELF(self),
                                                RVAL2GTKSTATEFLAGS(state),
                                                &progress);

    return result ? DBL2NUM(progress) : Qnil;
}

void
Init_gtk_stylecontext(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_STYLE_CONTEXT, "StyleContext", mGtk);

    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(add_class, 1);
    RG_DEF_METHOD(add_provider, 2);
    RG_DEF_METHOD(add_region, 2);
    RG_DEF_METHOD(classes, 0);
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
    RG_DEF_METHOD(invalidate, 0);
    RG_DEF_METHOD(junction_sides, 0);
    RG_DEF_METHOD(lookup_color, 1);
    RG_DEF_METHOD(lookup_icon_set, 1);
    RG_DEF_METHOD(path, 0);
    RG_DEF_METHOD(pop_animatable_region, 0);
    RG_DEF_METHOD(push_animatable_region, 1);
    RG_DEF_METHOD(regions, 0);
    RG_DEF_METHOD(remove_class, 1);
    RG_DEF_METHOD(remove_provider, 1);
    RG_DEF_METHOD(remove_region, 1);
    RG_DEF_METHOD(restore, 0);
    RG_DEF_METHOD(save, 0);
    RG_DEF_METHOD(set_background, 1);
    RG_DEF_METHOD(set_junction_sides, 1);
    RG_DEF_METHOD(set_path, 1);
    RG_DEF_METHOD(set_state, 1);
    RG_DEF_METHOD(state, 0);
    RG_DEF_METHOD(state_is_running, 1);
}
