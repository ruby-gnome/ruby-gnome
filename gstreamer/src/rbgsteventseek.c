
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
 *  Class: Gst::EventSeek < Gst::Event
 *
 *  A seek event.
 */

VALUE cGstEventSeek;

/*
 *  Constant: SEEK_METHOD_CUR
 *  Seek to a relative position.
 */
static VALUE constSeekMethodCur = INT2FIX(GST_SEEK_METHOD_CUR);

/*
 *  Constant: SEEK_METHOD_SET
 *  Seek to an absolute position.
 */
static VALUE constSeekMethodSet = INT2FIX(GST_SEEK_METHOD_SET);

/*
 *  Constant: SEEK_METHOD_END
 *  Seek relative to the end of the stream.
 */
static VALUE constSeekMethodEnd = INT2FIX(GST_SEEK_METHOD_END);

/*
 *  Constant: SEEK_FLAG_FLUSH
 *  Flush any pending data while seeking.
 */
static VALUE constSeekFlagFlush = INT2FIX(GST_SEEK_FLAG_FLUSH);

/*
 *  Constant: SEEK_FLAG_ACCURATE
 *  Seek as accuratly as possible.
 */
static VALUE constSeekFlagAccurate = INT2FIX(GST_SEEK_FLAG_ACCURATE);

/*
 *  Constant: SEEK_FLAG_KEY_UNIT
 *  Seek to a nearby key unit.
 */
static VALUE constSeekFlagKeyUnit = INT2FIX(GST_SEEK_FLAG_KEY_UNIT);

/*
 *  Constant: SEEK_FLAG_SEGMENT_LOOP
 *  Loop between start and stop in a segmented seek.
 */
static VALUE constSeekFlagSegmentLoop = INT2FIX(GST_SEEK_FLAG_SEGMENT_LOOP);

/*
 *  Class method: new(type, offset) -> anEvent
 *
 *  Allocate a new seek event with the given parameters.
 * 
 *  Meaningful types are:
 *      - Gst::EventSeek::METHOD_CUR
 *      - Gst::EventSeek::METHOD_SET
 *      - Gst::EventSeek::METHOD_END
 *
 *  Types can be OR'ed (|) with:
 *      - Gst::EventSeek::FLAG_FLUSH
 *      - Gst::EventSeek::FLAG_ACCURATE
 *      - Gst::EventSeek::FLAG_KEY_UNIT
 *      - Gst::EventSeek::FLAG_SEGMENT_LOOP
 */
static VALUE rb_gst_eventseek_new(self, type, offset)
    VALUE self, type, offset;
{
    GstEvent *event = gst_event_new_seek(FIX2INT(type),
                                         NUM2ULL(offset));
    if (event != NULL) {
        G_INITIALIZE(self, event);
    }
    return Qnil;
}

void Init_gst_eventseek(void) {
    VALUE c = rb_define_class_under(mGst, "EventSeek", 
                                    GTYPE2CLASS(GST_TYPE_EVENT));
    rb_define_method(c, "initialize", rb_gst_eventseek_new, 2);

    rb_define_const(c, "METHOD_CUR", constSeekMethodCur);
    rb_define_const(c, "METHOD_SET", constSeekMethodSet);
    rb_define_const(c, "METHOD_END", constSeekMethodEnd);
    
    rb_define_const(c, "FLAG_FLUSH",    constSeekFlagFlush);
    rb_define_const(c, "FLAG_ACCURATE", constSeekFlagAccurate);
    rb_define_const(c, "FLAG_KEY_UNIT", constSeekFlagKeyUnit);
    rb_define_const(c, "FLAG_SEGMENT_LOOP", constSeekFlagSegmentLoop);

    cGstEventSeek = c;
}

