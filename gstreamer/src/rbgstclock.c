
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
 *  Class: Gst::Clock
 *
 *  Abstract class for global clocks. 
 */

/*
 *  Method: speed -> anInteger
 *
 *  Gets the speed of the clock.  
 *  The returned value can be either a Fixnum or a Float.
 */
static VALUE rb_gst_clock_get_speed(self)
    VALUE self;
{
    return DBL2NUM(gst_clock_get_speed(RGST_CLOCK(self)));
}

/*
 *  Method: time -> aFixnum
 *
 *  Gets the time of the clock (in nanoseconds), as a Bignum.
 */
static VALUE rb_gst_clock_get_time(self)
    VALUE self;
{
    return ULL2NUM(gst_clock_get_time(RGST_CLOCK(self)));
}

/*
 *  Method: active? -> aBoolean
 *
 *  Checks if the clock is active.
 */
static VALUE rb_gst_clock_is_active(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_clock_is_active(RGST_CLOCK(self)));
}

/*
 *  Method: == aClockObject -> aBoolean
 *
 *  Checks if two Gst::Clock objects are synchronized on the same time.
 */
static VALUE rb_gst_clock_is_equal(self, other_clock)
    VALUE self, other_clock;
{
    if (NIL_P(other_clock)) {
        return Qfalse;
    }
    else {
        GstClock *c1 = RGST_CLOCK(self);
        GstClock *c2 = RGST_CLOCK(other_clock);
        return CBOOL2RVAL(GST_CLOCK_DIFF(c1, c2) == 0);
    }
}

void Init_gst_clock(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_CLOCK, "Clock", mGst);

    rb_define_method(c, "speed", rb_gst_clock_get_speed, 0);
    rb_define_method(c, "active?", rb_gst_clock_is_active, 0);
    rb_define_method(c, "time", rb_gst_clock_get_time, 0);

    rb_define_method(c, "==", rb_gst_clock_is_equal, 1);
}

