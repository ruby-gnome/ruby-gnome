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

#include "global.h"

#define RG_TARGET_NAMESPACE cBorder
#define _SELF(self) ((GtkBorder*)RVAL2BOXED(self, GTK_TYPE_BORDER))

#define ATTR_INT(name)\
static VALUE \
border_int_ ## name (VALUE self)\
{\
    return INT2NUM(_SELF(self)->name);\
}\
static VALUE \
border_int_set_ ## name (VALUE self, VALUE val)\
{\
    _SELF(self)->name = NUM2INT(val); \
    return self;\
}

ATTR_INT(left);
ATTR_INT(right);
ATTR_INT(top);
ATTR_INT(bottom);

#define DEFINE_ACCESSOR(gt, type, name)                                  \
    rb_define_method(gt, G_STRINGIFY(name), border_ ## type ## _## name, 0);\
    rb_define_method(gt, G_STRINGIFY(set_ ## name), border_ ## type ## _set_## name, 1);

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

    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, int, left);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, int, right);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, int, top);
    DEFINE_ACCESSOR(RG_TARGET_NAMESPACE, int, bottom);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
