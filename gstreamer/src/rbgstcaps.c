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

/* Class: Gst::Caps
 * Capabilities of pads. 
 */

/* Method: fixed?
 * Returns: true if the Gst::Caps has fixed properties (ie. 
 * it has no ranges or lists), false otherwise.
 */
static VALUE
rb_gst_caps_is_fixed (VALUE self)
{
	return CBOOL2RVAL (gst_caps_is_fixed (RGST_CAPS (self)));
}

void
Init_gst_caps (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_CAPS, "Caps", mGst);

	rb_define_method (c, "fixed?", rb_gst_caps_is_fixed, 0);
}
