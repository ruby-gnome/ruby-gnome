/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
 *  Copyright (C) 2002,2003 Masao Mutoh
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

#include "rbatkprivate.h"

#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
#define _SELF(r) ((AtkTextRectangle*)RVAL2BOXED(r, ATK_TYPE_TEXT_RECTANGLE))

/**********************************/
static AtkTextRectangle*
atk_text_rectangle_copy(const AtkTextRectangle* val)
{
    AtkTextRectangle* new_val;
    g_return_val_if_fail (val != NULL, NULL);
    new_val = g_new(AtkTextRectangle, 1);
    *new_val = *val;
    return new_val;
}

GType
atk_text_rectangle_get_type(void)
{
    static GType our_type = 0;
                                                                                
    if (our_type == 0)
        our_type = g_boxed_type_register_static ("AtkTextRectangle",
                    (GBoxedCopyFunc)atk_text_rectangle_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
atktextrect_initialize(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    AtkTextRectangle new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);

    G_INITIALIZE(self, &new);
    return Qnil;
}

/* Struct accessors */
static VALUE
atktextrect_x(VALUE self)
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
atktextrect_y(VALUE self)
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
atktextrect_w(VALUE self)
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
atktextrect_h(VALUE self)
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
atktextrect_set_x(VALUE self, VALUE x)
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
atktextrect_set_y(VALUE self, VALUE y)
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}

static VALUE
atktextrect_set_w(VALUE self, VALUE width)
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
atktextrect_set_h(VALUE self, VALUE height)
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}

static VALUE
atktextrect_to_a(VALUE self)
{
  AtkTextRectangle* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}
#endif

void
Init_atk_text_rectangle(void)
{
#ifdef HAVE_ATK_TEXT_GET_BOUNDED_RANGES
    VALUE rect = G_DEF_CLASS(ATK_TYPE_TEXT_RECTANGLE, "TextRectangle", mAtk);

    rb_define_method(rect, "initialize", atktextrect_initialize, 4);
    rb_define_method(rect, "x", atktextrect_x, 0);
    rb_define_method(rect, "y", atktextrect_y, 0);
    rb_define_method(rect, "width", atktextrect_w, 0);
    rb_define_method(rect, "height", atktextrect_h, 0);
    rb_define_method(rect, "set_x", atktextrect_set_x, 1);
    rb_define_method(rect, "set_y", atktextrect_set_y, 1);
    rb_define_method(rect, "set_width", atktextrect_set_w, 1);
    rb_define_method(rect, "set_height", atktextrect_set_h, 1);
    rb_define_method(rect, "to_a", atktextrect_to_a, 0);

    G_DEF_SETTERS(rect);
#endif
}
