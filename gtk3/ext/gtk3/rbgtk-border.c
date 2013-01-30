/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004 Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cBorder
#define _SELF(self) (RVAL2GTKBORDER(self))

static VALUE
rg_initialize(VALUE self, VALUE left, VALUE right, VALUE top, VALUE bottom)
{
    GtkBorder border;
    border.left = NUM2INT(left);
    border.right = NUM2INT(right);
    border.top = NUM2INT(top);
    border.bottom = NUM2INT(bottom);

    G_INITIALIZE(self, &border);
    return Qnil;
}

static VALUE
rg_left(VALUE self)
{
    return INT2NUM(_SELF(self)->left);
}

static VALUE
rg_set_left(VALUE self, VALUE val)
{
    _SELF(self)->left = NUM2INT(val);
    return self;
}

static VALUE
rg_right(VALUE self)
{
    return INT2NUM(_SELF(self)->right);
}

static VALUE
rg_set_right(VALUE self, VALUE val)
{
    _SELF(self)->right = NUM2INT(val);
    return self;
}

static VALUE
rg_top(VALUE self)
{
    return INT2NUM(_SELF(self)->top);
}

static VALUE
rg_set_top(VALUE self, VALUE val)
{
    _SELF(self)->top = NUM2INT(val);
    return self;
}

static VALUE
rg_bottom(VALUE self)
{
    return INT2NUM(_SELF(self)->bottom);
}

static VALUE
rg_set_bottom(VALUE self, VALUE val)
{
    _SELF(self)->bottom = NUM2INT(val);
    return self;
}

static VALUE
rg_to_a(VALUE self)
{
    GtkBorder* border = _SELF(self);
    return rb_ary_new3(4, INT2NUM(border->left), INT2NUM(border->right),
                       INT2NUM(border->top), INT2NUM(border->bottom));
}

void 
Init_gtk_border(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_BORDER, "Border", mGtk);

    RG_DEF_METHOD(initialize, 4);
    RG_DEF_METHOD(to_a, 0);

    RG_DEF_METHOD(left, 0);
    RG_DEF_METHOD(set_left, 1);
    RG_DEF_METHOD(right, 0);
    RG_DEF_METHOD(set_right, 1);
    RG_DEF_METHOD(top, 0);
    RG_DEF_METHOD(set_top, 1);
    RG_DEF_METHOD(bottom, 0);
    RG_DEF_METHOD(set_bottom, 1);
}
