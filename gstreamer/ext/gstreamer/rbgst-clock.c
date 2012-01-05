/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301  USA
 */

#include "rbgst-private.h"

#define RG_TARGET_NAMESPACE cClock
#define SELF(obj) (RVAL2GST_CLOCK(obj))

/*  Class: Gst::Clock
 *  Abstract class for global clocks.
 */

/* Method: resolution
 * Returns: the accuracy of the clock.
 */
static VALUE
rg_resolution(VALUE self)
{
    return ULL2NUM(gst_clock_get_resolution(SELF(self)));
}

/*
 * Method: set_resolution(resolution)
 * resolution: the accuracy of the clock.
 *
 * Sets the accuracy of the clock.
 *
 * Returns: self.
 */
static VALUE
rg_set_resolution(VALUE self, VALUE resolution)
{
    return ULL2NUM(gst_clock_set_resolution(SELF(self), NUM2ULL(resolution)));
}

/* Method: time
 * Returns: the time of the clock (in nanoseconds).
 */
static VALUE
rg_time(VALUE self)
{
    return ULL2NUM(gst_clock_get_time(SELF(self)));
}

/*
 * Method: ==(clock)
 * clock: a Gst::Clock object.
 *
 * Returns: true if two Gst::Clock objects are synchronized on the same time,
 * false otherwise.
 */
static VALUE
rg_operator_equal_p(VALUE self, VALUE other_clock)
{
    if (NIL_P(other_clock))
        return Qfalse;
    else {
        GstClock *c1 = SELF(self);
        GstClock *c2 = SELF(other_clock);

        return CBOOL2RVAL(GST_CLOCK_DIFF(c1, c2) == 0);
    }
}

void
Init_gst_clock (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE, rb_mGstClockTime;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_CLOCK, "Clock", mGst);

    rb_define_const(mGst, "SECOND", LL2NUM(GST_SECOND));
    rb_define_const(mGst, "MSECOND", LL2NUM(GST_MSECOND));
    rb_define_const(mGst, "USECOND", LL2NUM(GST_USECOND));
    rb_define_const(mGst, "NSECOND", LL2NUM(GST_NSECOND));

    rb_mGstClockTime = rb_define_module_under(mGst, "ClockTime");
    rb_define_const(rb_mGstClockTime, "NONE", ULL2NUM(GST_CLOCK_TIME_NONE));

    G_DEF_CLASS(GST_TYPE_CLOCK_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_CLOCK_FLAGS, "GST_CLOCK_");
    G_DEF_CLASS(GST_TYPE_CLOCK_RETURN, "Return", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_CLOCK_RETURN, "GST_CLOCK_");

    RG_DEF_METHOD(resolution, 0);
    RG_DEF_METHOD(set_resolution, 1);
    RG_DEF_METHOD(time, 0);
    RG_DEF_METHOD_OPERATOR("==", equal_p, 1);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);
}
