/*
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
 *
 * This file is part of Ruby/GStreamer.
 *
 * Ruby/GStreamer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Ruby/GStreamer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Ruby/GStreamer; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "rbgst.h"

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
    VALUE rb_cGstStaticCaps;

    rb_cGstStaticCaps = G_DEF_CLASS(GST_TYPE_STATIC_CAPS, "StaticCaps", mGst);

    rb_define_method(rb_cGstStaticCaps, "description", get_description, 0);
    rb_define_method(rb_cGstStaticCaps, "get", get, 0);
    rb_define_alias(rb_cGstStaticCaps, "to_caps", "get");

    G_DEF_SETTERS(rb_cGstStaticCaps);
}
