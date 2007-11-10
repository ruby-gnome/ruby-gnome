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
#define RVAL2GST_COPY_FLAGS(flags) \
    (RVAL2GFLAGS(flags, GST_TYPE_BUFFER_COPY_FLAGS))
#define GST_COPY_FLAGS2RVAL(flags) \
    (GFLAGS2RVAL(flags, GST_TYPE_BUFFER_COPY_FLAGS))

static VALUE
initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE size;
    GstBuffer *buffer;

    rb_scan_args(argc, argv, "01", &size);

    if (NIL_P(size))
        buffer = gst_buffer_new();
    else
        buffer = gst_buffer_new_and_alloc(NUM2UINT(size));

    G_INITIALIZE(self, buffer);

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

static VALUE
get_data(VALUE self)
{
    return CSTR2RVAL((char *)GST_BUFFER_DATA(SELF(self)));
}

static VALUE
set_data(VALUE self, VALUE data)
{
    GstBuffer *buffer;
    buffer = SELF(self);

    rb_ivar_set(self, rb_intern("data"), data);
    if (NIL_P(data)) {
        gst_buffer_set_data(buffer, NULL, 0);
    } else {
        gst_buffer_set_data(buffer,
                            (guint8 *)RVAL2CSTR(data),
                            RSTRING_LEN(data));
    }
    return Qnil;
}

static VALUE
get_size(VALUE self)
{
    return UINT2NUM(GST_BUFFER_SIZE(SELF(self)));
}

static VALUE
set_size(VALUE self, VALUE size)
{
    GST_BUFFER_SIZE(SELF(self)) = NUM2UINT(size);
    return Qnil;
}

static VALUE
get_timestamp(VALUE self)
{
    return ULL2NUM(GST_BUFFER_TIMESTAMP(SELF(self)));
}

static VALUE
set_timestamp(VALUE self, VALUE timestamp)
{
    GST_BUFFER_TIMESTAMP(SELF(self)) = NUM2ULL(timestamp);
    return Qnil;
}

static VALUE
valid_timestamp_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_TIMESTAMP_IS_VALID(SELF(self)));
}

static VALUE
get_duration(VALUE self)
{
    return ULL2NUM(GST_BUFFER_DURATION(SELF(self)));
}

static VALUE
set_duration(VALUE self, VALUE duration)
{
    GST_BUFFER_DURATION(SELF(self)) = NUM2ULL(duration);
    return Qnil;
}

static VALUE
valid_duration_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_DURATION_IS_VALID(SELF(self)));
}

static VALUE
get_caps(VALUE self)
{
    return GST_CAPS2RVAL(GST_BUFFER_CAPS(SELF(self)));
}

static VALUE
set_caps(VALUE self, VALUE caps)
{
    gst_buffer_set_caps(SELF(self), RVAL2GST_CAPS(caps));
    return Qnil;
}

static VALUE
get_offset(VALUE self)
{
    return ULL2NUM(GST_BUFFER_OFFSET(SELF(self)));
}

static VALUE
set_offset(VALUE self, VALUE offset)
{
    GST_BUFFER_OFFSET(SELF(self)) = NUM2ULL(offset);
    return Qnil;
}

static VALUE
valid_offset_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_OFFSET_IS_VALID(SELF(self)));
}

static VALUE
get_offset_end(VALUE self)
{
    return ULL2NUM(GST_BUFFER_OFFSET_END(SELF(self)));
}

static VALUE
set_offset_end(VALUE self, VALUE offset_end)
{
    GST_BUFFER_OFFSET_END(SELF(self)) = NUM2ULL(offset_end);
    return Qnil;
}

static VALUE
valid_offset_end_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_OFFSET_END_IS_VALID(SELF(self)));
}

static VALUE
discontinuity_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_IS_DISCONT(SELF(self)));
}

static VALUE
copy_metadata(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2, copied_dest;
    GstBuffer *dest;
    GstBufferCopyFlags flags;

    rb_scan_args(argc, argv, "11", &arg1, &arg2);
    if (argc == 1) {
        dest = gst_buffer_new();
        flags = RVAL2GST_COPY_FLAGS(arg1);
    } else {
        dest = SELF(arg1);
        flags = RVAL2GST_COPY_FLAGS(arg2);
    }

    gst_buffer_copy_metadata(dest, SELF(self), flags);
    copied_dest = GOBJ2RVAL(dest);
    if (argc == 1)
        gst_buffer_unref(dest);

    return copied_dest;
}

static VALUE
metadata_writable_p(VALUE self)
{
    return CBOOL2RVAL(gst_buffer_is_metadata_writable(SELF(self)));
}

static VALUE
metadata_writable_bang(VALUE self)
{
    GstBuffer *orig_buffer, *buffer;
    VALUE rb_buffer;

    orig_buffer = SELF(self);
    buffer = gst_buffer_make_metadata_writable(orig_buffer);
    rb_buffer = GOBJ2RVAL(buffer);
    DATA_PTR(self) = buffer;
    if (buffer != orig_buffer)
        gst_buffer_unref(orig_buffer);
    return rb_buffer;
}

static VALUE
create_sub(VALUE self, VALUE offset, VALUE size)
{
    GstBuffer *buffer;
    VALUE rb_buffer;

    buffer = gst_buffer_create_sub(SELF(self), NUM2UINT(offset), NUM2UINT(size));
    rb_buffer = GOBJ2RVAL(buffer);
    return rb_buffer;
}

void
Init_gst_buffer(void)
{
    VALUE rb_cGstBuffer, rb_cGstBufferCopyFlags;

    rb_cGstBuffer = G_DEF_CLASS(GST_TYPE_BUFFER, "Buffer", mGst);

    rb_define_const(rb_cGstBuffer, "OFFSET_NONE",
                    UINT2NUM(GST_BUFFER_OFFSET_NONE));

    G_DEF_CLASS(GST_TYPE_BUFFER_FLAG, "Flags", rb_cGstBuffer);
    G_DEF_CONSTANTS(rb_cGstBuffer, GST_TYPE_BUFFER_FLAG, "GST_BUFFER_");

    rb_cGstBufferCopyFlags = G_DEF_CLASS(GST_TYPE_BUFFER_COPY_FLAGS,
                                         "CopyFlags", rb_cGstBuffer);
    G_DEF_CONSTANTS(rb_cGstBuffer, GST_TYPE_BUFFER_COPY_FLAGS, "GST_BUFFER_");
    rb_define_const(rb_cGstBufferCopyFlags, "ALL",
                    GST_COPY_FLAGS2RVAL(GST_BUFFER_COPY_ALL));
    rb_define_const(rb_cGstBuffer, "COPY_ALL",
                    GST_COPY_FLAGS2RVAL(GST_BUFFER_COPY_ALL));

    rb_define_method(rb_cGstBuffer, "initialize", initialize, -1);

    rb_define_method(rb_cGstBuffer, "flags", get_flags, 0);
    rb_define_method(rb_cGstBuffer, "set_flags", set_flags, 1);
    rb_define_method(rb_cGstBuffer, "raise_flag", raise_flag, 1);
    rb_define_method(rb_cGstBuffer, "lower_flag", lower_flag, 1);
    rb_define_method(rb_cGstBuffer, "flag_raised?", flag_raised_p, 1);

    rb_define_method(rb_cGstBuffer, "data", get_data, 0);
    rb_define_method(rb_cGstBuffer, "set_data", set_data, 1);

    rb_define_method(rb_cGstBuffer, "size", get_size, 0);
    rb_define_method(rb_cGstBuffer, "set_size", set_size, 1);

    rb_define_method(rb_cGstBuffer, "timestamp", get_timestamp, 0);
    rb_define_method(rb_cGstBuffer, "set_timestamp", set_timestamp, 1);
    rb_define_method(rb_cGstBuffer, "valid_timestamp?", valid_timestamp_p, 0);

    rb_define_method(rb_cGstBuffer, "duration", get_duration, 0);
    rb_define_method(rb_cGstBuffer, "set_duration", set_duration, 1);
    rb_define_method(rb_cGstBuffer, "valid_duration?", valid_duration_p, 0);

    rb_define_method(rb_cGstBuffer, "caps", get_caps, 0);
    rb_define_method(rb_cGstBuffer, "set_caps", set_caps, 1);

    rb_define_method(rb_cGstBuffer, "offset", get_offset, 0);
    rb_define_method(rb_cGstBuffer, "set_offset", set_offset, 1);
    rb_define_method(rb_cGstBuffer, "valid_offset?", valid_offset_p, 0);

    rb_define_method(rb_cGstBuffer, "offset_end", get_offset_end, 0);
    rb_define_method(rb_cGstBuffer, "set_offset_end", set_offset_end, 1);
    rb_define_method(rb_cGstBuffer, "valid_offset_end?", valid_offset_end_p, 0);

    rb_define_method(rb_cGstBuffer, "discontinuity?", discontinuity_p, 0);

    rb_define_method(rb_cGstBuffer, "copy_metadata", copy_metadata, -1);

    rb_define_method(rb_cGstBuffer, "metadata_writable?",
                     metadata_writable_p, 0);
    rb_define_method(rb_cGstBuffer, "metadata_writable!",
                     metadata_writable_bang, 0);

    rb_define_method(rb_cGstBuffer, "create_sub", create_sub, 2);

    G_DEF_SETTERS(rb_cGstBuffer);
}
