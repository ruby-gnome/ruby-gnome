/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011-2012  Ruby-GNOME2 Project Team
 *  Copyright (C) 2007  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cBuffer
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
rg_initialize(int argc, VALUE *argv, VALUE self)
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
rg_flags(VALUE self)
{
    return GST_FLAGS2RVAL(GST_BUFFER_FLAGS(SELF(self)));
}

static VALUE
rg_set_flags(VALUE self, VALUE flag)
{
    GST_BUFFER_FLAGS(SELF(self)) = RVAL2GST_FLAGS(flag);
    return Qnil;
}

static VALUE
rg_raise_flag(VALUE self, VALUE flag)
{
    GST_MINI_OBJECT_FLAG_SET(SELF(self), RVAL2GST_FLAGS(flag));
    return Qnil;
}

static VALUE
rg_lower_flag(VALUE self, VALUE flag)
{
    GST_MINI_OBJECT_FLAG_UNSET(SELF(self), RVAL2GST_FLAGS(flag));
    return Qnil;
}

static VALUE
rg_flag_raised_p(VALUE self, VALUE flag)
{
    return CBOOL2RVAL(GST_MINI_OBJECT_FLAG_IS_SET(SELF(self),
                                                  RVAL2GST_FLAGS(flag)));
}

static VALUE
rg_data(VALUE self)
{
    GstBuffer *buffer;
    guint size;

    buffer = SELF(self);
    size = GST_BUFFER_SIZE(buffer);

    if (size == 0)
        return Qnil;
    else
        return rb_str_new((char *)GST_BUFFER_DATA(buffer), size);
}

static VALUE
rg_set_data(VALUE self, VALUE data)
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
rg_size(VALUE self)
{
    return UINT2NUM(GST_BUFFER_SIZE(SELF(self)));
}

static VALUE
rg_set_size(VALUE self, VALUE size)
{
    GST_BUFFER_SIZE(SELF(self)) = NUM2UINT(size);
    return Qnil;
}

static VALUE
rg_timestamp(VALUE self)
{
    return ULL2NUM(GST_BUFFER_TIMESTAMP(SELF(self)));
}

static VALUE
rg_set_timestamp(VALUE self, VALUE timestamp)
{
    GST_BUFFER_TIMESTAMP(SELF(self)) = NUM2ULL(timestamp);
    return Qnil;
}

static VALUE
rg_valid_timestamp_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_TIMESTAMP_IS_VALID(SELF(self)));
}

static VALUE
rg_duration(VALUE self)
{
    return ULL2NUM(GST_BUFFER_DURATION(SELF(self)));
}

static VALUE
rg_set_duration(VALUE self, VALUE duration)
{
    GST_BUFFER_DURATION(SELF(self)) = NUM2ULL(duration);
    return Qnil;
}

static VALUE
rg_valid_duration_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_DURATION_IS_VALID(SELF(self)));
}

static VALUE
rg_caps(VALUE self)
{
    return GST_CAPS2RVAL(GST_BUFFER_CAPS(SELF(self)));
}

static VALUE
rg_set_caps(VALUE self, VALUE caps)
{
    gst_buffer_set_caps(SELF(self), RVAL2GST_CAPS(caps));
    return Qnil;
}

static VALUE
rg_offset(VALUE self)
{
    return ULL2NUM(GST_BUFFER_OFFSET(SELF(self)));
}

static VALUE
rg_set_offset(VALUE self, VALUE offset)
{
    GST_BUFFER_OFFSET(SELF(self)) = NUM2ULL(offset);
    return Qnil;
}

static VALUE
rg_valid_offset_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_OFFSET_IS_VALID(SELF(self)));
}

static VALUE
rg_offset_end(VALUE self)
{
    return ULL2NUM(GST_BUFFER_OFFSET_END(SELF(self)));
}

static VALUE
rg_set_offset_end(VALUE self, VALUE offset_end)
{
    GST_BUFFER_OFFSET_END(SELF(self)) = NUM2ULL(offset_end);
    return Qnil;
}

static VALUE
rg_valid_offset_end_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_OFFSET_END_IS_VALID(SELF(self)));
}

static VALUE
rg_discontinuity_p(VALUE self)
{
    return CBOOL2RVAL(GST_BUFFER_IS_DISCONT(SELF(self)));
}

static VALUE
rg_copy_metadata(int argc, VALUE *argv, VALUE self)
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
rg_metadata_writable_p(VALUE self)
{
    return CBOOL2RVAL(gst_buffer_is_metadata_writable(SELF(self)));
}

static VALUE
rg_metadata_writable_bang(VALUE self)
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
rg_create_sub(VALUE self, VALUE offset, VALUE size)
{
    GstBuffer *buffer;
    VALUE rb_buffer;

    buffer = gst_buffer_create_sub(SELF(self), NUM2UINT(offset), NUM2UINT(size));
    rb_buffer = GOBJ2RVAL(buffer);
    return rb_buffer;
}

static VALUE
rg_span(VALUE self, VALUE offset, VALUE other, VALUE length)
{
    return GOBJ2RVAL(gst_buffer_span(SELF(self), NUM2UINT(offset),
                                     SELF(other), NUM2UINT(length)));
}

static VALUE
rg_span_p(VALUE self, VALUE other)
{
    return CBOOL2RVAL(gst_buffer_is_span_fast(SELF(self), SELF(other)));
}

void
Init_gst_buffer(VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE, rb_cGstBufferCopyFlags;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_BUFFER, "Buffer", mGst);

    rb_define_const(RG_TARGET_NAMESPACE, "OFFSET_NONE",
                    ULL2NUM(GST_BUFFER_OFFSET_NONE));

    G_DEF_CLASS(GST_TYPE_BUFFER_FLAG, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_BUFFER_FLAG, "GST_BUFFER_");

    rb_cGstBufferCopyFlags = G_DEF_CLASS(GST_TYPE_BUFFER_COPY_FLAGS,
                                         "CopyFlags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_BUFFER_COPY_FLAGS, "GST_BUFFER_");
    rb_define_const(rb_cGstBufferCopyFlags, "ALL",
                    GST_COPY_FLAGS2RVAL(GST_BUFFER_COPY_ALL));
    rb_define_const(RG_TARGET_NAMESPACE, "COPY_ALL",
                    GST_COPY_FLAGS2RVAL(GST_BUFFER_COPY_ALL));

    RG_DEF_METHOD(initialize, -1);

    RG_DEF_METHOD(flags, 0);
    RG_DEF_METHOD(set_flags, 1);
    RG_DEF_METHOD(raise_flag, 1);
    RG_DEF_METHOD(lower_flag, 1);
    RG_DEF_METHOD_P(flag_raised, 1);

    RG_DEF_METHOD(data, 0);
    RG_DEF_METHOD(set_data, 1);

    RG_DEF_METHOD(size, 0);
    RG_DEF_METHOD(set_size, 1);
    RG_DEF_ALIAS("length", "size");
    RG_DEF_ALIAS("set_length", "set_size");

    RG_DEF_METHOD(timestamp, 0);
    RG_DEF_METHOD(set_timestamp, 1);
    RG_DEF_METHOD_P(valid_timestamp, 0);

    RG_DEF_METHOD(duration, 0);
    RG_DEF_METHOD(set_duration, 1);
    RG_DEF_METHOD_P(valid_duration, 0);

    RG_DEF_METHOD(caps, 0);
    RG_DEF_METHOD(set_caps, 1);

    RG_DEF_METHOD(offset, 0);
    RG_DEF_METHOD(set_offset, 1);
    RG_DEF_METHOD_P(valid_offset, 0);

    RG_DEF_METHOD(offset_end, 0);
    RG_DEF_METHOD(set_offset_end, 1);
    RG_DEF_METHOD_P(valid_offset_end, 0);

    RG_DEF_METHOD_P(discontinuity, 0);

    RG_DEF_METHOD(copy_metadata, -1);

    RG_DEF_METHOD_P(metadata_writable, 0);
    RG_DEF_METHOD_BANG(metadata_writable, 0);

    RG_DEF_METHOD(create_sub, 2);

    RG_DEF_METHOD(span, 3);
    RG_DEF_METHOD_P(span, 1);
}
