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

/* Class: Gst::QueryType
 * Dynamically register new query types. 
 */

static GstQueryType *
query_type_copy (const GstQueryType* query_type)
{
	GstQueryType *new_query_type;
	g_return_val_if_fail (query_type != NULL, NULL);
	new_query_type = g_new (GstQueryType, 1);
	*new_query_type = *query_type;
	return new_query_type;
}

GType
gst_query_type_get_type2 (void)
{
	static GType our_type = 0;
	if (our_type == 0)
		our_type = g_boxed_type_register_static ("GstQueryTypeClass",
			(GBoxedCopyFunc)query_type_copy,
			(GBoxedFreeFunc)g_free);
	return our_type;
}

/*
 * Class method: find(nick)
 * nick: the nick of an existing query type.
 *
 * Returns: a reference to the Gst::QueryType object registered with the 
 * given nick, or nil if this query was not registered.
 */
static VALUE
rb_gst_querytype_find (VALUE self, VALUE nick)
{
	GstQueryType type = gst_query_type_get_by_nick (RVAL2CSTR (nick));
	return type != GST_QUERY_NONE 
		? RGST_QUERY_TYPE_NEW (&type)
		: Qnil; 
}

/*
 * Class method: each { |query_type| block }
 *
 * Calls the block for each registered query type, passing a reference
 * to the Gst::QueryType object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_querytype_each (VALUE self)
{
	const GList *list;

	for (list = gst_query_type_get_definitions ();
	     list != NULL;
	     list = g_list_next (list))
	{
		GstQueryTypeDefinition *def = (GstQueryTypeDefinition *) list->data;
		rb_yield (RGST_QUERY_TYPE_NEW (&(def->value)));
	} 
	return Qnil;
}

/* Method: type_id
 * Returns: the type id of this query type (see Gst::QueryType::Type).
 */
static VALUE
rb_gst_querytype_get_type_id (VALUE self)
{
	GstQueryType *querytype = RGST_QUERY_TYPE (self);
	return INT2FIX (*querytype);
}

/* Method: nick
 * Returns: the short nick of the query type.
 */
static VALUE
rb_gst_querytype_get_nick (VALUE self)
{
	GstQueryType *querytype = RGST_QUERY_TYPE (self);
	return CSTR2RVAL (gst_query_type_get_details (*querytype)->nick);
}

/* Method: description
 * Returns: a longer description of the query type.
 */
static VALUE
rb_gst_querytype_get_description (VALUE self)
{
	GstQueryType *querytype = RGST_QUERY_TYPE (self);
	return CSTR2RVAL (gst_query_type_get_details (*querytype)->description);
}

/*
 * Method: ==(query_type)
 * query_type: a Gst::QueryType object.
 *
 * Checks if two Gst::QueryType objects are registered under the
 * same nick.
 *
 * Returns: true on success, false on failure.
 */
static VALUE
rb_gst_querytype_is_equal (VALUE self, VALUE other_query)
{
	GstQueryType *q1, *q2;
	gchar *n1, *n2;

	if (NIL_P (other_query))
		return Qfalse;

	q1 = RGST_QUERY_TYPE (self);
	q2 = RGST_QUERY_TYPE (other_query);

	n1 = gst_query_type_get_details (*q1)->nick;
	n2 = gst_query_type_get_details (*q2)->nick;

	return CBOOL2RVAL( strcmp (n1, n2) == 0);
}

void
Init_gst_querytype (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_QUERY_TYPE2, "QueryType", mGst);

	rb_define_singleton_method (c, "each", rb_gst_querytype_each, 0);
	rb_define_singleton_method (c, "find", rb_gst_querytype_find, 1);

	rb_define_method (c, "type_id", rb_gst_querytype_get_type_id, 0);
	rb_define_method (c, "nick", rb_gst_querytype_get_nick,	0);
	rb_define_method (c, "description", rb_gst_querytype_get_description, 0);
	rb_define_method (c, "==", rb_gst_querytype_is_equal, 1);

	G_DEF_CLASS (GST_TYPE_QUERY_TYPE, "Type", c);
	G_DEF_CONSTANTS (c, GST_TYPE_QUERY_TYPE, "GST_QUERY_");
}
