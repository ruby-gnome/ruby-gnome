/*
 * Copyright (C) 2006 Sjoerd Simons <sjoerd@luon.net>
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 * Copyright (C) 2006 Zaheer Abbas Merali <zaheerabbas at merali dot org>
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
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
#include "rbgst-private.h"

/* Document-class: Gst::Query
 * Queries.
 */

static RGConvertTable table = {0};

static VALUE rb_cGstQueryPosition;
static VALUE rb_cGstQueryDuration;
static VALUE rb_cGstQueryLatency;
static VALUE rb_cGstQuerySeeking;
static VALUE rb_cGstQuerySegment;
static VALUE rb_cGstQueryConvert;
static VALUE rb_cGstQueryFormats;
static VALUE rb_cGstQuery;

static VALUE
get_superclass(void)
{
    return rb_cGstMiniObject;
}

static VALUE
instance2robj(gpointer instance)
{
    VALUE klass;
    GstQuery *query;

    query = instance;

    switch (GST_QUERY_TYPE(query)) {
       case GST_QUERY_POSITION:
         klass = rb_cGstQueryPosition;
         break;
       case GST_QUERY_DURATION:
         klass = rb_cGstQueryDuration;
         break;
       case GST_QUERY_LATENCY:
         klass = rb_cGstQueryLatency;
         break;
       case GST_QUERY_SEEKING:
         klass = rb_cGstQuerySeeking;
         break;
       case GST_QUERY_SEGMENT:
         klass = rb_cGstQuerySegment;
         break;
       case GST_QUERY_CONVERT:
         klass = rb_cGstQueryConvert;
         break;
       case GST_QUERY_FORMATS:
         klass = rb_cGstQueryFormats;
         break;
       default:
         klass = rb_cGstQuery;
         break;
    }

    gst_mini_object_ref(instance);
    return Data_Wrap_Struct(klass, NULL, _rbgst_mini_object_free, instance);
}

static VALUE
position_initialize(VALUE self, VALUE format)
{
    GstQuery *query;

    query = gst_query_new_position(RVAL2GST_FORMAT(format));

    G_INITIALIZE (self, query);
    return Qnil;
}

static VALUE
position_parse(VALUE self)
{
    GstFormat format;
    gint64 cur;

    gst_query_parse_position(RVAL2GST_QUERY(self), &format, &cur);

    return rb_ary_new3 (2, GST_FORMAT2RVAL(format), LL2NUM(cur));
}

static VALUE
duration_initialize(VALUE self, VALUE format)
{
    GstQuery *query;

    query = gst_query_new_duration(RVAL2GST_FORMAT(format));

    G_INITIALIZE (self, query);
    return Qnil;
}

static VALUE
duration_parse(VALUE self)
{
    GstFormat format;
    gint64 cur;

    gst_query_parse_duration(RVAL2GST_QUERY(self), &format, &cur);

    return rb_ary_new3 (2, GST_FORMAT2RVAL(format), LL2NUM(cur));
}

static VALUE
latency_initialize(VALUE self)
{
    GstQuery *query;

    query = gst_query_new_latency();

    G_INITIALIZE (self, query);
    return Qnil;
}

static VALUE
latency_parse(VALUE self)
{
    GstClockTime min_latency;
    GstClockTime max_latency;
    gboolean live;

    gst_query_parse_latency(RVAL2GST_QUERY(self), &live, &min_latency,
        &max_latency);

    return rb_ary_new3 (3, CBOOL2RVAL(live), ULL2NUM(min_latency),
       ULL2NUM(max_latency));
}


static VALUE
seeking_initialize(VALUE self, VALUE format)
{
    GstQuery *query;

    query = gst_query_new_seeking(RVAL2GST_FORMAT(format));

    G_INITIALIZE (self, query);
    return Qnil;
}

static VALUE
seeking_parse(VALUE self)
{
    GstFormat format;
    gboolean seekable;
    gint64 segment_start, segment_end;

    gst_query_parse_seeking(RVAL2GST_QUERY(self), &format, &seekable,
       &segment_start, &segment_end);

    return rb_ary_new3 (4, GST_FORMAT2RVAL(format), CBOOL2RVAL(seekable),
        LL2NUM(segment_start), LL2NUM(segment_end));
}


static VALUE
segment_initialize(VALUE self, VALUE format)
{
    GstQuery *query;

    query = gst_query_new_segment(RVAL2GST_FORMAT(format));

    G_INITIALIZE (self, query);
    return Qnil;
}

static VALUE
segment_parse(VALUE self)
{
    GstFormat format;
    gdouble rate;
    gint64 start_value, stop_value;

    gst_query_parse_segment(RVAL2GST_QUERY(self), &rate, &format,
       &start_value, &stop_value);

    return rb_ary_new3 (4, rb_dbl2big(rate), GST_FORMAT2RVAL(format),
        LL2NUM(start_value), LL2NUM(stop_value));
}

static VALUE
convert_initialize(VALUE self, VALUE src_format, VALUE value,
    VALUE dest_format)
{
    GstQuery *query;

    query = gst_query_new_convert(GST_FORMAT2RVAL(src_format),
                                  NUM2LL(value), GST_FORMAT2RVAL(dest_format));

    G_INITIALIZE (self, query);
    return Qnil;
}

static VALUE
convert_parse(VALUE self)
{
    GstFormat src_format, dest_format;
    gint64 src_value, dest_value;

    gst_query_parse_convert(RVAL2GST_QUERY(self), &src_format, &src_value,
       &dest_format, &dest_value);

    return rb_ary_new3 (4, GST_FORMAT2RVAL(src_format), LL2NUM(src_value),
        RVAL2GST_FORMAT(dest_format), LL2NUM(dest_value));
}


static VALUE
formats_initialize(VALUE self, VALUE format)
{
    GstQuery *query;

    query = gst_query_new_formats();

    G_INITIALIZE (self, query);
    return Qnil;
}

static VALUE
formats_parse(VALUE self)
{
    VALUE ret;
    guint n_formats;
    guint i;

    gst_query_parse_formats_length(RVAL2GST_QUERY(self), &n_formats);
    ret = rb_ary_new2(n_formats);
    for (i = 0 ; i< n_formats; i++) {
        GstFormat format;
        gst_query_parse_formats_nth(RVAL2GST_QUERY(self), i, &format);
        rb_ary_push (ret, GST_FORMAT2RVAL(format));
    }

    return ret;
}

void
Init_gst_query (void)
{
    table.type = GST_TYPE_QUERY;
    table.get_superclass = get_superclass;
    table.instance2robj = instance2robj;
    RG_DEF_CONVERSION(&table);

    rb_cGstQuery = G_DEF_CLASS (GST_TYPE_QUERY, "Query", mGst);

#define DEFINE_QUERY(type, lctype, arguments) \
    rb_cGstQuery ## type =                                          \
        rb_define_class_under(mGst, "Query" #type, rb_cGstQuery); \
    rb_define_method(rb_cGstQuery ## type, "initialize",            \
      lctype ## _initialize, arguments);                            \
    rb_define_method(rb_cGstQuery ## type, "parse",                 \
      lctype ## _parse, 0)                                          \

    DEFINE_QUERY(Position, position, 1);
    DEFINE_QUERY(Duration, duration, 1);
    DEFINE_QUERY(Latency, latency, 0);
    DEFINE_QUERY(Seeking, seeking, 1);
    DEFINE_QUERY(Segment, segment, 1);
    DEFINE_QUERY(Convert, convert, 2);
    DEFINE_QUERY(Formats, formats, 0);
#undef DEFINE_QUERY
}
