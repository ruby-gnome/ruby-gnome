/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
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

#define RG_TARGET_NAMESPACE cClockEntry

/* Class: Gst::ClockEntry
 * A wrapper for GstClockID, used to create and start timers
 * on Gst::Clock objects.
 */

static GstClockEntry *
clock_entry_copy (const GstClockEntry * clock_entry)
{
    GstClockEntry *new_clock_entry;

    g_return_val_if_fail (clock_entry != NULL, NULL);
    new_clock_entry = g_new (GstClockEntry, sizeof (GstClockEntry));
    *new_clock_entry = *clock_entry;
    return new_clock_entry;
}

GType
gst_clock_entry_get_type (void)
{
    static GType our_type = 0;

    if (our_type == 0) {
        our_type = g_boxed_type_register_static ("GstClockID",
                                                 (GBoxedCopyFunc)
                                                 clock_entry_copy,
                                                 (GBoxedFreeFunc) g_free);
    }
    return our_type;
}

/*
 * Class method: new(clock, time, interval=nil)
 * clock: a Gst::Clock.
 * time: a time period, in nanoseconds.
 * interval: an interval period, in nanoseconds.
 *
 * Creates a new Gst::ClockEntry object based on the given Gst::Clock.
 *
 * Two types of Gst::ClockEntry objects can be created:
 *
 * * One-shot: if the interval is ommited or nil, the entry will trigger a single shot notification, at the requested time (in nanoseconds);
 * * Periodic: if the interval is not nil, the timer entry will trigger a periodic notification, starting at time (in nanoseconds), and be fired with the given interval (also in nanoseconds).
 * 
 * The timer will be issued after Gst::ClockEntry#wait or
 * Gst::ClockEntry#wait_async.
 *
 * Returns: a new Gst::ClockEntry object.
 */
static VALUE
rg_initialize (int argc, VALUE * argv, VALUE self)
{
    VALUE clock, time, interval;
    GstClockID id;

    rb_scan_args (argc, argv, "21", &clock, &time, &interval);

    /*
     * Single-shot 
     */
    if (NIL_P (interval))
        id = gst_clock_new_single_shot_id (RVAL2GST_CLOCK (clock), NUM2ULL (time));
    /*
     * Periodic 
     */
    else
        id = gst_clock_new_periodic_id (RVAL2GST_CLOCK (clock),
                                        NUM2ULL (time), NUM2ULL (interval));

    G_INITIALIZE (self, GST_CLOCK_ENTRY (id));
    return Qnil;
}

/* Method: single_shot?
 * Returns: true if the entry is single-shot, false otherwise.
 */
static VALUE
rg_single_shot_p (VALUE self)
{
    return CBOOL2RVAL (GST_CLOCK_ENTRY_TYPE (RGST_CLOCK_ENTRY (self)) ==
                       GST_CLOCK_ENTRY_SINGLE);
}

/* Method: periodic?
 * Returns: true if the entry is periodic, false otherwise.
 */
static VALUE
rg_periodic_p (VALUE self)
{
    return CBOOL2RVAL (GST_CLOCK_ENTRY_TYPE (RGST_CLOCK_ENTRY (self)) ==
                       GST_CLOCK_ENTRY_PERIODIC);
}

/*
 * Method: unschedule
 *
 * Cancels an outstanding async notification request.
 *
 * Returns: self.
 */
static VALUE
rg_unschedule (VALUE self)
{
    gst_clock_id_unschedule (RGST_CLOCK_ENTRY (self));
    return self;
}

/*
 * Method: wait
 *
 * Performs a blocking wait on the entry.
 *
 * Returns: a return code (see Gst::Clock::Return).
 */
static VALUE
rg_wait (VALUE self)
{
    return GENUM2RVAL (gst_clock_id_wait (RGST_CLOCK_ENTRY (self), NULL),
                       GST_TYPE_CLOCK_RETURN);
}

struct __callback {
    GstClockID id;
    VALUE callback;
};

static GSList *__callbacks = NULL;

static struct __callback *
__callback_get (GstClockID id)
{
    GSList *i;

    for (i = __callbacks; i != NULL; i = g_slist_next (i)) {
        struct __callback *e = (struct __callback *)i->data;

        if (e->id == id)
            return e;
    }
    return NULL;
}

static gboolean
__callback_dispatcher (GstClock * clock, GstClockTime time, GstClockID id,
                       gpointer user_data)
{
    struct __callback *e = __callback_get (id);

    g_assert (e != NULL);
    return RVAL2CBOOL (rb_funcall (e->callback,
                                   rb_intern ("call"),
                                   3,
                                   RGST_CLOCK_NEW (clock),
                                   ULL2NUM (time), RGST_CLOCK_ENTRY_NEW (id)));
}

/*
 * Method: wait_async { |clock, time, clock_entry| ... }
 *
 * Registers a block code, which will be called passing references
 * to the Gst::Clock, the time (in nanoseconds) and the Gst::ClockEntry as
 * parameters.
 *
 * Returns: a return code (see Gst::Clock::Return).
 */
static VALUE
rg_wait_async (VALUE self)
{
    GstClockID id = (GstClockID) RGST_CLOCK_ENTRY (self);

    if (__callback_get (id) != NULL)
        rb_raise (rb_eRuntimeError,
                  "An asynch callback is already registred to this entry.");
    else {
        struct __callback *e = g_malloc (sizeof (struct __callback));

        g_assert (e != NULL);
        e->id = id;
        e->callback = rb_block_proc ();
        __callbacks = g_slist_append (__callbacks, e);
        return GENUM2RVAL (INT2FIX (gst_clock_id_wait_async (id,
                                                             __callback_dispatcher,
                                                             NULL)),
                           GST_TYPE_CLOCK_RETURN);
    }
    return Qnil;
}

/* Method: time
 * Returns: the requested time of the entry, in nanoseconds.
 */
static VALUE
rg_time (VALUE self)
{
    return ULL2NUM (GST_CLOCK_ENTRY_TIME (RGST_CLOCK_ENTRY (self)));
}

/* Method: interval
 * Returns: the interval of the entry (in nanoseconds), or 0 
 * if the entry is not periodic.
 */
static VALUE
rg_interval (VALUE self)
{
    return ULL2NUM (GST_CLOCK_ENTRY_INTERVAL (RGST_CLOCK_ENTRY (self)));
}

/* Method: clock
 * Returns: the owner clock of the entry, as a Gst::Clock object. 
 */
static VALUE
rg_clock (VALUE self)
{
    return RGST_CLOCK_NEW (GST_CLOCK_ENTRY_CLOCK (RGST_CLOCK_ENTRY (self)));
}

/* Method: status
 * Returns: the status of the entry (see Gst::ClockEntry::Return).
 */
static VALUE
rg_status (VALUE self)
{
    return GENUM2RVAL(GST_CLOCK_ENTRY_STATUS(RGST_CLOCK_ENTRY (self)),
                      GST_TYPE_CLOCK_RETURN);
}

void
Init_gst_clock_entry (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE = G_DEF_CLASS (GST_TYPE_CLOCK_ENTRY, "ClockEntry", mGst);

    RG_DEF_METHOD(initialize, -1);
    RG_DEF_METHOD_P(single_shot, 0);
    RG_DEF_METHOD_P(periodic, 0);
    RG_DEF_METHOD(wait, 0);
    RG_DEF_METHOD(wait_async, 0);
    RG_DEF_METHOD(unschedule, 0);
    RG_DEF_METHOD(clock, 0);
    RG_DEF_METHOD(time, 0);
    RG_DEF_METHOD(interval, 0);
    RG_DEF_METHOD(status, 0);

    G_DEF_CLASS (GST_TYPE_CLOCK_ENTRY_TYPE, "Type", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS (RG_TARGET_NAMESPACE, GST_TYPE_CLOCK_ENTRY_TYPE, "GST_CLOCK_ENTRY_");
    G_DEF_CLASS (GST_TYPE_CLOCK_RETURN, "Return", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS (RG_TARGET_NAMESPACE, GST_TYPE_CLOCK_RETURN, "GST_CLOCK_");
}
