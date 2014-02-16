/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2014 Ruby-GNOME2 Project Team
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

#if GTK_CHECK_VERSION(3, 10, 0)

#define RG_TARGET_NAMESPACE cStack
#define _SELF(self) (RVAL2GTKSTACK(self))

static VALUE
rg_initialize(VALUE self)
{
    RBGTK_INITIALIZE(self, gtk_stack_new());
    return Qnil;
}

static VALUE
rg_set_add_named(VALUE self, VALUE child_widget, VALUE name)
{
    gtk_stack_add_named(_SELF(self),
                        RVAL2GTKWIDGET(child_widget), RVAL2CSTR(name));
    return self;
}

static VALUE
rg_set_add_titled(VALUE self, VALUE child_widget, VALUE name, VALUE title)
{
    gtk_stack_add_titled(_SELF(self),
                        RVAL2GTKWIDGET(child_widget),
                        RVAL2CSTR(name),
                        RVAL2CSTR(title));
    return self;
}

static VALUE
rg_set_visible_child_full(VALUE self, VALUE name, VALUE transition_type)
{
    gtk_stack_set_visible_child_full(_SELF(self),
                                     RVAL2CSTR(name),
                                     RVAL2GTKSTACKTRANSITIONTYPE(transition_type));
    return self;
}

void
Init_gtk_stack(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_STACK, "Stack", mGtk);

    G_DEF_CLASS(GTK_TYPE_STACK_TRANSITION_TYPE, "TransitionType", RG_TARGET_NAMESPACE);
    RG_DEF_METHOD(initialize, 0);
    RG_DEF_METHOD(set_add_named, 2);
    RG_DEF_METHOD(set_add_titled, 3);
    RG_DEF_METHOD(set_visible_child_full, 2);
}
#endif
