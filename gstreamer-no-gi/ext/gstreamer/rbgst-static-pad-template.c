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

#include "rbgst-private.h"

/* Class: Gst::StaticPadTemplate
 */

#define RG_TARGET_NAMESPACE cStaticPadTemplate
#define SELF(self) RVAL2GST_STATIC_PAD_TEMPLATE(self)

static VALUE
rg_name(VALUE self)
{
    return CSTR2RVAL(SELF(self)->name_template);
}

static VALUE
rg_presence(VALUE self)
{
    return GENUM2RVAL(SELF(self)->presence, GST_TYPE_PAD_PRESENCE);
}

static VALUE
rg_direction(VALUE self)
{
    return GENUM2RVAL(SELF(self)->direction, GST_TYPE_PAD_DIRECTION);
}

static VALUE
rg_caps(VALUE self)
{
    return GST_STATIC_CAPS2RVAL(gst_static_pad_template_get_caps(SELF(self)));
}

void
Init_gst_static_pad_template (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_STATIC_PAD_TEMPLATE,
                                        "StaticPadTemplate", mGst);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(presence, 0);
    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD(caps, 0);
}
