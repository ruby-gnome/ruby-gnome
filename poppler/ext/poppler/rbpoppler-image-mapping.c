/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006-2008 Ruby-GNOME2 Project Team
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

#include "rbpoppler-private.h"

#define RG_TARGET_NAMESPACE cImageMapping

DEF_ACCESSOR_WITH_SETTER(image_mapping, area,
                         RVAL2POPPLERIMAGEMAPPING, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(image_mapping, image_id, RVAL2POPPLERIMAGEMAPPING, INT2NUM, NUM2INT)
#ifdef RB_POPPLER_CAIRO_AVAILABLE
static VALUE
rg_image(VALUE self)
{
    return rb_funcall(rb_iv_get(self, "@page"), rb_intern("get_image"),
                        1, INT2NUM(RVAL2POPPLERIMAGEMAPPING(self)->image_id));
}
#endif

void
Init_poppler_imagemapping(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_IMAGE_MAPPING,
                                "ImageMapping", mPoppler);

    rbg_define_method(RG_TARGET_NAMESPACE, "area", image_mapping_get_area, 0);
    rbg_define_method(RG_TARGET_NAMESPACE, "image_id", image_mapping_get_image_id, 0);
#ifdef RB_POPPLER_CAIRO_AVAILABLE
    RG_DEF_METHOD(image, 0);
#endif

    rbg_define_method(RG_TARGET_NAMESPACE, "set_area", image_mapping_set_area, 1);
    rbg_define_method(RG_TARGET_NAMESPACE, "set_image_id",
                     image_mapping_set_image_id, 1);
}
