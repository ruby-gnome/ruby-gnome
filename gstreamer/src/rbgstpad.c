
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
 *  Class: Gst::Pad < Gst::Object
 *
 *  The link between Gst::Element objects.
 */

/*
 *  Method: name -> aString
 *
 *  Gets the pad's name, as a String. 
 */

static VALUE rb_gst_pad_get_name(self)
    VALUE self;
{
    GstPad *pad = RGST_PAD(self);
    return CSTR2RVAL(gst_pad_get_name(pad));
}

/*
 *  Method: direction -> aFixnum
 *
 *  Gets the pad's direction, which can be:
 *      * Gst::Pad::DIRECTION_UNKNOWN;
 *      * Gst::Pad::DIRECTION_SRC;
 *      * Gst::Pad::DIRECTION_SINK.
 */

static VALUE rb_gst_pad_get_direction(self)
    VALUE self;
{
    GstPad *pad = RGST_PAD(self);
    return INT2FIX(GST_PAD_DIRECTION(pad));
}

/*
 *  Constant: PRESENCE_ALWAYS 
 *  The pad is always available.
 */
static VALUE constPresenceAlways = INT2FIX(GST_PAD_ALWAYS); 

/*
 *  Constant: PRESENCE_SOMETIMES 
 *  The pad will become available depending on the media stream.
 */
static VALUE constPresenceSometimes = INT2FIX(GST_PAD_SOMETIMES);

/*
 *  Constant: PRESENCE_REQUEST 
 *  The pad is only available on request 
 *  with Gst::Element#request_pad_by_name or 
 *  or Gst::Element#request_compatible_pad.
 */
static VALUE constPresenceRequest = INT2FIX(GST_PAD_REQUEST);

/*
 *  Constant: DIRECTION_UNKNOWN 
 *  Direction is unknown.
 */
static VALUE constDirectionUnknown = INT2FIX(GST_PAD_UNKNOWN);

/*
 *  Constant: DIRECTION_SRC
 *  The pad is a source pad.
 */
static VALUE constDirectionSrc = INT2FIX(GST_PAD_SRC);

/*
 *  Constant: DIRECTION_SINK
 *  The pad is a sink pad.
 */
static VALUE constDirectionSink = INT2FIX(GST_PAD_SINK);

/*
 *  Method: pad_template -> aPadTemplate
 *
 *  Gets the Gst::PadTemplate object of this pad.
 */
static VALUE rb_gst_pad_get_padtemplate(self)
    VALUE self;
{
    GstPad *pad;
    GstPadTemplate *templ;
    
    pad   = RGST_PAD(self);
    templ = gst_pad_get_pad_template(pad);
    if (templ != NULL) {
        gst_object_ref(GST_OBJECT(templ));
        return RGST_PAD_TEMPLATE_NEW(templ);
    }   
    return Qnil;
}

/*
 *  Method: provides_formats? -> aBoolean
 *
 *  Checks if the pad supports formats not handled by
 *  the default format dispatcher.
 */
static VALUE rb_gst_pad_provides_formats(self)
    VALUE self;
{
    GstPad *pad = RGST_PAD(self);
    return CBOOL2RVAL(GST_RPAD_FORMATSFUNC(pad) != gst_pad_get_formats_default);
}

/*
 *  Method: provides_event_masks? -> aBoolean
 *
 *  Checks if the pad supports event masks not handled by
 *  the default event masks dispatcher.
 */
static VALUE rb_gst_pad_provides_event_masks(self)
    VALUE self;
{
    GstPad *pad = RGST_PAD(self);
    return CBOOL2RVAL(GST_RPAD_EVENTMASKFUNC(pad) != gst_pad_get_event_masks_default);
}

/*
 *  Method: provides_query_types? -> aBoolean
 *
 *  Checks if the pad supports query types not handled by
 *  the default query types dispatcher.
 */
static VALUE rb_gst_pad_provides_query_types(self)
    VALUE self;
{
    GstPad *pad = RGST_PAD(self);
    return CBOOL2RVAL(GST_RPAD_QUERYTYPEFUNC(pad) != gst_pad_get_query_types_default);
}

/*
 *  Method: formats -> anArray
 *
 *  Gets the list of supported formats from the pad, in an array
 *  of Gst::Format objects.
 */
static VALUE rb_gst_pad_get_formats(self)
    VALUE self;
{
    const GstFormat *formats; 
    VALUE arr;

    arr = rb_ary_new();
    formats = gst_pad_get_formats(RGST_PAD(self));
    
    while (formats && *formats) {
        rb_ary_push(arr, RGST_FORMAT_NEW(formats));
        formats++;
    }
    return arr;
}

/*
 *  Method: event_masks -> anArray 
 *
 *  Gets the list of event masks from the pad, in an array 
 *  of Gst::EventMask objects.
 */
static VALUE rb_gst_pad_get_event_masks(self) 
    VALUE self;
{
    const GstEventMask *masks; 
    VALUE arr;

    arr = rb_ary_new();
    masks = gst_pad_get_event_masks(RGST_PAD(self));
    
    while (masks && masks->type) {
        rb_ary_push(arr, RGST_EVENT_MASK_NEW(masks));
        masks++;
    }
    return arr;
}

/*
 *  Method: query_types -> anArray
 *
 *  Gets a list of supported queries that can be performed on the pad,
 *  in an array of Gst::QueryType objects.
 */
static VALUE rb_gst_pad_get_query_types(self) 
    VALUE self;
{
    const GstQueryType *types; 
    GstPad *pad;
    VALUE arr;

    pad = RGST_PAD(self);
    arr = rb_ary_new();
    types = gst_pad_get_query_types(pad);
    
    while (types && *types) {
        rb_ary_push(arr, RGST_QUERY_TYPE_NEW((GstQueryType *)types));
        types++;
    }
    return arr;
}

/*
 *  Method: each_format { |aFormatObject| block } -> nil
 *
 *  Calls the block for each supported format from the pad, passing
 *  a reference to the Gst::Format object as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_pad_each_format(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_pad_get_formats(self));
}   
   
/*
 *  Method: each_event_mask { |anEventMaskObject| block } -> nil 
 *
 *  Calls the block for each event mask from the pad, passing
 *  a reference to the Gst::EventMask object as parameter.
 *
 *  Always returns nil.
 */ 
static VALUE rb_gst_pad_each_event_mask(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_pad_get_event_masks(self));
}   
    
/*
 *  Method: each_query_type { |aQueryTypeObject| block } -> nil
 *
 *  Calls the block for each supported query from the pad, passing
 *  a reference to the Gst::QueryType object as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_pad_each_query_type(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_pad_get_query_types(self));
}   
 
/*
 *  Method: link(other_pad) -> aBoolean
 *
 *  Links the current pad (source) to an other pad (sink).
 *  Returns true if the pads have been linked, false otherwise.
 */  
static VALUE rb_gst_pad_link(self, other_pad)
    VALUE self, other_pad;
{
    GstPad *srcpad  = RGST_PAD(self);
    GstPad *sinkpad = RGST_PAD(other_pad);
    return CBOOL2RVAL(gst_pad_link(srcpad, sinkpad)); 
}
 
/*
 *  Method: unlink(other_pad) -> self
 *
 *  Unlinks the current pad (source) from an other pad (sink).
 */  
static VALUE rb_gst_pad_unlink(self, other_pad)
    VALUE self, other_pad;
{
    GstPad *srcpad  = RGST_PAD(self);
    GstPad *sinkpad = RGST_PAD(other_pad);
    gst_pad_unlink(srcpad, sinkpad);
    return self; 
}

/*
 *  Constant: FLAG_DISABLED 
 *  The pad is disabled. 
 */
static VALUE constFlagDisabled = INT2FIX(GST_PAD_DISABLED);

/*
 *  Constant: FLAG_NEGOTIATING 
 *  The pad is currently negotiating with another pad. 
 */
static VALUE constFlagNegotiating = INT2FIX(GST_PAD_NEGOTIATING);

/*
 *  Method: disabled? -> aBoolean
 *  Checks if the Gst::Pad::FLAG_DISABLED flag is set on the object.
 */
static VALUE rb_gst_pad_is_disabled(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagDisabled);
}

/*
 *  Method: negotiating? -> aBoolean
 *  Checks if the Gst::Pad::FLAG_NEGOTIATING flag is set on the object.
 */
static VALUE rb_gst_pad_is_negotiating(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagNegotiating);
}

/*
 *  Method: query(aQueryType, aFormat = Gst::Format::DEFAULT) -> aFixnum
 *
 *  Performs a query on the pad.
 *
 *  Meaningful query types are:
 *      * Gst::QueryType::TOTAL;
 *      * Gst::QueryType::POSITION;
 *      * Gst::QueryType::LATENCY;
 *      * Gst::QueryType::JITTER;
 *      * Gst::QueryType::START;
 *      * Gst::QueryType::SEGMENT_END;
 *      * Gst::QueryType::RATE.
 *
 *  Meaningful formats are:
 *      * Gst::Format::DEFAULT;
 *      * Gst::Format::BYTES;
 *      * Gst::Format::TIME;
 *      * Gst::Format::BUFFERS;
 *      * Gst::Format::PERCENT;
 *      * Gst::Format::UNITS.
 *
 *  Returns a fixnum, or nil if the query could not be performed.
 */
static VALUE rb_gst_pad_query(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE query_type, format;
    GstFormat gstformat;
    gint64 value;

    rb_scan_args(argc, argv, "11", &query_type, &format);
    gstformat = NIL_P(format) ? GST_FORMAT_DEFAULT : FIX2INT(format);

    if (gst_pad_query(RGST_PAD(self), FIX2INT(query_type),
                      &gstformat, &value))
    {
        format = INT2FIX(&gstformat);
        return ULL2NUM(value);
    }
    return Qnil;
}

/*
 *  Method: send_event(anEvent) -> aBoolean
 *
 *  Sends an event to the pad, through a Gst::Event object. 
 *
 *  Returns true if the request event was successfully handled, false
 *  otherwise.
 */
static VALUE rb_gst_pad_send_event(self, event)
    VALUE self, event;
{
    return CBOOL2RVAL(gst_pad_send_event(RGST_PAD(self),
                                         RGST_EVENT(event)));
}

void Init_gst_pad(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_PAD, "Pad", mGst);
    
    rb_define_method(c, "direction", rb_gst_pad_get_direction, 0);
    
    rb_define_const(c, "PRESENCE_ALWAYS",    constPresenceAlways);
    rb_define_const(c, "PRESENCE_SOMETIMES", constPresenceSometimes);
    rb_define_const(c, "PRESENCE_REQUEST",   constPresenceRequest);
    
    rb_define_const(c, "DIRECTION_UNKNOWN",  constDirectionUnknown);
    rb_define_const(c, "DIRECTION_SRC",      constDirectionSrc);
    rb_define_const(c, "DIRECTION_SINK",     constDirectionSink);

    rb_define_method(c, "name", rb_gst_pad_get_name, 0);

    rb_define_method(c, "pad_template", rb_gst_pad_get_padtemplate, 0);

    rb_define_method(c, "provides_formats?",     rb_gst_pad_provides_formats,     0);
    rb_define_method(c, "provides_event_masks?", rb_gst_pad_provides_event_masks, 0);
    rb_define_method(c, "provides_query_types?", rb_gst_pad_provides_query_types, 0);

    rb_define_method(c, "formats",     rb_gst_pad_get_formats,     0);
    rb_define_method(c, "event_masks", rb_gst_pad_get_event_masks, 0);
    rb_define_method(c, "query_types", rb_gst_pad_get_query_types, 0);

    rb_define_method(c, "each_format",     rb_gst_pad_each_format,     0);
    rb_define_method(c, "each_event_mask", rb_gst_pad_each_event_mask, 0);
    rb_define_method(c, "each_query_type", rb_gst_pad_each_query_type, 0);

    rb_define_method(c, "link",   rb_gst_pad_link, 1);
    rb_define_method(c, "unlink", rb_gst_pad_unlink, 1);

    /*
     *  Flags:
     */

    rb_define_const(c, "FLAG_DISABLED",    constFlagDisabled); 
    rb_define_const(c, "FLAG_NEGOTIATING", constFlagNegotiating); 

    rb_define_method(c, "disabled?",    rb_gst_pad_is_disabled,    0);
    rb_define_method(c, "negotiating?", rb_gst_pad_is_negotiating, 0);

    rb_define_method(c, "query",      rb_gst_pad_query,      -1);
    rb_define_method(c, "send_event", rb_gst_pad_send_event,  1);
}

