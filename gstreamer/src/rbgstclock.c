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

/*  Method: speed
 *  Returns: the speed of the clock.  
 */
static VALUE
rb_gst_clock_get_speed (VALUE self)
{
    return DBL2NUM (gst_clock_get_speed (RGST_CLOCK (self)));
}

/*
 * Method: set_speed(speed)
 * speed: the speed of the clock.
 *
 * Sets the speed of the clock.
 * 1.0 is the default speed.
 *
 * Returns: self.
 */
static VALUE
rb_gst_clock_set_speed (VALUE self, VALUE speed)
{
    gst_clock_set_speed (RGST_CLOCK (self), DBL2NUM (speed));
    return self;
}

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

/* Method: active?
 * Returns: true if the clock is active, false otherwise.
 */
static VALUE
rb_gst_clock_is_active (VALUE self)
{
    return CBOOL2RVAL (gst_clock_is_active (RGST_CLOCK (self)));
}

/*
 * Method: set_active(state)
 * state: a boolean value.
 *
 * Activates or deactivates the clock based on the active parameter. 
 * As soon as the clock is activated, the time will start ticking.
 *
 * Returns: self.
 */
static VALUE
rb_gst_clock_set_active (VALUE self, VALUE active)
{
    gst_clock_set_active (RGST_CLOCK (self), RVAL2CBOOL (active));
    return self;
}

/*
 * Method: reset!
 *
 * Reset the clock to time 0.
 *
 * Returns: self.
 */
static VALUE
rb_gst_clock_reset (VALUE self)
{
    gst_clock_reset (RGST_CLOCK (self));
    return self;
}

/*
 * Method: handle_discont(time)
 * time: time in nanoseconds.
 *
 * Notifies the clock of a discontinuity in time (nanoseconds).
 *
 * It is possible that the clock was not updated by this call because only 
 * the first discontinuitity in the pipeline is honoured. 
 *
 * Returns: true if the clock was updated, otherwise false. 
 */
static VALUE
rb_gst_clock_handle_discont (VALUE self, VALUE time)
{
    return CBOOL2RVAL (gst_clock_handle_discont (RGST_CLOCK (self),
                                                 NUM2ULL (time)));
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

/*
 * Method: event_time
 *
 * Gets the "event time" of a given clock. An event on the clock happens 
 * whenever this method is called. This ensures that multiple events that 
 * happen shortly after each other are treated as if they happened at the same 
 * time. GStreamer uses to keep state changes of multiple elements in sync.
 *
 * Returns: the time of the event (in nanoseconds).
 */
static VALUE
rb_gst_clock_get_event_time (VALUE self)
{
    return ULL2NUM (gst_clock_get_event_time (RGST_CLOCK (self)));
}

/*
 * Method: get_event_time_delay(delay)
 * delay: time before the event actually occurs (in nanoseconds).
 *
 * Gets the "event time" of a given clock. An event on the clock happens 
 * whenever this method is called. This ensures that multiple events that 
 * happen shortly after each other are treated as if they happened at the same 
 * time. GStreamer uses to keep state changes of multiple elements in sync.
 * 
 * When calling this method, the specified delay will be added to the current 
 * time to produce the event time. This can be used for events that are 
 * scheduled to happen at some point in the future.
 * 
 * Returns: the time of the event (in nanoseconds).
 */
static VALUE
rb_gst_clock_get_event_time_delay (VALUE self, VALUE delay)
{
    return ULL2NUM (gst_clock_get_event_time_delay (RGST_CLOCK (self),
                                                    NUM2ULL (delay)));
}

/*
 * Method: next_id
 *
 * Gets the clock ID of the next event.
 * 
 * Returns: the clock ID of the next event, or nil is no event is pending.
 */
static VALUE
rb_gst_clock_get_next_id (VALUE self)
{
    GstClockID id = gst_clock_get_next_id (RGST_CLOCK (self));

    return id == NULL ? Qnil : RGST_CLOCK_ENTRY_NEW (id);
}

void
Init_gst_clock (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_CLOCK, "Clock", mGst);

    rb_define_method (c, "speed", rb_gst_clock_get_speed, 0);
    rb_define_method (c, "set_speed", rb_gst_clock_set_speed, 1);
    rb_define_method (c, "resolution", rb_gst_clock_get_resolution, 0);
    rb_define_method (c, "set_resolution", rb_gst_clock_set_resolution, 1);
    rb_define_method (c, "active?", rb_gst_clock_is_active, 0);
    rb_define_method (c, "set_active", rb_gst_clock_set_active, 1);
    rb_define_method (c, "time", rb_gst_clock_get_time, 0);
    rb_define_method (c, "event_time", rb_gst_clock_get_event_time, 0);
    rb_define_method (c, "get_event_time_delay",
                      rb_gst_clock_get_event_time_delay, 1);
    rb_define_method (c, "next_id", rb_gst_clock_get_next_id, 0);
    rb_define_alias (c, "next_entry", "next_id");
    rb_define_method (c, "reset!", rb_gst_clock_reset, 0);
    rb_define_method (c, "handle_discont?", rb_gst_clock_handle_discont, 1);
    rb_define_method (c, "==", rb_gst_clock_is_equal, 1);

    G_DEF_CLASS (GST_TYPE_CLOCK_FLAGS, "Flags", c);
    G_DEF_CONSTANTS (c, GST_TYPE_CLOCK_FLAGS, "GST_CLOCK_");
    G_DEF_CLASS (GST_TYPE_CLOCK_RETURN, "Return", c);
    G_DEF_CONSTANTS (c, GST_TYPE_CLOCK_RETURN, "GST_CLOCK_");
    G_DEF_SETTERS (c);
}
