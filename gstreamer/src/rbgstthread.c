
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
 *  Class: Gst::Thread < Gst::Bin
 *
 *  A Gst::Bin that will become a thread.
 */

/*
 *  Class method: new(aStringName=nil) -> aThreadObject
 *
 *  Constructs a new Gst::Thread object.
 *
 *  If element name is ommited (or nil), then the thread will receive a guaranteed
 *  unique name, consisting of the "thread" string and a number.
 *  If name is given, it will be given the name supplied.
 */
static VALUE rb_gst_thread_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GstElement *bin;
    VALUE name;

    rb_scan_args(argc, argv, "01", &name);

    bin = gst_thread_new(name != Qnil ? RVAL2CSTR(name) : NULL);
    if (bin != NULL) {
        RBGST_INITIALIZE(self, bin);
    }
    return Qnil;
}

void Init_gst_thread(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_THREAD, "Thread", mGst);
    rb_define_method(c, "initialize", rb_gst_thread_new, -1);
}

