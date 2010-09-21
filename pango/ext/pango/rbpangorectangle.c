/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbpangolayoutline.c -

  $Author: mutoh $
  $Date: 2005/02/13 17:31:33 $

  Copyright (C) 2002,2003 Masao Mutoh
************************************************/

#include "rbpango.h"

#define _SELF(s) ((PangoRectangle*)RVAL2BOXED(s, PANGO_TYPE_RECTANGLE))

/**********************************/
static PangoRectangle*
pango_rectangle_copy(ref)
    const PangoRectangle* ref;
{
  PangoRectangle* new_ref;
  g_return_val_if_fail (ref != NULL, NULL);
  new_ref = g_new(PangoRectangle, 1);
  *new_ref = *ref;
  return new_ref;
}

GType
pango_rectangle_get_type(void)
{
    static GType our_type = 0;

    if (our_type == 0)
        our_type = g_boxed_type_register_static ("PangoRectangle",
                    (GBoxedCopyFunc)pango_rectangle_copy,
                    (GBoxedFreeFunc)g_free);
    return our_type;
}
/**********************************/

static VALUE
pango_rectangle_initialize(self, x, y, width, height)
    VALUE self, x, y, width, height;
{
    PangoRectangle new;

    new.x = NUM2INT(x);
    new.y = NUM2INT(y);
    new.width = NUM2INT(width);
    new.height = NUM2INT(height);

    G_INITIALIZE(self, &new);
    return Qnil;
}


static VALUE
pango_rectangle_x(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->x);
}

static VALUE
pango_rectangle_y(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->y);
}

static VALUE
pango_rectangle_w(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->width);
}

static VALUE
pango_rectangle_h(self)
    VALUE self;
{
    return INT2NUM(_SELF(self)->height);
}

static VALUE
pango_rectangle_set_x(self, x)
    VALUE self, x;
{
    _SELF(self)->x = NUM2INT(x);
    return self;
}

static VALUE
pango_rectangle_set_y(self, y)
    VALUE self, y;
{
    _SELF(self)->y = NUM2INT(y);
    return self;
}
static VALUE
pango_rectangle_set_w(self, width)
    VALUE self, width;
{
    _SELF(self)->width = NUM2INT(width);
    return self;
}

static VALUE
pango_rectangle_set_h(self, height)
    VALUE self, height;
{
    _SELF(self)->height = NUM2INT(height);
    return self;
}
static VALUE
pango_rectangle_to_a(self)
    VALUE self;
{
  PangoRectangle* a = _SELF(self);
  return rb_ary_new3(4, INT2FIX(a->x), INT2FIX(a->y),
                     INT2FIX(a->width), INT2FIX(a->height));
}

static VALUE
pango_rectangle_ascent(self)
    VALUE self;
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_ASCENT(*r));
}

static VALUE
pango_rectangle_descent(self)
    VALUE self;
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_DESCENT(*r));
}

static VALUE
pango_rectangle_lbearing(self)
    VALUE self;
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_LBEARING(*r));
}

static VALUE
pango_rectangle_rbearing(self)
    VALUE self;
{
    PangoRectangle* r =_SELF(self);
    return INT2NUM(PANGO_RBEARING(*r));
}


void
Init_pango_rectangle()
{
    VALUE pRectangle = G_DEF_CLASS(PANGO_TYPE_RECTANGLE, "Rectangle", mPango);
    rb_define_method(pRectangle, "initialize", pango_rectangle_initialize, 4);
    rb_define_method(pRectangle, "x", pango_rectangle_x, 0);
    rb_define_method(pRectangle, "y", pango_rectangle_y, 0);
    rb_define_method(pRectangle, "width", pango_rectangle_w, 0);
    rb_define_method(pRectangle, "height", pango_rectangle_h, 0);
    rb_define_method(pRectangle, "set_x", pango_rectangle_set_x, 1);
    rb_define_method(pRectangle, "set_y", pango_rectangle_set_y, 1);
    rb_define_method(pRectangle, "set_width", pango_rectangle_set_w, 1);
    rb_define_method(pRectangle, "set_height", pango_rectangle_set_h, 1);
    rb_define_method(pRectangle, "to_a", pango_rectangle_to_a, 0);

    rb_define_method(pRectangle, "ascent", pango_rectangle_ascent, 0);
    rb_define_method(pRectangle, "descent", pango_rectangle_descent, 0);
    rb_define_method(pRectangle, "lbearing", pango_rectangle_lbearing, 0);
    rb_define_method(pRectangle, "rbearing", pango_rectangle_rbearing, 0);

    G_DEF_SETTERS(pRectangle);
}
