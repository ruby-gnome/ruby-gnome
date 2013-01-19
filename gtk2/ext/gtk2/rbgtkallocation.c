/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2002-2004  Masao Mutoh
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

static GtkAllocation *
gtk_allocation_copy(const GtkAllocation *alloc)
{
  GtkAllocation *result = g_new (GtkAllocation, 1);
  *result = *alloc;

  return result;
}

GType
gtk_allocation_get_type(void)
{
  static GType our_type = 0;
  if (our_type == 0)
    our_type = g_boxed_type_register_static ("GtkAllocation",
                    (GBoxedCopyFunc)gtk_allocation_copy,
                    (GBoxedFreeFunc)g_free);
  return our_type;
}

#define RG_TARGET_NAMESPACE cAllocation
#define _SELF(r) ((GtkAllocation*)RVAL2BOXED(r, GTK_TYPE_ALLOCATION))

static VALUE
rg_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    GtkAllocation new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);

    G_INITIALIZE(self, &new);
    return Qnil;
}

static VALUE
rg_intersect(VALUE self, VALUE other)
{
    GtkAllocation dest;
    gboolean ret = gdk_rectangle_intersect(_SELF(self), _SELF(other), &dest);
    return ret ? BOXED2RVAL(&dest, GTK_TYPE_ALLOCATION) : Qnil;
}

static VALUE
rg_union(VALUE self, VALUE other)
{
    GtkAllocation dest;
    gdk_rectangle_union(_SELF(self), _SELF(other), &dest);
    return BOXED2RVAL(&dest, GTK_TYPE_ALLOCATION);
}

/* Struct accessors */
static VALUE
rg_x(VALUE self)
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
rg_y(VALUE self)
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
rg_width(VALUE self)
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
rg_height(VALUE self)
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
rg_set_x(VALUE self, VALUE x)
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
rg_set_y(VALUE self, VALUE y)
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}

static VALUE
rg_set_width(VALUE self, VALUE width)
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
rg_set_height(VALUE self, VALUE height)
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}

static VALUE
rg_to_a(VALUE self)
{
  GtkAllocation* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}

static VALUE
rg_to_rect(VALUE self)
{
    return BOXED2RVAL(_SELF(self), GDK_TYPE_RECTANGLE);
}

void 
Init_gtk_allocation(VALUE mGtk)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GTK_TYPE_ALLOCATION, "Allocation", mGtk);

    RG_DEF_METHOD(initialize, 4);
    RG_DEF_METHOD(intersect, 1);
    RG_DEF_ALIAS("&", "intersect");
    RG_DEF_METHOD(union, 1);
    RG_DEF_ALIAS("|", "union");
    RG_DEF_METHOD(x, 0);
    RG_DEF_METHOD(y, 0);
    RG_DEF_METHOD(width, 0);
    RG_DEF_METHOD(height, 0);
    RG_DEF_METHOD(set_x, 1);
    RG_DEF_METHOD(set_y, 1);
    RG_DEF_METHOD(set_width, 1);
    RG_DEF_METHOD(set_height, 1);
    RG_DEF_METHOD(to_a, 0);
    RG_DEF_METHOD(to_rect, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
