/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#include "rbgst-private.h"

VALUE
rb_str_new_with_format (char *fmt, ...)
{
	char buf[1000];
	va_list ap;

	va_start (ap, fmt);
	vsnprintf (buf, sizeof buf, fmt, ap);
	va_end (ap);
	return CSTR2RVAL (buf);
}

VALUE
rb_ary_yield (VALUE arr)
{
	if (rb_block_given_p () == Qfalse)
		rb_raise (rb_eArgError, "No block given");
	rb_ary_each (arr); 
	return Qnil;
}

VALUE
rbgst_new_gstobject (void *o)
{
	GType gtype = G_OBJECT_TYPE (o);
	const gchar *gtypename = g_type_name (gtype);
        if (strncmp (gtypename, "Gst", 3) == 0)
            gtypename += 3;
        if (!rb_const_defined_at (mGst, rb_intern (gtypename)))
            G_DEF_CLASS (gtype, gtypename, mGst);
	return GOBJ2RVAL (o);
}

void
rbgst_initialize_gstobject(VALUE obj, GstObject *gstobj)
{
	gstobj = gst_object_ref (gstobj);
	gst_object_sink (gstobj);
	G_INITIALIZE (obj, gstobj);
}
