
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

/*
 *  Class: Gst::EventSegmentSeek < Gst::EventSeek
 *
 *  A segment seek event.
 */

/*
 *  Class method: new(type, start_offset, end_offset) -> anEvent
 *
 *  Allocate a new segment seek event with the given parameters.
 * 
 *  Meaningful types are:
 *      * Gst::EventSeek::METHOD_CUR;
 *      * Gst::EventSeek::METHOD_SET;
 *      * Gst::EventSeek::METHOD_END.
 *
 *  Types can be OR'ed (|) with:
 *      * Gst::EventSeek::FLAG_FLUSH;
 *      * Gst::EventSeek::FLAG_ACCURATE;
 *      * Gst::EventSeek::FLAG_KEY_UNIT;
 *      * Gst::EventSeek::FLAG_SEGMENT_LOOP.
 */
static VALUE rb_gst_eventsegmentseek_new(self, type, start_offset, end_offset)
    VALUE self, type, start_offset, end_offset;
{
    GstEvent *event = gst_event_new_segment_seek(FIX2INT(type),
                                                 NUM2ULL(start_offset),
                                                 NUM2ULL(end_offset));
    if (event != NULL) {
        G_INITIALIZE(self, event);
    }
    return Qnil;
}

void Init_gst_eventsegmentseek(void) {
    VALUE c = rb_define_class_under(mGst, "EventSegmentSeek",
                                    cGstEventSeek);
    rb_define_method(c, "initialize", rb_gst_eventsegmentseek_new, 3);
}

