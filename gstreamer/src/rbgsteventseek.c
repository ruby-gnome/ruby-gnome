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

/*  Class: Gst::EventSeek
 *  A seek event.
 */
VALUE cGstEventSeek;

/*
 * Class method: new(type, offset)
 * type: an event seek type (see Gst::EventSeek::Type).
 * offset: an offset.
 *
 * Allocates a new seek event with the given parameters.
 * Types (METHOD_) cam be OR'ed (|) with flags (FLAG_).
 *
 * Returns: a newly allocated Gst::EventSeek object.
 */
static VALUE
rb_gst_eventseek_new (VALUE self, VALUE type, VALUE offset)
{
	GstEvent *event = gst_event_new_seek (FIX2INT (type),
					      NUM2ULL (offset));
	if (event != NULL)
		G_INITIALIZE (self, event);
	return Qnil;
}

void
Init_gst_eventseek (void)
{
	VALUE c = rb_define_class_under (mGst, "EventSeek", GTYPE2CLASS(GST_TYPE_EVENT));
	
	rb_define_method (c, "initialize", rb_gst_eventseek_new, 2);

	G_DEF_CLASS (GST_TYPE_SEEK_TYPE, "Type", c);
	G_DEF_CONSTANTS (c, GST_TYPE_SEEK_TYPE, "GST_SEEK_");

	cGstEventSeek = c;
}
