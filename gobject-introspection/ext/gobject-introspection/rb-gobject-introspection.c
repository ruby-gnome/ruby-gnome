/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2012  Ruby-GNOME2 Project Team
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

#include "rb-gobject-introspection.h"

#define RG_TARGET_NAMESPACE rb_mGObjectIntrospection

void
Init_gobject_introspection(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = rb_define_module("GObjectIntrospection");

    rb_define_const(RG_TARGET_NAMESPACE, "BUILD_VERSION",
                    rb_ary_new3(3,
                                INT2FIX(GI_MAJOR_VERSION),
                                INT2FIX(GI_MINOR_VERSION),
                                INT2FIX(GI_MICRO_VERSION)));

    rb_gi_type_tag_init(RG_TARGET_NAMESPACE);
    rb_gi_base_info_init(RG_TARGET_NAMESPACE);
    rb_gi_repository_init(RG_TARGET_NAMESPACE);
}
