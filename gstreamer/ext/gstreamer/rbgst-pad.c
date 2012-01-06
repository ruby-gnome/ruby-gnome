/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2008  Ruby-GNOME2 Project Team
 *  Copyright (C) 2006, 2008 Sjoerd Simons <sjoerd@luon.net>
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

#define RG_TARGET_NAMESPACE cPad
#define SELF(self) RVAL2GST_PAD(self)

static VALUE RG_TARGET_NAMESPACE;

/* Class: Gst::Pad
 * The link between Gst::Element objects.
 */

/* Method: name
 * Returns: the pad's name.
 */

static VALUE
rg_name (VALUE self)
{
    GstPad *pad = RGST_PAD (self);
    return CSTR2RVAL (gst_pad_get_name (pad));
}

/* Method: direction
 * Returns: the pad's direction (see Gst::Pad::Direction).
 */

static VALUE
rg_direction (VALUE self)
{
    GstPad *pad = RGST_PAD (self);
    return GENUM2RVAL (GST_PAD_DIRECTION(pad), GST_TYPE_PAD_DIRECTION);
}

/* Method: pad_template
 * Returns: the Gst::PadTemplate object of this pad.
 */
static VALUE
rg_pad_template(VALUE self)
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
rg_provides_query_types_p (VALUE self)
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
rg_query_types (VALUE self)
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
rg_each_query_type (VALUE self)
{
    return rb_ary_yield (rg_query_types (self));
}

static VALUE
rg_linked_p(VALUE self)
{
    return CBOOL2RVAL(gst_pad_is_linked(SELF(self)));
}

static GstPadLinkReturn
rb_gst_pad_link_raw(VALUE self, VALUE other_pad)
{
    GstPad *src_pad;
    GstPad *sink_pad;

    src_pad = SELF(self);
    if (!RVAL2CBOOL(rb_obj_is_kind_of(other_pad, RG_TARGET_NAMESPACE))) {
        rb_raise(rb_eTypeError, "Not a Gst::Pad");
    }

    sink_pad = RVAL2GST_PAD(other_pad);
    return gst_pad_link(src_pad, sink_pad);
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
rg_link(VALUE self, VALUE other_pad)
{
    return GENUM2RVAL(rb_gst_pad_link_raw(self, other_pad),
                      GST_TYPE_PAD_LINK_RETURN);
}

/*
 * Method: >> other_pad
 * other_pad: a Gst::Pad.
 *
 * Links the current pad (source) to an other pad (sink).
 *
 * Returns: the destination pad, or nil if the link failed.
 *          (I can't understand why the return value is
 *          useful. Because I can't write like the
 *          following:
 *            element1["src"] >> element2["sink"] >> element2["src"] >> element3["sink"]
 *          I need to write like the following:
 *            element1["src"] >> element2["sink"]
 *            element2["src"] >> element3["sink"]
 *          Return values aren't used in this case.)
 */
static VALUE
rg_operator_link_shift(VALUE self, VALUE other_pad)
{
    if (rb_gst_pad_link_raw(self, other_pad) == GST_PAD_LINK_OK)
        return other_pad;
    else
        return Qnil;
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
rg_unlink(VALUE self, VALUE other_pad)
{
    GstPad *src_pad;
    GstPad *sink_pad;

    src_pad = SELF(self);
    if (!CBOOL2RVAL(rb_obj_is_kind_of(other_pad, RG_TARGET_NAMESPACE))) {
        rb_raise(rb_eTypeError, "Not a Gst::Pad");
    }
    sink_pad = RVAL2GST_PAD(other_pad);

    return CBOOL2RVAL(gst_pad_unlink(src_pad, sink_pad));
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
rg_query(VALUE self, VALUE query)
{
    return CBOOL2RVAL(gst_pad_query(SELF(self), RVAL2GST_QUERY(query)));
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
rg_send_event (VALUE self, VALUE event)
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
rg_caps(VALUE self)
{
    return GST_CAPS2RVAL_UNREF(gst_pad_get_caps(SELF(self)));
}

/*
 * Method: negotiated_caps
 *
 * Gets the capabilities of the pad element.
 *
 * Returns: a Gst::Caps object.
 */
static VALUE
rg_negotiated_caps(VALUE self)
{
    return GST_CAPS2RVAL_UNREF(gst_pad_get_negotiated_caps(SELF(self)));
}

/*
 * Method: set_caps
 *
 * Sets the capabilities of the pad element.
 *
 * Returns: true if the setting succeeded
 */
static VALUE
rg_set_caps(VALUE self, VALUE caps)
{
    return CBOOL2RVAL(gst_pad_set_caps(SELF(self), RVAL2GST_CAPS(caps)));
}

/*
 * Method: set_active
 *
 * Sets the active state of the pad.
 *
 * Returns: true if the setting succeeded
 */
static VALUE
rg_set_active(VALUE self, VALUE active)
{
    return CBOOL2RVAL(gst_pad_set_active(SELF(self), RVAL2CBOOL(active)));
}

/*
 * Method: peer
 *
 * Get the peer of a pad
 *
 * Returns: Returns the peer pad or Qnil if there is none
 */
static VALUE
rg_peer(VALUE self)
{
    return GST_PAD2RVAL_UNREF(gst_pad_get_peer(SELF(self)));
}

void
Init_gst_pad (VALUE mGst)
{
    RG_TARGET_NAMESPACE = G_DEF_CLASS (GST_TYPE_PAD, "Pad", mGst);

    RG_DEF_METHOD(direction, 0);
    RG_DEF_METHOD(name, 0);
    RG_DEF_METHOD(pad_template, 0);
    RG_DEF_METHOD_P(provides_query_types, 0);
    RG_DEF_METHOD(query_types, 0);
    RG_DEF_METHOD(each_query_type, 0);
    RG_DEF_METHOD_P(linked, 0);
    RG_DEF_METHOD(link, 1);
    RG_DEF_METHOD_OPERATOR(">>", link_shift, 1);
    RG_DEF_METHOD(unlink, 1);
    RG_DEF_METHOD(query, 1);
    RG_DEF_METHOD(send_event, 1);
    RG_DEF_METHOD(caps, 0);
    RG_DEF_METHOD(negotiated_caps, 0);
    RG_DEF_METHOD(set_caps, 1);
    RG_DEF_METHOD(set_active, 1);
    RG_DEF_METHOD(peer, 0);

    G_DEF_SETTERS(RG_TARGET_NAMESPACE);

    G_DEF_CLASS(GST_TYPE_PAD_LINK_RETURN, "LinkReturn", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_PAD_LINK_RETURN, "GST_PAD_");
    G_DEF_CLASS(GST_TYPE_PAD_DIRECTION, "Direction", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_PAD_DIRECTION, "GST_PAD_");
    G_DEF_CLASS(GST_TYPE_PAD_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_PAD_FLAGS, "GST_PAD_");
    G_DEF_CLASS(GST_TYPE_PAD_PRESENCE, "Presence", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_PAD_PRESENCE, "GST_PAD_");
}
