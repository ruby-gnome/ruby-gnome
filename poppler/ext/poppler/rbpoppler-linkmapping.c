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

#define RG_TARGET_NAMESPACE cLinkMapping

void
rectangle_set(PopplerRectangle *rect, VALUE rb_rect)
{
    *rect = *(RVAL2POPPLERRECTANGLE(rb_rect));
}

DEF_ACCESSOR_WITH_SETTER(link_mapping, area,
                         RVAL2POPPLERLINKMAPPING, RECT_ENTITY2RVAL, RECT_ENTITY_SET)
DEF_ACCESSOR(link_mapping, action, RVAL2POPPLERLINKMAPPING,
             POPPLERACTION2RVAL, RVAL2POPPLERACTION)

void
Init_poppler_linkmapping(VALUE mPoppler)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS(POPPLER_TYPE_LINK_MAPPING, "LinkMapping",
                               mPoppler);

    rb_define_method(RG_TARGET_NAMESPACE, "area", link_mapping_get_area, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "action", link_mapping_get_action, 0);

    rb_define_method(RG_TARGET_NAMESPACE, "set_area", link_mapping_set_area, 1);
    rb_define_method(RG_TARGET_NAMESPACE, "set_action", link_mapping_set_action, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
