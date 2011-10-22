/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2004  Masao Mutoh
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

#define _SELF(s) (ATK_IMAGE(RVAL2GOBJ(s)))

static VALUE
rbatkimage_get_image_position(VALUE self, VALUE coord_type)
{
    gint x, y;
    atk_image_get_image_position(_SELF(self), &x, &y, 
                                 RVAL2GENUM(coord_type, ATK_TYPE_COORD_TYPE));
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rbatkimage_get_image_description(VALUE self)
{
    return CSTR2RVAL(atk_image_get_image_description(_SELF(self)));
}

static VALUE
rbatkimage_set_image_description(VALUE self, VALUE description)
{
    gboolean ret = atk_image_set_image_description(_SELF(self),
                                                   RVAL2CSTR(description));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set image description");
    return self;
}

static VALUE
rbatkimage_get_image_size(VALUE self)
{
    gint width, height;
    atk_image_get_image_size(_SELF(self), &width, &height);
    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

void
Init_atk_image(void)
{
    VALUE image = G_DEF_INTERFACE(ATK_TYPE_IMAGE, "Image", mAtk);

    rb_define_method(image, "image_position", rbatkimage_get_image_position, 1);
    rb_define_method(image, "image_description", rbatkimage_get_image_description, 0);
    rb_define_method(image, "set_image_description", rbatkimage_set_image_description, 1);
    rb_define_method(image, "image_size", rbatkimage_get_image_size, 0);

    G_DEF_SETTERS(image);
}
