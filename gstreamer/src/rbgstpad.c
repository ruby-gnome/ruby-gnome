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

/* Class: Gst::Pad
 * The link between Gst::Element objects.
 */

/* Method: name
 * Returns: the pad's name.
 */

static VALUE
rb_gst_pad_get_name (VALUE self)
{
	GstPad *pad = RGST_PAD (self);
	return CSTR2RVAL (gst_pad_get_name (pad));
}

/* Method: direction
 * Returns: the pad's direction (see Gst::Pad::Direction).
 */

static VALUE
rb_gst_pad_get_direction (VALUE self)
{
	GstPad *pad = RGST_PAD (self);
	return GENUM2RVAL (GST_PAD_DIRECTION(pad), GST_TYPE_PAD_DIRECTION);
}

/* Method: pad_template
 * Returns: the Gst::PadTemplate object of this pad.
 */
static VALUE
rb_gst_pad_get_padtemplate (VALUE self)
{
	GstPadTemplate *templ = gst_pad_get_pad_template (RGST_PAD (self));
	if (templ != NULL) {
		gst_object_ref (GST_OBJECT (templ));
		return RGST_PAD_TEMPLATE_NEW (templ);
	}   
	return Qnil;
}

/*
 * Method: provides_formats?
 *
 * Checks if the pad supports formats not handled by
 * the default format dispatcher.
 *
 * Returns: true if supported, false otherwise.
 */
static VALUE
rb_gst_pad_provides_formats (VALUE self)
{
	GstPad *pad = RGST_PAD (self);
	return CBOOL2RVAL (GST_RPAD_FORMATSFUNC (pad) != gst_pad_get_formats_default);
}

/*
 * Method: provides_event_masks?
 *
 * Checks if the pad supports event masks not handled by
 * the default event masks dispatcher.
 *
 * Returns: true if supported, false otherwise.
 */
static VALUE
rb_gst_pad_provides_event_masks (VALUE self)
{
	GstPad *pad = RGST_PAD (self);
	return CBOOL2RVAL (GST_RPAD_EVENTMASKFUNC (pad) != gst_pad_get_event_masks_default);
}

/*
 * Method: provides_query_types?
 *
 * Checks if the pad supports query types not handled by
 * the default query types dispatcher.
 *
 * Returns: true if supported, false otherwise.
 */
static VALUE
rb_gst_pad_provides_query_types (VALUE self)
{
	GstPad *pad = RGST_PAD (self);
	return CBOOL2RVAL (GST_RPAD_QUERYTYPEFUNC (pad) != gst_pad_get_query_types_default);
}

/*
 * Method: formats
 *
 * Gets the list of supported formats from the pad.
 *
 * Returns: an array of Gst::Format objects.
 */
static VALUE
rb_gst_pad_get_formats (VALUE self)
{
	const GstFormat *formats; 
	VALUE arr;

	arr = rb_ary_new ();
	formats = gst_pad_get_formats (RGST_PAD (self));
	
	while (formats && *formats) {
		rb_ary_push (arr, RGST_FORMAT_NEW (formats));
		formats++;
	}
	return arr;
}

/*
 * Method: event_masks
 *
 * Gets the list of event masks from the pad.
 *
 * Returns: an array of Gst::EventMask objects.
 */
static VALUE
rb_gst_pad_get_event_masks (VALUE self) 
{
	const GstEventMask *masks; 
	VALUE arr;

	arr = rb_ary_new ();
	masks = gst_pad_get_event_masks (RGST_PAD (self));
	
	while (masks && masks->type) {
		rb_ary_push (arr, RGST_EVENT_MASK_NEW (masks));
		masks++;
	}
	return arr;
}

/*
 * Method: query_types
 *
 * Gets a list of supported queries that can be performed on the pad.
 *
 * Returns: an array of Gst::QueryType objects.
 */
static VALUE
rb_gst_pad_get_query_types (VALUE self)
{
	const GstQueryType *types; 
	VALUE arr;

	arr = rb_ary_new ();
	types = gst_pad_get_query_types (RGST_PAD (self));
	
	while (types && *types) {
		rb_ary_push (arr, RGST_QUERY_TYPE_NEW ((GstQueryType *)types));
		types++;
	}
	return arr;
}

/*
 * Method: each_format { |format| ... }
 *
 * Calls the block for each supported format from the pad, passing
 * a reference to the Gst::Format object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_pad_each_format (VALUE self)
{
	return rb_ary_yield (rb_gst_pad_get_formats (self));
}   
   
/*
 * Method: each_event_mask { |event_mask| ... }
 *
 * Calls the block for each event mask from the pad, passing
 * a reference to the Gst::EventMask object as parameter.
 *
 * Returns: always nil.
 */ 
static VALUE
rb_gst_pad_each_event_mask (VALUE self)
{
	return rb_ary_yield (rb_gst_pad_get_event_masks (self));
}   
	
/*
 * Method: each_query_type { |query_type| ... } 
 *
 * Calls the block for each supported query from the pad, passing
 * a reference to the Gst::QueryType object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_pad_each_query_type (VALUE self)
{
	return rb_ary_yield (rb_gst_pad_get_query_types (self));
}   
 
/*
 * Method: link(other_pad)
 * other_pad: a Gst::Pad.
 *
 * Links the current pad (source) to an other pad (sink).
 *
 * Returns: true if the pads have been linked, false otherwise.
 */  
static VALUE
rb_gst_pad_link (VALUE self, VALUE other_pad)
{
	GstPad *srcpad = RGST_PAD (self);
	GstPad *sinkpad = RGST_PAD (other_pad);
	return CBOOL2RVAL (gst_pad_link (srcpad, sinkpad)); 
}
 
/*
 * Method: unlink(other_pad)
 * other_pad: a Gst::Pad.
 *
 * Unlinks the current pad (source) from an other pad (sink).
 *
 * Returns: self.
 */  
static VALUE
rb_gst_pad_unlink(VALUE self, VALUE other_pad)
{
	GstPad *srcpad = RGST_PAD (self);
	GstPad *sinkpad = RGST_PAD (other_pad);
	gst_pad_unlink (srcpad, sinkpad);
	return self; 
}

/*
 * Method: disabled?
 *
 * Checks if the Gst::Pad::FLAG_DISABLED flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_pad_is_disabled (VALUE self)
{
	return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_PAD (self), GST_PAD_DISABLED));
}

/*
 * Method: negotiating?
 *
 * Checks if the Gst::Pad::FLAG_NEGOTIATING flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_pad_is_negotiating (VALUE self)
{
	return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_PAD (self), GST_PAD_NEGOTIATING));
}

/*
 * Method: query(query_type, format=Gst::Format::DEFAULT)
 * query_type: a query type (see Gst::QueryType::Type).
 * format: a format (see Gst::Format::Type).
 *
 * Performs a query on the pad.
 *
 * Returns: the query result value, or nil if the query could not be performed.
 */
static VALUE
rb_gst_pad_query (int argc, VALUE *argv, VALUE self)
{
	VALUE query_type, format;
	GstFormat gstformat;
	gint64 value;

	rb_scan_args (argc, argv, "11", &query_type, &format);
	gstformat = NIL_P (format) ? GST_FORMAT_DEFAULT : RVAL2GENUM (format, GST_TYPE_FORMAT);

	if (gst_pad_query (RGST_PAD(self),
			   RVAL2GENUM (query_type, GST_TYPE_QUERY_TYPE),
			   &gstformat,
			   &value)) {
		format = GENUM2RVAL (gstformat, GST_TYPE_FORMAT);
		return ULL2NUM (value);
	}
	return Qnil;
}

/*
 * Method: send_event(event)
 * event: a Gst::Event.
 *
 * Sends an event to the pad, through a Gst::Event object. 
 *
 * Returns: true if the request event was successfully handled, false
 * otherwise.
 */
static VALUE
rb_gst_pad_send_event (VALUE self, VALUE event)
{
	return CBOOL2RVAL (gst_pad_send_event (RGST_PAD (self),
					       RGST_EVENT (event)));
}

/*
 * Method: caps
 *
 * Gets the capabilities of the pad element.
 *
 * Returns: a Gst::Caps object.
 */
static VALUE
rb_gst_pad_get_caps (VALUE self)
{
    GstCaps *caps = gst_pad_get_caps (RGST_PAD (self));
    return caps != NULL
        ? RGST_CAPS_NEW (caps)
        : Qnil;    
}

/*
 * Method: each_caps { |caps| ... }
 *
 * Calls the block for each capability of the pad, 
 * passing a reference to the Gst::Caps object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_pad_each_caps (VALUE self)
{
    VALUE ary = rb_gst_pad_get_caps (self);
	if (!NIL_P (ary))
        rb_ary_yield (ary);
    return Qnil;
}

void
Init_gst_pad (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_PAD, "Pad", mGst);
	
	rb_define_method (c, "direction", rb_gst_pad_get_direction, 0);
	rb_define_method (c, "name", rb_gst_pad_get_name, 0);
	rb_define_method (c, "pad_template", rb_gst_pad_get_padtemplate, 0);
	rb_define_method (c, "provides_formats?", rb_gst_pad_provides_formats, 0);
	rb_define_method (c, "provides_event_masks?", rb_gst_pad_provides_event_masks, 0);
	rb_define_method (c, "provides_query_types?", rb_gst_pad_provides_query_types, 0);
	rb_define_method (c, "formats",	 rb_gst_pad_get_formats, 0);
	rb_define_method (c, "event_masks", rb_gst_pad_get_event_masks, 0);
	rb_define_method (c, "query_types", rb_gst_pad_get_query_types, 0);
	rb_define_method (c, "each_format", rb_gst_pad_each_format, 0);
	rb_define_method (c, "each_event_mask", rb_gst_pad_each_event_mask, 0);
	rb_define_method (c, "each_query_type", rb_gst_pad_each_query_type, 0);
	rb_define_method (c, "link", rb_gst_pad_link, 1);
	rb_define_method (c, "unlink", rb_gst_pad_unlink, 1);
	rb_define_method (c, "disabled?", rb_gst_pad_is_disabled, 0);
	rb_define_method (c, "negotiating?", rb_gst_pad_is_negotiating, 0);
	rb_define_method (c, "query", rb_gst_pad_query,	-1);
	rb_define_method (c, "send_event", rb_gst_pad_send_event, 1);
	rb_define_method (c, "caps", rb_gst_pad_get_caps, 0);
	rb_define_method (c, "each_caps", rb_gst_pad_each_caps ,0);

	G_DEF_CLASS (GST_TYPE_PAD_LINK_RETURN, "LinkReturn", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_LINK_RETURN, "GST_PAD_");
	G_DEF_CLASS (GST_TYPE_PAD_DIRECTION, "Direction", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_DIRECTION, "GST_PAD_");
	G_DEF_CLASS (GST_TYPE_PAD_FLAGS, "Flags", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_FLAGS, "GST_PAD_");
	G_DEF_CLASS (GST_TYPE_PAD_PRESENCE, "Presence", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_PRESENCE, "GST_PAD_");
}
