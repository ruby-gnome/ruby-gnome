/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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

/* Module: Gst::Parse
 * Parses command-line syntax into a pipeline. 
 */

/*
 * Class method: launch(command)
 * command: a command line.
 *
 * Creates a new pipeline based on command line syntax.
 * On failure, an exception is thrown with a specific
 * error message.   
 *
 * Returns: a reference to a new Gst::Bin on success,
 * an exception on failure.
 */
static VALUE
rb_gst_parse_launch (VALUE self, VALUE command)
{
#if !defined(GST_DISABLE_PARSE)
	GError *error;
	GstBin *bin;

	error = NULL;
	bin = gst_parse_launch (RVAL2CSTR (command), &error);
	if (bin != NULL)
		return RGST_BIN_NEW (bin);
	rb_raise (rb_eRuntimeError, error->message);	
#else
	rb_raise (rb_eRuntimeError, 
		  "This function has been disabled "
		  "when GStreamer was compiled");
#endif
	return Qnil;
}

void
Init_gst_parse (void)
{
	VALUE c = rb_define_module_under (mGst, "Parse"); 
	rb_define_module_function (c, "launch", rb_gst_parse_launch, 1);
}
