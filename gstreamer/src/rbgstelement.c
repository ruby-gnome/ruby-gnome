
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
 *  Class: Gst::Element < Gst::Object
 *
 *  Base class for all pipeline elements.
 */

/*
 *  Constant: STATE_NULL
 *  The state of the element is reseted.
 */
static VALUE constStateNull = INT2FIX(GST_STATE_NULL);

/*
 *  Constant: STATE_READY
 *  The element is ready to start processing data. 
 *  Some elements might have a non trivial way to initialize themselves.
 */
static VALUE constStateReady = INT2FIX(GST_STATE_READY);

/*
 *  Constant: STATE_PAUSED
 *  Means there really is data flowing temporary stops the data flow. 
 */
static VALUE constStatePaused = INT2FIX(GST_STATE_PAUSED);

/*
 *  Constant: STATE_PLAYING
 *  Means there really is data flowing through the graph. 
 */
static VALUE constStatePlaying = INT2FIX(GST_STATE_PLAYING);

/*
 *  Constant: STATE_FAILURE
 *  The element could not perform the state change.
 */
static VALUE constStateFailure = INT2FIX(GST_STATE_FAILURE);

/*
 *  Constant: STATE_SUCCESS
 *  The element successfully changed its state.
 */
static VALUE constStateSuccess = INT2FIX(GST_STATE_SUCCESS);

/*
 *  Constant: STATE_ASYNC
 *  The element will asynchronously change its state as soon as possible.
 */
static VALUE constStateAsync = INT2FIX(GST_STATE_ASYNC);

/*
 *  Method: set_state(aFixnum) -> aFixnum
 *
 *  Sets the state of the element. 
 *
 *  This function will try to set the requested state by going through all 
 *  the intermediary states and calling the class's state change function 
 *  for each.
 *
 *  Returns an integer code, according to the result of the operation:
 *
 *    - Gst::Element::STATE_FAILURE;
 *    - Gst::Element::STATE_SUCCESS;
 *    - Gst::Element::STATE_ASYNC.
 */
static VALUE rb_gst_element_set_state(self, value)
    VALUE self, value;
{
    GstElement *element = RGST_ELEMENT(self);
    int state = FIX2INT(value);
    
    if (state != GST_STATE_NULL && 
        state != GST_STATE_READY &&
        state != GST_STATE_PAUSED &&
        state != GST_STATE_PLAYING)
    {
        rb_raise(rb_eArgError, 
            "Invalid Gst::Element state %d", 
            state);
    }
   
    return INT2FIX(gst_element_set_state(element, state));
}

/*
 *  Method: state -> aFixnum
 *
 *  Gets the state of the element:
 *
 *    - Gst::Element::STATE_NULL;
 *    - Gst::Element::STATE_READY;
 *    - Gst::Element::STATE_PAUSED;
 *    - Gst::Element::STATE_PLAYING.
 */
static VALUE rb_gst_element_get_state(self)
    VALUE self;
{
    GstElement *element = RGST_ELEMENT(self);
    switch (gst_element_get_state(element)) {
        case GST_STATE_NULL:    return constStateNull;
        case GST_STATE_READY:   return constStateReady;
        case GST_STATE_PAUSED:  return constStatePaused;
        case GST_STATE_PLAYING: return constStatePlaying;
        default:                assert(0);  /* should not be here */
    }
    return Qnil;
}

/*
 *  Method: stop -> aFixnum
 *
 *  Calls Gst::Element#set_state with Gst::Element::STATE_NULL.
 */
static VALUE rb_gst_element_stop(self)
    VALUE self;
{
    return rb_gst_element_set_state(self, constStateNull);
}
    
/*
 *  Method: ready -> aFixnum
 *
 *  Calls Gst::Element#set_state with Gst::Element::STATE_READY.
 */
static VALUE rb_gst_element_ready(self)
    VALUE self;
{
    return rb_gst_element_set_state(self, constStateReady);
}
    
/*
 *  Method: pause -> aFixnum
 *
 *  Calls Gst::Element#set_state with Gst::Element::STATE_PAUSED.
 */
static VALUE rb_gst_element_pause(self)
    VALUE self;
{
    return rb_gst_element_set_state(self, constStatePaused);
}
    
/*
 *  Method: play -> aFixnum
 *
 *  Calls Gst::Element#set_state with Gst::Element::STATE_PLAYING.
 */
static VALUE rb_gst_element_play (self)
    VALUE self;
{
    return rb_gst_element_set_state(self, constStatePlaying);
}
    
/*
 *  Method: stopped? -> aBoolean
 *
 *  Checks if the current state equals Gst::Element::STATE_NULL.
 */
static VALUE rb_gst_element_is_stopped(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_element_get_state(self) == constStateNull);
}
    
/*
 *  Method: ready? -> aBoolean
 *
 *  Checks if the current state equals Gst::Element::STATE_READY.
 */
static VALUE rb_gst_element_is_ready(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_element_get_state(self) == constStateReady);
}
    
/*
 *  Method: paused? -> aBoolean
 *
 *  Checks if the current state equals Gst::Element::STATE_PAUSED.
 */
static VALUE rb_gst_element_is_paused(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_element_get_state(self) == constStatePaused);
}
    
/*
 *  Method: playing? -> aBoolean
 *
 *  Checks if the current state equals Gst::Element::STATE_PLAYING.
 */
static VALUE rb_gst_element_is_playing(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_element_get_state(self) == constStatePlaying);
}
/*
 *  Method: wait_state_change -> self
 *
 *  Waits and blocks until the element changed its state.
 */
static VALUE rb_gst_element_wait_state_change(self)
    VALUE self;
{
    GstElement *element = RGST_ELEMENT(self);
    gst_element_wait_state_change(element);
    return self;
}

static int check_property(element, name, value)
    GstElement *element;
    const gchar *name;
    GValue *value; 
{
    GParamSpec *pspec;
    GObject *object;
    int type;

    assert(element != NULL);
    assert(name    != NULL);
    assert(value   != NULL);

    object = (GObject *) element;

    pspec = g_object_class_find_property(G_OBJECT_GET_CLASS(object), 
        name);
    
    if (pspec == NULL) {
        rb_raise(rb_eArgError, 
                 "This element has no property named %s", 
                 name);
    }

    type = G_PARAM_SPEC_VALUE_TYPE(pspec);
    g_value_init(value, type);
    return type;
}

/*
 *  Method: set_property(aString, anObject) -> self
 *
 *  Sets the value anObject for the property named aString.
 *
 *  If the element uses threadsafe properties, the property will 
 *  be put on the async queue.
 *
 *  Note that properties may be set through accessors methods, 
 *  which are generated on-the-fly according to the Gst::Element type.
 *
 *  Example:
 *      e = Gst::ElementFactory.make("filesrc")
 *      e.set_property("location", "a_file.ogg")
 *      # This does exactly the same
 *      e.location = "a_file.ogg"
 */
static VALUE rb_gst_element_set_property(self, name, value)
    VALUE self, name, value;
{
    GstElement *element;
    GValue gvalue = { 0, };
    const gchar *gname;
    int type;
   
    gname   = RVAL2CSTR(name);
    element = RGST_ELEMENT(self); 

    type = check_property(element, gname, &gvalue);
    rbgobj_rvalue_to_gvalue(value, &gvalue);
    gst_element_set_property(element, RVAL2CSTR(name), &gvalue);
    return self; 
}

/*
 *  Method: get_property(aString) -> anObject
 *
 *  Returns the value associated with the property named 
 *  aString, or nil if the property has no value.
 *
 *  Note that properties may be read through accessors methods, 
 *  which are generated on-the-fly according to the Gst::Element type.
 *
 *  Example:
 *      e = Gst::ElementFactory.make("filesrc")
 *      # ...
 *      puts "Location is " + e.get_property("location")
 *      # This does exactly the same
 *      puts "Location is " + e.location
 */ 
static VALUE rb_gst_element_get_property(self, name)
    VALUE self, name;
{
    GstElement *element;
    GValue gvalue = { 0, };
    const gchar *gname;
    
    element = RGST_ELEMENT(self);    
    gname   = RVAL2CSTR(name);

    check_property(element, gname, &gvalue);
    gst_element_get_property(element, RVAL2CSTR(name), &gvalue);
    return GVAL2RVAL(&gvalue); 
}

/*
 *  Method: each_property { |aStringName, aStringDescr, anObject| block } -> nil
 *
 *  Calls the block for each property of the element, passing
 *  name (aString), description (aStringDescr) and value (anObject) of the property 
 *  as parameters.
 */
static VALUE rb_gst_element_each_property(self)
    VALUE self;
{
    GParamSpec **pspec;
    GObject *object;
    guint count;
    int i;

    if (rb_block_given_p() == Qfalse) {
        rb_raise(rb_eArgError, "No block given");
    }

    object = (GObject *) RGST_ELEMENT(self); 
    pspec  = g_object_class_list_properties(G_OBJECT_GET_CLASS(object), 
        &count);
    
    for (i = 0; i < count; i++) {
        VALUE name, descr, val, arr;

        name  = CSTR2RVAL(g_param_spec_get_name(pspec[i]));
        descr = CSTR2RVAL(g_param_spec_get_blurb(pspec[i]));
        val   = rb_gst_element_get_property(self, name);

        arr = rb_ary_new();
        rb_ary_push(arr, name);
        rb_ary_push(arr, descr);
        rb_ary_push(arr, val);
        rb_yield(arr);
    }
    return Qnil; 
}

/*
 *  Method: eos -> self
 *
 *  Perform the actions needed to bring the element in the EOS 
 *  (end of stream) state. 
 */
static VALUE rb_gst_element_set_eos(self)
    VALUE self;
{
    gst_element_set_eos(RGST_ELEMENT(self));
    return self;
}

/*
 *  Method: sched_yield -> self
 *
 *  Requests a yield operation for the element. The scheduler 
 *  will typically give control to another element.
 */
static VALUE rb_gst_element_sched_yield(self)
    VALUE self;
{
    GstElement *element = RGST_ELEMENT(self);
    gst_element_yield(element);
    return self;
}

/*
 *  Method: sched_interrupt -> self
 *
 *  Requests the scheduler of this element to interrupt the 
 *  execution of this element and scheduler another one.
 */
static VALUE rb_gst_element_sched_interrupt(self)
    VALUE self;
{
    GstElement *element = RGST_ELEMENT(self);
    gst_element_interrupt(element);
    return self;
}

/*
 *  Method: link(anElement) -> anElement
 *
 *  Links this element (source) to anElement (destination). 
 *
 *  The functions looks for existing pads and request pads that 
 *  aren't linked yet. If multiple links are possible, only one 
 *  is established.
 *
 *  Returns the destination element (anElement).
 */
static VALUE rb_gst_element_link(self, other_element)
    VALUE self, other_element;
{
    GstElement *element1, *element2;

    element1 = RGST_ELEMENT(self);
    element2 = RGST_ELEMENT(other_element);
    return gst_element_link(element1, element2) == TRUE 
        ? other_element 
        : Qnil;
}

/*
 *  Method: requires_clock? -> aBoolean
 *
 *  Checks if the element requires a clock.
 */
static VALUE rb_gst_element_requires_clock(self)
    VALUE self;
{
    GstElement *element = RGST_ELEMENT(self);
    return CBOOL2RVAL(gst_element_requires_clock(element));
}

/*
 *  Method: provides_clock? -> aBoolean
 *
 *  Checks if the element provides a clock.
 */
static VALUE rb_gst_element_provides_clock(self)
    VALUE self;
{
    GstElement *element = RGST_ELEMENT(self);
    return CBOOL2RVAL(gst_element_provides_clock(element));
}

/*
 *  Method: clock -> aClockObject
 *
 *  Gets the clock of the element, as a Gst::Clock object.
 *  Returns nil if this element does not provide a clock.
 */
static VALUE rb_gst_element_get_clock(self)
    VALUE self;
{
    GstElement *element;
    GstClock *clock;

    element = RGST_ELEMENT(self);
    clock   = gst_element_get_clock(element);
    return clock != NULL
        ? RGST_CLOCK_NEW(clock)
        : Qnil;
}

/*
 *  Method: pads -> anArray
 *
 *  Gets a list of the pads associated with the element, in an array
 *  of Gst::Pad objects.
 */
static VALUE rb_gst_element_get_pads(self)
    VALUE self;
{
    GstElement *element;
    const GList *list;
    VALUE arr;

    element = RGST_ELEMENT(self);
    arr     = rb_ary_new();

    for (list  = gst_element_get_pad_list(element); 
         list != NULL; 
         list  = g_list_next(list))
    {
        GstPad *pad = GST_PAD(list->data);
        /*
         *  Increment the ref count of the Pad, since it was not
         *  created with gst_pad_new(), and it will be unref() from
         *  the GC.  
         */
        gst_object_ref(GST_OBJECT(pad));
        rb_ary_push(arr, RGST_PAD_NEW(pad));
    }
    return arr; 
}

/*
 *  Method: each_pad { |aGstPad| block } -> nil
 *
 *  Calls the block for each pad associated with the element, passing a 
 *  reference to the Gst::Pad as parameter.
 */
static VALUE rb_gst_element_each_pad(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_element_get_pads(self));
}

/*
 *  Method: get_pad(aNameString) -> aPad
 *
 *  Retrieves a Gst::Pad object from the element by name.
 *  Returns nil if the pad cannot be found.
 */
static VALUE rb_gst_element_get_pad(self, pad_name)
    VALUE self, pad_name;
{
    GstElement *element;
    GstPad *pad;
    
    element = RGST_ELEMENT(self);
    pad = gst_element_get_pad(element, RVAL2CSTR(pad_name));
    return pad != NULL 
        ? RGST_PAD_NEW(pad)
        : Qnil;
}

/*
 *  Method: link_pads(other_element) -> aBoolean
 *
 *  Links the "src" named pad of the current element to the
 *  "sink" named pad of the destination element, returning
 *  true on success.
 *
 *  If you want to link specific named pads, you should use
 *  the Gst::Pad#link method directly.
 *
 *  Example:
 *
 *      element1.link_pads(element2)
 *      # This does the same
 *      element1.get_pad("src").link(element2.get_pad("sink"))
 */
static VALUE rb_gst_element_link_pads(self, other_element)
    VALUE self, other_element;
{
    return CBOOL2RVAL(gst_element_link_pads(RGST_ELEMENT(self),
                                         "src",
                                         RGST_ELEMENT(other_element),
                                         "sink"));
}

/*
 *  Method: unlink_pads(other_element) -> self
 *
 *  Unlinks the "src" named pad of the current element from the
 *  "sink" named pad of the destination element.
 *
 *  If you want to unlink specific named pads, you should use
 *  the Gst::Pad#unlink method directly.
 *
 *  Example:
 *
 *      element1.unlink_pads(element2)
 *      # This does the same
 *      element1.get_pad("src").unlink(element2.get_pad("sink"))
 */
static VALUE rb_gst_element_unlink_pads(self, other_element)
    VALUE self, other_element;
{
    gst_element_unlink_pads(RGST_ELEMENT(self),
                            "src",
                            RGST_ELEMENT(other_element),
                            "sink");
    return self;
}

/*
 *  Method: add_ghost_pad(aPad, aPadNameString=nil) = aPad
 *
 *  Creates a ghost pad from the given pad, and adds it to the list of
 *  pads of the element.
 *
 *  The second parameter defines the ghost pad name.  
 *  When ommited (or nil),  the ghost pad will receive the same name as
 *  the given pad.
 *
 *  Returns the ghost pad which was created, or nil.
 */
static VALUE rb_gst_element_add_ghost_pad(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GstPad *pad, *newpad;
    VALUE rpad, rname;
    const gchar *name;

    rb_scan_args(argc, argv, "11", &rpad, &rname);
    
    pad = RGST_PAD(rpad);
    name = (rname != Qnil) ? RVAL2CSTR(rname) : gst_pad_get_name(pad);
    newpad = gst_element_add_ghost_pad(RGST_ELEMENT(self),
                                       pad, name);
    return newpad != NULL
        ? RGST_PAD_NEW(newpad)
        : Qnil;
}

/*
 *  Constant: FLAG_COMPLEX
 *  This element is complex (for some def.) and generally requires a cothread.
 */
static VALUE constFlagComplex = INT2FIX(GST_ELEMENT_COMPLEX);

/*
 *  Constant: FLAG_DECOUPLED 
 *  Input and output pads aren't directly coupled to each other 
 *  (examples: queues, multi-output async readers, etc...).
 */
static VALUE constFlagDecoupled = INT2FIX(GST_ELEMENT_DECOUPLED);

/*
 *  Constant: FLAG_THREAD_SUGGESTED
 *  This element should be placed in a thread if at all possible.
 */
static VALUE constFlagThreadSuggested = INT2FIX(GST_ELEMENT_THREAD_SUGGESTED);

/*
 *  Constant: FLAG_INFINITE_LOOP 
 *  This element, for some reason, has a loop function that performs
 *  an infinite loop without calls to Gst::Element#sched_yield.
 */
static VALUE constFlagInfiniteLoop = INT2FIX(GST_ELEMENT_INFINITE_LOOP);

/*
 *  Constant: FLAG_NEW_LOOPFUNC 
 *  There is a new lookfunction ready for placement.
 */
static VALUE constFlagNewLoopfunc = INT2FIX(GST_ELEMENT_NEW_LOOPFUNC);

/*
 *  Constant: FLAG_EVENT_AWARE 
 *  The element can handle events.
 */
static VALUE constFlagEventAware = INT2FIX(GST_ELEMENT_EVENT_AWARE);

/*
 *  Constant: FLAG_USE_THREADSAFE_PROPERTIES
 *  Uses threadsafe property acessors (get/set). 
 */
static VALUE constFlagThreadsafeProperties = INT2FIX(GST_ELEMENT_USE_THREADSAFE_PROPERTIES);

/*
 *  Method: complex? -> aBoolean 
 *  Checks if the Gst::Element::FLAG_COMPLEX flag is set on the object.
 */
static VALUE rb_gst_element_is_complex(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagComplex);
}

/*
 *  Method: decoupled? -> aBoolean 
 *  Checks if the Gst::Element::FLAG_DECOUPLED flag is set on the object.
 */
static VALUE rb_gst_element_is_decoupled(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagDecoupled);
}

/*
 *  Method: thread_suggested? -> aBoolean 
 *  Checks if the Gst::Element::FLAG_THREAD_SUGGESTED flag is set on the 
 *  object.
 */
static VALUE rb_gst_element_is_thread_suggested(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagThreadSuggested);
}

/*
 *  Method: has_infinite_loop? -> aBoolean 
 *  Checks if the Gst::Element::FLAG_INFINITE_LOOP flag is set on the object.
 */
static VALUE rb_gst_element_has_infinite_loop(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagInfiniteLoop);
}

/*
 *  Method: has_new_loopfunc? -> aBoolean 
 *  Checks if the Gst::Element::FLAG_NEW_LOOPFUNC flag is set on the object.
 */
static VALUE rb_gst_element_has_new_loopfunc(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagNewLoopfunc);
}

/*
 *  Method: event_aware? -> aBoolean 
 *  Checks if the Gst::Element::FLAG_EVENT_AWARE flag is set on the object.
 */
static VALUE rb_gst_element_is_event_aware(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagEventAware);
}

/*
 *  Method: use_threadsafe_properties? -> aBoolean 
 *  Checks if the Gst::Element::FLAG_USE_THREADSAFE_PROPERTIES flag 
 *  is set on the object.
 */
static VALUE rb_gst_element_use_threadsafe_properties(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, 
                      constFlagThreadsafeProperties);
}

/*
 *  Method: indexable? -> aBoolean
 *  Checks if the element can be indexed.
 */
static VALUE rb_gst_element_is_indexable(self)
    VALUE self;
{
    GstElement *element = RGST_ELEMENT(self);
    return CBOOL2RVAL(gst_element_is_indexable(element));
}

/*
 *  Method: query(aQueryType, aFormat = Gst::Format::DEFAULT) -> aFixnum
 *
 *  Performs a query on the element.
 *
 *  Meaningful query types are:
 *      - Gst::QueryType::TOTAL
 *      - Gst::QueryType::POSITION
 *      - Gst::QueryType::LATENCY
 *      - Gst::QueryType::JITTER
 *      - Gst::QueryType::START
 *      - Gst::QueryType::SEGMENT_END
 *      - Gst::QueryType::RATE
 *
 *  Meaningful formats are:
 *      - Gst::Format::DEFAULT
 *      - Gst::Format::BYTES
 *      - Gst::Format::TIME
 *      - Gst::Format::BUFFERS
 *      - Gst::Format::PERCENT
 *      - Gst::Format::UNITS
 *
 *  Returns a fixnum, or nil if the query could not be performed.
 */
static VALUE rb_gst_element_query(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE query_type, format;
    GstFormat gstformat;
    gint64 value;

    rb_scan_args(argc, argv, "11", &query_type, &format);
    gstformat = NIL_P(format) ? GST_FORMAT_DEFAULT : FIX2INT(format);
    
    if (gst_element_query(RGST_ELEMENT(self), FIX2INT(query_type),
                          &gstformat, &value))
    {
        format = INT2FIX(gstformat);        
        return ULL2NUM(value);
    }
    return Qnil;
}

/*
 *  Method: send_event(anEvent) -> aBoolean
 *
 *  Sends an event to an element, through a Gst::Event object. 
 *  If the element doesn't implement an event handler, the event will be 
 *  forwarded to a random sink pad.
 *
 *  Returns true if the request event was successfully handled, false
 *  otherwise.
 */
static VALUE rb_gst_element_send_event(self, event)
    VALUE self, event;
{
    return CBOOL2RVAL(gst_element_send_event(RGST_ELEMENT(self),
                                             RGST_EVENT(event)));
}

/*
 *  Creates the Gst::Element class.
 */
void Init_gst_element(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_ELEMENT, "Element", mGst); 

    /*
     *  States
     */
   
    rb_define_method(c, "set_state", rb_gst_element_set_state, 1);
    rb_define_method(c, "state",     rb_gst_element_get_state, 0);
    rb_define_alias(c, "state=", "set_state");
    
    rb_define_method(c, "stop",  rb_gst_element_stop,  0);
    rb_define_method(c, "ready", rb_gst_element_ready, 0);
    rb_define_method(c, "pause", rb_gst_element_pause, 0);
    rb_define_method(c, "play",  rb_gst_element_play,  0);

    rb_define_method(c, "stopped?", rb_gst_element_is_stopped, 0);
    rb_define_method(c, "ready?",   rb_gst_element_is_ready,   0);
    rb_define_method(c, "paused?",  rb_gst_element_is_paused,  0);
    rb_define_method(c, "playing?", rb_gst_element_is_playing, 0);
    
    rb_define_method(c, "wait_state_change", 
                     rb_gst_element_wait_state_change, 0);
    
    rb_define_const(c, "STATE_NULL",    constStateNull);
    rb_define_const(c, "STATE_READY",   constStateReady);
    rb_define_const(c, "STATE_PAUSED",  constStatePaused);
    rb_define_const(c, "STATE_PLAYING", constStatePlaying);
    rb_define_const(c, "STATE_FAILURE", constStateFailure);
    rb_define_const(c, "STATE_SUCCESS", constStateSuccess);
    rb_define_const(c, "STATE_ASYNC",   constStateAsync);

    rb_define_method(c, "eos", rb_gst_element_set_eos, 0);

    /*
     *  Properties
     */

    rb_define_method(c, "set_property",  rb_gst_element_set_property,  2);
    rb_define_method(c, "get_property",  rb_gst_element_get_property,  1);
    rb_define_method(c, "each_property", rb_gst_element_each_property, 0);

    /*
     *  Scheduler
     */

    rb_define_method(c, "sched_yield",     rb_gst_element_sched_yield,     0);
    rb_define_method(c, "sched_interrupt", rb_gst_element_sched_interrupt, 0);

    /*
     *  Links
     */

    rb_define_method(c, "link", rb_gst_element_link, 1);
    rb_define_alias(c, ">>", "link");

    /*
     *  Clock interaction
     */

    rb_define_method(c, "provides_clock?", rb_gst_element_provides_clock, 0);
    rb_define_method(c, "requires_clock?", rb_gst_element_requires_clock, 0);
    rb_define_method(c, "clock", rb_gst_element_get_clock, 0);

    /*
     *  Pads
     */

    rb_define_method(c, "each_pad", rb_gst_element_each_pad, 0);
    rb_define_method(c, "pads",     rb_gst_element_get_pads, 0);

    rb_define_method(c, "get_pad", rb_gst_element_get_pad, 1);

    rb_define_method(c, "link_pads",   rb_gst_element_link_pads,   1);
    rb_define_method(c, "unlink_pads", rb_gst_element_unlink_pads, 1);

    rb_define_method(c, "add_ghost_pad", rb_gst_element_add_ghost_pad, -1);

    /*
     *  Flags
     */

    rb_define_method(c, "complex?",   rb_gst_element_is_complex, 0);
    rb_define_method(c, "decoupled?", rb_gst_element_is_decoupled, 0);
    rb_define_method(c, "thread_suggested?",  
                     rb_gst_element_is_thread_suggested, 0);
    rb_define_method(c, "has_infinite_loop?", 
                     rb_gst_element_has_infinite_loop, 0);
    rb_define_method(c, "has_new_loopfunc?",  
                     rb_gst_element_has_new_loopfunc, 0);
    rb_define_method(c, "event_aware?", 
                     rb_gst_element_is_event_aware, 0);
    rb_define_method(c, "use_threadsafe_properties?", 
                     rb_gst_element_use_threadsafe_properties, 0);

    rb_define_const(c, "FLAG_COMPLEX",         constFlagComplex); 
    rb_define_const(c, "FLAG_DECOUPLED",       constFlagDecoupled); 
    rb_define_const(c, "FLAG_THEAD_SUGGESTED", constFlagThreadSuggested); 
    rb_define_const(c, "FLAG_INFINITE_LOOP",   constFlagInfiniteLoop); 
    rb_define_const(c, "FLAG_NEW_LOOPFUNC",    constFlagNewLoopfunc); 
    rb_define_const(c, "FLAG_EVENT_AWARE",     constFlagEventAware); 
    rb_define_const(c, "FLAG_USE_THREADSAFE_PROPERTIES", 
                    constFlagThreadsafeProperties); 

    rb_define_method(c, "indexable?", rb_gst_element_is_indexable, 0);
    rb_define_method(c, "query", rb_gst_element_query, -1);
    rb_define_method(c, "send_event", rb_gst_element_send_event, 1);
}

