/*
 * Copyright (C) 2003 Laurent Sansonetti <lrz@gnome.org>
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

static VALUE
get_element_obj (VALUE arr, GstElement *element)
{
	int i;
	for (i = 0; i < RARRAY (arr)->len; i++) {
		VALUE obj = rb_ary_entry(arr, i);
		if (element == RGST_ELEMENT (obj))
			return obj;
	}
	g_assert (0);
	return Qnil;
}

static VALUE
rb_gst_bin_get_internal_array (VALUE self)
{
	VALUE arr;
  
	arr = rb_iv_get (self, "@elements");
	if (arr == Qnil) { 
		arr = rb_ary_new ();
		rb_iv_set (self, "@elements", arr);
	}
	g_assert (arr != Qnil);

	return arr;
}

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
rb_gst_bin_new (int argc, VALUE *argv, VALUE self)
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
	const GList *list;
	GstBin *bin;
	int length;
	
	bin = RGST_BIN (self);
	for (list = gst_bin_get_list (bin), length = 0; 
	     list != NULL; 
	     list = g_list_next (list), length++) 
		;	/* do nothing */
	return INT2FIX (length);
}

/*
 * Method: elements
 *
 * Returns: an array of all Gst::Element objects in the container.
 */
static VALUE
rb_gst_bin_get_elements (VALUE self)
{
	VALUE arr, arr2;
	GstBin *bin;
	const GList *list;
  
	arr  = rb_gst_bin_get_internal_array (self);
	arr2 = rb_ary_new ();
	bin  = RGST_BIN (self);
	list = gst_bin_get_list (bin);

	for (list = gst_bin_get_list (bin); 
	     list != NULL; 
	     list = g_list_next(list))
		rb_ary_push (arr2, get_element_obj (arr, GST_ELEMENT (list->data)));

	return arr2;
}

enum {
	RB_BIN_ADD,
	RB_BIN_REMOVE
};

static VALUE
rb_gst_bin_add_or_remove (int argc, VALUE *argv, VALUE self, int what)
{
	GstBin *bin;
	VALUE arr;
	int i, size;
   
	bin  = RGST_BIN (self);
	arr  = rb_gst_bin_get_internal_array (self);
	size = FIX2INT (rb_gst_bin_length (self));
 
	if (argc == 0)
		rb_raise(rb_eArgError, "Wrong # of arguments (need at least 1)");
  
	for (i = 0; i < argc; i++) {
		GstElement *element;
		VALUE obj;

		obj = argv[i];
		if (NIL_P (obj))
			rb_raise (rb_eArgError, "nil not allowed (argument %d)", i + 1);
		element = RGST_ELEMENT (obj);
		switch (what) {
			case RB_BIN_ADD:
				gst_bin_add (bin, element);
				if (size == (FIX2INT (rb_gst_bin_length (self)) - 1)) {
					rb_ary_push (arr, obj);
					size++;
				}
				else
					rb_raise (rb_eException,
						  "Could not add element %s", 
						  gst_element_get_name (element));
				break;

			case RB_BIN_REMOVE:
				gst_bin_remove (bin, element);
				if (size == (FIX2INT (rb_gst_bin_length (self)) + 1)) {
					rb_ary_delete (arr,  obj);
					size--;
				}
				else
					rb_raise(rb_eException,
						 "Could not remove element %s",
						 gst_element_get_name (element));
				break;
			
			default:
				g_assert (0);
		}
	}
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
rb_gst_bin_add (int argc, VALUE *argv, VALUE self)
{
	return rb_gst_bin_add_or_remove (argc, argv, self, RB_BIN_ADD);
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
rb_gst_bin_remove (int argc, VALUE *argv, VALUE self)
{
	return rb_gst_bin_add_or_remove (argc, argv, self, RB_BIN_REMOVE);
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
	return element != NULL
		? RGST_ELEMENT_NEW (element)
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
	return element != NULL
		? RGST_ELEMENT_NEW (element)
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
	return clock != NULL
		? RGST_CLOCK_NEW (clock)
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

struct __iterate_callback {
	VALUE callback;
	GstBin *bin;
};

static GSList *__pre_iterate_list = NULL;
static GSList *__post_iterate_list = NULL;

static struct __iterate_callback *
__iterate_get (GSList *list, GstBin *bin) 
{
	GSList *i;
	g_assert (bin != NULL);	/* list may be NULL */
	for (i = list;
	     i != NULL;
	     i = g_slist_next (i)) {
		struct __iterate_callback *e = (struct __iterate_callback *) i->data;
		if (e->bin == bin)
			return e;
	}
	return NULL;
}

static void
__iterate_call (struct __iterate_callback *elem)
{
	g_assert (elem != NULL);
	rb_funcall (elem->callback, 
		    rb_intern ("call"), 
		    1, 
		    RGST_BIN_NEW (elem->bin));
}

static void
__pre_iterate_dispatcher (GstBin *bin, gpointer user_data)
{
	__iterate_call (__iterate_get (__pre_iterate_list, bin));
}

static void
__post_iterate_dispatcher (GstBin *bin, gpointer user_data)
{
	__iterate_call (__iterate_get (__post_iterate_list, bin));
}

static struct __iterate_callback *
__iterate_new (VALUE rbin)
{
	struct __iterate_callback *elem;
	elem = g_malloc (sizeof (struct __iterate_callback));
	g_assert (elem != NULL);
	elem->callback = G_BLOCK_PROC ();
	elem->bin = RGST_BIN (rbin);
	return elem;
}

/*
 * Method: on_pre_iterate { |bin| ... }
 *
 * Attaches a block code which will be executed before every iteration 
 * of the bin.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_bin_on_pre_iterate (VALUE self)
{
	if (__iterate_get (__pre_iterate_list, RGST_BIN (self)) != NULL)
		rb_raise (rb_eRuntimeError,
			  "A pre_iterate function is already attached on this bin!");
	else {
		__pre_iterate_list = g_slist_append (__pre_iterate_list,  
						     __iterate_new (self));
		gst_bin_set_pre_iterate_function (RGST_BIN (self), 
						  __pre_iterate_dispatcher, 
						  NULL);
	}
	return Qnil;
}

/*
 *  Method: on_post_iterate { |bin| ... }
 *
 *  Attaches a callback which will be executed after every iteration 
 *  of the bin.
 *
 *  Returns: always nil.
 */
static VALUE
rb_gst_bin_on_post_iterate (VALUE self)
{
	if (__iterate_get (__post_iterate_list, RGST_BIN (self)) != NULL)
		rb_raise (rb_eRuntimeError, 
			  "A post_iterate function is already attached on this bin!");
	else {
		__post_iterate_list = g_slist_append (__post_iterate_list,
						      __iterate_new(self));
		gst_bin_set_post_iterate_function (RGST_BIN (self), 
						   __post_iterate_dispatcher, 
						   NULL);
	}
	return Qnil;
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

	rb_define_method (c, "length", rb_gst_bin_length, 0);
	rb_define_alias (c, "size", "length");

	rb_define_method (c, "iterate", rb_gst_bin_iterate, 0);

	rb_define_method (c, "clock", rb_gst_bin_get_clock, 0);
	rb_define_method (c, "auto_clock", rb_gst_bin_auto_clock, 0);
	rb_define_method (c, "use_clock", rb_gst_bin_use_clock, 1);

	rb_define_method (c, "on_post_iterate", rb_gst_bin_on_post_iterate, 0);
	rb_define_method (c, "on_pre_iterate", rb_gst_bin_on_pre_iterate, 0);

	G_DEF_CLASS (GST_TYPE_BIN_FLAGS, "Flags", c);
	G_DEF_CONSTANTS (c, GST_TYPE_BIN_FLAGS, "GST_BIN_");
}

