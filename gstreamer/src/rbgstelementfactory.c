
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

/* Class: Gst::ElementFactory
 * Creates Gst::Element instances.
 */

/*
 * Class method: make(factory_name, element_name)
 * factory_name: a name of an existing factory.
 * element_name: a name which will be attributed to the element.
 *
 * Creates a new Gst::Element of the type defined by the given element factory.
 *
 * If element name is ommited (or nil), then the element will receive a guaranteed 
 * unique name, consisting of the element factory name and a number. 
 * If name is given, it will be given the name supplied.
 *
 * 	# Creates a 'mad' GStreamer element, named 'foo':
 * 	elem1 = Gst::ElementFactory.make("mad", "foo")
 *	  
 * 	# This line does exactly the same thing:
 * 	elem2 = Gst::ElementFactory.find("mad").create("foo")
 *
 * Returns: a newly created object based on Gst::Element.
 */
static VALUE
rb_gst_elementfactory_make (int argc, VALUE *argv, VALUE self)
{
	GstElement *element;
	VALUE fname, ename;

	rb_scan_args (argc, argv, "11", &fname, &ename);

	element = gst_element_factory_make (RVAL2CSTR(fname),
		NIL_P (ename) ? NULL : RVAL2CSTR (ename));

	return element != NULL
		? RGST_ELEMENT_NEW (element)
		: Qnil;
}

/*
 * Class method: find(factory_name)
 * factory_name: a name of an existing factory.
 *
 * Searches for an element factory of the given name.
 *
 * Returns: a Gst::ElementFactory object if found, nil otherwise.
 */
static VALUE
rb_gst_elementfactory_find (VALUE self, VALUE factory_name)
{
	GstElementFactory *factory = gst_element_factory_find (RVAL2CSTR (factory_name));
	return factory != NULL 
		? RGST_ELEMENT_FACTORY_NEW (factory)
		: Qnil;
}

/*
 * Method: create(element_name=nil)
 * element_name: a name which will be attributed to the element.
 *
 * Creates a new element of the type defined by the element factory.
 *
 * If element name is ommited (or nil), then the element will receive a guaranteed 
 * unique name, consisting of the element factory name and a number. 
 * If name is given, it will be given the name supplied.
 *
 * Returns: a newly created object based on Gst::Element.
 */
static VALUE
rb_gst_elementfactory_create (int argc, VALUE *argv, VALUE self)
{
	GstElement *element;
	VALUE name;

	rb_scan_args(argc, argv, "01", &name);

	element = gst_element_factory_create (RGST_ELEMENT_FACTORY (self), 
		NIL_P (name) ? NULL : RVAL2CSTR (name));

	return element != NULL
		? RGST_ELEMENT_NEW (element)
		: Qnil;
}

static VALUE
rb_gst_elementfactory_to_s (VALUE self)
{
	GstElementFactory *factory = RGST_ELEMENT_FACTORY (self); 
	return rb_str_new_with_format ("Element: %s (%s)",
			      GST_PLUGIN_FEATURE_NAME(factory),
			      factory->details.longname);
}

/*
 * Method: pad_templates
 *
 * Requests all pad templates of factory.
 *
 * Returns: an array of Gst::PadTemplate objects.
 */
static VALUE
rb_gst_elementfactory_get_pad_templates (VALUE self)
{
    GstElementFactory *factory;
    const GList *list;
    VALUE arr;

    factory = RGST_ELEMENT_FACTORY(self);
    for (list = gst_element_factory_get_static_pad_templates(factory);
         list != NULL;
         list = g_list_next(list)) {
        GstStaticPadTemplate *pad = list->data;
        rb_ary_push(arr,
                    rb_ary_new3(3,
                                CSTR2RVAL(pad->name_template),
                                GENUM2RVAL(pad->direction, GST_TYPE_PAD_DIRECTION),
                                GENUM2RVAL(pad->presence, GST_TYPE_PAD_PRESENCE)
                                /* RGST_CAPS_NEW(&(pad->static_caps.caps)) */));
    }
    return arr;
}

/*
 * Method: each_pad_template { |pad_template| ... }
 *
 * Calls the block for each pad template of the factory, passing a
 * reference to the Gst::PadTemplate as parameter.
 *
 * Returns: always nil. 
 */
static VALUE
rb_gst_elementfactory_each_pad_template (VALUE self)
{
	return rb_ary_yield (rb_gst_elementfactory_get_pad_templates (self));
}

/*
 * Method: details
 *
 * Gets some public information about the factory, 
 * mostly for the benefit of editors.
 *
 * This information is encapsulated in a Hash object, 
 * with the following (String) keys:
 *
 *   * longname: long (English) element name.
 *   * klass: type of element, as hierarchy.
 *   * description: a short description about the element.
 *   * author: some information about the author(s).
 *
 * Here is an example.
 *
 *	# Prints all details related to the 'mad' element:
 *	Gst::ElementFactory.find("mad").details do |k, v| 
 *		p "#{k}: #{v}"
 *	end 
 *
 * Returns: a Hash.
 */
static VALUE
rb_gst_elementfactory_get_details (VALUE self)
{
	GstElementFactory *factory;
	VALUE hash;
  
	factory = RGST_ELEMENT_FACTORY (self); 
	
	hash = rb_hash_new();

	rb_hash_aset (hash, CSTR2RVAL ("longname"), CSTR2RVAL (factory->details.longname));
	rb_hash_aset (hash, CSTR2RVAL ("klass"), CSTR2RVAL (factory->details.klass)); 
	rb_hash_aset (hash, CSTR2RVAL ("description"), CSTR2RVAL (factory->details.description));
	rb_hash_aset (hash, CSTR2RVAL ("author"), CSTR2RVAL (factory->details.author));

	return hash;
}

void
Init_gst_elementfactory (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_ELEMENT_FACTORY, "ElementFactory", mGst);

	rb_define_singleton_method(c, "make", rb_gst_elementfactory_make, -1);
	rb_define_singleton_method(c, "find", rb_gst_elementfactory_find, 1);
	rb_define_method(c, "create", rb_gst_elementfactory_create, -1);
	rb_define_method(c, "details", rb_gst_elementfactory_get_details, 0);
	rb_define_method(c, "to_s", rb_gst_elementfactory_to_s, 0);
	rb_define_method(c, "pad_templates", rb_gst_elementfactory_get_pad_templates, 0);
	rb_define_method(c, "each_pad_template", rb_gst_elementfactory_each_pad_template, 0);
}
