/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

#define SELF(self) RVAL2GST_PAD_TEMPLATE(self)

/* Class: Gst::PadTemplate
 * Describe the media type of a Gst::Pad.
 */

/*
 * Method: name
 *
 * Gets the name of the pad template.
 * This overrides Gst::Object#name.
 *
 * Returns: the name of the pad template.
 */
static VALUE
get_name(VALUE self)
{
    /*
     *  We can't call Gst::Object#name since the real name
     *  of the pad template is in the "name_template" field of
     *  GstPadTemplate.
     */
    return CSTR2RVAL(GST_PAD_TEMPLATE_NAME_TEMPLATE(SELF(self)));
}

/* Method: presence
 * Returns: the pad's presence (see Gst::Pad::Presence).
 */
static VALUE
get_presence(VALUE self)
{
    return GENUM2RVAL(GST_PAD_TEMPLATE_PRESENCE(SELF(self)),
                      GST_TYPE_PAD_PRESENCE);
}

/* Method: direction
 * Returns: the pad's direction (see Gst::Pad::Direction).
 */
static VALUE
get_direction(VALUE self)
{
    return GENUM2RVAL(GST_PAD_TEMPLATE_DIRECTION(SELF(self)),
                      GST_TYPE_PAD_DIRECTION);
}

/*
 * Method: caps
 *
 * Gets the capabilities of the pad template element.
 *
 * Returns: a Gst::Caps object.
 */
static VALUE
get_caps(VALUE self)
{
    return GST_CAPS2RVAL(gst_pad_template_get_caps(RGST_PAD_TEMPLATE(self)));
}

/*
 * Method: each_caps { |caps| ... }
 *
 * Calls the block for each capability of the pad template,
 * passing a reference to the Gst::Caps object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
each_caps(VALUE self)
{
    return rb_ary_yield(get_caps(self));
}

/* Method: has_caps?
 * Returns: true if the pad template has capabilities, false otherwise.
 */
static VALUE
has_caps_p(VALUE self)
{
    return CBOOL2RVAL(SELF(self)->caps != NULL);
}

void
Init_gst_pad_template (void)
{
    VALUE cGstPadTemplate;

    cGstPadTemplate = G_DEF_CLASS(GST_TYPE_PAD_TEMPLATE, "PadTemplate", mGst);

    rb_define_method(cGstPadTemplate, "name", get_name, 0);
    rb_define_method(cGstPadTemplate, "presence", get_presence, 0);
    rb_define_method(cGstPadTemplate, "direction", get_direction, 0);
    rb_define_method(cGstPadTemplate, "caps", get_caps, 0);
    rb_define_method(cGstPadTemplate, "each_caps", each_caps, 0);
    rb_define_method(cGstPadTemplate, "has_caps?", has_caps_p, 0);
    rb_define_alias(cGstPadTemplate, "have_caps?", "has_caps?");

    G_DEF_CLASS(GST_TYPE_PAD_TEMPLATE_FLAGS, "Flags", cGstPadTemplate);
    G_DEF_CONSTANTS(cGstPadTemplate, GST_TYPE_PAD_TEMPLATE_FLAGS,
                    "GST_PAD_TEMPLATE_");
}
