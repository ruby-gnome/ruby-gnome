
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
 *  Class: Gst::Bin < Gst::Element
 *
 *  Base container element.
 */

static VALUE get_element_obj(arr, element)
    VALUE arr;
    GstElement *element;
{
    int i;
    for (i = 0; i < RARRAY(arr)->len; i++) {
        VALUE obj = rb_ary_entry(arr, i);
        if (element == RGST_ELEMENT(obj))
            return obj;
    }
    assert(0);
    return Qnil;
}

static VALUE rb_gst_bin_get_internal_array(self)
    VALUE self;
{
    VALUE arr;
  
    arr = rb_iv_get(self, "@elements");
    if (arr == Qnil) { 
        arr = rb_ary_new();
        rb_iv_set(self, "@elements", arr);
    }
    assert (arr != Qnil);

    return arr;
}

/*
 *  Class method: new(aStringName=nil) -> aBinObject
 *
 *  Constructs a new Gst::Bin object.
 *
 *  If element name is ommited (or nil), then the bin will receive a guaranteed
 *  unique name, consisting of the "bin" string and a number.
 *  If name is given, it will be given the name supplied.
 */
static VALUE rb_gst_bin_new(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GstElement *bin;
    VALUE name;

    rb_scan_args(argc, argv, "01", &name);

    bin = gst_bin_new(name != Qnil ? RVAL2CSTR(name) : NULL);
    if (bin != NULL) {
        RBGST_INITIALIZE(self, bin);
    }
    return Qnil;
}

/*
 *  Method: length -> aFixnum
 *
 *  Returns the number of elements in the container.
 */
static VALUE rb_gst_bin_length(self)
    VALUE self;
{
    const GList *list;
    GstBin *bin;
    int length;
    
    bin = RGST_BIN(self);
    for (list = gst_bin_get_list(bin), length = 0; 
         list != NULL; 
         list = g_list_next(list), length++) 
    ;
    return INT2FIX(length);
}

/*
 *  Method: elements -> anArray
 *
 *  Returns an array of all Gst::Element objects in the container.
 */
static VALUE rb_gst_bin_get_elements(self)
    VALUE self;
{
    VALUE arr, arr2;
    GstBin *bin;
    const GList *list;
  
    arr  = rb_gst_bin_get_internal_array(self);
    arr2 = rb_ary_new();
    bin  = RGST_BIN(self);
    list = gst_bin_get_list(bin);

    for (list = gst_bin_get_list(bin); 
         list != NULL; 
         list = g_list_next(list)) 
    {
        rb_ary_push(arr2, get_element_obj(arr, GST_ELEMENT(list->data)));
    }

    return arr2;
}

enum {
    RB_BIN_ADD,
    RB_BIN_REMOVE
};

static VALUE rb_gst_bin_add_or_remove(argc, argv, self, what)
    int argc;
    VALUE *argv;
    VALUE self;
    int what;
{
    GstBin *bin;
    VALUE arr;
    int i, size;
   
    bin  = RGST_BIN(self);
    arr  = rb_gst_bin_get_internal_array(self);
    size = FIX2INT(rb_gst_bin_length(self));
 
    if (argc == 0) {
        rb_raise(rb_eArgError, "Wrong # of arguments (need at least 1)");
    }
  
    for (i = 0; i < argc; i++) {
        GstElement *element;
        VALUE obj;

        obj = argv[i];
        if (NIL_P(obj)) {
            rb_raise(rb_eArgError, "nil not allowed (argument %d)", i + 1);
        }
        element = RGST_ELEMENT(obj);
        switch (what) {
            case RB_BIN_ADD:
                gst_bin_add(bin, element);
                if (size == (FIX2INT(rb_gst_bin_length(self)) - 1)) {
                    rb_ary_push(arr, obj);
                    size++;
                }
                else {
                    rb_raise(rb_eException, "Could not add element %s", 
                        gst_element_get_name(element));
                }
                break;

            case RB_BIN_REMOVE:
                gst_bin_remove(bin, element);
                if (size == (FIX2INT(rb_gst_bin_length(self)) + 1)) {
                    rb_ary_delete(arr,  obj);
                    size--;
                }
                else {
                    rb_raise(rb_eException, "Could not remove element %s",
                        gst_element_get_name(element));
                }
                break;
            
            default:
                assert(0);
        }
    }
    return arr;
}

/*
 *  Method: add(*elements) -> anArray
 *
 *  Adds one or more Gst::Element objects to the bin.
 *
 *  Returns an array of all Gst::Element objects in the container.
 */
static VALUE rb_gst_bin_add(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    return rb_gst_bin_add_or_remove(argc, argv, self, RB_BIN_ADD);
}

/*
 *  Method: remove(*elements) -> anArray
 *
 *  Removes one or more Gst::Element objects from the bin, unparenting 
 *  as well.
 *
 *  Returns an array of all Gst::Element objects in the container.
 */
static VALUE rb_gst_bin_remove(argc, argv, self)
    int argc;
    VALUE *argv;
    VALUE self;
{
    return rb_gst_bin_add_or_remove(argc, argv, self, RB_BIN_REMOVE);
}

/*
 *  Method: remove_all -> []
 *
 *  Removes all Gst::Element objects in the bin.
 *
 *  Returns an empty array.
 */
static VALUE rb_gst_bin_remove_all(self)
    VALUE self;
{
    VALUE arr;
    int i;
    
    arr = rb_gst_bin_get_elements(self);
    for (i = 0; i < RARRAY(arr)->len; i++) {
        VALUE element = rb_ary_entry(arr, i);
        rb_gst_bin_remove(1, &element, self);
    }
    return rb_ary_clear(arr);
}

/*
 *  Method: each_element { |aGstElement| block } -> nil
 *
 *  Calls the block for each element in the bin, passing a reference to
 *  the Gst::Element as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_bin_each_element(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_bin_get_elements(self));
}

/*
 *  Method: get_by_name(aName) -> anElement
 *
 *  Gets the element with the given name from the bin, as a reference to 
 *  a Gst::Element object.  Returns nil if the bin does not contain an element
 *  with the given name.
 */
static VALUE rb_gst_bin_get_by_name(self, name) 
    VALUE self, name;
{
    GstElement *element = gst_bin_get_by_name(RGST_BIN(self),
                                              RVAL2CSTR(name));
    return element != NULL
        ? RGST_ELEMENT_NEW(element)
        : Qnil;
}

/*
 *  Method: get_by_name_recurse_up(aName) -> anElement
 *
 *  Gets the element with the given name from the bin, as a reference to 
 *  a Gst::Element object. If the element is not found, a recursion is 
 *  performed on the parent bin.
 *
 *  Returns nil if no element with the given name is found.
 */
static VALUE rb_gst_bin_get_by_name_recurse_up(self, name) 
    VALUE self, name;
{
    GstElement *element = gst_bin_get_by_name_recurse_up(RGST_BIN(self),
                                                         RVAL2CSTR(name));
    return element != NULL
        ? RGST_ELEMENT_NEW(element)
        : Qnil;
}

/*
 *  Method: iterate -> aBoolean
 *
 *  Iterates over the elements in this bin.
 *
 *  Returns true if the bin did something useful.  This value can be used
 *  to determine if the bin is in EOS ("end of stream").
 */
static VALUE rb_gst_bin_iterate(self)
    VALUE self;
{
    return CBOOL2RVAL(gst_bin_iterate(RGST_BIN(self)));
}

/*
 *  Method: clock -> aClockObject
 *
 *  Gets the current clock of the (scheduler of the) bin,
 *  as a Gst::Clock object.
 *  This method overrides Gst::Element#get_clock.
 */
static VALUE rb_gst_bin_get_clock(self)
    VALUE self;
{
    GstBin *bin;
    GstClock *clock;
    
    bin   = RGST_BIN(self);
    clock = gst_bin_get_clock(bin);
    return clock != NULL
        ? RGST_CLOCK_NEW(clock)
        : Qnil;
}

/*
 *  Method: auto_clock -> self
 *
 *  Let the bin select a clock automatically.
 */
static VALUE rb_gst_bin_auto_clock(self)
    VALUE self;
{
    gst_bin_auto_clock(RGST_BIN(self));
    return self;
}

/*
 *  Method: use_clock(aClock) -> self
 *
 *  Force the bin to use the given clock.  Use nil to force it 
 *  to use no clock at all.
 */
static VALUE rb_gst_bin_use_clock(self, clock)
    VALUE self, clock;
{
    gst_bin_use_clock(RGST_BIN(self),
                      NIL_P(clock) ? NULL : RGST_CLOCK(clock));
    return self;
}

/*
 *  Constant: FLAG_MANAGER
 *  This bin is a manager of child elements, i.e. a Gst::Pipeline or
 *  a Gst::Thread. 
 */
static VALUE constFlagManager = INT2FIX(GST_BIN_FLAG_MANAGER);

/*
 *  Constant: FLAG_SCHEDULABLE
 *  This bin iterates itself.
 */
static VALUE constFlagSchedulable = INT2FIX(GST_BIN_SELF_SCHEDULABLE);

/*
 *  Constant: FLAG_PREFER_COTHREADS
 *  This bin prefers to have cothreads when its an option, over chain-based.
 */
static VALUE constFlagPreferCothreads = INT2FIX(GST_BIN_FLAG_PREFER_COTHREADS);

/*
 *  Constant: FLAG_FIXED_CLOCK
 *  This bin has a fixed clock.
 */
static VALUE constFlagFixedClock = INT2FIX(GST_BIN_FLAG_FIXED_CLOCK);

/*
 *  Method: manager? -> aBoolean
 *  Checks if the Gst::Bin::FLAG_MANAGER flag is set on the object.
 */
static VALUE rb_gst_bin_is_manager(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagManager);
}

/*
 *  Method: schedulable? -> aBoolean
 *  Checks if the Gst::Bin::FLAG_SCHEDULABLE flag is set on the object.
 */
static VALUE rb_gst_bin_is_schedulable(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagSchedulable);
}

/*
 *  Method: prefers_cothreads? -> aBoolean
 *  Checks if the Gst::Bin::FLAG_PREFER_COTHREADS flag is set on the object.
 */
static VALUE rb_gst_bin_prefers_cothreads(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagPreferCothreads);
}

/*
 *  Method: has_fixed_clock? -> aBoolean
 *  Checks if the Gst::Bin::FLAG_FIXED_CLOCK flag is set on the object.
 */
static VALUE rb_gst_bin_has_fixed_clock(self)
    VALUE self;
{
    return rb_funcall(self, rb_intern("flag?"), 1, constFlagFixedClock);
}

void Init_gst_bin(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_BIN, "Bin", mGst);

    rb_define_method(c, "initialize", rb_gst_bin_new, -1);

    rb_define_method(c, "add",        rb_gst_bin_add,        -1);
    rb_define_method(c, "remove",     rb_gst_bin_remove,     -1);
    rb_define_method(c, "remove_all", rb_gst_bin_remove_all,  0);
    rb_define_alias(c, "clear", "remove_all");

    rb_define_method(c, "elements", rb_gst_bin_get_elements, 0);
    rb_define_method(c, "each_element", rb_gst_bin_each_element, 0);

    rb_define_method(c, "get_by_name", rb_gst_bin_get_by_name, 1);
    rb_define_method(c, "get_by_name_recurse_up", 
                     rb_gst_bin_get_by_name_recurse_up, 1);

    rb_define_method(c, "length", rb_gst_bin_length, 0);
    rb_define_alias(c, "size", "length");

    rb_define_method(c, "iterate", rb_gst_bin_iterate, 0);

    rb_define_method(c, "clock", rb_gst_bin_get_clock, 0);
    rb_define_method(c, "auto_clock", rb_gst_bin_auto_clock, 0);
    rb_define_method(c, "use_clock", rb_gst_bin_use_clock, 1);

    /*
     *  Flags
     */

    rb_define_method(c, "manager?",           rb_gst_bin_is_manager, 0);
    rb_define_method(c, "schedulable?",       rb_gst_bin_is_schedulable, 0);
    rb_define_method(c, "prefers_cothreads?", rb_gst_bin_prefers_cothreads, 0);
    rb_define_method(c, "has_fixed_clock?",   rb_gst_bin_has_fixed_clock, 0);

    rb_define_const(c, "FLAG_MANAGER",          constFlagManager);
    rb_define_const(c, "FLAG_SCHEDULABLE",      constFlagSchedulable);
    rb_define_const(c, "FLAG_PREFER_COTHREADS", constFlagPreferCothreads);
    rb_define_const(c, "FLAG_FIXED_CLOCK",      constFlagFixedClock);
}

