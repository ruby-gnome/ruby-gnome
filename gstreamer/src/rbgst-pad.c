/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 * Copyright (C) 2006, 2008 Sjoerd Simons <sjoerd@luon.net>
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

static VALUE cGstPad;

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
	return CBOOL2RVAL (GST_PAD_QUERYTYPEFUNC (pad) != gst_pad_get_query_types_default);
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
 * Returns: a Gst::Pad::LinkReturn
 */  
static VALUE
rb_gst_pad_link (VALUE self, VALUE other_pad)
{
    GstPad *srcpad = RGST_PAD (self);
    GstPad *sinkpad;

    if (!rb_obj_is_kind_of(other_pad, cGstPad)) {
        rb_raise(rb_eTypeError, "Not a Gst::Pad");
    }

    sinkpad = RGST_PAD (other_pad);
    return GENUM2RVAL (gst_pad_link (srcpad, sinkpad),
        GST_TYPE_PAD_LINK_RETURN);
}

/*
 * Method: >> other_pad
 * other_pad: a Gst::Pad.
 *
 * Links the current pad (source) to an other pad (sink).
 *
 * Returns: the destination pad, or nil if the link failed.
 */
static VALUE
rb_gst_pad_link2 (VALUE self, VALUE other_pad)  {
    GstPad *srcpad = RGST_PAD (self);
    GstPad *sinkpad;

    if (!rb_obj_is_kind_of(other_pad, cGstPad)) {
        rb_raise(rb_eTypeError, "Not a Gst::Pad");
    }

    sinkpad = RGST_PAD (other_pad);
    return gst_pad_link(srcpad, sinkpad) == GST_PAD_LINK_OK ? other_pad :  Qnil;
}


/*
 * Method: unlink(other_pad)
 * other_pad: a Gst::Pad.
 *
 * Unlinks the current pad (source) from an other pad (sink).
 *
 * Returns: true if the pads were unlinked and false if the pads weren't linked.
 */  
static VALUE
rb_gst_pad_unlink(VALUE self, VALUE other_pad)
{
    GstPad *srcpad = RGST_PAD (self);
    GstPad *sinkpad;

    if (!rb_obj_is_kind_of(other_pad, cGstPad)) {
        rb_raise(rb_eTypeError, "Not a Gst::Pad");
    }
    sinkpad = RGST_PAD (other_pad);

    return CBOOL2RVAL(gst_pad_unlink (srcpad, sinkpad));
}

/*
 * Method: query(query)
 * query: a query (see Gst::Query).
 *
 * Performs a query on the pad.
 *
 * Returns: the query result value, or nil if the query could not be performed.
 */
/*
 * Method: query
 */
static VALUE
rb_gst_pad_query(VALUE self, VALUE query) {
    return CBOOL2RVAL(gst_pad_query(RGST_PAD(self), RVAL2GOBJ(query)));
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
    VALUE ret;
    GstCaps *caps = gst_pad_get_caps (RGST_PAD (self));

    if (caps == NULL) {
        return Qnil;
    }

    ret = GST_CAPS2RVAL(caps);

    /* get_caps gave us a ref we don't need, so drop it */
    gst_caps_unref (caps);

    return ret;
}

/*
 * Method: negotiatedcaps
 *
 * Gets the capabilities of the pad element.
 *
 * Returns: a Gst::Caps object.
 */
static VALUE
rb_gst_pad_get_negotiated_caps (VALUE self)
{
    VALUE ret;
    GstCaps *caps = gst_pad_get_negotiated_caps (RGST_PAD (self));

    if (caps == NULL) {
        return Qnil;
    }

    ret = GST_CAPS2RVAL(caps);

    /* get_negotiated_caps gave us a ref we don't need, so drop it */
    gst_caps_unref (caps);

    return ret;
}

/*
 * Method: set_caps
 *
 * Sets the capabilities of the pad element.
 *
 * Returns: true if the setting succeeded
 */
static VALUE
rb_gst_pad_set_caps(VALUE self, VALUE caps) {
    return CBOOL2RVAL(gst_pad_set_caps(RGST_PAD(self), RGST_CAPS(caps)));
}

/*
 * Method: peer
 *
 * Get the peer of a pad
 *
 * Returns: Returns the peer pad or Qnil if there is none
 */
static VALUE
rb_gst_pad_get_peer(VALUE self) {
    VALUE ret;
    GstPad *pad = gst_pad_get_peer (RGST_PAD (self));

    if (pad == NULL) {
        return Qnil;
    }

    ret = GOBJ2RVAL(pad);
    /* gst_pad_get_peer gave us a ref, which we don't need anymore so drop it */
    g_object_unref (pad);
    return ret;
}


void
Init_gst_pad (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_PAD, "Pad", mGst);

	cGstPad = c;

	rb_define_method (c, "direction", rb_gst_pad_get_direction, 0);
	rb_define_method (c, "name", rb_gst_pad_get_name, 0);
	rb_define_method (c, "pad_template", rb_gst_pad_get_padtemplate, 0);
	rb_define_method (c, "provides_query_types?", rb_gst_pad_provides_query_types, 0);
	rb_define_method (c, "query_types", rb_gst_pad_get_query_types, 0);
	rb_define_method (c, "each_query_type", rb_gst_pad_each_query_type, 0);
	rb_define_method (c, "link", rb_gst_pad_link, 1);
	rb_define_method (c, ">>", rb_gst_pad_link2, 1);
	rb_define_method (c, "unlink", rb_gst_pad_unlink, 1);
	rb_define_method (c, "query", rb_gst_pad_query, 1);
	rb_define_method (c, "send_event", rb_gst_pad_send_event, 1);
	rb_define_method (c, "caps", rb_gst_pad_get_caps, 0);
	rb_define_method (c, "negotiated_caps", rb_gst_pad_get_negotiated_caps, 0);
	rb_define_method (c, "set_caps", rb_gst_pad_set_caps, 1);
	rb_define_alias(c, "caps=", "set_caps");

	rb_define_method (c, "peer", rb_gst_pad_get_peer, 0);


	G_DEF_CLASS (GST_TYPE_PAD_LINK_RETURN, "LinkReturn", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_LINK_RETURN, "GST_PAD_");
	G_DEF_CLASS (GST_TYPE_PAD_DIRECTION, "Direction", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_DIRECTION, "GST_PAD_");
	G_DEF_CLASS (GST_TYPE_PAD_FLAGS, "Flags", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_FLAGS, "GST_PAD_");
	G_DEF_CLASS (GST_TYPE_PAD_PRESENCE, "Presence", c);
	G_DEF_CONSTANTS (c, GST_TYPE_PAD_PRESENCE, "GST_PAD_");
}
