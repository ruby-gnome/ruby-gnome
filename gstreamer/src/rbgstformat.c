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

/* Class: Gst::Format
 * Dynamically register new formats. 
 */

static GstFormat *
format_copy (const GstFormat* format)
{
	GstFormat *new_format;
	g_return_val_if_fail (format != NULL, NULL);
	new_format = g_new (GstFormat, sizeof (GstFormat));
	*new_format = *format;
	return new_format;
}

GType
gst_format_get_type2 (void)
{
	static GType our_type = 0;
	if (our_type == 0)
		our_type = g_boxed_type_register_static ("GstFormatClass",
			(GBoxedCopyFunc)format_copy,
			(GBoxedFreeFunc)g_free);
	return our_type;
}

/*
 * Class method: find(nick)
 * nick: the nick of an existing format.
 *
 * Returns: a reference to the Gst::Format object registered with the
 * given nick, or nil if this query was not registered.
 */
static VALUE
rb_gst_format_find (VALUE self, VALUE nick)
{
	GstFormat format = gst_format_get_by_nick (RVAL2CSTR (nick));
	return format != GST_FORMAT_UNDEFINED
		? RGST_FORMAT_NEW (&format)
		: Qnil;
}

/*
 * Class method: each { |format| ... }
 *
 * Calls the block for each registered format, passing a reference
 * to the Gst::Format object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_format_each (VALUE self)
{
	const GList *list;

	for (list = gst_format_get_definitions ();
	     list != NULL;
	     list = g_list_next (list))	{
		GstFormatDefinition *def = (GstFormatDefinition *) list->data;
		rb_yield (RGST_FORMAT_NEW (&(def->value)));
	}
	return Qnil;
}

/* Method: type_id
 * Returns: the type id of this format (see Gst::Format::Type).
 */
static VALUE
rb_gst_format_get_type_id (VALUE self)
{
	GstFormat *format = RGST_FORMAT (self);
	return GENUM2RVAL (*format, GST_TYPE_FORMAT);
}

/* Method: nick
 * Returns: the short nick of the format.
 */
static VALUE
rb_gst_format_get_nick (VALUE self)
{
	GstFormat *format = RGST_FORMAT (self);
	return CSTR2RVAL (gst_format_get_details (*format)->nick);
}

/* Method: description
 * Returns: a longer description of the format.
 */
static VALUE
rb_gst_format_get_description (VALUE self)
{
	GstFormat *format = RGST_FORMAT (self);
	return CSTR2RVAL (gst_format_get_details (*format)->description);
}

/*
 * Method: ==(format)
 * format: a Gst::Format.
 *
 * Checks if two Gst::Format objects are registered under the
 * same nick.
 *
 * Returns: true on success, false on failure.
 */
static VALUE
rb_gst_format_is_equal (VALUE self, VALUE other_format)
{
	GstFormat *f1, *f2;
	gchar *n1, *n2;

	if (NIL_P (other_format))
		return Qfalse;

	f1 = RGST_FORMAT (self);
	f2 = RGST_FORMAT (other_format);

	n1 = gst_format_get_details (*f1)->nick;
	n2 = gst_format_get_details (*f2)->nick;

	return CBOOL2RVAL (strcmp (n1, n2) == 0);
}

void
Init_gst_format (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_FORMAT2, "Format", mGst);

	rb_define_singleton_method (c, "each", rb_gst_format_each, 0);
	rb_define_singleton_method (c, "find", rb_gst_format_find, 1);

	rb_define_method (c, "type_id", rb_gst_format_get_type_id, 0);
	rb_define_method (c, "nick",	rb_gst_format_get_nick,	0);
	rb_define_method (c, "description", rb_gst_format_get_description, 0);
	rb_define_method (c, "==", rb_gst_format_is_equal, 1);

	G_DEF_CLASS (GST_TYPE_FORMAT, "Type", c);
	G_DEF_CONSTANTS (c, GST_TYPE_FORMAT, "GST_FORMAT_");
}
