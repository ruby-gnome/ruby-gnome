
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
 *  Class: Gst::SystemClock < Gst::Clock
 *
 *  The Gst::SystemClock class is an implementation of Gst::Clock 
 *  using the system time. 
 */

/*
 *  Class method: obtain -> aSystemClockObject
 *
 *  Gets a handle to the default system clock, as a reference
 *  to a Gst::SystemClock object.
 */
static VALUE rb_gst_systemclock_obtain(self)
{
    return RGST_SYSTEM_CLOCK_NEW(gst_system_clock_obtain());
}

void Init_gst_systemclock(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_SYSTEM_CLOCK, "SystemClock", mGst);
    rb_define_singleton_method(c, "obtain", rb_gst_systemclock_obtain, 0);
}

