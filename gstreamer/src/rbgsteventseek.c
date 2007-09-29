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

/*  Class: Gst::EventSeek
 *  A seek event.
 */
VALUE cGstEventSeek;

/*
 * Class method: new(type, offset)
 * type: an event seek type (see Gst::EventSeek::Type).
 * offset: an offset.
 *
 * Allocates a new seek event with the given parameters.
 * Types (METHOD_) cam be OR'ed (|) with flags (FLAG_).
 *
 * Returns: a newly allocated Gst::EventSeek object.
 */
static VALUE
rb_gst_event_seek_new(VALUE self, VALUE rate, VALUE format, VALUE flags,
                      VALUE start_type, VALUE start,
                      VALUE stop_type, VALUE stop)
{
    GstEvent *event;

    event = gst_event_new_seek(NUM2DBL(rate),
                               GENUM2RVAL(format, GST_TYPE_FORMAT),
                               GFLAGS2RVAL(flags, GST_TYPE_SEEK_FLAGS),
                               GENUM2RVAL(start_type, GST_TYPE_SEEK_TYPE),
                               NUM2LL(start),
                               GENUM2RVAL(stop_type, GST_TYPE_SEEK_TYPE),
                               NUM2LL(stop));
    if (event != NULL)
        G_INITIALIZE(self, event);
    return Qnil;
}

void
Init_gst_eventseek (void)
{
    VALUE c = rb_define_class_under(mGst, "EventSeek", rb_cGstEvent);

    rb_define_method(c, "initialize", rb_gst_event_seek_new, 7);

    G_DEF_CLASS(GST_TYPE_SEEK_TYPE, "Type", c);
    G_DEF_CONSTANTS(c, GST_TYPE_SEEK_TYPE, "GST_SEEK_");

    cGstEventSeek = c;
}
