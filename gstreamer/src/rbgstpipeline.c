
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

/* Class: Gst::Pipeline
 * In almost all cases, you'll want to use a Gst::Pipeline when creating a 
 * filter graph. The Gst::Pipeline will manage all the scheduling issues, 
 * including threading, as well as provide simple interfaces to common 
 * functions, like 'Play'.
 */

/*
 * Class method: new(name=nil)
 * name: a name for the pipeline.
 *
 * Constructs a new Gst::Pipeline object.
 *
 * If element name is ommited (or nil), then the pipeline will receive a guaranteed
 * unique name, consisting of the "pipeline" string and a number.
 * If name is given, it will be given the name supplied.
 *
 * Returns: a newly constructed Gst::Pipeline.
 */
static VALUE
rb_gst_pipeline_new (int argc, VALUE *argv, VALUE self)
{
	GstElement *bin;
	VALUE name;

	rb_scan_args (argc, argv, "01", &name);

	bin = gst_pipeline_new (NIL_P (name) ? NULL : RVAL2CSTR(name));
	if (bin != NULL)
		RBGST_INITIALIZE (self, bin);
	return Qnil;
}

void
Init_gst_pipeline(void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_PIPELINE, "Pipeline", mGst);
	rb_define_method (c, "initialize", rb_gst_pipeline_new, -1);
}
