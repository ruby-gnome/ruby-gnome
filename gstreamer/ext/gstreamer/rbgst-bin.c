/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/*
 *  Copyright (C) 2011  Ruby-GNOME2 Project Team
 *  Copyright (C) 2003, 2004 Laurent Sansonetti <lrz@gnome.org>
 *                      2004 Erwan Loisant <erwan@loisant.org>
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

#define RG_TARGET_NAMESPACE cBin
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
rg_initialize(int argc, VALUE *argv, VALUE self)
{
    GstElement *bin;
    VALUE name;

    rb_scan_args(argc, argv, "01", &name);

    bin = gst_bin_new(RVAL2CSTR_ACCEPT_NIL(name));
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
rg_size(VALUE self)
{
    return INT2NUM(GST_BIN_NUMCHILDREN(SELF(self)));
}

/*
 * Method: children(interface=nil)
 * interface: an interface (Ruby class).
 *
 * Returns: If interface is nil, an array of all
 * Gst::Element objects in the container. Otherwise, an
 * array of Gst::Element objects in the container that
 * implements the interface.
 */
static VALUE
rg_children(int argc, VALUE *argv, VALUE self)
{
    VALUE children, iface;

    rb_scan_args(argc, argv, "01", &iface);

    if (NIL_P(iface)) {
        const GList *node;
        children = rb_ary_new();
        for (node = GST_BIN_CHILDREN(SELF(self));
             node;
             node = g_list_next(node)) {
            rb_ary_push(children, GST_ELEMENT2RVAL(node->data));
        }
    } else {
        GstIterator *iter;
        iter = gst_bin_iterate_all_by_interface(SELF(self),
                                                CLASS2GTYPE(iface));
        children = _rbgst_collect_elements(iter);
    }

    return children;
}

/*
 * Method: children_recurse()
 *
 * Returns: an array of all Gst::Element objects in the container and child
 * bins.
 */
static VALUE
rg_children_recurse(VALUE self)
{
   GstIterator *iter;
   VALUE children;

   iter = gst_bin_iterate_recurse(SELF(self));
   children = _rbgst_collect_elements(iter);

    return children;
}

static VALUE
rg_children_cookie(VALUE self)
{
    return UINT2NUM(GST_BIN_CHILDREN_COOKIE(SELF(self)));
}

static VALUE
rg_child_bus(VALUE self)
{
    return GST_BUS2RVAL(SELF(self)->child_bus);
}

static VALUE
rg_messages(VALUE self)
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
rg_polling_p(VALUE self)
{
    return CBOOL2RVAL(SELF(self)->polling);
}

static VALUE
rg_clock_dirty_p(VALUE self)
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
rg_provided_clock(VALUE self)
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
rg_set_provided_clock(VALUE self, VALUE clock)
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

static VALUE
rg_clock_provider(VALUE self)
{
    return GST_ELEMENT2RVAL(SELF(self)->clock_provider);
}

/*
 * Method: <<(element)
 * element: a Gst::Element object.
 *
 * Adds a Gst::Element object to the bin.
 *
 * Returns: self.
 */
static VALUE
rg_operator_add(VALUE self, VALUE element)
{
    VALUE klass = GTYPE2CLASS(GST_TYPE_ELEMENT);

    if (!RVAL2CBOOL(rb_obj_is_kind_of(element, klass))) {
        rb_raise(rb_eTypeError, "Gst::Element expected");
    }
    gst_bin_add(SELF(self), RVAL2GST_ELEMENT(element));
    G_CHILD_ADD(self, element);
    return self;
}

/*
 * Method: add(*elements)
 * elements: a list of Gst::Element objects.
 *
 * Adds one or more Gst::Element objects to the bin.
 *
 * Returns: nil.
 */
static VALUE
rg_add(int argc, VALUE *argv, VALUE self)
{
    int i;

    for (i = 0; i < argc; i++) {
        rg_operator_add(self, argv[i]);
    }
    return Qnil;
}

/*
 * Method: remove(*elements)
 * elements: a list of Gst::Element objects.
 *
 * Removes one or more Gst::Element objects from the bin, unparenting 
 * as well.
 *
 * Returns: nil.
 */
static VALUE
rg_remove(int argc, VALUE *argv, VALUE self)
{
    int i;
    GstBin *bin;

    bin = SELF(self);
    for (i = 0; i < argc; i++) {
        G_CHILD_REMOVE(self, argv[i]);
        gst_bin_remove(bin, RVAL2GST_ELEMENT(argv[i]));
    }
    return Qnil;
}

/*
 * Method: clear
 *
 * Removes all Gst::Element objects in the bin.
 *
 * Returns: nil.
 */
static VALUE
rg_clear(VALUE self)
{
    GstBin *bin;
    GList *node, *children;

    bin = SELF(self);
    children = g_list_copy(GST_BIN_CHILDREN(bin));
    for (node = children; node; node = g_list_next(node)) {
        gst_bin_remove(bin, node->data);
    }
    g_list_free(children);
    return Qnil;
}

/*
 * Method: each(interface=nil) {|element| ...}
 *
 * Calls the block for each element in the bin, passing a reference to
 * the Gst::Element as parameter. If the interface isn't nil,
 * iterated elements should implement the interface.
 *
 * Returns: always nil.
 */
static VALUE
rg_each(int argc, VALUE *argv, VALUE self)
{
    return rb_ary_yield(rg_children(argc, argv, self));
}

/*
 * Method: each_recurse() {|element| ...}
 *
 * Calls the block for each element in the bin and its child bins, passing a
 * reference to the Gst::Element as parameter. 
 *
 * Returns: always nil.
 */
static VALUE
rg_each_recurse(VALUE self)
{
    return rb_ary_yield(rg_children_recurse(self));
}

/*
 * Method: get_child(index)
 * Method: get_child(name, recurse=false)
 * Method: get_child(interface)
 * index: an index.
 * name: a name.
 * recurse: search recursively.
 * interface: an interface (Ruby class).
 *
 * 1st: Gets the index-th element.
 *
 * 2nd: Gets the element with the given name from the
 * bin, as a reference to a Gst::Element object. If the
 * element is not found and recurse is true, a recursion is
 * performed on the parent bin.
 *
 * 3nd: Looks for the first element inside the bin that implements the
 * given interface. If such an element is found, it returns the element.
 * If you want all elements that implement the interface, use
 * Gst::Bin#get_all_by_interface. The method recurses bins inside bins.
 *
 * Returns: a Gst::Element reference, or nil if the bin does not contain
 * an element with the given name nor implementing the interface.
 */
static VALUE
rg_get_child(int argc, VALUE *argv, VALUE self)
{
    VALUE index_or_name_or_interface, recurse;
    GstElement *element = NULL;

    rb_scan_args(argc, argv, "11", &index_or_name_or_interface, &recurse);

    if (RVAL2CBOOL(rb_obj_is_kind_of(index_or_name_or_interface, rb_cInteger))) {
        int index;
        GList *node;
        index = NUM2INT(index_or_name_or_interface);
        node = g_list_nth(GST_BIN_CHILDREN(SELF(self)), index);
        if (node)
            element = node->data;
    } else if (RVAL2CBOOL(rb_obj_is_kind_of(index_or_name_or_interface,
                                            rb_cString))) {
        char *name;
        name = RVAL2CSTR(index_or_name_or_interface);

        if (RVAL2CBOOL(recurse)) {
            element = gst_bin_get_by_name_recurse_up(SELF(self), name);
        } else {
            element = gst_bin_get_by_name(SELF(self), name);
        }
    } else {
        GType iface;
        iface = CLASS2GTYPE(index_or_name_or_interface);
        element = gst_bin_get_by_interface(SELF(self), iface);
    }

    return GST_ELEMENT2RVAL(element);
}

static VALUE
rg_sinks(VALUE self)
{
    return _rbgst_collect_elements(gst_bin_iterate_sinks(SELF(self)));
}

static VALUE
rg_sources(VALUE self)
{
    return _rbgst_collect_elements(gst_bin_iterate_sources(SELF(self)));
}

#ifdef GST_DEBUG_BIN_TO_DOT_FILE
static VALUE
rg_to_dot_file(VALUE self, VALUE details, VALUE filename)
{
    GST_DEBUG_BIN_TO_DOT_FILE(SELF(self), NUM2INT(details), RVAL2CSTR(filename));
    return Qnil;
}

static VALUE
rg_to_dot_file_with_ts(VALUE self, VALUE details, VALUE filename)
{
    GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS(SELF(self),
                                      NUM2INT(details),
                                      RVAL2CSTR(filename));
    return Qnil;
}
#endif

void
Init_gst_bin (VALUE mGst)
{
    VALUE RG_TARGET_NAMESPACE;

    RG_TARGET_NAMESPACE = G_DEF_CLASS(GST_TYPE_BIN, "Bin", mGst);

    rb_include_module(RG_TARGET_NAMESPACE, rb_mEnumerable);

    RG_DEF_METHOD(initialize, -1);

    RG_DEF_METHOD(size, 0);
    RG_DEF_ALIAS("length", "size");

    RG_DEF_METHOD(children, -1);
    RG_DEF_METHOD(each, -1);

    RG_DEF_METHOD(children_recurse, 0);
    RG_DEF_METHOD(each_recurse, 0);

    RG_DEF_METHOD(children_cookie, 0);
    RG_DEF_METHOD(child_bus, 0);
    RG_DEF_METHOD(messages, 0);
    RG_DEF_METHOD_P(polling, 0);
    RG_DEF_METHOD_P(clock_dirty, 0);
    RG_DEF_METHOD(provided_clock, 0);
    RG_DEF_METHOD(set_provided_clock, 1);
    RG_DEF_METHOD(clock_provider, 0);

    RG_DEF_METHOD_OPERATOR("<<", add, 1);
    RG_DEF_METHOD(add, -1);
    RG_DEF_METHOD(remove, -1);
    RG_DEF_METHOD(clear, 0);

    RG_DEF_METHOD(get_child, -1);

    RG_DEF_METHOD(sinks, 0);
    RG_DEF_METHOD(sources, 0);

#ifdef GST_DEBUG_BIN_TO_DOT_FILE
    RG_DEF_METHOD(to_dot_file, 2);
    RG_DEF_METHOD(to_dot_file_with_ts, 2);
#endif

    G_DEF_CLASS(GST_TYPE_BIN_FLAGS, "Flags", RG_TARGET_NAMESPACE);
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_BIN_FLAGS, "GST_BIN_");
#ifdef GST_DEBUG_BIN_TO_DOT_FILE
    G_DEF_CONSTANTS(RG_TARGET_NAMESPACE, GST_TYPE_DEBUG_GRAPH_DETAILS,
        "GST_DEBUG_GRAPH_");
#endif
}
