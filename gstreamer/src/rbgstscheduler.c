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

/* Class: Gst::Scheduler
 * Abstract class for objects that manage the scheduling of groups of elements.
 */

/* Method: setup
 * Prepares the scheduler.
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_setup (VALUE self)
{
    gst_scheduler_setup (RGST_SCHEDULER (self));
    return self;
}

/* Method: reset 
 * Resets the scheduler.
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_reset (VALUE self)
{
    gst_scheduler_reset (RGST_SCHEDULER (self));
    return self;
}

/*
 * Method: add(something)
 * something: either a Gst::Element or a Gst::Scheduler.
 *
 * Adds either a Gst::Element or a Gst::Scheduler object inside the
 * current scheduler.  In case of a scheduler object, the current scheduler
 * will be notified that it has to monitor the given scheduler.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_add (VALUE self, VALUE something)
{
    if (rb_obj_is_kind_of (something, GTYPE2CLASS (GST_TYPE_ELEMENT)))
        gst_scheduler_add_element (RGST_SCHEDULER (self),
                                   RGST_ELEMENT (something));
    else if (rb_obj_is_kind_of (something, GTYPE2CLASS (GST_TYPE_SCHEDULER)))
        gst_scheduler_add_scheduler (RGST_SCHEDULER (self),
                                     RGST_SCHEDULER (something));
    return self;
}

/*
 * Method: remove(something)
 * something: either a Gst::Element or a Gst::Scheduler.
 *
 * Removes either a Gst::Element or a Gst::Scheduler object inside the
 * current scheduler.  In case of a scheduler object, the current scheduler
 * will be notified that it has to stop monitoring the given scheduler.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_remove (VALUE self, VALUE something)
{
    if (rb_obj_is_kind_of (something, GTYPE2CLASS (GST_TYPE_ELEMENT)))
        gst_scheduler_remove_element (RGST_SCHEDULER (self),
                                      RGST_ELEMENT (something));
    else if (rb_obj_is_kind_of (something, GTYPE2CLASS (GST_TYPE_SCHEDULER)))
        gst_scheduler_remove_scheduler (RGST_SCHEDULER (self),
                                        RGST_SCHEDULER (something));
    return self;
}

/*
 * state_transition(element, transition)
 * element: the Gst::Element with the state transition.
 * transition: the state transition.
 *
 * Tells the scheduler that an element changed its state.
 * 
 * Returns: a Gst::Element::State enumeration indicating success or failure
 * of the state transition.
 */
static VALUE
rb_gst_scheduler_state_transition (VALUE self, VALUE element, VALUE transition)
{
    return
        GENUM2RVAL (gst_scheduler_state_transition
                    (RGST_SCHEDULER (self), RGST_ELEMENT (element),
                     FIX2INT (transition)), GST_TYPE_ELEMENT_STATE_RETURN);
}

/*
 * Method: scheduling_change(element) 
 * element: the Gst::Element that changed its scheduling strategy.
 *
 * Tells the scheduler that an element changed its scheduling strategy. 
 * An element could, for example, change its loop function or changes from a 
 * loop based element to a chain based element.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_scheduling_change (VALUE self, VALUE element)
{
    gst_scheduler_scheduling_change (RGST_SCHEDULER (self),
                                     RGST_ELEMENT (element));
    return self;
}

/*
 * Method: lock_element(element)
 * element: the Gst::Element to lock.
 *
 * Acquires a lock on the given element in the current scheduler.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_lock_element (VALUE self, VALUE element)
{
    gst_scheduler_lock_element (RGST_SCHEDULER (self), RGST_ELEMENT (element));
    return self;
}

/*
 * Method: unlock_element(element)
 * element: the Gst::Element to unlock.
 *
 * Releases the lock on the given element in the current scheduler.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_unlock_element (VALUE self, VALUE element)
{
    gst_scheduler_unlock_element (RGST_SCHEDULER (self),
                                  RGST_ELEMENT (element));
    return self;
}

/*
 * Method: yield(element)
 * element: the Gst::Element requesting a yield.
 *
 * Tells the scheduler to schedule another element.
 *
 * Returns: true if the element should save its state, false if the scheduler
 * can perform this action itself.
 */
static VALUE
rb_gst_scheduler_yield (VALUE self, VALUE element)
{
    return
        CBOOL2RVAL (gst_scheduler_yield
                    (RGST_SCHEDULER (self), RGST_ELEMENT (element)));
}

/*
 * Method: interrupt(element)
 * element: the Gst::Element requesting an interrupt.
 *
 * Tells the scheduler to interrupt execution of this element.
 *
 * Returns: true if the element should return nil from the chain/get method.
 */
static VALUE
rb_gst_scheduler_interrupt (VALUE self, VALUE element)
{
    return
        CBOOL2RVAL (gst_scheduler_interrupt
                    (RGST_SCHEDULER (self), RGST_ELEMENT (element)));
}

/*
 * Method: error(element)
 * element: the Gst::Element with the error.
 *
 * Tells the scheduler an element was in error.
 * 
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_error (VALUE self, VALUE element)
{
    gst_scheduler_error (RGST_SCHEDULER (self), RGST_ELEMENT (element));
    return self;
}

/*
 * Method: pad_link(srcpad, sinkpad)
 * srcpad: the source Gst::Pad to link.
 * sinkpad: the sink Gst::Pad to link to.
 *
 * Links the given source pad to the given sink pad.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_pad_link (VALUE self, VALUE srcpad, VALUE sinkpad)
{
    gst_scheduler_pad_link (RGST_SCHEDULER (self), RGST_PAD (srcpad),
                            RGST_PAD (sinkpad));
    return self;
}

/*
 * Method: pad_unlink(srcpad, sinkpad)
 * srcpad: the source Gst::Pad to unlink.
 * sinkpad: the sink Gst::Pad to unlink from.
 *
 * Unlinks the given source pad from the given sink pad.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_pad_unlink (VALUE self, VALUE srcpad, VALUE sinkpad)
{
    gst_scheduler_pad_unlink (RGST_SCHEDULER (self), RGST_PAD (srcpad),
                              RGST_PAD (sinkpad));
    return self;
}

/*
 * Method: pad_select(*pads)
 * pads: a set of Gst::Pad objects to select on.
 * 
 * Registers the given list of pads for a select operation.
 *
 * Returns: the Gst::Pad which received a buffer. 
 */
static VALUE
rb_gst_scheduler_pad_select (VALUE self, VALUE ary)
{
    GList *list;
    GstPad *pad;
    int i;

    rb_raise (rb_eArgError, "Need at least one parameter");

    for (i = 0; i < RARRAY (ary)->len; i++) {
        VALUE entry = rb_ary_entry (ary, i);

        list = g_list_append (list, RGST_PAD (entry));
    }

    pad = gst_scheduler_pad_select (RGST_SCHEDULER (self), list);
    g_list_free (list);

    return pad != NULL ? RGST_PAD_NEW (pad) : Qnil;
}

/*
 * Method: clock_wait(element, clock)
 * element: the Gst::Element that wants to wait.
 * clock: the Gst::Clock to use.
 *
 * Waits till the clock reaches a specific time.
 *
 * Returns: the status of the operation (see Gst::Clock::Return).
 */
static VALUE
rb_gst_scheduler_clock_wait (VALUE self, VALUE element, VALUE id)
{
    return
        GENUM2RVAL (gst_scheduler_clock_wait
                    (RGST_SCHEDULER (self), RGST_ELEMENT (element),
                     (GstClockID) RGST_CLOCK (id), NULL /*NUM2ULL (jitter) */ ),
                    GST_TYPE_CLOCK_RETURN);
}

/*
 * Method: iterate
 *
 * Performs one iteration on the scheduler.
 * 
 * Returns: a boolean indicating something useful has happened.
 */
static VALUE
rb_gst_scheduler_iterate (VALUE self)
{
    return CBOOL2RVAL (gst_scheduler_iterate (RGST_SCHEDULER (self)));
}

/*
 * Method: use_clock(clock)
 * clock: the Gst::Clock to set.
 *
 * Forces the scheduler to use the given clock. The scheduler will always use 
 * the given clock even if new clock providers are added to this scheduler.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_use_clock (VALUE self, VALUE clock)
{
    gst_scheduler_use_clock (RGST_SCHEDULER (self), RGST_CLOCK (clock));
    return self;
}

/*
 * Method: set_clock(clock)
 * clock: the Gst::Clock to set.
 *
 * Sets the clock for the scheduler. The clock will be distributed to all the 
 * elements managed by the scheduler.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_set_clock (VALUE self, VALUE clock)
{
    gst_scheduler_set_clock (RGST_SCHEDULER (self), RGST_CLOCK (clock));
    return self;
}

/*
 * Method: clock
 *
 * Gets the current clock used by the scheduler.
 *
 * Returns: a Gst::Clock, or nil if no clock is set.
 */
static VALUE
rb_gst_scheduler_get_clock (VALUE self)
{
    GstClock *clock = gst_scheduler_get_clock (RGST_SCHEDULER (self));

    return clock != NULL ? RGST_CLOCK_NEW (clock) : Qnil;
}

/*
 * Method: auto_clock
 *
 * Lets the scheduler select a clock automatically.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_auto_clock (VALUE self)
{
    gst_scheduler_auto_clock (RGST_SCHEDULER (self));
    return self;
}

/*
 * Method: show
 *
 * Dumps the state of the scheduler.
 *
 * Returns: self.
 */
static VALUE
rb_gst_scheduler_show (VALUE self)
{
    gst_scheduler_show (RGST_SCHEDULER (self));
    return self;
}

/*
 * Method: state
 *
 * Gets the current state of the scheduler.
 *
 * Returns: the current state (see Gst::Scheduler::State).
 */
static VALUE
rb_gst_scheduler_get_state (VALUE self)
{
    return GENUM2RVAL (GST_SCHEDULER_STATE (RGST_SCHEDULER (self)),
                       GST_TYPE_SCHEDULER_STATE);
}

void
Init_gst_scheduler (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_SCHEDULER, "Scheduler", mGst);

    rb_undef_method (c, "initialize");
    rb_define_method (c, "setup", rb_gst_scheduler_setup, 0);
    rb_define_method (c, "reset", rb_gst_scheduler_reset, 0);
    rb_define_method (c, "add", rb_gst_scheduler_add, 1);
    rb_define_alias (c, "<<", "add");
    rb_define_method (c, "remove", rb_gst_scheduler_remove, 1);
    rb_define_method (c, "state_transition", rb_gst_scheduler_state_transition,
                      2);
    rb_define_method (c, "scheduling_change",
                      rb_gst_scheduler_scheduling_change, 1);
    rb_define_method (c, "lock_element", rb_gst_scheduler_lock_element, 1);
    rb_define_method (c, "unlock_element", rb_gst_scheduler_unlock_element, 1);
    rb_define_method (c, "yield", rb_gst_scheduler_yield, 1);
    rb_define_method (c, "interrupt", rb_gst_scheduler_interrupt, 1);
    rb_define_method (c, "error", rb_gst_scheduler_error, 1);
    rb_define_method (c, "pad_link", rb_gst_scheduler_pad_link, 2);
    rb_define_method (c, "pad_unlink", rb_gst_scheduler_pad_unlink, 2);
    rb_define_method (c, "pad_select", rb_gst_scheduler_pad_select, -2);
    rb_define_method (c, "clock_wait", rb_gst_scheduler_clock_wait, 2);
    rb_define_method (c, "iterate", rb_gst_scheduler_iterate, 0);
    rb_define_method (c, "use_clock", rb_gst_scheduler_use_clock, 1);
    rb_define_method (c, "clock", rb_gst_scheduler_get_clock, 0);
    rb_define_method (c, "set_clock", rb_gst_scheduler_set_clock, 1);
    rb_define_method (c, "auto_clock", rb_gst_scheduler_auto_clock, 0);
    rb_define_method (c, "show", rb_gst_scheduler_show, 0);
    rb_define_method (c, "state", rb_gst_scheduler_get_state, 0);

    /* XXX do not seem to be used in gst itself... we define them anyway */
    G_DEF_CLASS (GST_TYPE_SCHEDULER_FLAGS, "Flags", c);
    G_DEF_CONSTANTS (c, GST_TYPE_SCHEDULER_FLAGS, "GST_SCHEDULER_");
    
    G_DEF_CLASS (GST_TYPE_SCHEDULER_STATE, "State", c);
    G_DEF_CONSTANTS (c, GST_TYPE_SCHEDULER_STATE, "GST_SCHEDULER_");
}
