/*
 * Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *                     2004 Erwan Loisant <erwan@loisant.org>
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

/* Class: Gst::Bin
 * Base container element.
 */

/*
 * Class method: new(name=nil)
 * name: a name for the bin.
 *
 * Constructs a new Gst::Bin object.
 *
 * If element name is ommited (or nil), then the bin will receive a guaranteed
 * unique name, consisting of the "bin" string and a number.
 * If name is given, it will be given the name supplied.
 *
 * Returns: a newly allocated Gst::Bin object.
 */
static VALUE
rb_gst_bin_new (int argc, VALUE * argv, VALUE self)
{
    GstElement *bin;
    VALUE name;

    rb_scan_args (argc, argv, "01", &name);

    bin = gst_bin_new (NIL_P (name) ? NULL : RVAL2CSTR (name));
    if (bin != NULL)
        RBGST_INITIALIZE (self, bin);
    return Qnil;
}

/*
 * Method: length
 *
 * Returns: the number of elements in the container.
 */
static VALUE
rb_gst_bin_length (VALUE self)
{
    return
        INT2FIX (g_list_length ((GList *) gst_bin_get_list (RGST_BIN (self))));
}

/*
 * Method: elements
 *
 * Returns: an array of all Gst::Element objects in the container.
 */
static VALUE
rb_gst_bin_get_elements (VALUE self)
{
    const GList *list;

    VALUE arr = rb_ary_new ();

    for (list = gst_bin_get_list (RGST_BIN (self));
         list != NULL; list = g_list_next (list))
        rb_ary_push (arr, RGST_ELEMENT_NEW (list->data));

    return arr;
}

/*
 * Method: add(*elements)
 * elements: a list of Gst::Element objects.
 *
 * Adds one or more Gst::Element objects to the bin.
 *
 * Returns: an array of all Gst::Element objects in the container.
 */
static VALUE
rb_gst_bin_add (int argc, VALUE * argv, VALUE self)
{
    int i;
    VALUE klass = GTYPE2CLASS(GST_TYPE_ELEMENT);
    GstBin* bin = RGST_BIN(self);

    for (i = 0; i < argc; i++) {
       if (!rb_obj_is_kind_of(argv[i], klass)) {
	 rb_raise(rb_eTypeError, "Gst::Element expected");
       }
       gst_bin_add (bin, RGST_ELEMENT (argv[i]));
    }
    return rb_gst_bin_get_elements (self);
}

/*
 * Method: remove(*elements)
 * elements: a list of Gst::Element objects.
 *
 * Removes one or more Gst::Element objects from the bin, unparenting 
 * as well.
 *
 * Returns: an array of all Gst::Element objects in the container.
 */
static VALUE
rb_gst_bin_remove (int argc, VALUE * argv, VALUE self)
{
    int i;

    for (i = 0; i < argc; i++)
        gst_bin_remove (RGST_BIN (self), RGST_ELEMENT (argv[i]));
    return rb_gst_bin_get_elements (self);
}

/*
 * Method: remove_all
 *
 * Removes all Gst::Element objects in the bin.
 *
 * Returns: an empty array.
 */
static VALUE
rb_gst_bin_remove_all (VALUE self)
{
    VALUE arr;
    int i;

    arr = rb_gst_bin_get_elements (self);
    for (i = 0; i < RARRAY (arr)->len; i++) {
        VALUE element = rb_ary_entry (arr, i);
        rb_gst_bin_remove (1, &element, self);
    }
    return rb_ary_clear (arr);
}

/*
 * Method: each_element { |element| ... }
 *
 * Calls the block for each element in the bin, passing a reference to
 * the Gst::Element as parameter.
 *
 * Returns: always  nil.
 */
static VALUE
rb_gst_bin_each_element (VALUE self)
{
    return rb_ary_yield (rb_gst_bin_get_elements (self));
}

/*
 * Method: get_by_name(name)
 * name: a name.
 *
 * Gets the element with the given name from the bin, as a reference to 
 * a Gst::Element object.
 *
 * Returns: a Gst::Element reference, or nil if the bin does not contain
 * an element with the given name.
 */
static VALUE
rb_gst_bin_get_by_name (VALUE self, VALUE name)
{
    GstElement *element = gst_bin_get_by_name (RGST_BIN (self),
                                               RVAL2CSTR (name));

    return element != NULL ? RGST_ELEMENT_NEW (element)
        : Qnil;
}

/*
 * Method: get_by_name_recurse_up(name)
 * name: a name.
 *
 * Gets the element with the given name from the bin, as a reference to 
 * a Gst::Element object. If the element is not found, a recursion is 
 * performed on the parent bin.
 *
 * Returns: a Gst::Element reference, or nil if no element with the
 * given name is found.
 */
static VALUE
rb_gst_bin_get_by_name_recurse_up (VALUE self, VALUE name)
{
    GstElement *element = gst_bin_get_by_name_recurse_up (RGST_BIN (self),
                                                          RVAL2CSTR (name));

    return element != NULL ? RGST_ELEMENT_NEW (element)
        : Qnil;
}

/*
 * Method: iterate
 *
 * Iterates over the elements in this bin.
 *
 * Returns: true if the bin did something useful, or false (this value can
 * be used to determine if the bin is in EOS ("end of stream")).
 */
static VALUE
rb_gst_bin_iterate (VALUE self)
{
    return CBOOL2RVAL (gst_bin_iterate (RGST_BIN (self)));
}

/*
 * Method: clock
 *
 * Gets the current clock of the (scheduler of the) bin,
 * as a Gst::Clock object.
 * This method overrides Gst::Element#get_clock.
 *
 * Returns: a Gst::Clock object, or nil.
 */
static VALUE
rb_gst_bin_get_clock (VALUE self)
{
    GstClock *clock;

    clock = gst_bin_get_clock (RGST_BIN (self));
    return clock != NULL ? RGST_CLOCK_NEW (clock)
        : Qnil;
}

/*
 * Method: auto_clock
 *
 * Let the bin select a clock automatically.
 *
 * Returns: self.
 */
static VALUE
rb_gst_bin_auto_clock (VALUE self)
{
    gst_bin_auto_clock (RGST_BIN (self));
    return self;
}

/*
 * Method: use_clock(clock)
 * clock: a Gst::Clock.
 *
 * Forces the bin to use the given clock.  Use nil to force it 
 * to use no clock at all.
 *
 * Returns: self.
 */
static VALUE
rb_gst_bin_use_clock (VALUE self, VALUE clock)
{
    gst_bin_use_clock (RGST_BIN (self),
                       NIL_P (clock) ? NULL : RGST_CLOCK (clock));
    return self;
}

/*
 * Method: get_by_interface(interface)
 * interface: an interface (Ruby class).
 *
 * Looks for the first element inside the bin that implements the 
 * given interface. If such an element is found, it returns the element.
 * If you want all elements that implement the interface, use 
 * Gst::Bin#get_all_by_interface. The method recurses bins inside bins.
 *
 * Returns: An element inside the bin implementing the interface, as a
 * Gst::Element object.
 */
static VALUE
rb_gst_bin_get_by_if (VALUE self, VALUE klass)
{
    return RGST_ELEMENT_NEW (gst_bin_get_by_interface (RGST_BIN (self),
                                                       CLASS2GTYPE (klass)));
}

/*
 * Method: get_all_by_interface(interface)
 * interface: an interface (Ruby class).
 *
 * Looks for all elements inside the bin that implements the given
 * interface.  The method recurses bins inside bins.
 *
 * Returns: a list of elements inside the bin implementing the interface,
 * as an Array of Gst::Element objects.
 */
static VALUE
rb_gst_bin_get_all_by_if (VALUE self, VALUE klass)
{
    GList *list;

    VALUE arr = rb_ary_new ();

    for (list =
         gst_bin_get_all_by_interface (RGST_BIN (self), CLASS2GTYPE (klass));
         list != NULL; list = g_list_next (list))
        rb_ary_push (arr, RGST_ELEMENT_NEW (list->data));

    g_list_free (list);
    return arr;
}

/*
 * Method: each_by_interface(interface)
 * interface: an interface (Ruby class).
 * 
 * Calls the block for each element inside the bin that implements the
 * given interface, passing a reference to the Gst::Element as parameter.
 * This method recurses bins inside bins.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_bin_each_by_if (VALUE self, VALUE klass)
{
    return rb_ary_yield (rb_gst_bin_get_all_by_if (self, klass));
}

/*
 * Method: sync_children_state
 *
 * Tries to set the state of the children of this bin to the same state of 
 * the bin by calling Gst::Element#set_state for each child not already having
 * a synchronized state.
 *
 * Returns: the worst return value of any Gst::Element#set_state (see
 * Gst::Element::State).
 */
static VALUE
rb_gst_bin_sync_children_state (VALUE self)
{
    return GENUM2RVAL (gst_bin_sync_children_state (RGST_BIN (self)),
                       GST_TYPE_ELEMENT_STATE_RETURN);
}

/*
 * Method: child_state_change(oldstate, newstate, child)
 * oldstate: the old child state (see Gst::Element::State).
 * newstate: the new child state (see Gst::Element::State).
 * child: a Gst::Element that signaled a changed state.
 *
 * An internal method to inform the parent bin about a state change of
 * a child.
 *
 * Returns: self.
 */
static VALUE
rb_gst_bin_child_state_change (VALUE self, VALUE oldstate, VALUE newstate,
                               VALUE child)
{
    gst_bin_child_state_change (RGST_BIN (self),
                                RVAL2GENUM (oldstate,
                                            GST_TYPE_ELEMENT_STATE_RETURN),
                                RVAL2GENUM (newstate,
                                            GST_TYPE_ELEMENT_STATE_RETURN),
                                RGST_ELEMENT (child));
    return self;
}

void
Init_gst_bin (void)
{
    VALUE c = G_DEF_CLASS (GST_TYPE_BIN, "Bin", mGst);

    rb_define_method (c, "initialize", rb_gst_bin_new, -1);

    rb_define_method (c, "add", rb_gst_bin_add, -1);
    rb_define_method (c, "remove", rb_gst_bin_remove, -1);
    rb_define_method (c, "remove_all", rb_gst_bin_remove_all, 0);
    rb_define_alias (c, "clear", "remove_all");

    rb_define_method (c, "elements", rb_gst_bin_get_elements, 0);
    rb_define_method (c, "each_element", rb_gst_bin_each_element, 0);

    rb_define_method (c, "get_by_name", rb_gst_bin_get_by_name, 1);
    rb_define_method (c, "get_by_name_recurse_up",
                      rb_gst_bin_get_by_name_recurse_up, 1);
    rb_define_alias (c, "[]", "get_by_name");
    rb_define_method (c, "get_by_interface", rb_gst_bin_get_by_if, 1);
    rb_define_method (c, "get_all_by_interface", rb_gst_bin_get_all_by_if, 1);
    rb_define_method (c, "each_by_interface", rb_gst_bin_each_by_if, 1);

    rb_define_method (c, "length", rb_gst_bin_length, 0);
    rb_define_alias (c, "size", "length");

    rb_define_method (c, "iterate", rb_gst_bin_iterate, 0);

    rb_define_method (c, "clock", rb_gst_bin_get_clock, 0);
    rb_define_method (c, "auto_clock", rb_gst_bin_auto_clock, 0);
    rb_define_method (c, "use_clock", rb_gst_bin_use_clock, 1);

    rb_define_method (c, "sync_children_state", rb_gst_bin_sync_children_state,
                      0);
    rb_define_method (c, "child_state_change", rb_gst_bin_child_state_change,
                      3);

    G_DEF_CLASS (GST_TYPE_BIN_FLAGS, "Flags", c);
    G_DEF_CONSTANTS (c, GST_TYPE_BIN_FLAGS, "GST_BIN_");
}
