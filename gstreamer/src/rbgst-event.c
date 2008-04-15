/*
 * Copyright (C) 2006 Sjoerd Simons <sjoerd@luon.net>
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
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

/* Document-class: Gst::Event
 * Events.
 */

static RGConvertTable table = {0};

static VALUE rb_cGstEvent;
static VALUE rb_cGstEventFlushStart;
static VALUE rb_cGstEventFlushStop;
static VALUE rb_cGstEventEOS;
static VALUE rb_cGstEventNewSegment;
static VALUE rb_cGstEventTag;
static VALUE rb_cGstEventBufferSize;
static VALUE rb_cGstEventQOS;
static VALUE rb_cGstEventSeek;
static VALUE rb_cGstEventNavigation;
static VALUE rb_cGstEventLatency;

static VALUE
get_superclass(void)
{
    return rb_cGstMiniObject;
}

static VALUE
instance2robj(gpointer instance)
{
    VALUE klass;
    GstEvent *event;

    event = instance;

    switch (GST_EVENT_TYPE(event)) {
       case GST_EVENT_FLUSH_START:
           klass = rb_cGstEventFlushStart;
           break;
       case GST_EVENT_FLUSH_STOP:
           klass = rb_cGstEventFlushStop;
           break;
       case GST_EVENT_EOS:
           klass = rb_cGstEventEOS;
           break;
       case GST_EVENT_NEWSEGMENT:
           klass = rb_cGstEventNewSegment;
           break;
       case GST_EVENT_TAG:
           klass = rb_cGstEventTag;
           break;
       case GST_EVENT_BUFFERSIZE:
           klass = rb_cGstEventBufferSize;
           break;
       case GST_EVENT_QOS:
           klass = rb_cGstEventQOS;
           break;
       case GST_EVENT_SEEK:
           klass = rb_cGstEventSeek;
           break;
       case GST_EVENT_NAVIGATION:
           klass = rb_cGstEventNavigation;
           break;
       case GST_EVENT_LATENCY:
           klass = rb_cGstEventLatency;
           break;
       default:
           klass = rb_cGstEvent;
           break;
    }

    gst_mini_object_ref(instance);
    return Data_Wrap_Struct(klass, NULL, _rbgst_mini_object_free, instance);
}

static VALUE
flush_start_initialize(VALUE self)
{
    GstEvent *event;

    event = gst_event_new_flush_start();

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
flush_stop_initialize(VALUE self)
{
    GstEvent *event;

    event = gst_event_new_flush_start();

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
eos_initialize(VALUE self)
{
    GstEvent *event;

    event = gst_event_new_eos();

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
newsegment_initialize(VALUE self, VALUE update, VALUE rate, VALUE applied_rate,
                      VALUE format, VALUE start, VALUE stop, VALUE position)
{
    GstEvent *event;

    event = gst_event_new_new_segment_full(RVAL2CBOOL(update),
                                           NUM2DBL(rate),
                                           NUM2DBL(applied_rate),
                                           RVAL2GST_FORMAT(format),
                                           NUM2LL(start),
                                           NUM2LL(stop),
                                           NUM2LL(position));

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
newsegment_parse(VALUE self)
{
    gboolean update;
    gdouble rate, applied_rate;
    GstFormat format;
    gint64 start, stop, position;

    gst_event_parse_new_segment_full(RGST_EVENT(self), &update, &rate,
                                     &applied_rate, &format, &start, &stop,
                                     &position);

    return rb_ary_new3(7, CBOOL2RVAL(update), DBL2NUM(update), DBL2NUM(rate),
                       GST_FORMAT2RVAL(format), LL2NUM(start), LL2NUM(stop),
                       LL2NUM(position));
}

static VALUE
tag_initialize(VALUE self, VALUE taglist)
{
    GstEvent *event;
    event = gst_event_new_tag(RVAL2GST_STRUCT(taglist));

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
tag_parse(VALUE self)
{
    GstTagList *taglist;

    gst_event_parse_tag(RGST_EVENT(self), &taglist);

    return GST_STRUCT2RVAL(taglist);
}

static VALUE
buffersize_initialize(VALUE self, VALUE format, VALUE minsize, VALUE maxsize,
                      VALUE async)
{
    GstEvent *event;

    event = gst_event_new_buffer_size(RVAL2GST_FORMAT(format), NUM2LL(minsize),
                                      NUM2LL(maxsize), RVAL2CBOOL(async));

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
buffersize_parse(VALUE self)
{
    GstFormat format;
    gint64 minsize;
    gint64 maxsize;
    gboolean async;

    gst_event_parse_buffer_size(RGST_EVENT(self), &format, &minsize,
                                &maxsize, &async);

    return rb_ary_new3(4, GST_FORMAT2RVAL(format), LL2NUM(minsize),
                       LL2NUM(maxsize), CBOOL2RVAL(async));
}

static VALUE
qos_initialize(VALUE self, VALUE portion, VALUE clockdiff, VALUE timestamp)
{
    GstEvent *event;

    event = gst_event_new_qos(NUM2DBL(portion), NUM2LL(clockdiff),
                              NUM2ULL(timestamp));

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
qos_parse(VALUE self)
{
    gdouble proportion;
    GstClockTimeDiff diff;
    GstClockTime timestamp;

    gst_event_parse_qos(RGST_EVENT(self), &proportion, &diff, &timestamp);

    return rb_ary_new3(3, DBL2NUM(proportion), LL2NUM(diff),
                       ULL2NUM(timestamp));
}

static VALUE
seek_initialize(VALUE self, VALUE rate, VALUE format, VALUE flags,
   VALUE start_type, VALUE start, VALUE stop_type, VALUE stop)
{
    GstEvent *event;

    event = gst_event_new_seek(NUM2DBL(rate),
                               RVAL2GST_FORMAT(format),
                               RVAL2GFLAGS(flags, GST_TYPE_SEEK_FLAGS),
                               RVAL2GENUM(start_type, GST_TYPE_SEEK_TYPE),
                               NUM2ULL(start),
                               RVAL2GENUM(stop_type, GST_TYPE_SEEK_TYPE),
                               NUM2ULL(stop));

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
seek_parse(VALUE self)
{
    gdouble rate;
    GstFormat format;
    GstSeekFlags flags;
    GstSeekType start_type, stop_type;
    gint64 start, stop;

    gst_event_parse_seek(RGST_EVENT(self), &rate, &format, &flags, &start_type,
                         &start, &stop_type, &stop);

    return rb_ary_new3(6, DBL2NUM(rate), GST_FORMAT2RVAL(format),
                       GFLAGS2RVAL(start_type, GST_TYPE_SEEK_FLAGS),
                       LL2NUM(start),
                       GFLAGS2RVAL(stop_type, GST_TYPE_SEEK_FLAGS),
                       LL2NUM(stop));
}

static VALUE
navigation_initialize(VALUE self, VALUE structure)
{
    GstEvent *event;

    event = gst_event_new_navigation(RVAL2GST_STRUCT(structure));

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
latency_initialize(VALUE self, VALUE latency)
{
    GstEvent *event;

    event = gst_event_new_latency(NUM2ULL(latency));

    G_INITIALIZE(self, event);
    return Qnil;
}

static VALUE
latency_parse(VALUE self)
{
    GstClockTime latency;

    gst_event_parse_latency(RGST_EVENT(self), &latency);

    return LL2NUM(latency);
}

static VALUE
rbgst_event_get_type(VALUE self)
{
  return GENUM2RVAL(GST_EVENT_TYPE(RVAL2GST_EVENT(self)), GST_TYPE_EVENT_TYPE);
}

void
Init_gst_event (void)
{
    table.type = GST_TYPE_EVENT;
    table.get_superclass = get_superclass;
    table.instance2robj = instance2robj;
    RG_DEF_CONVERSION(&table);

    rb_cGstEvent = G_DEF_CLASS (GST_TYPE_EVENT, "Event", mGst);
    rb_define_method(rb_cGstEvent, "get_type", rbgst_event_get_type, 0);
    G_DEF_CONSTANTS(rb_cGstEvent, GST_TYPE_EVENT_TYPE, "GST_EVENT_");

#define DEFINE_EVENT(type, lctype, arguments) \
    rb_cGstEvent ## type =                                          \
        rb_define_class_under(mGst, "Event" #type, rb_cGstEvent);   \
    rb_define_method(rb_cGstEvent ## type, "initialize",            \
                     lctype ## _initialize, arguments)

#define DEFINE_EVENT_PARSE(type, lctype, arguments)                 \
    DEFINE_EVENT(type, lctype, arguments);                          \
    rb_define_method(rb_cGstEvent ## type, "parse",                 \
                     lctype ## _parse, 0)

    DEFINE_EVENT(FlushStart, flush_start, 0);
    DEFINE_EVENT(FlushStop,  flush_stop, 0);
    DEFINE_EVENT(EOS, eos, 0);
    DEFINE_EVENT_PARSE(NewSegment, newsegment, 7);
    DEFINE_EVENT_PARSE(Tag, tag, 1);
    DEFINE_EVENT_PARSE(BufferSize, buffersize, 4);
    DEFINE_EVENT_PARSE(QOS, qos, 3);
    DEFINE_EVENT_PARSE(Seek, seek, 7);
    DEFINE_EVENT(Navigation, navigation, 1);
    DEFINE_EVENT_PARSE(Latency, latency, 1);

#undef DEFINE_EVENT_PARSE
#undef DEFINE_EVENT
}
