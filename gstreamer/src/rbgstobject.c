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

/* Class: Gst::Object
 * Basis for the GST object hierarchy.
 */

/*
 * Method: destroyed?
 *
 * Checks if the Gst::Object::FLAG_DESTROYED flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_object_is_destroyed (VALUE self)
{
	return CBOOL2RVAL (GST_OBJECT_DESTROYED (RGST_OBJECT (self)));
}

/*
 * Method: floating?
 *
 * Checks if the Gst::Object::FLAG_FLOATING flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_object_is_floating (VALUE self)
{
	return CBOOL2RVAL (GST_OBJECT_FLOATING (RGST_OBJECT (self)));
}

void
Init_gst_object (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_OBJECT, "Object", mGst);   
 
	rb_define_method(c, "destroyed?", rb_gst_object_is_destroyed, 0);
	rb_define_method(c, "floating?",  rb_gst_object_is_floating,  0);

	G_DEF_CLASS (GST_TYPE_OBJECT_FLAGS, "Flags", c);
	G_DEF_CONSTANTS (c, GST_TYPE_OBJECT_FLAGS, "GST_");
}
