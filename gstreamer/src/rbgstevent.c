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

/* Class: Gst::Event
 * Event definitions.
 */

/*
 * Class method: new(type)
 * type: an event type (see Gst::Event::Type).
 *
 * Allocates a new event of the given type.
 *
 * Returns: a newly allocated Gst::Event object.
 */
static VALUE
rb_gst_event_new (VALUE self, VALUE type)
{
	GstEvent *event = gst_event_new (RVAL2GENUM (type, GST_TYPE_EVENT_TYPE));
	if (event != NULL)
		G_INITIALIZE (self, event);
	return Qnil;
}

/* Method: src
 * Returns: the source Gst::Object that generated this event.
 */
static VALUE
rb_gst_event_src (VALUE self)
{
	GstObject *object = GST_EVENT_SRC (RGST_EVENT (self));
	return object != NULL
		? RGST_OBJECT_NEW (object)
		: Qnil;
}

/* Method: timestamp
 * Returns: the timestamp of this event.
 */
static VALUE
rb_gst_event_timestamp (VALUE self)
{
	return ULL2NUM (GST_EVENT_TIMESTAMP (RGST_EVENT (self)));
}

void
Init_gst_event (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_EVENT, "Event", mGst);
	
	rb_define_method (c, "initialize", rb_gst_event_new, 1);
	rb_define_method (c, "src", rb_gst_event_src, 0);
	rb_define_method (c, "timestamp", rb_gst_event_timestamp, 0);

	G_DEF_CLASS (GST_TYPE_EVENT_TYPE, "Type", c);
	G_DEF_CONSTANTS (c, GST_TYPE_EVENT_TYPE, "GST_EVENT_");
	G_DEF_CLASS (GST_TYPE_EVENT_FLAG, "Flag", c);
	G_DEF_CONSTANTS (c, GST_TYPE_EVENT_FLAG, "GST_");
}
