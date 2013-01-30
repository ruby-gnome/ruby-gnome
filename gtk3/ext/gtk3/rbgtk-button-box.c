/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002,2003 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cButtonBox
#define _SELF(self) (RVAL2GTKBUTTONBOX(self))

static VALUE
rg_initialize(VALUE self, VALUE orientation)
{
    RBGTK_INITIALIZE(self, gtk_button_box_new(RVAL2GTKORIENTATION(orientation)));

    return Qnil;
}

static VALUE
rg_set_child_secondary(VALUE self, VALUE child, VALUE is_secondary)
{
    gtk_button_box_set_child_secondary(_SELF(self), 
                                       RVAL2GTKWIDGET(child),
                                       RVAL2CBOOL(is_secondary));
    return self;
}

static VALUE
rg_get_child_secondary(VALUE self, VALUE child)
{
    return CBOOL2RVAL(gtk_button_box_get_child_secondary(_SELF(self), 
                                                         RVAL2GTKWIDGET(child)));
}

#if GTK_CHECK_VERSION(3, 2, 0)
static VALUE
rg_get_child_non_homogeneous(VALUE self, VALUE child)
{
    return CBOOL2RVAL(gtk_button_box_get_child_non_homogeneous(_SELF(self),
                                                               RVAL2GTKWIDGET(child)));
}

static VALUE
rg_set_child_non_homogeneous(VALUE self, VALUE child, VALUE non_homogeneous)
{
    gtk_button_box_set_child_non_homogeneous(_SELF(self),
                                             RVAL2GTKWIDGET(child),
                                             RVAL2CBOOL(non_homogeneous));

    return self;
}
#endif

void 
Init_gtk_button_box(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_BUTTON_BOX, "ButtonBox", mGtk);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(set_child_secondary, 2);
    RG_DEF_METHOD(get_child_secondary, 1);
#if GTK_CHECK_VERSION(3, 2, 0)
    RG_DEF_METHOD(get_child_non_homogeneous, 1);
    RG_DEF_METHOD(set_child_non_homogeneous, 2);
#endif

    G_DEF_CLASS(GTK_TYPE_BUTTON_BOX_STYLE, "Style", RG_TARGET_NAMESPACE);
}
