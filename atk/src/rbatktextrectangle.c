/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatktextrectangle.c -

  $Author: mutoh $
  $Date: 2004/10/19 13:33:43 $

  Copyright (C) 2004 Masao Mutoh

  This file is derived from rbgdkrectangle.c.
  rbgdkrectangle.c -
  Copyright (C) 2002,2003 Masao Mutoh

  rbgdkrectangle.c file is derived from rbgdkregion.c.
  rbgdkregion.c -
  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "rbatk.h"

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
atktextrect_initialize(self, x, y, width, height)
    VALUE self, x, y, width, height;
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
atktextrect_x(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
atktextrect_y(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
atktextrect_w(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
atktextrect_h(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
atktextrect_set_x(self, x)
    VALUE self, x;
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
atktextrect_set_y(self, y)
    VALUE self, y;
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}

static VALUE
atktextrect_set_w(self, width)
    VALUE self, width;
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
atktextrect_set_h(self, height)
    VALUE self, height;
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}

static VALUE
atktextrect_to_a(self)
    VALUE self;
{
  AtkTextRectangle* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}
#endif

void
Init_atk_text_rectangle()
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
