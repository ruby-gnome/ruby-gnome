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

/* Class: Gst::Element
 * Base class for all pipeline elements.
 */

/*
 * Method: set_state(state)
 * state: the state you want to set (see Gst::Element::State).
 *
 * Sets the state of the element. 
 *
 * This method will try to set the requested state by going through all 
 * the intermediary states and calling the class's state change function 
 * for each.
 *
 * Returns: a code (see Gst::Element::StateReturn).
 */
static VALUE
rb_gst_element_set_state (VALUE self, VALUE value)
{
    const int state = RVAL2GENUM (value, GST_TYPE_ELEMENT_STATE);

    return GENUM2RVAL (gst_element_set_state (RGST_ELEMENT (self), state),
                       GST_TYPE_ELEMENT_STATE_RETURN);
}

/* Method: state
 * Returns: the state of the element (see Gst::Element::State).
 */
static VALUE
rb_gst_element_get_state (VALUE self)
{
    return GENUM2RVAL (gst_element_get_state (RGST_ELEMENT (self)),
                       GST_TYPE_ELEMENT_STATE);
}

/*
 * Method: stop
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_NULL.
 *
 * Returns: a code (see Gst::Element::StateReturn).
 */
static VALUE
rb_gst_element_stop (VALUE self)
{
    return
        GENUM2RVAL (gst_element_set_state (RGST_ELEMENT (self), GST_STATE_NULL),
                    GST_TYPE_ELEMENT_STATE_RETURN);
}

/*
 * Method: ready
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_READY.
 *
 * Returns: a code (see Gst::Element::StateReturn).
 */
static VALUE
rb_gst_element_ready (VALUE self)
{
    return
        GENUM2RVAL (gst_element_set_state
                    (RGST_ELEMENT (self), GST_STATE_READY),
                    GST_TYPE_ELEMENT_STATE_RETURN);
}

/*
 * Method: pause
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_PAUSED.
 *
 * Returns: a code (see Gst::Element::StateReturn).
 */
static VALUE
rb_gst_element_pause (VALUE self)
{
    return
        GENUM2RVAL (gst_element_set_state
                    (RGST_ELEMENT (self), GST_STATE_PAUSED),
                    GST_TYPE_ELEMENT_STATE_RETURN);
}

/*
 * Method: play
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_PLAYING.
 *
 * Returns: a code (see Gst::Element::StateReturn).
 */
static VALUE
rb_gst_element_play (VALUE self)
{
    return
        GENUM2RVAL (gst_element_set_state
                    (RGST_ELEMENT (self), GST_STATE_PLAYING),
                    GST_TYPE_ELEMENT_STATE_RETURN);
}

/* Method: stopped?
 * Returns: true if the current state is set to Gst::Element::STATE_NULL,
 * false otherwise.
 */
static VALUE
rb_gst_element_is_stopped (VALUE self)
{
    return CBOOL2RVAL (gst_element_get_state (RGST_ELEMENT (self)) ==
                       GST_STATE_NULL);
}

/* Method: ready?
 * Returns: true if the current state equals Gst::Element::STATE_READY,
 * false otherwise.
 */
static VALUE
rb_gst_element_is_ready (VALUE self)
{
    return CBOOL2RVAL (gst_element_get_state (RGST_ELEMENT (self)) ==
                       GST_STATE_READY);
}

/* Method: paused?
 * Returns: true if the current state equals Gst::Element::STATE_PAUSED,
 * false otherwise.
 */
static VALUE
rb_gst_element_is_paused (VALUE self)
{
    return CBOOL2RVAL (gst_element_get_state (RGST_ELEMENT (self)) ==
                       GST_STATE_PAUSED);
}

/* Method: playing?
 * Returns: true if the current state equals Gst::Element::STATE_PLAYING,
 * false otherwise.
 */
static VALUE
rb_gst_element_is_playing (VALUE self)
{
    return CBOOL2RVAL (gst_element_get_state (RGST_ELEMENT (self)) ==
                       GST_STATE_PLAYING);
}

/*
 * Method: wait_state_change
 *
 * Waits and blocks until the element changed its state.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_wait_state_change (VALUE self)
{
    gst_element_wait_state_change (RGST_ELEMENT (self));
    return self;
}

static int
check_property (GstElement * element, const gchar * name, GValue * value)
{
    GParamSpec *pspec;
    GObject *object;
    int type;

    g_assert (element != NULL);
    g_assert (name != NULL);
    g_assert (value != NULL);

    object = (GObject *) element;

    pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (object), name);

    if (pspec == NULL)
        rb_raise (rb_eArgError, "This element has no property named %s", name);

    type = G_PARAM_SPEC_VALUE_TYPE (pspec);
    g_value_init (value, type);
    return type;
}

/*
 * Method: set_property(name, obj)
 * name: a property name.
 * obj: an object.
 *
 * Sets the value anObject for a named property.
 *
 * If the element uses threadsafe properties, the property will 
 * be put on the async queue.
 *
 * Note that properties may be set through accessors methods, 
 * which are generated on-the-fly according to the Gst::Element type.
 *
 * 	e = Gst::ElementFactory.make("filesrc")
 * 	e.set_property("location", "a_file.ogg")
 * 	
 * 	# This does exactly the same
 * 	e.location = "a_file.ogg"
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_set_property (VALUE self, VALUE name, VALUE value)
{
    GstElement *element;
    GValue gvalue = { 0, };
    int type;

    element = RGST_ELEMENT (self);

    type = check_property (element, RVAL2CSTR (name), &gvalue);
    rbgobj_rvalue_to_gvalue (value, &gvalue);
    gst_element_set_property (element, RVAL2CSTR (name), &gvalue);
    return self;
}

/*
 * Method: get_property(name)
 * name: a property name.
 *
 * Returns the value associated with the named property,
 * or nil if the property has no value.
 *
 * Note that properties may be read through accessors methods, 
 * which are generated on-the-fly according to the Gst::Element type.
 *
 *	e = Gst::ElementFactory.make("filesrc")
 * 	# ...
 *	
 * 	puts "Location is " + e.get_property("location")
 *	# This does exactly the same
 *	puts "Location is " + e.location
 *
 * Returns: the object associated with the named property.
 */
static VALUE
rb_gst_element_get_property (VALUE self, VALUE name)
{
    GstElement *element;
    GValue gvalue = { 0, };
    const gchar *gname;
    VALUE ret;

    element = RGST_ELEMENT (self);
    gname = RVAL2CSTR (name);

    check_property (element, gname, &gvalue);
    gst_element_get_property (element, RVAL2CSTR (name), &gvalue);
    /*
     * FIXME: returns nil if property doesn't exist yet 
     */
    ret = GVAL2RVAL (&gvalue);
    g_value_unset (&gvalue);
    return ret;
}

/*
 * Method: each_property { |name, descr, obj| ... }
 *
 * Calls the block for each property of the element, passing
 * name, description and value of the property as parameters.
 *
 * Returns: always nil. 
 */
static VALUE
rb_gst_element_each_property (VALUE self)
{
    GParamSpec **pspec;
    GObject *object;
    guint count;
    int i;

    if (rb_block_given_p () == Qfalse)
        rb_raise (rb_eArgError, "No block given");

    object = (GObject *) RGST_ELEMENT (self);
    pspec = g_object_class_list_properties (G_OBJECT_GET_CLASS (object),
                                            &count);

    for (i = 0; i < count; i++) {
        VALUE name, descr, val, arr;

        name = CSTR2RVAL (g_param_spec_get_name (pspec[i]));
        descr = CSTR2RVAL (g_param_spec_get_blurb (pspec[i]));
        val = rb_gst_element_get_property (self, name);

        arr = rb_ary_new ();
        rb_ary_push (arr, name);
        rb_ary_push (arr, descr);
        rb_ary_push (arr, val);
        rb_yield (arr);
    }
    return Qnil;
}

/*
 * Method: eos
 *
 * Performs the actions needed to bring the element in the EOS 
 * (end of stream) state. 
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_set_eos (VALUE self)
{
    gst_element_set_eos (RGST_ELEMENT (self));
    return self;
}

/*
 * Method: sched_yield
 *
 * Requests a yield operation for the element. The scheduler 
 * will typically give control to another element.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_sched_yield (VALUE self)
{
    gst_element_yield (RGST_ELEMENT (self));
    return self;
}

/*
 * Method: sched_interrupt
 *
 * Requests the scheduler of this element to interrupt the 
 * execution of this element and scheduler another one.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_sched_interrupt (VALUE self)
{
    gst_element_interrupt (RGST_ELEMENT (self));
    return self;
}

/*
 * Method: link(element)
 * element: a Gst::Element object.
 *
 * Links this element (source) to the provided element (destination). 
 *
 * The method looks for existing pads and request pads that 
 * aren't linked yet. If multiple links are possible, only one 
 * is established.
 *
 * Returns: the destination element, or nil if the link failed.
 */
static VALUE
rb_gst_element_link (VALUE self, VALUE other_element)
{
    GstElement *element1, *element2;

    element1 = RGST_ELEMENT (self);
    element2 = RGST_ELEMENT (other_element);
    return gst_element_link (element1, element2) == TRUE ? other_element : Qnil;
}

/* Method: requires_clock?
 * Returns: true if the element requires a clock, false otherwise.
 */
static VALUE
rb_gst_element_requires_clock (VALUE self)
{
    return CBOOL2RVAL (gst_element_requires_clock (RGST_ELEMENT (self)));
}

/* Method: provides_clock?
 * Returns: true if the element provides a clock, false otherwise.
 */
static VALUE
rb_gst_element_provides_clock (VALUE self)
{
    return CBOOL2RVAL (gst_element_provides_clock (RGST_ELEMENT (self)));
}

/* Method: clock
 * Returns: the clock of the element (as a Gst::Clock object), or nil
 * if this element does not provide a clock.
 */
static VALUE
rb_gst_element_get_clock (VALUE self)
{
    GstClock *clock;

    clock = gst_element_get_clock (RGST_ELEMENT (self));
    return clock != NULL ? RGST_CLOCK_NEW (clock)
        : Qnil;
}

/*
 * Method: set_clock(clock)
 * clock: the Gst::Clock to set for the element.
 *
 * Sets the clock for the element.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_set_clock (VALUE self, VALUE clock)
{
    gst_element_set_clock (RGST_ELEMENT (self), RGST_CLOCK (clock));
    return self;
}

/*
 * Method: pads
 *
 * Gets a list of the pads associated with the element, in an array
 * of Gst::Pad objects.
 *
 * Returns: an Array of Gst::Pad objects.
 */
static VALUE
rb_gst_element_get_pads (VALUE self)
{
    const GList *list;
    VALUE arr;

    arr = rb_ary_new ();

    for (list = gst_element_get_pad_list (RGST_ELEMENT (self));
         list != NULL; list = g_list_next (list)) {
        GstPad *pad = GST_PAD (list->data);

        /*
         *  Increment the ref count of the Pad, since it was not
         *  created with gst_pad_new(), and it will be unref() from
         *  the GC.  
         */
        gst_object_ref (GST_OBJECT (pad));
        rb_ary_push (arr, RGST_PAD_NEW (pad));
    }
    return arr;
}

/*
 * Method: each_pad { |pad| ... }
 *
 * Calls the block for each pad associated with the element, passing a 
 * reference to the Gst::Pad as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_element_each_pad (VALUE self)
{
    return rb_ary_yield (rb_gst_element_get_pads (self));
}

/*
 * Method: get_pad(name)
 * name: the name of a pad.
 *
 * Retrieves a Gst::Pad object from the element by name.
 *
 * Returns: a Gst::Pad object, or nil if the pad cannot be found.
 */
static VALUE
rb_gst_element_get_pad (VALUE self, VALUE pad_name)
{
    GstPad *pad = gst_element_get_pad (RGST_ELEMENT (self),
                                       RVAL2CSTR (pad_name));

    return pad != NULL ? RGST_PAD_NEW (pad)
        : Qnil;
}

/*
 * Method: link_pads(element)
 * element: a Gst::Element.
 *
 * Links the "src" named pad of the current element to the
 * "sink" named pad of the destination element, returning
 * true on success.
 *
 * If you want to link specific named pads, you should use
 * the Gst::Pad#link method directly:
 *
 *	element1.link_pads(element2)
 *	
 * 	# This does the same
 *	element1.get_pad("src").link(element2.get_pad("sink"))
 *
 * Returns: true on success, false on failure.
 */
static VALUE
rb_gst_element_link_pads (VALUE self, VALUE other_element)
{
    return CBOOL2RVAL (gst_element_link_pads (RGST_ELEMENT (self),
                                              "src",
                                              RGST_ELEMENT (other_element),
                                              "sink"));
}

/*
 * Method: unlink_pads(element)
 * element: a Gst::Element.
 *
 * Unlinks the "src" named pad of the current element from the
 * "sink" named pad of the destination element.
 *
 * If you want to unlink specific named pads, you should use
 * the Gst::Pad#unlink method directly:
 *
 * 	element1.unlink_pads(element2)
 *	
 *	# This does the same
 *	element1.get_pad("src").unlink(element2.get_pad("sink"))
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_unlink_pads (self, other_element)
        VALUE self, other_element;
{
    gst_element_unlink_pads (RGST_ELEMENT (self),
                             "src", RGST_ELEMENT (other_element), "sink");
    return self;
}

/*
 * Method: add_ghost_pad(pad, pad_name=nil)
 * pad: a Gst::Pad.
 * pad_name: a name which will be attributed to the ghost pad.
 *
 * Creates a ghost pad from the given pad, and adds it to the list of
 * pads of the element.
 *
 * The second parameter defines the ghost pad name.  
 * When ommited (or nil), the ghost pad will receive the same name as
 * the given pad.
 *
 * Returns: the ghost pad which was created, or nil.
 */
static VALUE
rb_gst_element_add_ghost_pad (int argc, VALUE * argv, VALUE self)
{
    GstPad *pad, *newpad;
    VALUE rpad, rname;
    const gchar *name;

    rb_scan_args (argc, argv, "11", &rpad, &rname);

    pad = RGST_PAD (rpad);
    name = (NIL_P (rname)) ? gst_pad_get_name (pad) : RVAL2CSTR (rname);
    newpad = gst_element_add_ghost_pad (RGST_ELEMENT (self), pad, name);
    return newpad != NULL ? RGST_PAD_NEW (newpad)
        : Qnil;
}

/* Method: indexable?
 * Returns: true if the element can be indexed, false otherwise.
 */
static VALUE
rb_gst_element_is_indexable (VALUE self)
{
    return CBOOL2RVAL (gst_element_is_indexable (RGST_ELEMENT (self)));
}

/*
 * Method: query(query_type, format=Gst::Format::DEFAULT)
 * query_type: a query type (see Gst::QueryType::Types).
 * format: a format (see Gst::Format::Types).
 *
 * Performs a query on the element.
 *
 * Returns: a Fixnum value returned by the query, or nil if the query
 * could not be performed.
 */
static VALUE
rb_gst_element_query (int argc, VALUE * argv, VALUE self)
{
    VALUE query_type, format;
    GstFormat gstformat;
    gint64 value;

    rb_scan_args (argc, argv, "11", &query_type, &format);
    gstformat =
        NIL_P (format) ? GST_FORMAT_DEFAULT : RVAL2GENUM (format,
                                                          GST_TYPE_FORMAT);

    if (gst_element_query (RGST_ELEMENT (self),
                           RVAL2GENUM (query_type, GST_TYPE_QUERY_TYPE),
                           &gstformat, &value)) {
        format = INT2FIX (gstformat);
        return ULL2NUM (value);
    }
    return Qnil;
}

/*
 * Method: send_event(event)
 * event: a Gst::Event object.
 *
 * Sends an event to an element, through a Gst::Event object. 
 * If the element doesn't implement an event handler, the event will be 
 * forwarded to a random sink pad.
 *
 * Returns: true if the request event was successfully handled, false
 * otherwise.
 */
static VALUE
rb_gst_element_send_event (VALUE self, VALUE event)
{
    return CBOOL2RVAL (gst_element_send_event (RGST_ELEMENT (self),
                                               RGST_EVENT (event)));
}

/*
 * Method: complex?
 *
 * Checks if the Gst::Element::COMPLEX flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_element_is_complex (VALUE self)
{
    return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_ELEMENT (self),
                                        GST_ELEMENT_COMPLEX));
}

/*
 * Method: decoupled?
 *
 * Checks if the Gst::Element::DECOUPLED flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_element_is_decoupled (VALUE self)
{
    return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_ELEMENT (self),
                                        GST_ELEMENT_DECOUPLED));
}

/*
 * Method: thread_suggested?
 *
 * Checks if the Gst::Element::THREAD_SUGGESTED flag is set on the 
 * object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_element_is_thread_suggested (VALUE self)
{
    return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_ELEMENT (self),
                                        GST_ELEMENT_THREAD_SUGGESTED));
}

/*
 * Method: has_infinite_loop?
 *
 * Checks if the Gst::Element::INFINITE_LOOP flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_element_has_infinite_loop (VALUE self)
{
    return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_ELEMENT (self),
                                        GST_ELEMENT_INFINITE_LOOP));
}

/*
 * Method: has_new_loopfunc?
 *
 * Checks if the Gst::Element::NEW_LOOPFUNC flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_element_has_new_loopfunc (VALUE self)
{
    return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_ELEMENT (self),
                                        GST_ELEMENT_NEW_LOOPFUNC));
}

/*
 * Method: event_aware?
 *
 * Checks if the Gst::Element::EVENT_AWARE flag is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_element_is_event_aware (VALUE self)
{
    return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_ELEMENT (self),
                                        GST_ELEMENT_EVENT_AWARE));
}

/*
 * Method: use_threadsafe_properties? 
 *
 * Checks if the Gst::Element::USE_THREADSAFE_PROPERTIES flag 
 * is set on the object.
 *
 * Returns: true if the flag is set, false otherwise.
 */
static VALUE
rb_gst_element_use_threadsafe_properties (VALUE self)
{
    return CBOOL2RVAL (GST_FLAG_IS_SET (RGST_ELEMENT (self),
                                        GST_ELEMENT_USE_THREADSAFE_PROPERTIES));
}

/*
 * Method: enable_threadsafe_properties
 * 
 * Installs an asynchronous queue, a mutex and pre- and post-run functions on this element so 
 * that properties on the element can be set in a threadsafe way.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_enable_threadsafe_properties (VALUE self)
{
    gst_element_enable_threadsafe_properties (RGST_ELEMENT (self));
    return self;
}

/*
 * Method: disable_threadsafe_properties
 * 
 * Removes the threadsafe properties, post- and pre-run locks from this element.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_disable_threadsafe_properties (VALUE self)
{
    gst_element_disable_threadsafe_properties (RGST_ELEMENT (self));
    return self;
}

/*
 * Method: set_pending_properties
 * 
 * Sets all pending properties on the threadsafe properties enabled element.
 * 
 * Returns: self.
 */
static VALUE
rb_gst_element_set_pending_properties (VALUE self)
{
    gst_element_set_pending_properties (RGST_ELEMENT (self));
    return self;
}

/*
 * Method: time
 *
 * Queries the element's time.
 *
 * Returns:  the current stream time (in nanoseconds) in Gst::Element::STATE_PLAYING, the 
 * element base time in Gst::Element::STATE_PAUSED, or -1 otherwise.
 */
static VALUE
rb_gst_element_get_time (VALUE self)
{
    return ULL2NUM (gst_element_get_time (RGST_ELEMENT (self)));
}

/*
 * Method: set_time(time)
 * time: time to set (in nanoseconds).
 *
 * Sets the current time of the element. This method can be used when handling discont events. 
 * You can only call this method on an element with a clock in Gst::Element::STATE_PAUSED or 
 * Gst::Element::STATE_PLAYING. You might want to have a look at Gst::Element#adjust_time, if 
 * you want to adjust by a difference as that is more accurate.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_set_time (VALUE self, VALUE time)
{
    gst_element_set_time (RGST_ELEMENT (self), NUM2ULL (time));
    return self;
}

/*
 * Method: set_time_delay(time, delay)
 * time: time to set (in nanoseconds).
 * delay: a delay to discount from the given time (also in nanoseconds).
 *
 * Sets the current time of the element to time - delay. This method can be used when handling 
 * discont events in elements writing to an external buffer, i. e., an audio sink that writes 
 * to a sound card that buffers the sound before playing it. The delay should be the current 
 * buffering delay.
 *
 * You can only call this method on an element with a clock in Gst::Element::STATE_PAUSED or 
 * Gst::Element::STATE_PLAYING. You might want to have a look at Gst::Element#adjust_time, if 
 * you want to adjust by a difference as that is more accurate.
 * 
 * Returns: self.
 */
static VALUE
rb_gst_element_set_time_delay (VALUE self, VALUE time, VALUE delay)
{
    gst_element_set_time_delay (RGST_ELEMENT (self), NUM2ULL (time),
                                NUM2ULL (delay));
    return self;
}

/*
 * Method: adjust_time(diff)
 * diff: difference to adjust (in nanoseconds).
 *
 * Adjusts the current time of the element by the specified difference. This method can be used
 * when handling discont events.
 *
 * You can only call this method on an element with a clock in Gst::Element::STATE_PAUSED or 
 * Gst::Element::STATE_PLAYING. It is more accurate than Gst::Element#set_time.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_adjust_time (VALUE self, VALUE diff)
{
    gst_element_adjust_time (RGST_ELEMENT (self), NUM2ULL (diff));
    return self;
}

/* 
 * Method: index
 *
 * Gets the index from the element. 
 *
 * Returns: a Gst::Index or nil when no index was set on the element.
 */
static VALUE
rb_gst_element_get_index (VALUE self)
{
    GstIndex *index = gst_element_get_index (RGST_ELEMENT (self));

    return index != NULL ? RGST_INDEX_NEW (index)
        : Qnil;
}

/*
 * Method: set_index(index)
 * index: the index to set, as a Gst::Index.
 * 
 * Sets the specified index on the element.
 *
 * Returns: self. 
 */
static VALUE
rb_gst_element_set_index (VALUE self, VALUE index)
{
    gst_element_set_index (RGST_ELEMENT (self), RGST_INDEX (index));
    return self;
}

/* 
 * Method: scheduler
 *
 * Gets the scheduler of the element.
 *
 * Returns: a Gst::Scheduler, or nil if the element has no scheduler.
 */
static VALUE
rb_gst_element_get_scheduler (VALUE self)
{
    GstScheduler *scheduler = gst_element_get_scheduler (RGST_ELEMENT (self));

    return scheduler != NULL ? RGST_SCHEDULER_NEW (scheduler) : Qnil;
}

/*
 * Method: set_scheduler(scheduler)
 * scheduler: the Gst::Scheduler to set.
 *
 * Sets the scheduler of the element. For internal use only, unless you're 
 * writing a new bin subclass.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_set_scheduler (VALUE self, VALUE scheduler)
{
    gst_element_set_scheduler (RGST_ELEMENT (self), RGST_SCHEDULER (scheduler));
    return self;
}

/*
 * Method: get_static_pad(name)
 * name: the name of the static Gst::Pad to retrieve.
 *
 * Retrieves a pad from the element by name.  This version only retrieves
 * already existing (i.e. 'static') pads.
 *
 * Returns: the requested Gst::Pad if found, otherwise nil.
 */
static VALUE
rb_gst_element_get_static_pad (VALUE self, VALUE name)
{
    GstPad *pad =
        gst_element_get_static_pad (RGST_ELEMENT (self), RVAL2CSTR (name));
    return pad != NULL ? RGST_PAD_NEW (pad) : Qnil;
}

/*
 * Method: get_request_pad(name)
 * name: the name of the request Gst::Pad to retrieve.
 *
 * Retrieves a pad from the element by name.  This version only retrieves
 * request pads.
 *
 * Returns: the requested Gst::Pad if found, otherwise nil.
 */
static VALUE
rb_gst_element_get_request_pad (VALUE self, VALUE name)
{
    GstPad *pad =
        gst_element_get_request_pad (RGST_ELEMENT (self), RVAL2CSTR (name));
    return pad != NULL ? RGST_PAD_NEW (pad) : Qnil;
}

/*
 * Method: release_request_pad(pad)
 * pad: the Gst::Pad to release.
 *
 * Makes the element free the previously requested pad ass obtained with
 * Gst::Element#get_requested_pad.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_release_request_pad (VALUE self, VALUE pad)
{
    gst_element_release_request_pad (RGST_ELEMENT (self), RGST_PAD (pad));
    return self;
}

/*
 * Method: add_pad(pad)
 * pad: the Gst::Pad to add to the element.
 *
 * Adds a pad (link point) to the element.  Pads are automatically activated
 * when the element is in state Gst::Element::PLAYING.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_add_pad (VALUE self, VALUE pad)
{
    gst_element_add_pad (RGST_ELEMENT (self), RGST_PAD (pad));
    return self;
}

/*
 * Method: remove_pad(pad)
 * pad: the Gst::Pad to remove from the element.
 *
 * Removes the given pad from the element.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_remove_pad (VALUE self, VALUE pad)
{
    gst_element_remove_pad (RGST_ELEMENT (self), RGST_PAD (pad));
    return self;
}

/*
 * Method: get_compatible_pad(pad, caps=nil)
 * pad: the Gst::Pad to find a compatible one for.
 * caps: the Gst::Caps to use as a filter.
 *
 * Looks for an unlinked pad to which the given pad can link to.  It is not
 * guaranteed that linking the pads will work, though it should work in most
 * cases.  You can eventually pass a Gst::Caps that will act as a filter, but
 * this is not mandatory.
 *
 * Returns: the Gst::Pad to which a link can be made, or nil if one could not
 * be found.
 */
static VALUE
rb_gst_element_get_compatible_pad (int argc, VALUE * argv, VALUE self)
{
    VALUE pad, caps;
    GstPad *pad2;

    rb_scan_args (argc, argv, "11", &pad, &caps);

    pad2 = NIL_P (caps)
        ? gst_element_get_compatible_pad (RGST_ELEMENT (self), RGST_PAD (pad))
        : gst_element_get_compatible_pad_filtered (RGST_ELEMENT (self),
                                                   RGST_PAD (pad),
                                                   RGST_CAPS (caps));
    return pad2 != NULL ? RGST_PAD_NEW (pad2) : Qnil;
}

/*
 * Method: get_pad_template(name)
 * name: the name of the Gst::PadTemplate to get.
 *
 * Retrieves a Gst::PadTemplate from this element with the given name.
 *
 * Returns: the Gst::PadTemplate with the given name, or nil if none was found.
 */
static VALUE
rb_gst_element_get_pad_template (VALUE self, VALUE name)
{
    GstPadTemplate *pad =
        gst_element_get_pad_template (RGST_ELEMENT (self), RVAL2CSTR (name));
    return pad != NULL ? RGST_PAD_TEMPLATE_NEW (pad) : Qnil;
}

/*
 * Method: pad_templates
 *
 * Retrieves a list of pad templates associated with the element.
 * 
 * Returns: an Array of Gst::PadTemplate objects.
 */
static VALUE
rb_gst_element_get_pad_templates (VALUE self)
{
    GList *list;
    VALUE ary;

    ary = rb_ary_new ();

    for (list = gst_element_get_pad_template_list (RGST_ELEMENT (self));
         list != NULL; list = g_list_next (list))
        rb_ary_push (ary, RGST_PAD_TEMPLATE_NEW (list->data));

    g_list_free (list);
    return ary;
}

/*
 * Method: each_pad_template { |pad_template| ... }
 *
 * Calls the block for each pad template associated with the element,
 * passing a reference to a Gst::PadTemplate object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_element_each_pad_template (VALUE self)
{
    return rb_ary_yield (rb_gst_element_get_pad_templates (self));
}

/*
 * Method: get_compatible_pad_template(pad)
 * pad: the Gst::PadTemplate to find a compatible template for.
 *
 * Retrieves a pad template from the element that is compatible with the given
 * pad template.  Pads from compatible templates can be linked together.
 *
 * Returns: a compatible Gst::PadTemplate, or nil if none was found.
 */
static VALUE
rb_gst_element_get_compatible_pad_template (VALUE self, VALUE pad)
{
    GstPadTemplate *pad2 =
        gst_element_get_compatible_pad_template (RGST_ELEMENT (self),
                                                 RGST_PAD_TEMPLATE (pad));

    return pad2 != NULL ? RGST_PAD_TEMPLATE_NEW (pad2) : Qnil;
}

/*
 * Method: seek(seek_type, offset)
 * seek_type: the method to use for seeking (see Gst::EventSeek::Type).
 * offset: the offset to seek to.
 *
 * Sends a seek event (Gst::EventSseek) to the element.
 * 
 * Returns: true if the event was handled.
 */
static VALUE
rb_gst_element_seek (VALUE self, VALUE seek_type, VALUE offset)
{
    return
        CBOOL2RVAL (gst_element_seek
                    (RGST_ELEMENT (self),
                     RVAL2GFLAGS (seek_type, GST_TYPE_SEEK_TYPE),
                     NUM2ULL (offset)));
}

/*
 * Method: locked_state?
 *
 * Checks if the state of an element is locked. If the state of an element is 
 * locked, state changes of the parent don't affect the element. This way you 
 * can leave currently unused elements inside bins. Just lock their state 
 * before changing the state from Gst::Element::STATE_NULL.
 *
 * Returns: true if the element's state is locked.
 */
static VALUE
rb_gst_element_is_locked_state (VALUE self)
{
    return CBOOL2RVAL (gst_element_is_locked_state (RGST_ELEMENT (self)));
}

/*
 * Method: set_locked_state(state)
 * state: whether to lock the element's state.
 *
 * Locks the state of an element, so state changes of the parent don't affect 
 * this element anymore.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_set_locked_state (VALUE self, VALUE state)
{
    gst_element_set_locked_state (RGST_ELEMENT (self), RVAL2CBOOL (state));
    return self;
}

/*
 * Method: sync_state_with_parent
 *
 * Tries to change the state of the element to the same as its parent. If this 
 * method returns false, the state of element is undefined.
 *
 * Returns: true if the element's state could be synced to the parent's state.
 */
static VALUE
rb_gst_element_sync_state_with_parent (VALUE self)
{
    return
        CBOOL2RVAL (gst_element_sync_state_with_parent (RGST_ELEMENT (self)));
}

/*
 * Method: managing_bin
 *
 * Gets the managing bin (a pipeline or a thread, for example) of an element.
 *
 * Returns: a Gst::Bin, or nil on failure.
 */
static VALUE
rb_gst_element_get_managing_bin (VALUE self)
{
    GstBin *bin;

    bin = gst_element_get_managing_bin (RGST_ELEMENT (self));
    return bin != NULL ? RGST_BIN_NEW (bin) : Qnil;
}

/*
 * Method: no_more_pads
 *
 * Uses this method to signal that the element does not expect any more pads 
 * to show up in the current pipeline. This method should be called whenever 
 * pads have been added by the element itself. 
 * Elements with Gst::Pad::SOMETIMES pad templates use this in combination 
 * with autopluggers to figure out that the element is done initializing its 
 * pads.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_no_more_pads (VALUE self)
{
    gst_element_no_more_pads (RGST_ELEMENT (self));
    return self;
}

void
Init_gst_element (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_ELEMENT, "Element", mGst);

    rb_define_method (c, "set_state", rb_gst_element_set_state, 1);
    rb_define_method (c, "state", rb_gst_element_get_state, 0);
    rb_define_method (c, "stop", rb_gst_element_stop, 0);
    rb_define_method (c, "ready", rb_gst_element_ready, 0);
    rb_define_method (c, "pause", rb_gst_element_pause, 0);
    rb_define_method (c, "play", rb_gst_element_play, 0);
    rb_define_method (c, "stopped?", rb_gst_element_is_stopped, 0);
    rb_define_method (c, "ready?", rb_gst_element_is_ready, 0);
    rb_define_method (c, "paused?", rb_gst_element_is_paused, 0);
    rb_define_method (c, "playing?", rb_gst_element_is_playing, 0);
    rb_define_method (c, "wait_state_change",
                      rb_gst_element_wait_state_change, 0);
    rb_define_method (c, "eos", rb_gst_element_set_eos, 0);
    rb_define_method (c, "set_property", rb_gst_element_set_property, 2);
    rb_define_method (c, "get_property", rb_gst_element_get_property, 1);
    rb_define_method (c, "each_property", rb_gst_element_each_property, 0);
    rb_define_method (c, "sched_yield", rb_gst_element_sched_yield, 0);
    rb_define_method (c, "sched_interrupt", rb_gst_element_sched_interrupt, 0);
    rb_define_method (c, "link", rb_gst_element_link, 1);
    rb_define_alias (c, ">>", "link");
    rb_define_method (c, "provides_clock?", rb_gst_element_provides_clock, 0);
    rb_define_method (c, "requires_clock?", rb_gst_element_requires_clock, 0);
    rb_define_method (c, "clock", rb_gst_element_get_clock, 0);
    rb_define_method (c, "set_clock", rb_gst_element_set_clock, 1);
    rb_define_method (c, "time", rb_gst_element_get_time, 0);
    rb_define_method (c, "set_time", rb_gst_element_set_time, 1);
    rb_define_method (c, "set_time_delay", rb_gst_element_set_time_delay, 2);
    rb_define_method (c, "adjust_time", rb_gst_element_adjust_time, 1);
    rb_define_method (c, "each_pad", rb_gst_element_each_pad, 0);
    rb_define_method (c, "pads", rb_gst_element_get_pads, 0);
    rb_define_method (c, "get_pad", rb_gst_element_get_pad, 1);
    rb_define_method (c, "get_static_pad", rb_gst_element_get_static_pad, 1);
    rb_define_method (c, "get_request_pad", rb_gst_element_get_request_pad, 1);
    rb_define_method (c, "release_request_pad",
                      rb_gst_element_release_request_pad, 1);
    rb_define_method (c, "get_compatible_pad",
                      rb_gst_element_get_compatible_pad, -1);
    rb_define_method (c, "get_pad_template", rb_gst_element_get_pad_template,
                      1);
    rb_define_method (c, "pad_templates", rb_gst_element_get_pad_templates, 0);
    rb_define_method (c, "each_pad_template", rb_gst_element_each_pad_template,
                      0);
    rb_define_method (c, "get_compatible_pad_template",
                      rb_gst_element_get_compatible_pad_template, 1);
    rb_define_method (c, "link_pads", rb_gst_element_link_pads, 1);
    rb_define_method (c, "unlink_pads", rb_gst_element_unlink_pads, 1);
    rb_define_method (c, "add_pad", rb_gst_element_add_pad, 1);
    rb_define_method (c, "remove_pad", rb_gst_element_remove_pad, 1);
    rb_define_method (c, "add_ghost_pad", rb_gst_element_add_ghost_pad, -1);
    rb_define_alias (c, "remove_ghost_pad", "remove_pad");
    rb_define_method (c, "indexable?", rb_gst_element_is_indexable, 0);
    rb_define_method (c, "query", rb_gst_element_query, -1);
    rb_define_method (c, "send_event", rb_gst_element_send_event, 1);
    rb_define_method (c, "seek", rb_gst_element_seek, 2);
    rb_define_method (c, "index", rb_gst_element_get_index, 0);
    rb_define_method (c, "set_index", rb_gst_element_set_index, 1);
    rb_define_method (c, "scheduler", rb_gst_element_get_scheduler, 0);
    rb_define_method (c, "set_scheduler", rb_gst_element_set_scheduler, 1);
    rb_define_method (c, "locked_state?", rb_gst_element_is_locked_state, 0);
    rb_define_method (c, "set_locked_state", rb_gst_element_set_locked_state,
                      1);
    rb_define_method (c, "sync_state_with_parent",
                      rb_gst_element_sync_state_with_parent, 0);
    rb_define_method (c, "managing_bin", rb_gst_element_get_managing_bin, 0);
    rb_define_method (c, "no_more_pads", rb_gst_element_no_more_pads, 0);

    rb_define_method (c, "complex?", rb_gst_element_is_complex, 0);
    rb_define_method (c, "decoupled?", rb_gst_element_is_decoupled, 0);
    rb_define_method (c, "thread_suggested?",
                      rb_gst_element_is_thread_suggested, 0);
    rb_define_method (c, "has_infinite_loop?",
                      rb_gst_element_has_infinite_loop, 0);
    rb_define_method (c, "has_new_loopfunc?",
                      rb_gst_element_has_new_loopfunc, 0);
    rb_define_method (c, "event_aware?", rb_gst_element_is_event_aware, 0);
    rb_define_method (c, "use_threadsafe_properties?",
                      rb_gst_element_use_threadsafe_properties, 0);

    rb_define_method (c, "enable_threadsafe_properties",
                      rb_gst_element_enable_threadsafe_properties, 0);
    rb_define_method (c, "disable_threadsafe_properties",
                      rb_gst_element_disable_threadsafe_properties, 0);
    rb_define_method (c, "set_pending_properties",
                      rb_gst_element_set_pending_properties, 0);

    G_DEF_SETTERS (c);

    G_DEF_CLASS (GST_TYPE_ELEMENT_STATE_RETURN, "StateReturn", c);
    G_DEF_CONSTANTS (c, GST_TYPE_ELEMENT_STATE_RETURN, "GST_");
    G_DEF_CLASS (GST_TYPE_ELEMENT_STATE, "State", c);
    G_DEF_CONSTANTS (c, GST_TYPE_ELEMENT_STATE, "GST_");
    G_DEF_CLASS (GST_TYPE_ELEMENT_FLAGS, "Types", c);
    G_DEF_CONSTANTS (c, GST_TYPE_ELEMENT_FLAGS, "GST_ELEMENT_");

    /*
     * TODO:
     * gst_element_clock_wait () 
     */
}
