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

/*  Class: Gst::Clock
 *  Abstract class for global clocks. 
 */

/* Method: resolution
 * Returns: the accuracy of the clock.
 */
static VALUE
rb_gst_clock_get_resolution (VALUE self)
{
    return ULL2NUM (gst_clock_get_resolution (RGST_CLOCK (self)));
}

/*
 * Method: set_resolution(resolution)
 * resolution: the accurary of the clock.
 *
 * Sets the accuracy of the clock.
 *
 * Returns: self.
 */
static VALUE
rb_gst_clock_set_resolution (VALUE self, VALUE resolution)
{
    gst_clock_set_resolution (RGST_CLOCK (self), ULL2NUM (resolution));
    return self;
}

/* Method: time
 * Returns: the time of the clock (in nanoseconds).
 */
static VALUE
rb_gst_clock_get_time (VALUE self)
{
    return ULL2NUM (gst_clock_get_time (RGST_CLOCK (self)));
}

/*
 * Method: ==(clock)
 * clock: a Gst::Clock object.
 *
 * Returns: true if two Gst::Clock objects are synchronized on the same time,
 * false otherwise.
 */
static VALUE
rb_gst_clock_is_equal (VALUE self, VALUE other_clock)
{
    if (NIL_P (other_clock))
        return Qfalse;
    else {
        GstClock *c1 = RGST_CLOCK (self);
        GstClock *c2 = RGST_CLOCK (other_clock);

        return CBOOL2RVAL (GST_CLOCK_DIFF (c1, c2) == 0);
    }
}

void
Init_gst_clock (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_CLOCK, "Clock", mGst);

    rb_define_method (c, "resolution", rb_gst_clock_get_resolution, 0);
    rb_define_method (c, "set_resolution", rb_gst_clock_set_resolution, 1);
    rb_define_method (c, "time", rb_gst_clock_get_time, 0);
    rb_define_method (c, "==", rb_gst_clock_is_equal, 1);

    G_DEF_CLASS (GST_TYPE_CLOCK_FLAGS, "Flags", c);
    G_DEF_CONSTANTS (c, GST_TYPE_CLOCK_FLAGS, "GST_CLOCK_");
    G_DEF_CLASS (GST_TYPE_CLOCK_RETURN, "Return", c);
    G_DEF_CONSTANTS (c, GST_TYPE_CLOCK_RETURN, "GST_CLOCK_");
    G_DEF_SETTERS (c);
}
