
/*
 * Copyright (C) 2003 Laurent Sansonetti <laurent@datarescue.be>
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

/*
 *  Class: Gst::QueryType
 *
 *  Dynamically register new query types. 
 */

static GstQueryType* query_type_copy(const GstQueryType* query_type) {
    GstQueryType* new_query_type;
    g_return_val_if_fail (query_type != NULL, NULL);
    new_query_type = g_new(GstQueryType, 1);
    *new_query_type = *query_type;
    return new_query_type;
}

GType gst_query_type_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstQueryType",
            (GBoxedCopyFunc)query_type_copy,
            (GBoxedFreeFunc)g_free);
    }
    return our_type;
}

/*
 *  Class method: find(aNickString) -> aQueryTypeObject
 *
 *  Returns a reference to the Gst::QueryType object registered with the 
 *  given nick, or nil if this query was not registered.
 */
static VALUE rb_gst_querytype_find(self, nick)
    VALUE self, nick;
{
    GstQueryType type = gst_query_type_get_by_nick(RVAL2CSTR(nick));
    return type != GST_QUERY_NONE 
        ? RGST_QUERY_TYPE_NEW(&type)
        : Qnil; 
}

/*
 *  Class method: each { |aQueryTypeObject| block } -> nil
 *
 *  Calls the block for each registered querytype, passing a reference
 *  to the Gst::QueryType object as parameter. 
 */
static VALUE rb_gst_querytype_each(self)
    VALUE self;
{
    const GList *list;

    for (list = gst_query_type_get_definitions();
         list != NULL;
         list = g_list_next(list))
    {
        GstQueryTypeDefinition *def = (GstQueryTypeDefinition *) list->data;
        rb_yield(RGST_QUERY_TYPE_NEW(&(def->value)));
    } 
    return Qnil;
}

/*
 *  Method: type_id -> aFixnum
 *
 *  Gets the type id of this querytype, as an integer value.
 *
 *  Meaningful types are:
 *    - Gst::QueryType::NONE;
 *    - Gst::QueryType::TOTAL;
 *    - Gst::QueryType::POSITION;
 *    - Gst::QueryType::LATENCY;
 *    - Gst::QueryType::JITTER;
 *    - Gst::QueryType::START;
 *    - Gst::QueryType::SEGMENT_END;
 *    - Gst::QueryType::RATE.
 */
static VALUE rb_gst_querytype_get_type_id(self)
    VALUE self;
{
    GstQueryType *querytype = RGST_QUERY_TYPE(self);
    return INT2FIX(*querytype);
}

/*
 *  Method: nick -> aString
 *
 *  Gets the short nick of the querytype. 
 */
static VALUE rb_gst_querytype_get_nick(self)
    VALUE self;
{
    GstQueryType *querytype = RGST_QUERY_TYPE(self);
    return CSTR2RVAL(gst_query_type_get_details(*querytype)->nick);
}

/*
 *  Method: description -> aString
 *
 *  Gets a longer description of the querytype. 
 */
static VALUE rb_gst_querytype_get_description(self)
    VALUE self;
{
    GstQueryType *querytype = RGST_QUERY_TYPE(self);
    return CSTR2RVAL(gst_query_type_get_details(*querytype)->description);
}

/*
 *  Method: == aQueryTypeObject -> aBoolean
 *
 *  Checks if two Gst::QueryType objects are registered under the
 *  same nick.
 */
static VALUE rb_gst_querytype_is_equal(self, other_query)
    VALUE self, other_query;
{
    GstQueryType *q1, *q2;
    gchar *n1, *n2;

    if (NIL_P(other_query)) {
        return Qfalse;
    }

    q1 = RGST_QUERY_TYPE(self);
    q2 = RGST_QUERY_TYPE(other_query);

    n1 = gst_query_type_get_details(*q1)->nick;
    n2 = gst_query_type_get_details(*q2)->nick;

    return CBOOL2RVAL(strcmp(n1, n2) == 0);
}

/*
 *  Constant: NONE
 *  Invalid query type.
 */
static VALUE constNone = INT2FIX(GST_QUERY_NONE);

/*
 *  Constant: TOTAL 
 *  Total length of stream.
 */
static VALUE constTotal = INT2FIX(GST_QUERY_TOTAL);

/*
 *  Constant: POSITION
 *  Current position in stream.
 */
static VALUE constPosition = INT2FIX(GST_QUERY_POSITION);

/*
 *  Constant: LATENCY
 *  Latency of stream.
 */
static VALUE constLatency = INT2FIX(GST_QUERY_LATENCY);

/*
 *  Constant: JITTER
 *  Current jitter of stream.
 */
static VALUE constJitter = INT2FIX(GST_QUERY_JITTER);

/*
 *  Constant: START
 *  Start of configured segment.
 */
static VALUE constStart = INT2FIX(GST_QUERY_START);

/*
 *  Constant: SEGMENT_END
 *  End of configured segment.
 */
static VALUE constSegmentEnd = INT2FIX(GST_QUERY_SEGMENT_END);

/*
 *  Constant: RATE
 *  Current rate of the stream.
 */
static VALUE constRate = INT2FIX(GST_QUERY_RATE);

void Init_gst_querytype(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_QUERY_TYPE, "QueryType", mGst);

    rb_define_singleton_method(c, "each", rb_gst_querytype_each, 0);
    rb_define_singleton_method(c, "find", rb_gst_querytype_find, 1);

    rb_define_method(c, "type_id", rb_gst_querytype_get_type_id, 0);
    rb_define_method(c, "nick",    rb_gst_querytype_get_nick,    0);
    rb_define_method(c, "description", 
                     rb_gst_querytype_get_description, 0);

    rb_define_method(c, "==", rb_gst_querytype_is_equal, 1);

    rb_define_const(c, "NONE",        constNone);
    rb_define_const(c, "TOTAL",       constTotal);
    rb_define_const(c, "POSITION",    constPosition);
    rb_define_const(c, "LATENCY",     constLatency);
    rb_define_const(c, "JITTER",      constJitter);
    rb_define_const(c, "START",       constStart);
    rb_define_const(c, "SEGMENT_END", constSegmentEnd);
    rb_define_const(c, "RATE",        constRate);
}

