
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
 *  Method: set_speed(speed) -> self
 *
 *  Sets the speed of the clock, as a Fixnum or a Float.
 *  1.0 is default speed.
 */
static VALUE rb_gst_clock_set_speed(self, speed)
    VALUE self, speed;
{
    gst_clock_set_speed(RGST_CLOCK(self), DBL2NUM(speed));
    return self;
}

/*
 *  Method: resolution -> aBignum
 *
 *  Gets the accuracy of the clock, as a Bignum.
 */
static VALUE rb_gst_clock_get_resolution(self)
    VALUE self;
{
    return ULL2NUM(gst_clock_get_resolution(RGST_CLOCK(self)));
}

/*
 *  Method: set_resolution(resolution) -> self
 *
 *  Sets the accuracy of the clock, as a Bignum.
 */
static VALUE rb_gst_clock_set_resolution(self, resolution)
    VALUE self, resolution;
{
    gst_clock_set_resolution(RGST_CLOCK(self), ULL2NUM(resolution));
    return self;
}

/*
 *  Method: time -> aBignum
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
 *  Method: set_active(aBoolean) -> self
 *
 *  Activates or deactivates the clock based on the active parameter. 
 *  As soon as the clock is activated, the time will start ticking.
 */
static VALUE rb_gst_clock_set_active(self, active)
    VALUE self, active;
{
    gst_clock_set_active(RGST_CLOCK(self), RVAL2CBOOL(active));
    return self;
}

/*
 *  Method: reset! -> self
 *
 *  Reset the clock to time 0.
 */
static VALUE rb_gst_clock_reset(self)
    VALUE self;
{
    gst_clock_reset(RGST_CLOCK(self));
    return self;
}

/*
 *  Method: handle_discont(aBignum) -> aBoolean 
 *
 *  Notifies the clock of a discontinuity in time (nanoseconds).
 *  Returns true if the clock was updated, otherwise false. 
 *  It is possible that the clock was not updated by this call because only 
 *  the first discontinuitity in the pipeline is honoured. 
 */
static VALUE rb_gst_clock_handle_discont(self, time)
    VALUE self, time;
{
    return CBOOL2RVAL(gst_clock_handle_discont(RGST_CLOCK(self), 
                                               NUM2ULL(time)));
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

/*
 *  Constant: FLAG_CAN_DO_SINGLE_SYNC
 *  This clock can do a single sync timeout request.
 */
static VALUE constFlagCanDoSingleSync = INT2FIX(GST_CLOCK_FLAG_CAN_DO_SINGLE_SYNC);

/*
 *  Constant: FLAG_CAN_DO_SINGLE_ASYNC
 *  This clock can do a single async timeout request.
 */
static VALUE constFlagCanDoSingleAsync = INT2FIX(GST_CLOCK_FLAG_CAN_DO_SINGLE_ASYNC);

/*
 *  Constant: FLAG_CAN_DO_PERIODIC_SYNC
 *  This clock can do sync periodic timeout requests.
 */
static VALUE constFlagCanDoPeriodicSync= INT2FIX(GST_CLOCK_FLAG_CAN_DO_PERIODIC_SYNC);

/*
 *  Constant: FLAG_CAN_DO_PERIODIC_ASYNC
 *  This clock can do async periodic timeout callbacks.
 */
static VALUE constFlagCanDoPeriodicAsync = INT2FIX(GST_CLOCK_FLAG_CAN_DO_PERIODIC_ASYNC);

/*
 *  Constant: FLAG_CAN_DO_SET_RESOLUTION
 *  The resolution of this clock can be changed.
 */
static VALUE constFlagCanSetResolution = INT2FIX(GST_CLOCK_FLAG_CAN_SET_RESOLUTION);

/*
 *  Constant: FLAG_CAN_DO_SET_SPEED
 *  The speed of this clock can be changed.
 */
static VALUE constFlagCanSetSpeed = INT2FIX(GST_CLOCK_FLAG_CAN_SET_SPEED);

/*
 *  Method: can_do_single_sync? -> aBoolean
 *
 *  Checks if the Gst::Element::FLAG_CAN_DO_SINGLE_SYNC flag is set on the 
 *  object.
 */
static VALUE rb_gst_clock_can_do_single_sync(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagCanDoSingleSync);
}

/*
 *  Method: can_do_single_async? -> aBoolean
 *
 *  Checks if the Gst::Element::FLAG_CAN_DO_SINGLE_ASYNC flag is set on the 
 *  object.
 */
static VALUE rb_gst_clock_can_do_single_async(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagCanDoSingleAsync);
}

/*
 *  Method: can_do_periodic_sync? -> aBoolean
 *
 *  Checks if the Gst::Element::FLAG_CAN_DO_PERIODIC_SYNC flag is set on the 
 *  object.
 */
static VALUE rb_gst_clock_can_do_periodic_sync(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagCanDoPeriodicSync);
}

/*
 *  Method: can_do_periodic_async? -> aBoolean
 *
 *  Checks if the Gst::Element::FLAG_CAN_DO_PERIODIC_ASYNC flag is set on the 
 *  object.
 */
static VALUE rb_gst_clock_can_do_periodic_async(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagCanDoPeriodicAsync);
}

/*
 *  Method: can_set_resolution? -> aBoolean
 *
 *  Checks if the Gst::Element::FLAG_CAN_SET_RESOLUTION flag is set on the 
 *  object.
 */
static VALUE rb_gst_clock_can_set_resolution(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagCanSetResolution);
}

/*
 *  Method: can_set_speed? -> aBoolean
 *
 *  Checks if the Gst::Element::FLAG_CAN_SET_SPEED flag is set on the 
 *  object.
 */
static VALUE rb_gst_clock_can_set_speed(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagCanSetSpeed);
}

/*
 *  Constant: STOPPED
 *  The clock is stopped.
 */
static VALUE constStopped = INT2FIX(GST_CLOCK_STOPPED);

/*
 *  Constant: TIMEOUT
 *  The operation timed out.
 */
static VALUE constTimeout = INT2FIX(GST_CLOCK_TIMEOUT);

/*
 *  Constant: EARLY
 *  The operation was scheduled too late.
 */
static VALUE constEarly = INT2FIX(GST_CLOCK_EARLY);

/*
 *  Constant: ERROR
 *  An error occured.
 */
static VALUE constError = INT2FIX(GST_CLOCK_ERROR);

/*
 *  Constant: UNSUPPORTED
 *  Operation is not supported. 
 */
static VALUE constUnsupported = INT2FIX(GST_CLOCK_UNSUPPORTED);

void Init_gst_clock(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_CLOCK, "Clock", mGst);

    rb_define_method(c, "speed",     rb_gst_clock_get_speed, 0);
    rb_define_method(c, "set_speed", rb_gst_clock_set_speed, 1);
    
    rb_define_method(c, "resolution",     rb_gst_clock_get_resolution, 0);
    rb_define_method(c, "set_resolution", rb_gst_clock_set_resolution, 1);
    
    rb_define_method(c, "active?",    rb_gst_clock_is_active, 0);
    rb_define_method(c, "set_active", rb_gst_clock_set_active, 1); 
    
    rb_define_method(c, "time",   rb_gst_clock_get_time, 0);
    rb_define_method(c, "reset!", rb_gst_clock_reset,    0);

    rb_define_method(c, "handle_discont", rb_gst_clock_handle_discont, 1);

    rb_define_method(c, "==", rb_gst_clock_is_equal, 1);

    rb_define_method(c, "can_do_single_sync?",
                     rb_gst_clock_can_do_single_sync, 0);
    rb_define_method(c, "can_do_single_async?",
                     rb_gst_clock_can_do_single_async, 0);
    rb_define_method(c, "can_do_periodic_sync?",
                     rb_gst_clock_can_do_periodic_sync, 0);
    rb_define_method(c, "can_do_periodic_async?",
                     rb_gst_clock_can_do_periodic_async, 0);
    rb_define_method(c, "can_set_resolution?",
                     rb_gst_clock_can_set_resolution, 0);
    rb_define_method(c, "can_set_speed?",
                     rb_gst_clock_can_set_speed, 0);

    rb_define_const(c, "FLAG_CAN_DO_SINGLE_SYNC",   
                    constFlagCanDoSingleSync);
    rb_define_const(c, "FLAG_CAN_DO_SINGLE_ASYNC",  
                    constFlagCanDoSingleAsync);
    rb_define_const(c, "FLAG_CAN_DO_PERIODIC_SYNC", 
                    constFlagCanDoPeriodicSync);
    rb_define_const(c, "FLAG_CAN_DO_PERIODIC_ASYNC",
                    constFlagCanDoPeriodicAsync);
    rb_define_const(c, "FLAG_CAN_SET_RESOLUTION",
                    constFlagCanSetResolution);
    rb_define_const(c, "FLAG_CAN_SET_SPEED",    
                    constFlagCanSetSpeed);

    rb_define_const(c, "STOPPED",     constStopped);
    rb_define_const(c, "TIMEOUT",     constTimeout);
    rb_define_const(c, "EARLY",       constEarly);
    rb_define_const(c, "ERROR",       constError);
    rb_define_const(c, "UNSUPPORTED", constUnsupported);

    G_DEF_SETTERS(c);
}

