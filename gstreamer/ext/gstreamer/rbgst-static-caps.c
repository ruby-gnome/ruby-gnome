/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
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

#include "rbgst.h"

#define RG_TARGET_NAMESPACE cStaticCaps
#define SELF(obj) (RVAL2GST_STATIC_CAPS(obj))

static VALUE
get_description(VALUE self)
{
    return CSTR2RVAL(SELF(self)->string);
}

static VALUE
get(VALUE self)
{
    return GST_CAPS2RVAL(gst_static_caps_get(SELF(self)));
}

void
Init_gst_static_caps(void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_STATIC_CAPS, "StaticCaps", mGst);

    rb_define_method(RG_TARGET_NAMESPACE, "description", get_description, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "get", get, 0);
    rb_define_alias(RG_TARGET_NAMESPACE, "to_caps", "get");

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
