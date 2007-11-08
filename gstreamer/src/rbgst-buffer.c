/*
 * Copyright (C) 2007 Ruby-GNOME2 Project Team
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

#define SELF(obj) (RVAL2GST_BUF(obj))
#define RVAL2GST_FLAGS(flags) \
    (RVAL2GFLAGS(flags, GST_TYPE_BUFFER_FLAG))
#define GST_FLAGS2RVAL(flags) \
    (GFLAGS2RVAL(flags, GST_TYPE_BUFFER_FLAG))

static VALUE
initialize(VALUE self)
{
    G_INITIALIZE(self, gst_buffer_new());
    return Qnil;
}

static VALUE
get_flags(VALUE self)
{
    return GST_FLAGS2RVAL(GST_BUFFER_FLAGS(SELF(self)));
}

static VALUE
set_flags(VALUE self, VALUE flag)
{
    GST_BUFFER_FLAGS(SELF(self)) = RVAL2GST_FLAGS(flag);
    return Qnil;
}

static VALUE
raise_flag(VALUE self, VALUE flag)
{
    GST_MINI_OBJECT_FLAG_SET(SELF(self), RVAL2GST_FLAGS(flag));
    return Qnil;
}

static VALUE
lower_flag(VALUE self, VALUE flag)
{
    GST_MINI_OBJECT_FLAG_UNSET(SELF(self), RVAL2GST_FLAGS(flag));
    return Qnil;
}

static VALUE
flag_raised_p(VALUE self, VALUE flag)
{
    return CBOOL2RVAL(GST_MINI_OBJECT_FLAG_IS_SET(SELF(self),
                                                  RVAL2GST_FLAGS(flag)));
}

void
Init_gst_buffer(void)
{
    VALUE rb_cGstBuffer;

    rb_cGstBuffer = G_DEF_CLASS(GST_TYPE_BUFFER, "Buffer", mGst);

    G_DEF_CLASS(GST_TYPE_BUFFER_FLAG, "Flag", rb_cGstBuffer);
    G_DEF_CONSTANTS(rb_cGstBuffer, GST_TYPE_BUFFER_FLAG, "GST_BUFFER_");

    rb_define_method(rb_cGstBuffer, "initialize", initialize, 0);

    rb_define_method(rb_cGstBuffer, "flags", get_flags, 0);
    rb_define_method(rb_cGstBuffer, "set_flags", set_flags, 1);
    rb_define_method(rb_cGstBuffer, "raise_flag", raise_flag, 1);
    rb_define_method(rb_cGstBuffer, "lower_flag", lower_flag, 1);
    rb_define_method(rb_cGstBuffer, "flag_raised?", flag_raised_p, 1);

    G_DEF_SETTERS(rb_cGstBuffer);
}
