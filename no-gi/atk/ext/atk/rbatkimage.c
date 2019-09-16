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

#define RG_TARGET_NAMESPACE mImage
#define _SELF(s) (RVAL2ATKIMAGE(s))

static VALUE
rg_image_position(VALUE self, VALUE coord_type)
{
    gint x, y;
    atk_image_get_image_position(_SELF(self), &x, &y, 
                                 RVAL2ATKCOORDTYPE(coord_type));
    return rb_assoc_new(INT2NUM(x), INT2NUM(y));
}

static VALUE
rg_image_description(VALUE self)
{
    return CSTR2RVAL(atk_image_get_image_description(_SELF(self)));
}

static VALUE
rg_set_image_description(VALUE self, VALUE description)
{
    gboolean ret = atk_image_set_image_description(_SELF(self),
                                                   RVAL2CSTR(description));
    if (! ret) rb_raise(rb_eRuntimeError, "Can't set image description");
    return self;
}

static VALUE
rg_image_size(VALUE self)
{
    gint width, height;
    atk_image_get_image_size(_SELF(self), &width, &height);
    return rb_assoc_new(INT2NUM(width), INT2NUM(height));
}

void
Init_atk_image(VALUE mAtk)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_INTERFACE(ATK_TYPE_IMAGE, "Image", mAtk);

    RG_DEF_METHOD(image_position, 1);
    RG_DEF_METHOD(image_description, 0);
    RG_DEF_METHOD(set_image_description, 1);
    RG_DEF_METHOD(image_size, 0);
}
