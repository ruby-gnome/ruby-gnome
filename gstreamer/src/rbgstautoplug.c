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

/* Class: Gst::Autoplug
 * Automatically create and link elements.
 */

/*
 * Method: signal_new_object(obj)
 * obj: a Gst::Object.
 *
 * Emits a 'new_object' signal.  Autopluggers are supposed to emit
 * this signal whenever a new object has been added to the
 * autoplugged pipeline.
 *
 * Returns: self.
 */
static VALUE
rb_gst_autoplug_signal_new_object (VALUE self, VALUE object)
{
	gst_autoplug_signal_new_object (RGST_AUTOPLUG (self),
					RGST_OBJECT (object));
	return self;
}

/*
 * XXX to_caps and to_renderers should handle variable arguments as targets.
 */

/*
 * Method: to_caps(src, sink)
 * src: a Gst::Caps.
 * sink: a Gst::Caps.
 *
 * Performs the autoplugging procedure on the autoplugger, 
 * connecting the given source and sink Gst::Caps.
 *
 * Returns: a new Gst::Element object that connects these caps or nil.
 */
static VALUE 
rb_gst_autoplug_to_caps (VALUE self, VALUE srccaps, VALUE sinkcaps)
{
	GstElement *element = gst_autoplug_to_caps (RGST_AUTOPLUG (self),
						    RGST_CAPS (srccaps),
						    RGST_CAPS (sinkcaps),
						    NULL);
	return element != NULL
		? RGST_ELEMENT_NEW (element)
		: Qnil;
}

/*
 * Method: to_renderers(src_caps, sink_element)
 * src_caps: a Gst::Caps.
 * sink_element: a Gst::Element.
 *
 * Performs the autoplugging procedure on the autoplugger, 
 * connecting the given source Gst::Caps to the target Gst::Element.
 *
 * Returns: a new Gst::Element object that connects the caps to the target 
 * element or nil.
 */
static VALUE
rb_gst_autoplug_to_renderers (VALUE self, VALUE srccaps, VALUE sinkelement)
{
	GstElement *element = gst_autoplug_to_renderers (RGST_AUTOPLUG (self),
							 RGST_CAPS (srccaps),
							 RGST_ELEMENT (sinkelement), 
							 NULL);
	return element != NULL
		? RGST_ELEMENT_NEW (element)
		: Qnil;
}

void
Init_gst_autoplug (void) 
{
	VALUE c = G_DEF_CLASS (GST_TYPE_AUTOPLUG, "Autoplug", mGst);
	rb_define_method (c, "signal_new_object", rb_gst_autoplug_signal_new_object, 1);
	rb_define_method (c, "to_caps", rb_gst_autoplug_to_caps, 2);
	rb_define_method (c, "to_renderers", rb_gst_autoplug_to_renderers, 2);
}
