/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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
rb_gst_padtemplate_get_name (VALUE self)
{
	/* 
	 *  We can't call Gst::Object#name since the real name
	 *  of the pad template is in the "name_template" field of 
	 *  GstPadTemplate.
	 */
	GstPadTemplate *pad = RGST_PAD_TEMPLATE (self);
	return CSTR2RVAL (GST_PAD_TEMPLATE_NAME_TEMPLATE (pad));
}

/* Method: presence
 * Returns: the pad's presence (see Gst::Pad::Presence).
 */
static VALUE
rb_gst_padtemplate_get_presence (VALUE self)
{
	GstPadTemplate *pad = RGST_PAD_TEMPLATE (self);
	return GENUM2RVAL (pad->presence, GST_TYPE_PAD_PRESENCE);
}

/* Method: direction
 * Returns: the pad's direction (see Gst::Pad::Direction).
 */
static VALUE
rb_gst_padtemplate_get_direction (VALUE self)
{
	GstPadTemplate *pad = RGST_PAD_TEMPLATE (self);
	return GENUM2RVAL (pad->direction, GST_TYPE_PAD_DIRECTION);
}

/*
 * Method: caps
 *
 * Gets the capabilities of the pad template element.
 *
 * Returns: a Gst::Caps object.
 */
static VALUE
rb_gst_padtemplate_get_caps (VALUE self)
{
	return RGST_CAPS_NEW (gst_pad_template_get_caps (RGST_PAD_TEMPLATE (self)));
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
rb_gst_padtemplate_each_caps (VALUE self)
{
	return rb_ary_yield (rb_gst_padtemplate_get_caps (self));
}

/* Method: has_caps?
 * Returns: true if the pad template has capabilities, false otherwise.
 */
static VALUE
rb_gst_padtemplate_has_caps (VALUE self)
{
	GstPadTemplate *pad = RGST_PAD_TEMPLATE (self);
	return CBOOL2RVAL (pad->caps != NULL);
}

void 
Init_gst_padtemplate (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_PAD_TEMPLATE, "PadTemplate", mGst);

	rb_define_method (c, "name", rb_gst_padtemplate_get_name, 0);
	rb_define_method (c, "presence", rb_gst_padtemplate_get_presence, 0);
	rb_define_method (c, "direction", rb_gst_padtemplate_get_direction, 0);
	rb_define_method (c, "caps", rb_gst_padtemplate_get_caps, 0);
	rb_define_method (c, "each_caps", rb_gst_padtemplate_each_caps, 0);
	rb_define_method (c, "has_caps?", rb_gst_padtemplate_has_caps, 0);

	G_DEF_CLASS (GST_TYPE_PAD_TEMPLATE_FLAGS, "Flags", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_TEMPLATE_FLAGS, "GST_PAD_TEMPLATE_");
}
