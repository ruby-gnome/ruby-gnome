
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
 *  Class: Gst::Format
 *
 *  Dynamically register new formats. 
 */

static GstFormat* format_copy(const GstFormat* format) {
    GstFormat* new_format;
    g_return_val_if_fail (format != NULL, NULL);
    new_format = g_new(GstFormat, sizeof(GstFormat));
    *new_format = *format;
    return new_format;
}

GType gst_format_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstFormat",
            (GBoxedCopyFunc)format_copy,
            (GBoxedFreeFunc)g_free);
    }
    return our_type;
}

/*
 *  Class method: find(aNickString) -> aFormatObject
 *
 *  Returns a reference to the Gst::Format object registered with the
 *  given nick, or nil if this query was not registered.
 */
static VALUE rb_gst_format_find(self, nick)
    VALUE self, nick;
{
    GstFormat format = gst_format_get_by_nick(RVAL2CSTR(nick));
    return format != GST_FORMAT_UNDEFINED
        ? RGST_FORMAT_NEW(&format)
        : Qnil;
}

/*
 *  Class method: each { |aFormatObject| block } -> nil
 *
 *  Calls the block for each registered format, passing a reference
 *  to the Gst::Format object as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_format_each(self)
    VALUE self;
{
    const GList *list;

    for (list = gst_format_get_definitions();
         list != NULL;
         list = g_list_next(list))
    {
        GstFormatDefinition *def = (GstFormatDefinition *) list->data;
        rb_yield(RGST_FORMAT_NEW(&(def->value)));
    }
    return Qnil;
}

/*
 *  Method: type_id -> aFixnum
 *
 *  Gets the type id of this format:
 *    * Gst::Format::UNDEFINED;
 *    * Gst::Format::DEFAULT;
 *    * Gst::Format::BYTES;
 *    * Gst::Format::TIME;
 *    * Gst::Format::BUFFERS;
 *    * Gst::Format::PERCENT;
 *    * Gst::Format::UNITS.
 */
static VALUE rb_gst_format_get_type_id(self)
    VALUE self;
{
    GstFormat *format = RGST_FORMAT(self);
    return INT2FIX(*format);
}

/*
 *  Method: nick -> aString
 *
 *  Gets the short nick of the format, as a String. 
 */
static VALUE rb_gst_format_get_nick(self)
    VALUE self;
{
    GstFormat *format = RGST_FORMAT(self);
    return CSTR2RVAL(gst_format_get_details(*format)->nick);
}

/*
 *  Method: description -> aString
 *
 *  Gets a longer description of the format, as a String. 
 */
static VALUE rb_gst_format_get_description(self)
    VALUE self;
{
    GstFormat *format = RGST_FORMAT(self);
    return CSTR2RVAL(gst_format_get_details(*format)->description);
}

/*
 *  Method: == aFormatObject -> aBoolean
 *
 *  Checks if two Gst::Format objects are registered under the
 *  same nick.
 */
static VALUE rb_gst_format_is_equal(self, other_format)
    VALUE self, other_format;
{
    GstFormat *f1, *f2;
    gchar *n1, *n2;

    if (NIL_P(other_format)) {
        return Qfalse;
    }

    f1 = RGST_FORMAT(self);
    f2 = RGST_FORMAT(other_format);

    n1 = gst_format_get_details(*f1)->nick;
    n2 = gst_format_get_details(*f2)->nick;

    return CBOOL2RVAL(strcmp(n1, n2) == 0);
}

/*
 *  Constant: UNDEFINED
 *  Undefined format.
 */
static VALUE constUndefined = INT2FIX(GST_FORMAT_UNDEFINED);

/*
 *  Constant: DEFAULT
 *  The default format of the pad/element.
 */
static VALUE constDefault = INT2FIX(GST_FORMAT_DEFAULT);

/*
 *  Constant: BYTES
 *  Bytes.
 */
static VALUE constBytes = INT2FIX(GST_FORMAT_BYTES);

/*
 *  Constant: TIME
 *  Time in nanoseconds.
 */
static VALUE constTime = INT2FIX(GST_FORMAT_TIME);

/*
 *  Constant: BUFFERS
 *  Buffers.
 */
static VALUE constBuffers = INT2FIX(GST_FORMAT_BUFFERS);

/*
 *  Constant: PERCENT
 *  Percentage of stream.
 */
static VALUE constPercent = INT2FIX(GST_FORMAT_PERCENT);

/*
 *  Constant: UNITS
 *  Frames for video, samples for audio, other definitions as defined by
 *  the media type.
 */
static VALUE constUnits = INT2FIX(GST_FORMAT_UNITS);

void Init_gst_format(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_FORMAT, "Format", mGst);

    rb_define_singleton_method(c, "each", rb_gst_format_each, 0);
    rb_define_singleton_method(c, "find", rb_gst_format_find, 1);

    rb_define_method(c, "type_id", rb_gst_format_get_type_id, 0);
    rb_define_method(c, "nick",    rb_gst_format_get_nick,    0);
    rb_define_method(c, "description", 
                     rb_gst_format_get_description, 0);

    rb_define_method(c, "==", rb_gst_format_is_equal, 1);

    rb_define_const(c, "UNDEFINED", constUndefined);
    rb_define_const(c, "DEFAULT",   constDefault);
    rb_define_const(c, "BYTES",     constBytes);
    rb_define_const(c, "TIME",      constTime);
    rb_define_const(c, "BUFFERS",   constBuffers);
    rb_define_const(c, "PERCENT",   constPercent);
    rb_define_const(c, "UNITS",     constUnits);
}

