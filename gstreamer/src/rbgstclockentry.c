
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
 *  Class: Gst::ClockEntry
 *
 *  A wrapper for GstClockID, used to create and start timers
 *  on Gst::Clock objects.
 */

static GstClockEntry* clock_entry_copy(const GstClockEntry* clock_entry) {
    GstClockEntry* new_clock_entry;
    g_return_val_if_fail (clock_entry != NULL, NULL);
    new_clock_entry = g_new(GstClockEntry, sizeof(GstClockEntry));
    *new_clock_entry = *clock_entry;
    return new_clock_entry;
}

GType gst_clock_entry_get_type(void) {
    static GType our_type = 0;
    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstClockID",
            (GBoxedCopyFunc)clock_entry_copy,
            (GBoxedFreeFunc)g_free);
    }
    return our_type;
}

/*
 *  Class method: new(aClock, aTime, anInterval=nil) -> aClockEntry
 *
 *  Creates a new Gst::ClockEntry object based on the given Gst::Clock.
 *
 *  Two types of Gst::ClockEntry objects can be created:
 *
 *      * One-shot: if anInterval is ommited or nil, the entry will trigger 
 *        a single shot notification, at the requested aTime (in nanoseconds);
 *      * Periodic: if anInterval is not nil, the timer entry will trigger a
 *        periodic notification, starting at aTime (in nanoseconds), and 
 *        be fired with the given interval (also in nanoseconds).
 *
 *  The timer will be issued after Gst::ClockEntry#wait 
 *  or Gst::ClockEntry#wait_async.
 */
static VALUE rb_gst_clock_entry_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    VALUE clock, time, interval;
    GstClockID id;

    rb_scan_args(argc, argv, "21", &clock, &time, &interval);

    // Single-shot.
    if (NIL_P(interval)) {
        id = gst_clock_new_single_shot_id(RGST_CLOCK(clock),
                                          NUM2ULL(time));
    }
    // Periodic.
    else {
        id = gst_clock_new_periodic_id(RGST_CLOCK(clock),
                                       NUM2ULL(time),
                                       NUM2ULL(interval));
    }

    G_INITIALIZE(self, GST_CLOCK_ENTRY(id)); 
    return Qnil;
}

/*
 *  Method: single_shot? -> aBoolean
 *
 *  Checks if the entry is single-shot.
 */
static VALUE rb_gst_clock_entry_is_single_shot(self)
    VALUE self;
{
    return CBOOL2RVAL(GST_CLOCK_ENTRY_TYPE(RGST_CLOCK_ENTRY(self)) == GST_CLOCK_ENTRY_SINGLE);
}

/*
 *  Method: periodic? -> aBoolean
 *
 *  Checks if the entry is periodic.
 */
static VALUE rb_gst_clock_entry_is_periodic(self)
    VALUE self;
{
    return CBOOL2RVAL(GST_CLOCK_ENTRY_TYPE(RGST_CLOCK_ENTRY(self)) == GST_CLOCK_ENTRY_PERIODIC);
}

/*
 *  Method: unschedule -> self
 *
 *  Cancel an outstanding async notification request.
 */
static VALUE rb_gst_clock_entry_unschedule(self)
    VALUE self;
{
    gst_clock_id_unschedule(RGST_CLOCK_ENTRY(self));
    return self;
}

/*
 *  Method: wait -> aFixnum
 *
 *  Performs a blocking wait on the entry.
 *
 *  Returns a code (as a Fixnum):
 *      * Gst::Clock::STOPPED;
 *      * Gst::Clock::TIMEOUT;
 *      * Gst::Clock::EARLY;
 *      * Gst::Clock::ERROR;
 *      * Gst::Clock::UNSUPPORTED.
 */
static VALUE rb_gst_clock_entry_wait(self)
    VALUE self;
{
    return INT2FIX(gst_clock_id_wait(RGST_CLOCK_ENTRY(self), NULL));
}

struct __callback {
    GstClockID id;
    VALUE callback;
};

static GSList *__callbacks = NULL;

static struct __callback *__callback_get(id)
    GstClockID id;
{
    GSList *i;
    for (i = __callbacks;
         i != NULL;
         i = g_slist_next(i))
    {
        struct __callback *e = (struct __callback *) i->data;
        if (e->id == id) {
            return e;
        }
    }
    return NULL;
}

static gboolean __callback_dispatcher(clock, time, id, user_data)
    GstClock *clock;
    GstClockTime time;
    GstClockID id;
    gpointer user_data;
{
    struct __callback *e = __callback_get(id);
    assert(e != NULL);
    return RVAL2CBOOL(rb_funcall(e->callback, 
                                 rb_intern("call"), 
                                 3, 
                                 RGST_CLOCK_NEW(clock),
                                 ULL2NUM(time),
                                 RGST_CLOCK_ENTRY_NEW(id)));
}

/*
 *  Method: wait_async { |aClock, aTime, aEntryClock| ... } -> aFixnum
 *
 *  Register a block code, which will be called passing references
 *  to the Gst::Clock, the time (in nanoseconds) and the Gst::ClockEntry as
 *  parameters.
 *
 *  Returns a code (as a Fixnum):
 *      * Gst::Clock::STOPPED;
 *      * Gst::Clock::TIMEOUT;
 *      * Gst::Clock::EARLY;
 *      * Gst::Clock::ERROR;
 *      * Gst::Clock::UNSUPPORTED.
 */
static VALUE rb_gst_clock_entry_wait_async(self)
    VALUE self;
{
    VALUE ret = Qnil;
    GstClockID id = (GstClockID) RGST_CLOCK_ENTRY(self);
    
    if (__callback_get(id) != NULL) {
        rb_raise(rb_eRuntimeError,
                 "An asynch callback is already registred to this entry.");
    }
    else {
        struct __callback *e = g_malloc(sizeof(struct __callback));
        assert(e != NULL);
        e->id = id;
        e->callback = G_BLOCK_PROC();
        __callbacks = g_slist_append(__callbacks, e);
        ret = INT2FIX(gst_clock_id_wait_async(id,
                                              __callback_dispatcher,
                                              NULL));
    }
    return ret;
}

/*
 *  Method: unlock -> self
 *
 *  Unlock the entry. 
 */
static VALUE rb_gst_clock_entry_unlock(self)
    VALUE self;
{
    gst_clock_id_unlock(RGST_CLOCK_ENTRY(self));
    return self;
}

/*
 *  Method: time -> aBignum
 *
 *  Gets the requested time of the entry, in nanoseconds.
 */
static VALUE rb_gst_clock_entry_get_time(self)
    VALUE self;
{
    return ULL2NUM(GST_CLOCK_ENTRY_TIME(RGST_CLOCK_ENTRY(self)));
}

/*
 *  Method: interval -> aBignum
 *
 *  Gets the interval of the entry, in nanoseconds.
 *  Returns 0 if the entry is not periodic.
 */
static VALUE rb_gst_clock_entry_get_interval(self)
    VALUE self;
{
    return ULL2NUM(GST_CLOCK_ENTRY_INTERVAL(RGST_CLOCK_ENTRY(self)));
}

/*
 *  Method: clock -> aClock
 *
 *  Gets the owner clock of the entry, as a Gst::Clock object. 
 */
static VALUE rb_gst_clock_entry_get_clock(self)
    VALUE self;
{
    return RGST_CLOCK_NEW(GST_CLOCK_ENTRY_CLOCK(RGST_CLOCK_ENTRY(self)));
}

/*
 *  Method: status -> aFixnum
 *
 *  Gets the status of the entry, as a Fixnum integer.
 *  Possible values are:
 *      * Gst::ClockEntry::OK;
 *      * Gst::ClockEntry::EARLY;
 *      * Gst::ClockEntry::RESTART.
 */
static VALUE rb_gst_clock_entry_get_status(self)
    VALUE self;
{
    return INT2FIX(GST_CLOCK_ENTRY_STATUS(RGST_CLOCK_ENTRY(self)));
}

/*
 *  Constant: OK
 *  Timeout happened.
 */
static VALUE constOk = INT2FIX(GST_CLOCK_ENTRY_OK);

/*
 *  Constant: EARLY 
 *  Request for timeout that has been passed.
 */
static VALUE constEarly = INT2FIX(GST_CLOCK_ENTRY_EARLY);

/*
 *  Constant: RESTART
 *  Need to restart the timeout request.
 */
static VALUE constRestart = INT2FIX(GST_CLOCK_ENTRY_RESTART);

void Init_gst_clock_entry(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_CLOCK_ENTRY, "ClockEntry", mGst);

    rb_define_method(c, "initialize", rb_gst_clock_entry_new, -1);

    rb_define_method(c, "single_shot?", rb_gst_clock_entry_is_single_shot, 0);
    rb_define_method(c, "periodic?",    rb_gst_clock_entry_is_periodic,    0);

    rb_define_method(c, "wait",       rb_gst_clock_entry_wait,       0);
    rb_define_method(c, "wait_async", rb_gst_clock_entry_wait_async, 0);

    rb_define_method(c, "unschedule", rb_gst_clock_entry_unschedule, 0);
    rb_define_method(c, "unlock",     rb_gst_clock_entry_unlock,     0);

    rb_define_method(c, "clock",     rb_gst_clock_entry_get_clock,     0);
    rb_define_method(c, "time",      rb_gst_clock_entry_get_time,      0);
    rb_define_method(c, "interval",  rb_gst_clock_entry_get_interval,  0);
    rb_define_method(c, "status",    rb_gst_clock_entry_get_status,    0);
 
    rb_define_const(c, "OK",      constOk);
    rb_define_const(c, "EARLY",   constEarly);
    rb_define_const(c, "RESTART", constRestart);
}

