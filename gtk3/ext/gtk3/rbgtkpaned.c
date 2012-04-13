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

#define RG_TARGET_NAMESPACE cPaned
#define _SELF(self) (RVAL2GTKPANED(self))

static VALUE
rg_initialize(VALUE self, VALUE orientation)
{
    RBGTK_INITIALIZE(self, gtk_paned_new(RVAL2GTKORIENTATION(orientation)));

    return Qnil;
}

static VALUE
rg_add1(VALUE self, VALUE child)
{
    gtk_paned_add1(_SELF(self), RVAL2GTKWIDGET(child));
    return self;
}

static VALUE
rg_add2(VALUE self, VALUE child)
{
    gtk_paned_add2(_SELF(self), RVAL2GTKWIDGET(child));
    return self;
}

static VALUE
rg_pack1(VALUE self, VALUE child, VALUE resize, VALUE shrink)
{
    gtk_paned_pack1(_SELF(self), RVAL2GTKWIDGET(child),
                    RVAL2CBOOL(resize), RVAL2CBOOL(shrink));
    return self;
}

static VALUE
rg_pack2(VALUE self, VALUE child, VALUE resize, VALUE shrink)
{
    gtk_paned_pack2(_SELF(self), RVAL2GTKWIDGET(child),
                    RVAL2CBOOL(resize), RVAL2CBOOL(shrink));
    return self;
}

static VALUE
rg_child1(VALUE self)
{
    return GOBJ2RVAL(gtk_paned_get_child1(_SELF(self)));
}

static VALUE
rg_child2(VALUE self)
{
    return GOBJ2RVAL(gtk_paned_get_child2(_SELF(self)));
}

static VALUE
rg_handle_window(VALUE self)
{
    return GOBJ2RVAL(gtk_paned_get_handle_window(_SELF(self)));
}

void 
Init_gtk_paned(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_PANED, "Paned", mGtk);

    RG_DEF_METHOD(initialize, 1);
    RG_DEF_METHOD(add1, 1);
    RG_DEF_METHOD(add2, 1);
    RG_DEF_METHOD(pack1, 3);
    RG_DEF_METHOD(pack2, 3);
    RG_DEF_METHOD(child1, 0);
    RG_DEF_METHOD(child2, 0);
    RG_DEF_METHOD(handle_window, 0);
}
