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

/* Class: Gst::EventSize
 * A size event.
 */

/*
 * Class method: new(format, value)
 * format: a format (see Gst::Format::Type).
 * value: the event value.
 *
 * Creates a new size event with the given values.
 *
 * Returns: a newly created Gst::EventSize object.
 */
static VALUE
rb_gst_eventsize_new (VALUE self, VALUE format, VALUE value)
{
	GstEvent *event = gst_event_new_size (FIX2INT (format),
					      NUM2ULL (value));
	if (event != NULL)
		G_INITIALIZE (self, event);
	return Qnil;
}

void
Init_gst_eventsize (void)
{
	VALUE c = rb_define_class_under (mGst, "EventSize", GTYPE2CLASS (GST_TYPE_EVENT));
	rb_define_method (c, "initialize", rb_gst_eventsize_new, 2);
}
