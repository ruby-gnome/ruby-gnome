/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 * Copyright (C) 2007, 2008 Ruby-GNOME2 Project Team
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
#include "rbgst-private.h"

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_IO_H
#  include <io.h>
#  define pipe(phandles) _pipe(phandles, 128, _O_BINARY)
#endif
#include <fcntl.h>
#include <errno.h>

#define SELF(self) RVAL2GST_ELEMENT(self)

#define GST_STATE_CHANGE_RETURN2RVAL(object) \
    (GENUM2RVAL(object, GST_TYPE_STATE_CHANGE_RETURN))

#define NOTIFY_MESSAGE "R"
#define NOTIFY_MESSAGE_SIZE 1

typedef struct _SetStateData {
    GstStateChangeReturn result;
    GstState state;
} SetStateData;

typedef struct _GetStateData {
    GstStateChangeReturn result;
    GstState state;
    GstState pending;
    GstClockTime timeout;
} GetStateData;

typedef struct _QueryData {
    gboolean result;
    GstQuery *query;
} QueryData;

typedef struct _SendEventData {
    gboolean result;
    GstEvent *event;
} SendEventData;

typedef struct _ThreadData {
    GstElement *element;
    int notify_fd;
    union {
	SetStateData set_state_data;
	GetStateData get_state_data;
	QueryData query_data;
	SendEventData send_event_data;
    } data;
} ThreadData;

static RGConvertTable table = {0};
static VALUE rb_cGstElement;
static ID id_gtype;
static GThreadPool *set_state_thread_pool;
static GThreadPool *get_state_thread_pool;
static GThreadPool *query_thread_pool;
static GThreadPool *send_event_thread_pool;

static void
define_class_if_need(VALUE klass, GType type)
{
    _rbgst_define_class_if_need(klass, type, "Element");
}

static VALUE
instance2robj(gpointer instance)
{
    VALUE klass;
    GType type;

    type = G_TYPE_FROM_INSTANCE(instance);
    klass = GTYPE2CLASS(type);
    define_class_if_need(klass, type);
    return rbgst_object_instance2robj(instance);
}


/* Class: Gst::Element
 * Base class for all pipeline elements.
 */

static void
do_in_thread(GThreadPool *pool, ThreadData *data)
{
    int notify_fds[2];
    char buf[NOTIFY_MESSAGE_SIZE];
    GError *error = NULL;

    if (pipe(notify_fds) != 0)
        rb_sys_fail("failed to create a pipe to synchronize threaded operation");

    data->notify_fd = notify_fds[1];
    g_thread_pool_push(pool, data, &error);
    if (error) {
        close(notify_fds[0]);
        close(notify_fds[1]);
        RAISE_GERROR(error);
    }

    rb_thread_wait_fd(notify_fds[0]);

    read(notify_fds[0], buf, NOTIFY_MESSAGE_SIZE);


    close(notify_fds[0]);
    close(notify_fds[1]);
}

static void
set_state_in_thread(gpointer data, gpointer user_data)
{
    ThreadData *thread_data = (ThreadData *)data;
    SetStateData *set_state_data;

    set_state_data = &(thread_data->data.set_state_data);
    set_state_data->result = gst_element_set_state(thread_data->element,
                                                   set_state_data->state);
    write(thread_data->notify_fd, NOTIFY_MESSAGE, NOTIFY_MESSAGE_SIZE);
}

static VALUE
rb_gst_element_set_state_internal(VALUE self, GstState state)
{
    VALUE result;
    ThreadData *thread_data;
    SetStateData *set_state_data;

    thread_data = g_slice_new(ThreadData);
    thread_data->element = SELF(self);
    set_state_data = &(thread_data->data.set_state_data);
    set_state_data->state = state;

    do_in_thread(set_state_thread_pool, thread_data);

    result = GST_STATE_CHANGE_RETURN2RVAL(set_state_data->result);

    g_slice_free(ThreadData, thread_data);

    return result;
}

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
 * Returns: a code (see Gst::Element::StateChangeReturn).
 */
static VALUE
rb_gst_element_set_state(VALUE self, VALUE state)
{
    return rb_gst_element_set_state_internal(self,
					     RVAL2GENUM(state, GST_TYPE_STATE));
}

static void
get_state_in_thread(gpointer data, gpointer user_data)
{
    ThreadData *thread_data = (ThreadData *)data;
    GetStateData *get_state_data;

    get_state_data = &(thread_data->data.get_state_data);
    get_state_data->result = gst_element_get_state(thread_data->element,
						   &(get_state_data->state),
						   &(get_state_data->pending),
						   get_state_data->timeout);
    write(thread_data->notify_fd, NOTIFY_MESSAGE, NOTIFY_MESSAGE_SIZE);
}

/* Method: get_state(timeout=nil)
 */
static VALUE
rb_gst_element_get_state(int argc, VALUE *argv, VALUE self)
{
    VALUE result, timeout;
    ThreadData *thread_data;
    GetStateData *get_state_data;

    rb_scan_args(argc, argv, "01", &timeout);

    thread_data = g_slice_new(ThreadData);
    thread_data->element = SELF(self);
    get_state_data = &(thread_data->data.get_state_data);
    if (NIL_P(timeout))
	get_state_data->timeout = GST_CLOCK_TIME_NONE;
    else
	get_state_data->timeout = NUM2ULL(timeout);

    do_in_thread(get_state_thread_pool, thread_data);

    result = rb_ary_new3(3,
			 GST_STATE_CHANGE_RETURN2RVAL(get_state_data->result),
			 GST_STATE2RVAL(get_state_data->state),
			 GST_STATE2RVAL(get_state_data->pending));

    g_slice_free(ThreadData, thread_data);

    return result;
}

/*
 * Method: stop
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_NULL.
 *
 * Returns: a code (see Gst::Element::StateChangeReturn).
 */
static VALUE
rb_gst_element_stop(VALUE self)
{
    return rb_gst_element_set_state_internal(self, GST_STATE_NULL);
}

/*
 * Method: ready
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_READY.
 *
 * Returns: a code (see Gst::Element::StateChangeReturn).
 */
static VALUE
rb_gst_element_ready(VALUE self)
{
    return rb_gst_element_set_state_internal(self, GST_STATE_READY);
}

/*
 * Method: pause
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_PAUSED.
 *
 * Returns: a code (see Gst::Element::StateChangedReturn).
 */
static VALUE
rb_gst_element_pause(VALUE self)
{
    return rb_gst_element_set_state_internal(self, GST_STATE_PAUSED);
}

/*
 * Method: play
 *
 * This method calls Gst::Element#set_state with Gst::Element::STATE_PLAYING.
 *
 * Returns: a code (see Gst::Element::StateChangedReturn).
 */
static VALUE
rb_gst_element_play(VALUE self)
{
    return rb_gst_element_set_state_internal(self, GST_STATE_PLAYING);
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
rb_gst_element_link(VALUE self, VALUE other_element)
{
    GstElement *element1, *element2;

    element1 = SELF(self);
    element2 = SELF(other_element);
    return gst_element_link(element1, element2) == TRUE ? other_element : Qnil;
}

/*
 * Method: unlink(element)
 * element: a Gst::Element object.
 *
 * Unlinks this element (source) to the provided element (destination). 
 *
 * The method looks for all source pads of the source elemnt that are
 * linked to the destination element and unlinkes them.
 *
 */
static VALUE
rb_gst_element_unlink(VALUE self, VALUE other_element)
{
    GstElement *element1, *element2;

    element1 = SELF(self);
    element2 = SELF(other_element);
    gst_element_unlink(element1, element2);
    return self;
}

/*
 * Method: link_filtered(element, caps)
 * element: a Gst::Element object.
 * caps: a Gst::Caps object.
 *
 * Links this element (source) to the provided element (destination), 
 * filtered by the given caps.
 *
 * The method looks for existing pads and request pads that 
 * aren't linked yet. If multiple links are possible, only one 
 * is established.
 *
 * Returns: the destination element, or nil if the link failed.
 */
static VALUE
rb_gst_element_link_filtered(VALUE self, VALUE other_element, VALUE rcaps)
{
    GstElement *element1, *element2;
    GstCaps *caps;
    
    element1 = SELF(self);
    element2 = SELF(other_element);
    caps = RGST_CAPS(rcaps);
    return gst_element_link_filtered(element1, element2, caps)
        ? other_element 
        : Qnil;
}

/* Method: requires_clock?
 * Returns: true if the element requires a clock, false otherwise.
 */
static VALUE
rb_gst_element_requires_clock(VALUE self)
{
    return CBOOL2RVAL(gst_element_requires_clock(SELF(self)));
}

/* Method: provides_clock?
 * Returns: true if the element provides a clock, false otherwise.
 */
static VALUE
rb_gst_element_provides_clock(VALUE self)
{
    return CBOOL2RVAL(gst_element_provides_clock(SELF(self)));
}

/* Method: clock
 * Returns: the clock of the element (as a Gst::Clock object), or nil
 * if this element does not provide a clock.
 */
static VALUE
rb_gst_element_get_clock(VALUE self)
{
    GstClock *clock;

    clock = gst_element_get_clock(SELF(self));
    return clock != NULL ? RGST_CLOCK_NEW(clock)
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
rb_gst_element_set_clock(VALUE self, VALUE clock)
{
    gst_element_set_clock(SELF(self), RVAL2GST_CLOCK(clock));
    return self;
}

typedef void (*EachPadCallback)(VALUE pad, VALUE user_data);
typedef struct _EachPadData {
    VALUE self;
    EachPadCallback callback;
    VALUE user_data;
    GstIterator *iterator;
} EachPadData;

static VALUE
rb_gst_element_each_pad_body(VALUE user_data)
{
    GstPad *pad;
    gboolean done = FALSE;
    EachPadData *data;

    data = (EachPadData *)user_data;

    while (!done) {
        switch (gst_iterator_next(data->iterator, (gpointer)&pad)) {
	  case GST_ITERATOR_OK:
	    data->callback(GST_PAD2RVAL(pad), data->user_data);
	    gst_object_unref(pad);
	    break;
	  case GST_ITERATOR_RESYNC:
	    gst_iterator_resync(data->iterator);
	    break;
	  case GST_ITERATOR_ERROR:
	    rb_raise(rb_eIndexError, "Pad iteration failed");
	    break;
	  case GST_ITERATOR_DONE:
	    done = TRUE;
	    break;
        }
    }

    return Qnil;
}

static VALUE
rb_gst_element_each_pad_ensure(VALUE user_data)
{
    EachPadData *data = (EachPadData *)user_data;

    gst_iterator_free(data->iterator);
    return Qnil;
}

static VALUE
rb_gst_element_each_pad_with_callback(VALUE self,
				      EachPadCallback callback,
				      VALUE user_data)
{
    EachPadData data;

    data.self = self;
    data.callback = callback;
    data.user_data = user_data;
    data.iterator = gst_element_iterate_pads(SELF(self));
    return rb_ensure(rb_gst_element_each_pad_body, (VALUE)(&data),
		     rb_gst_element_each_pad_ensure, (VALUE)(&data));
}

/*
 * Method: each_pad { |pad| ... }
 *
 * Calls the block for each pad associated with the element, passing a
 * reference to the Gst::Pad as parameter. Throws an IndexError on errors.
 * Note that the elements might be yielded multiple times if the iterator had
 * to resync.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_element_each_pad(VALUE self)
{
    rb_gst_element_each_pad_with_callback(self, (EachPadCallback)rb_yield, Qnil);
    return Qnil;
}

static void
collect_pad(VALUE pad, VALUE pads)
{
    rb_ary_push(pads, pad);
}

static VALUE
rb_gst_element_get_pads(VALUE self)
{
    VALUE pads;

    pads = rb_ary_new();
    rb_gst_element_each_pad_with_callback(self, collect_pad, pads);
    return pads;
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
rb_gst_element_get_pad(VALUE self, VALUE pad_name)
{
    GstPad *pad = gst_element_get_pad(SELF(self),
                                       RVAL2CSTR(pad_name));

    return pad != NULL ? RGST_PAD_NEW(pad)
        : Qnil;
}

/*
 * Method: link_pads(source_pad_name, element, destination_pad_name)
 * element: a Gst::Element.
 *
 * Links the source_pad_name pad of the current element to the
 * destination_pad_name pad of the destination element, returning
 * true on success.
 *
 * Returns: true on success, false on failure.
 */
static VALUE
rb_gst_element_link_pads(VALUE self, VALUE source_pad_name,
			 VALUE other_element, VALUE destination_pad_name)
{
    return CBOOL2RVAL(gst_element_link_pads(SELF(self),
					    RVAL2CSTR(source_pad_name),
					    SELF(other_element),
					    RVAL2CSTR(destination_pad_name)));
}

/*
 * Method: unlink_pads(source_pad_name, element, destination_pad_name)
 * element: a Gst::Element.
 *
 * Unlinks the source_pad_name named pad of the current element from the
 * destination_pad_name named pad of the destination element.
 *
 * Returns: self.
 */
static VALUE
rb_gst_element_unlink_pads(VALUE self, VALUE source_pad_name,
			   VALUE other_element, VALUE destination_pad_name)
{
    gst_element_unlink_pads(SELF(self),
			    RVAL2CSTR(source_pad_name),
			    SELF(other_element),
			    RVAL2CSTR(destination_pad_name));
    return self;
}

/* Method: indexable?
 * Returns: true if the element can be indexed, false otherwise.
 */
static VALUE
rb_gst_element_is_indexable(VALUE self)
{
    return CBOOL2RVAL(gst_element_is_indexable(SELF(self)));
}

static void
query_in_thread(gpointer data, gpointer user_data)
{
    ThreadData *thread_data = (ThreadData *)data;
    QueryData *query_data;

    query_data = &(thread_data->data.query_data);
    query_data->result = gst_element_query(thread_data->element,
					   query_data->query);
    write(thread_data->notify_fd, NOTIFY_MESSAGE, NOTIFY_MESSAGE_SIZE);
}

/*
 * Method: query(query)
 * query: a query type (see Gst::Query).
 *
 * Performs a query on the element.
 *
 * Returns: true if the query is performed, false otherwise.
 */
static VALUE
rb_gst_element_query(VALUE self, VALUE query)
{
    VALUE result;
    ThreadData *thread_data;
    QueryData *query_data;

    thread_data = g_slice_new(ThreadData);
    thread_data->element = SELF(self);
    query_data = &(thread_data->data.query_data);
    query_data->query = RVAL2GST_QUERY(query);

    do_in_thread(query_thread_pool, thread_data);

    result = CBOOL2RVAL(query_data->result);

    g_slice_free(ThreadData, thread_data);

    return result;
}

static void
send_event_in_thread(gpointer data, gpointer user_data)
{
    ThreadData *thread_data = (ThreadData *)data;
    SendEventData *send_event_data;

    send_event_data = &(thread_data->data.send_event_data);
    send_event_data->result = gst_element_send_event(thread_data->element,
						     send_event_data->event);
    write(thread_data->notify_fd, NOTIFY_MESSAGE, NOTIFY_MESSAGE_SIZE);
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
rb_gst_element_send_event(VALUE self, VALUE event)
{
    VALUE result;
    ThreadData *thread_data;
    SendEventData *send_event_data;

    thread_data = g_slice_new(ThreadData);
    thread_data->element = SELF(self);
    send_event_data = &(thread_data->data.send_event_data);
    send_event_data->event = RVAL2GST_EVENT(event);

    gst_event_ref(send_event_data->event);
    do_in_thread(send_event_thread_pool, thread_data);

    result = CBOOL2RVAL(send_event_data->result);

    g_slice_free(ThreadData, thread_data);

    return result;
}

/*
 * Method: base_time
 *
 * Queries the element's time.
 *
 * Returns:  the current stream time (in nanoseconds) in Gst::Element::STATE_PLAYING, the 
 * element base time in Gst::Element::STATE_PAUSED, or -1 otherwise.
 */
static VALUE
rb_gst_element_get_base_time(VALUE self)
{
    return ULL2NUM(gst_element_get_base_time(SELF(self)));
}

/*
 * Method: set_base_time(time)
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
rb_gst_element_set_base_time(VALUE self, VALUE time)
{
    gst_element_set_base_time(SELF(self), NUM2ULL(time));
    return Qnil;
}

/* 
 * Method: index
 *
 * Gets the index from the element. 
 *
 * Returns: a Gst::Index or nil when no index was set on the element.
 */
static VALUE
rb_gst_element_get_index(VALUE self)
{
    GstIndex *index = gst_element_get_index(SELF(self));

    return index != NULL ? RGST_INDEX_NEW(index)
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
rb_gst_element_set_index(VALUE self, VALUE index)
{
    gst_element_set_index(SELF(self), RGST_INDEX(index));
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
rb_gst_element_get_static_pad(VALUE self, VALUE name)
{
    GstPad *pad =
        gst_element_get_static_pad(SELF(self), RVAL2CSTR(name));
    return pad != NULL ? RGST_PAD_NEW(pad) : Qnil;
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
rb_gst_element_get_request_pad(VALUE self, VALUE name)
{
    GstPad *pad =
        gst_element_get_request_pad(SELF(self), RVAL2CSTR(name));
    return pad != NULL ? GST_PAD2RVAL_UNREF(pad) : Qnil;
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
rb_gst_element_release_request_pad(VALUE self, VALUE pad)
{
    gst_element_release_request_pad(SELF(self), RGST_PAD(pad));
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
rb_gst_element_add_pad(VALUE self, VALUE pad)
{
    gst_element_add_pad(SELF(self), RGST_PAD(pad));
    G_CHILD_ADD(self, pad);
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
rb_gst_element_remove_pad(VALUE self, VALUE pad)
{
    gst_element_remove_pad(SELF(self), RGST_PAD(pad));
    return self;
}

/*
 * Class Method: get_pad_template(name)
 * name: the name of the Gst::PadTemplate to get.
 *
 * Retrieves a Gst::PadTemplate from this element with the given name.
 *
 * Returns: the Gst::PadTemplate with the given name, or nil if none was found.
 */
static VALUE
rb_gst_element_get_pad_template(VALUE self, VALUE name)
{
    GstElementClass *element_class;
    GstPadTemplate *template;

    element_class = g_type_class_peek(CLASS2GTYPE(self));
    template = gst_element_class_get_pad_template(element_class,
                                                  RVAL2CSTR(name));
    return template == NULL ? Qnil : RGST_PAD_TEMPLATE_NEW(template);
}

/*
 * Class Method: pad_templates
 *
 * Retrieves a list of pad templates associated with the element.
 * 
 * Returns: an Array of Gst::PadTemplate objects.
 */
static VALUE
rb_gst_element_get_pad_templates(VALUE self)
{
    GstElementClass *element_class;
    GList *list, *node;
    VALUE ary;

    ary = rb_ary_new();

    element_class = g_type_class_peek(CLASS2GTYPE(self));
    list = gst_element_class_get_pad_template_list(element_class);
    for (node = list; node != NULL; node = g_list_next(node)) {
        rb_ary_push(ary, RGST_PAD_TEMPLATE_NEW(node->data));
    }
    g_list_free(list);
    return ary;
}

/*
 * Class Method: each_pad_template { |pad_template| ... }
 *
 * Calls the block for each pad template associated with the element,
 * passing a reference to a Gst::PadTemplate object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_element_each_pad_template(VALUE self)
{
    return rb_ary_yield(rb_gst_element_get_pad_templates(self));
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
rb_gst_element_seek(VALUE self, VALUE rate, VALUE format, VALUE flags,
                    VALUE cur_type, VALUE cur, VALUE stop_type, VALUE stop)
{
    return CBOOL2RVAL(gst_element_seek(SELF(self),
                                       NUM2DBL(rate),
                                       RVAL2GENUM(format, GST_TYPE_FORMAT),
                                       RVAL2GFLAGS(flags, GST_TYPE_SEEK_FLAGS),
                                       RVAL2GENUM(cur_type, GST_TYPE_SEEK_TYPE),
                                       NUM2LL(cur),
                                       RVAL2GENUM(stop_type, GST_TYPE_SEEK_TYPE),
                                       NUM2LL(stop)));
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
rb_gst_element_is_locked_state(VALUE self)
{
    return CBOOL2RVAL(gst_element_is_locked_state(SELF(self)));
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
rb_gst_element_set_locked_state(VALUE self, VALUE state)
{
    gst_element_set_locked_state(SELF(self), RVAL2CBOOL(state));
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
rb_gst_element_sync_state_with_parent(VALUE self)
{
    return
        CBOOL2RVAL(gst_element_sync_state_with_parent(SELF(self)));
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
rb_gst_element_no_more_pads(VALUE self)
{
    gst_element_no_more_pads(SELF(self));
    return self;
}

static VALUE 
rb_gst_element_found_tag_sig(guint num, const GValue *values)
{
    GstElement *element, *source;
    GstTagList *tag_list;

    element = g_value_get_object(&values[0]);
    source = g_value_get_object(&values[1]);
    tag_list = g_value_get_boxed(&values[2]);

    return rb_ary_new3(3,
                       GST_ELEMENT2RVAL(element),
                       GST_ELEMENT2RVAL(source),
                       GST_STRUCT2RVAL(tag_list));
}

static void
initialize_thread_pool(GThreadPool **pool, GFunc function)
{
    GError *error = NULL;

    *pool = g_thread_pool_new(function, NULL, -1, FALSE, &error);
    if (error)
	RAISE_GERROR(error);
}

void
Init_gst_element(void)
{
    table.type = GST_TYPE_ELEMENT;
    table.instance2robj = instance2robj;
    RG_DEF_CONVERSION(&table);

    id_gtype = rb_intern("gtype");

    initialize_thread_pool(&set_state_thread_pool, set_state_in_thread);
    initialize_thread_pool(&get_state_thread_pool, get_state_in_thread);
    initialize_thread_pool(&query_thread_pool, query_in_thread);
    initialize_thread_pool(&send_event_thread_pool, send_event_in_thread);

    rb_cGstElement = G_DEF_CLASS(GST_TYPE_ELEMENT, "Element", mGst);

    rb_define_singleton_method(rb_cGstElement, "get_pad_template",
                               rb_gst_element_get_pad_template, 1);
    rb_define_singleton_method(rb_cGstElement, "pad_templates",
                               rb_gst_element_get_pad_templates, 0);
    rb_define_singleton_method(rb_cGstElement, "each_pad_template",
                               rb_gst_element_each_pad_template, 0);

    rb_define_method(rb_cGstElement, "set_state", rb_gst_element_set_state, 1);
    rb_define_method(rb_cGstElement, "get_state", rb_gst_element_get_state, -1);
    rb_define_method(rb_cGstElement, "stop", rb_gst_element_stop, 0);
    rb_define_method(rb_cGstElement, "ready", rb_gst_element_ready, 0);
    rb_define_method(rb_cGstElement, "pause", rb_gst_element_pause, 0);
    rb_define_method(rb_cGstElement, "play", rb_gst_element_play, 0);
    rb_define_method(rb_cGstElement, "link", rb_gst_element_link, 1);
    rb_define_alias(rb_cGstElement, ">>", "link");
    rb_define_method(rb_cGstElement, "unlink", rb_gst_element_unlink, 1);
    rb_define_method(rb_cGstElement, "link_filtered", rb_gst_element_link_filtered, 2);
    rb_define_method(rb_cGstElement, "provides_clock?", rb_gst_element_provides_clock, 0);
    rb_define_alias(rb_cGstElement, "provide_clock?", "provides_clock?");
    rb_define_method(rb_cGstElement, "requires_clock?", rb_gst_element_requires_clock, 0);
    rb_define_alias(rb_cGstElement, "require_clock?", "requires_clock?");
    rb_define_method(rb_cGstElement, "clock", rb_gst_element_get_clock, 0);
    rb_define_method(rb_cGstElement, "set_clock", rb_gst_element_set_clock, 1);
    rb_define_method(rb_cGstElement, "base_time", rb_gst_element_get_base_time, 0);
    rb_define_method(rb_cGstElement, "set_base_time", rb_gst_element_set_base_time, 1);
    rb_define_method(rb_cGstElement, "each_pad", rb_gst_element_each_pad, 0);
    rb_define_method(rb_cGstElement, "pads", rb_gst_element_get_pads, 0);
    rb_define_method(rb_cGstElement, "get_pad", rb_gst_element_get_pad, 1);
    rb_define_alias(rb_cGstElement, "[]", "get_pad");
    rb_define_method(rb_cGstElement, "get_static_pad", rb_gst_element_get_static_pad, 1);
    rb_define_method(rb_cGstElement, "get_request_pad", rb_gst_element_get_request_pad, 1);
    rb_define_method(rb_cGstElement, "release_request_pad",
                     rb_gst_element_release_request_pad, 1);
    rb_define_method(rb_cGstElement, "link_pads", rb_gst_element_link_pads, 3);
    rb_define_method(rb_cGstElement, "unlink_pads", rb_gst_element_unlink_pads, 3);
    rb_define_method(rb_cGstElement, "add_pad", rb_gst_element_add_pad, 1);
    rb_define_method(rb_cGstElement, "remove_pad", rb_gst_element_remove_pad, 1);
    rb_define_method(rb_cGstElement, "indexable?", rb_gst_element_is_indexable, 0);
    rb_define_method(rb_cGstElement, "query", rb_gst_element_query, 1);
    rb_define_method(rb_cGstElement, "send_event", rb_gst_element_send_event, 1);
    rb_define_method(rb_cGstElement, "seek", rb_gst_element_seek, 7);
    rb_define_method(rb_cGstElement, "index", rb_gst_element_get_index, 0);
    rb_define_method(rb_cGstElement, "set_index", rb_gst_element_set_index, 1);
    rb_define_method(rb_cGstElement, "locked_state?", rb_gst_element_is_locked_state, 0);
    rb_define_method(rb_cGstElement, "set_locked_state", rb_gst_element_set_locked_state,
                     1);
    rb_define_method(rb_cGstElement, "sync_state_with_parent",
                     rb_gst_element_sync_state_with_parent, 0);
    rb_define_method(rb_cGstElement, "no_more_pads", rb_gst_element_no_more_pads, 0);

    G_DEF_SETTERS(rb_cGstElement);

    G_DEF_CLASS(GST_TYPE_STATE, "State", mGst);
    G_DEF_CONSTANTS(mGst, GST_TYPE_STATE, "GST_");
    G_DEF_CLASS(GST_TYPE_STATE_CHANGE_RETURN, "StateChangeReturn", mGst);
    G_DEF_CONSTANTS(mGst, GST_TYPE_STATE_CHANGE_RETURN, "GST_");
    G_DEF_CLASS(GST_TYPE_STATE_CHANGE, "StateChange", mGst);
    G_DEF_CONSTANTS(mGst, GST_TYPE_STATE_CHANGE, "GST_");
    G_DEF_CLASS(GST_TYPE_ELEMENT_FLAGS, "Flags", rb_cGstElement);
    G_DEF_CONSTANTS(rb_cGstElement, GST_TYPE_ELEMENT_FLAGS, "GST_ELEMENT_");

    /*
     * TODO:
     * gst_element_clock_wait() 
     */

    G_DEF_SIGNAL_FUNC(rb_cGstElement, "found-tag", 
                      (GValToRValSignalFunc)rb_gst_element_found_tag_sig);
}
