/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbatkhypertext.c -

  $Author: mutoh $
  $Date: 2004/02/19 17:16:54 $

  Copyright (C) 2003 Masao Mutoh
************************************************/
#include "rbatk.h"

#define _SELF(s) (ATK_IMAGE(RVAL2GOBJ(s)))

static VALUE
rbatkimage_get_image_position(self, coord_type)
    VALUE self, coord_type;
{
    gint x, y;
    atk_image_get_image_position(_SELF(self), &x, &y, 
                                 RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rbatkimage_get_image_description(self)
    VALUE self;
{
    return CSTR2RVAL(atk_image_get_image_description(_SELF(self)));
}

static VALUE
rbatkimage_set_image_description(self, description)
    VALUE self, description;
{
    gboolean ret = atk_image_set_image_description(_SELF(self),
                                                   RVAL2CSTR(description));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set image description");
    return self;
}

static VALUE
rbatkimage_get_image_size(self)
    VALUE self;
{
    gint width, height;
    atk_image_get_image_size(_SELF(self), &width, &height);
    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

void
Init_atk_image()
{
    VALUE image = G_DEF_INTERFACE(ATK_TYPE_IMAGE, "Image", mAtk);

    rb_define_method(image, "image_position", rbatkimage_get_image_position, 1);
    rb_define_method(image, "image_description", rbatkimage_get_image_description, 0);
    rb_define_method(image, "set_image_description", rbatkimage_set_image_description, 1);
    rb_define_method(image, "image_size", rbatkimage_get_image_size, 0);

    G_DEF_SETTERS(image);
}
