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

#define SELF(self) RVAL2GST_BIN(self)

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
rb_gst_bin_initialize(int argc, VALUE *argv, VALUE self)
{
    GstElement *bin;
    VALUE name;

    rb_scan_args(argc, argv, "01", &name);

    bin = gst_bin_new(RVAL2CSTR2(name));
    if (bin)
        RBGST_INITIALIZE(self, bin);
    return Qnil;
}

/*
 * Method: size
 *
 * Returns: the number of elements in the container.
 */
static VALUE
rb_gst_bin_size(VALUE self)
{
    return INT2NUM(GST_BIN_NUMCHILDREN(SELF(self)));
}

/*
 * Method: children
 *
 * Returns: an array of all Gst::Element objects in the container.
 */
static VALUE
rb_gst_bin_get_children(VALUE self)
{
    const GList *list;
    VALUE children;

    children = rb_ary_new();
    for (list = GST_BIN_CHILDREN(SELF(self)); list; list = g_list_next(list)) {
        rb_ary_push(children, GST_ELEMENT2RVAL(list->data));
    }

    return children;
}

static VALUE
rb_gst_bin_get_children_cookie(VALUE self)
{
    return UINT2NUM(GST_BIN_CHILDREN_COOKIE(SELF(self)));
}

static VALUE
rb_gst_bin_get_child_bus(VALUE self)
{
    return GST_BUS2RVAL(SELF(self)->child_bus);
}

static VALUE
rb_gst_bin_get_messages(VALUE self)
{
    GList *node;
    VALUE messages;

    messages = rb_ary_new();
    for (node = SELF(self)->messages; node; node = g_list_next(node)) {
        rb_ary_push(messages, GST_MSG2RVAL(node->data));
    }

    return messages;
}

static VALUE
rb_gst_bin_polling_p(VALUE self)
{
    return CBOOL2RVAL(SELF(self)->polling);
}

static VALUE
rb_gst_bin_clock_dirty_p(VALUE self)
{
    return CBOOL2RVAL(SELF(self)->clock_dirty);
}

/*
 * Method: provided_clock
 *
 * Gets the current clock of the (scheduler of the) bin,
 * as a Gst::Clock object.
 *
 * Returns: a Gst::Clock object, or nil.
 */
static VALUE
rb_gst_bin_get_provided_clock(VALUE self)
{
    return GST_CLOCK2RVAL(SELF(self)->provided_clock);
}

/*
 * Method: provided_clock=(clock)
 * clock: a Gst::Clock.
 *
 * Forces the bin to use the given clock.  Use nil to force it 
 * to use no clock at all.
 *
 * Returns: self.
 */
static VALUE
rb_gst_bin_set_provided_clock(VALUE self, VALUE clock)
{
    GstBin *bin;

    bin = SELF(self);
    if (bin->provided_clock)
        g_object_unref(bin->provided_clock);

    bin->provided_clock = RVAL2GST_CLOCK(clock);
    if (bin->provided_clock)
        g_object_ref(bin->provided_clock);

    return self;
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
    return rb_gst_bin_get_children(self);
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
    return rb_gst_bin_get_children(self);
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

    arr = rb_gst_bin_get_children(self);
    for (i = 0; i < RARRAY (arr)->len; i++) {
        VALUE element = rb_ary_entry (arr, i);
        rb_gst_bin_remove (1, &element, self);
    }
    return rb_ary_clear (arr);
}

/*
 * Method: each {|element| ...}
 *
 * Calls the block for each element in the bin, passing a reference to
 * the Gst::Element as parameter.
 *
 * Returns: always  nil.
 */
static VALUE
rb_gst_bin_each_element (VALUE self)
{
    return rb_ary_yield (rb_gst_bin_get_children (self));
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
 * Method: iterate_elements
 *
 * Iterates over the elements in this bin.
 *
 * Returns: true if the bin did something useful, or false (this value can
 * be used to determine if the bin is in EOS ("end of stream")).
 */
/* static VALUE */
/* rb_gst_bin_iterate_elements(VALUE self) */
/* { */
/*     return CBOOL2RVAL(gst_bin_iterate_elements(RGST_BIN(self))); */
/* } */

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
 * Method: iterate_all_by_interface(interface)
 * interface: an interface (Ruby class).
 *
 * Looks for all elements inside the bin that implements the given
 * interface.  The method recurses bins inside bins.
 *
 * Returns: a list of elements inside the bin implementing the interface,
 * as an Array of Gst::Element objects.
 */
/* static VALUE */
/* rb_gst_bin_iterate_all_by_if(VALUE self, VALUE klass) */
/* { */
/*     for (list = */
/*          gst_bin_get_all_by_interface (RGST_BIN (self), CLASS2GTYPE (klass)); */
/*          list != NULL; list = g_list_next (list)) */
/*         rb_ary_push (arr, RGST_ELEMENT_NEW (list->data)); */

/*     g_list_free (list); */
/*     return arr; */
/* } */

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
/* static VALUE */
/* rb_gst_bin_each_by_if (VALUE self, VALUE klass) */
/* { */
/*     return rb_ary_yield (rb_gst_bin_get_all_by_if (self, klass)); */
/* } */

void
Init_gst_bin (void)
{
    VALUE rb_cGstBin;

    rb_cGstBin = G_DEF_CLASS(GST_TYPE_BIN, "Bin", mGst);

    rb_include_module(rb_cGstBin, rb_mEnumerable);

    rb_define_method(rb_cGstBin, "initialize", rb_gst_bin_initialize, -1);

    rb_define_method(rb_cGstBin, "size", rb_gst_bin_size, 0);
    rb_define_alias(rb_cGstBin, "length", "size");

    rb_define_method(rb_cGstBin, "children", rb_gst_bin_get_children, 0);
    rb_define_method(rb_cGstBin, "each", rb_gst_bin_each_element, 0);

    rb_define_method(rb_cGstBin, "children_cookie",
                     rb_gst_bin_get_children_cookie, 0);
    rb_define_method(rb_cGstBin, "child_bus", rb_gst_bin_get_child_bus, 0);
    rb_define_method(rb_cGstBin, "messages", rb_gst_bin_get_messages, 0);
    rb_define_method(rb_cGstBin, "polling?", rb_gst_bin_polling_p, 0);
    rb_define_method(rb_cGstBin, "clock_dirty?", rb_gst_bin_clock_dirty_p, 0);
    rb_define_method(rb_cGstBin, "provided_clock",
                     rb_gst_bin_get_provided_clock, 0);
    rb_define_method(rb_cGstBin, "set_provided_clock",
                     rb_gst_bin_set_provided_clock, 1);


    rb_define_method(rb_cGstBin, "add", rb_gst_bin_add, -1);
    rb_define_alias(rb_cGstBin, "<<", "add");
    rb_define_method(rb_cGstBin, "remove", rb_gst_bin_remove, -1);
    rb_define_method(rb_cGstBin, "remove_all", rb_gst_bin_remove_all, 0);
    rb_define_alias(rb_cGstBin, "clear", "remove_all");

    rb_define_method(rb_cGstBin, "get_by_name", rb_gst_bin_get_by_name, 1);
    rb_define_method(rb_cGstBin, "get_by_name_recurse_up",
                      rb_gst_bin_get_by_name_recurse_up, 1);
    rb_define_alias(rb_cGstBin, "[]", "get_by_name");
    rb_define_method(rb_cGstBin, "get_by_interface", rb_gst_bin_get_by_if, 1);
    /* rb_define_method(rb_cGstBin, "iterate_all_by_interface", rb_gst_bin_iterate_all_by_if, 1); */
    /* rb_define_method(rb_cGstBin, "each_by_interface", rb_gst_bin_each_by_if, 1); */

    /* rb_define_method(rb_cGstBin, "iterate_elements", rb_gst_bin_iterate_elements, 0); */

    G_DEF_SETTERS(rb_cGstBin);

    G_DEF_CLASS (GST_TYPE_BIN_FLAGS, "Flags", rb_cGstBin);
    G_DEF_CONSTANTS(rb_cGstBin, GST_TYPE_BIN_FLAGS, "GST_BIN_");
}
