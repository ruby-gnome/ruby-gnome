/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cSystemClock

/* Class: Gst::SystemClock
 * The Gst::SystemClock class is an implementation of Gst::Clock 
 * using the system time. 
 */

/*
 * Class method: obtain
 *
 * Returns: a handle to the default system clock, as a reference
 * to a Gst::SystemClock object.
 */
static VALUE
rg_s_obtain (self)
{
    return RGST_SYSTEM_CLOCK_NEW (gst_system_clock_obtain ());
}

void
Init_gst_systemclock (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS (GST_TYPE_SYSTEM_CLOCK, "SystemClock", mGst);
    RG_DEF_SMETHOD(obtain, 0);
}
