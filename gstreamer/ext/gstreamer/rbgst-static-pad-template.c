/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
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

/* Class: Gst::StaticPadTemplate
 */

#define RG_TARGET_NAMESPACE cStaticPadTemplate
#define SELF(self) RVAL2GST_STATIC_PAD_TEMPLATE(self)

static VALUE
get_name(VALUE self)
{
    return CSTR2RVAL(SELF(self)->name_template);
}

static VALUE
get_presence(VALUE self)
{
    return GENUM2RVAL(SELF(self)->presence, GST_TYPE_PAD_PRESENCE);
}

static VALUE
get_direction(VALUE self)
{
    return GENUM2RVAL(SELF(self)->direction, GST_TYPE_PAD_DIRECTION);
}

static VALUE
get_caps(VALUE self)
{
    return GST_STATIC_CAPS2RVAL(gst_static_pad_template_get_caps(SELF(self)));
}

void
Init_gst_static_pad_template (void)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_STATIC_PAD_TEMPLATE,
                                        "StaticPadTemplate", mGst);
    rb_define_method(RG_TARGET_NAMESPACE, "name", get_name, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "presence", get_presence, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "direction", get_direction, 0);
    rb_define_method(RG_TARGET_NAMESPACE, "caps", get_caps, 0);
}
