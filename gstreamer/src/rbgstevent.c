
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
 *  Class: Gst::Event
 *
 *  Event definitions.
 */

/*
 *  Constant: UNKNOWN
 *  Unknown event.
 */
static VALUE constUnknown = INT2FIX(GST_EVENT_UNKNOWN);

/*
 *  Constant: EOS
 *  And end-of-stream event.
 */
static VALUE constEos = INT2FIX(GST_EVENT_EOS);

/*
 *  Constant: FLUSH
 *  A flush event.
 */
static VALUE constFlush = INT2FIX(GST_EVENT_FLUSH);

/*
 *  Constant: EMPTY
 *  An empty event.
 */
static VALUE constEmpty = INT2FIX(GST_EVENT_EMPTY);

/*
 *  Constant: DISCONTINUOUS
 *  A discontinuous event to indicate the stream has a discontinuity.
 */
static VALUE constDiscontinuous = INT2FIX(GST_EVENT_DISCONTINUOUS);

/*
 *  Constant: NEW_MEDIA
 *  A new media stream is started.
 */
static VALUE constNewMedia = INT2FIX(GST_EVENT_NEW_MEDIA);

/*
 *  Constant: QOS
 *  A quality of service event.
 */
static VALUE constQos = INT2FIX(GST_EVENT_QOS);

/*
 *  Constant: SEEK
 *  A seek event.
 */
static VALUE constSeek = INT2FIX(GST_EVENT_SEEK);

/*
 *  Constant: SEEK_SEGMENT
 *  A segment seek with start and stop position.
 */
static VALUE constSeekSegment = INT2FIX(GST_EVENT_SEEK_SEGMENT);

/*
 *  Constant: SEGMENT_DONE
 *  The event that will be emited when the segment seek has ended.
 */
static VALUE constSegmentDone = INT2FIX(GST_EVENT_SEGMENT_DONE);

/*
 *  Constant: SIZE
 *  A size suggestion for a peer element.
 */
static VALUE constSize = INT2FIX(GST_EVENT_SIZE);

/*
 *  Constant: RATE
 *  Adjust the output rate of an element.
 */
static VALUE constRate = INT2FIX(GST_EVENT_RATE);

/*
 *  Constant: FILLER
 *  A dummy event that should be ignored by plugins.
 */
static VALUE constFiller = INT2FIX(GST_EVENT_FILLER);

/*
 *  Constant: TS_OFFSET
 *  An event to set the time offset on buffers.
 */
static VALUE constTsOffset = INT2FIX(GST_EVENT_TS_OFFSET);

/*
 *  Constant: INTERRUPT
 *  An interrupt event.
 */
static VALUE constInterrupt = INT2FIX(GST_EVENT_INTERRUPT);

/*
 *  Class method: new(anIntegerType) -> anEvent
 *
 *  Allocate a new event of the given type.
 *
 *  Meaningful types are:
 *      - Gst::Event::UNKNOWN
 *      - Gst::Event::EOS
 *      - Gst::Event::FLUSH
 *      - Gst::Event::EMPTY
 *      - Gst::Event::DISCONTINUOUS
 *      - Gst::Event::NEW::MEDIA
 *      - Gst::Event::QOS
 *      - Gst::Event::SEEK
 *      - Gst::Event::SEEK::SEGMENT
 *      - Gst::Event::SEGMENT::DONE
 *      - Gst::Event::SIZE
 *      - Gst::Event::RATE
 *      - Gst::Event::FILLER
 *      - Gst::Event::TS::OFFSET
 *      - Gst::Event::INTERRUPT 
 */
static VALUE rb_gst_event_new(self, rtype)
    VALUE self, rtype;
{
    int type = FIX2INT(rtype);

    if ( type != GST_EVENT_UNKNOWN
      && type != GST_EVENT_EOS
      && type != GST_EVENT_FLUSH
      && type != GST_EVENT_EMPTY
      && type != GST_EVENT_DISCONTINUOUS
      && type != GST_EVENT_NEW_MEDIA
      && type != GST_EVENT_QOS
      && type != GST_EVENT_SEEK
      && type != GST_EVENT_SEEK_SEGMENT
      && type != GST_EVENT_SEGMENT_DONE
      && type != GST_EVENT_SIZE
      && type != GST_EVENT_RATE
      && type != GST_EVENT_FILLER
      && type != GST_EVENT_TS_OFFSET
      && type != GST_EVENT_INTERRUPT )
    {
        rb_raise(rb_eArgError, "Invalid type");
    }
    else {
        GstEvent *event = gst_event_new(type);
        if (event != NULL) {
            G_INITIALIZE(self, event);
        }
    }
    return Qnil;
}

/*
 *  Method: src -> anObject
 *
 *  Gets the source Gst::Object that generated this event.
 */
static VALUE rb_gst_event_src(self)
    VALUE self;
{
    GstObject *object = GST_EVENT_SRC(RGST_EVENT(self));
    return object != NULL
        ? RGST_OBJECT_NEW(object)
        : Qnil;
}

/*
 *  Method: timestamp -> aFixnum
 *
 *  Gets the timestamp of this event, as a Bignum.
 */
static VALUE rb_gst_event_timestamp(self)
    VALUE self;
{
    return ULL2NUM(GST_EVENT_TIMESTAMP(RGST_EVENT(self)));
}

void Init_gst_event(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_EVENT, "Event", mGst);
    
    rb_define_method(c, "initialize", rb_gst_event_new, 1);
    rb_define_method(c, "src",        rb_gst_event_src, 0);
    rb_define_method(c, "timestamp",  rb_gst_event_timestamp, 0);

    rb_define_const(c, "UNKONWN",       constUnknown);
    rb_define_const(c, "EOS",           constEos);
    rb_define_const(c, "FLUSH",         constFlush);
    rb_define_const(c, "EMPTY",         constEmpty);
    rb_define_const(c, "DISCONTINUOUS", constDiscontinuous);
    rb_define_const(c, "NEW_MEDIA",     constNewMedia);
    rb_define_const(c, "QOS",           constQos);
    rb_define_const(c, "SEEK",          constSeek);
    rb_define_const(c, "SEEK_SEGMENT",  constSeekSegment);
    rb_define_const(c, "SEGMENT_DONE",  constSegmentDone);
    rb_define_const(c, "SIZE",          constSize);
    rb_define_const(c, "RATE",          constRate);
    rb_define_const(c, "FILLER",        constFiller);
    rb_define_const(c, "TS_OFFSET",     constTsOffset);
    rb_define_const(c, "INTERRUPT",     constInterrupt);
}

