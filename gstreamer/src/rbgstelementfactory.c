
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

/*
 *  Class: Gst::ElementFactory < Gst::PluginFeature
 *
 *  Creates Gst::Element instances.
 */

/*
 *  Class method: make(aFactoryNameString, anElementNameString=nil) -> aGstElement
 *
 *  Creates a new Gst::Element of the type defined by the given element factory.
 *
 *  If element name is ommited (or nil), then the element will receive a guaranteed 
 *  unique name, consisting of the element factory name and a number. 
 *  If name is given, it will be given the name supplied.
 *
 *  Example: 
 *
 *    # Creates a 'mad' GStreamer element, named 'foo':
 *    elem1 = Gst::ElementFactory.make("mad", "foo")
 *	  
 *    # This line does exactly the same thing:
 *    elem2 = Gst::ElementFactory.find("mad").create("foo")
 */
static VALUE rb_gst_elementfactory_make(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GstElement *element;
    VALUE fname, ename;

    rb_scan_args(argc, argv, "11", &fname, &ename);

    element = gst_element_factory_make(RVAL2CSTR(fname), 
        ename != Qnil ? RVAL2CSTR(ename) : NULL);

    return element != NULL
        ? RGST_ELEMENT_NEW(element)
        : Qnil;
}

/*
 *  Class method: find(aFactoryNameString) -> aGstElementFactory
 *
 *  Searches for an element factory of the given name.
 *  If found, returns a Gst::ElementFactory object.  Otherwise, returns nil.
 */
static VALUE rb_gst_elementfactory_find(self, factory_name)
    VALUE self, factory_name;
{
    GstElementFactory *factory = gst_element_factory_find(RVAL2CSTR(factory_name));
    return factory != NULL 
        ? RGST_ELEMENT_FACTORY_NEW(factory)
        : Qnil;
}

/*
 *  Method: create(anElementNameString=nil) -> aGstElement
 *
 *  Creates a new element of the type defined by the element factory.
 *
 *  If element name is ommited (or nil), then the element will receive a guaranteed 
 *  unique name, consisting of the element factory name and a number. 
 *  If name is given, it will be given the name supplied.
 */
static VALUE rb_gst_elementfactory_create(argc, argv, self)
    int argc;
    VALUE *argv, self;
{
    GstElementFactory *factory;
    GstElement *element;
    VALUE name;

    rb_scan_args(argc, argv, "01", &name);

    factory = RGST_ELEMENT_FACTORY(self);
    element = gst_element_factory_create(factory, 
        name != Qnil ? RVAL2CSTR(name) : NULL);

    return element != NULL
        ? RGST_ELEMENT_NEW(element)
        : Qnil;
}

/*
 *  Method: rank -> aFixnum
 *
 *  Gets the rank of the factory:
 *      
 *    * Gst::ElementFactory::RANK_MARGINAL;
 *    * Gst::ElementFactory::RANK_NONE;
 *    * Gst::ElementFactory::RANK_PRIMARY;
 *    * Gst::ElementFactory::RANK_SECONDARY.
 */
static VALUE rb_gst_elementfactory_get_rank(self)
    VALUE self;
{
    GstElementFactory *factory = RGST_ELEMENT_FACTORY(self);
    return INT2FIX(factory->rank);    
}

/*
 *  Constant: RANK_MARGINAL
 *  The element is only marginally usefull for autoplugging.
 */
static VALUE constRankMarginal = INT2FIX(GST_ELEMENT_RANK_MARGINAL);

/*
 *  Constant: RANK_NONE
 *  The plugin may not be used in autoplugging. 
 */
static VALUE constRankNone = INT2FIX(GST_ELEMENT_RANK_NONE);

/*
 *  Constant: RANK_PRIMARY
 *  The plugin is well suited for autoplugging. 
 */
static VALUE constRankPrimary = INT2FIX(GST_ELEMENT_RANK_PRIMARY);

/*
 *  Constant: RANK_SECONDARY
 *  The plugin is suited for autoplugging but only as a second candidate. 
 */
static VALUE constRankSecondary = INT2FIX(GST_ELEMENT_RANK_SECONDARY);

/*
 *  Method: rank_marginal? -> aBoolean
 *
 *  Checks if the rank of the factory equals Gst::ElementFactory::RANK_MARGINAL.
 */
static VALUE rb_gst_elementfactory_rank_marginal(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_elementfactory_get_rank(self) == constRankMarginal);
}

/*
 *  Method: rank_none? -> aBoolean
 *
 *  Checks if the rank of the factory equals Gst::ElementFactory::RANK_NONE.
 */
static VALUE rb_gst_elementfactory_rank_none(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_elementfactory_get_rank(self) == constRankNone);
}

/*
 *  Method: rank_primary? -> aBoolean
 *
 *  Checks if the rank of the factory equals Gst::ElementFactory::RANK_PRIMARY.
 */
static VALUE rb_gst_elementfactory_rank_primary(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_elementfactory_get_rank(self) == constRankPrimary);
}

/*
 *  Method: rank_secondary? -> aBoolean
 *
 *  Checks if the rank of the factory equals Gst::ElementFactory::RANK_SECONDARY.
 */
static VALUE rb_gst_elementfactory_rank_secondary(self)
    VALUE self;
{
    return CBOOL2RVAL(rb_gst_elementfactory_get_rank(self) == constRankSecondary);
}

/*
 *  Method: to_s -> aString
 *
 *  Gets a String representing the factory.
 */
static VALUE rb_gst_elementfactory_to_s(self)
    VALUE self;
{
    GstElementFactory *factory = RGST_ELEMENT_FACTORY(self); 
    return rb_str_format("Element: %s (%s)",
                         GST_PLUGIN_FEATURE_NAME(factory),
                         factory->details->longname);
}

/*
 *  Method: pad_templates -> anArray
 *
 *  Requests all pad templates of factory, in an array 
 *  of Gst::PadTemplate objects.  
 */
static VALUE rb_gst_elementfactory_get_pad_templates(self)
    VALUE self;
{
    GstElementFactory *factory;
    GList *list;
    VALUE arr;

    factory = RGST_ELEMENT_FACTORY(self); 
    arr = rb_ary_new();
    for (list = factory->padtemplates; list != NULL; list = g_list_next(list)) {
        GstPadTemplate *pad = GST_PAD_TEMPLATE(list->data);
        rb_ary_push(arr, RGST_PAD_TEMPLATE_NEW(pad));
    }
    return arr;
}

/*
 *  Method: each_pad_template { |aPadTemplateObject| block } -> nil
 *
 *  Calls the block for each pad template of the factory, passing a
 *  reference to the Gst::PadTemplate as parameter.
 *
 *  Always returns nil.
 */
static VALUE rb_gst_elementfactory_each_pad_template(self)
    VALUE self;
{
    return rb_ary_yield(rb_gst_elementfactory_get_pad_templates(self));
}

/*
 *  Method: details -> aHash
 *
 *  Gets some public information about the factory, 
 *  mostly for the benefit of editors.
 *
 *  This information is encapsulated in a Hash object, 
 *  with the following (String) keys:
 *
 *    * longname: long (English) element name.
 *    * klass: type of element, as hierarchy.
 *    * license: license under which the element is provided.
 *    * description: a short description about the element.
 *    * version: version of the element.
 *    * author: some information about the author(s).
 *    * copyright: some copyright details (year, etc..).
 *
 *  Example: 
 *
 *    # Prints all details related to the 'mad' element:
 *    Gst::ElementFactory.find("mad").details do |k, v| 
 *        p "#{k}: #{v}"
 *    end 
 */
static VALUE rb_gst_elementfactory_get_details(self)
    VALUE self;
{
    GstElementFactory *factory;
    GstElementDetails *details;
    VALUE hash;
  
    factory = RGST_ELEMENT_FACTORY(self); 
    details = factory->details;
    assert(details != NULL);
    
    hash = rb_hash_new();

    rb_hash_aset(hash, CSTR2RVAL("longname"),    CSTR2RVAL(details->longname));
    rb_hash_aset(hash, CSTR2RVAL("klass"),       CSTR2RVAL(details->klass)); 
    rb_hash_aset(hash, CSTR2RVAL("license"),     CSTR2RVAL(details->license));  
    rb_hash_aset(hash, CSTR2RVAL("description"), CSTR2RVAL(details->description));
    rb_hash_aset(hash, CSTR2RVAL("version"),     CSTR2RVAL(details->version));
    rb_hash_aset(hash, CSTR2RVAL("author"),      CSTR2RVAL(details->author));
    rb_hash_aset(hash, CSTR2RVAL("copyright"),   CSTR2RVAL(details->copyright));

    return hash;
}

/*
 *  Creates the Gst::ElementFactory class.
 */
void Init_gst_elementfactory(void) {
    VALUE c = G_DEF_CLASS(GST_TYPE_ELEMENT_FACTORY, "ElementFactory", mGst);

    rb_define_singleton_method(c, "make", rb_gst_elementfactory_make, -1);
    rb_define_singleton_method(c, "find", rb_gst_elementfactory_find,  1);
    
    rb_define_method(c, "create", rb_gst_elementfactory_create,   -1);
    rb_define_method(c, "rank",   rb_gst_elementfactory_get_rank,  0);

    rb_define_method(c, "details", rb_gst_elementfactory_get_details, 0);
    
    rb_define_method(c, "to_s", rb_gst_elementfactory_to_s, 0);

    rb_define_method(c, "pad_templates",     rb_gst_elementfactory_get_pad_templates, 0);
    rb_define_method(c, "each_pad_template", rb_gst_elementfactory_each_pad_template, 0);

    rb_define_method(c, "rank_marginal?",  rb_gst_elementfactory_rank_marginal,  0);
    rb_define_method(c, "rank_none?",      rb_gst_elementfactory_rank_none,      0);
    rb_define_method(c, "rank_primary?",   rb_gst_elementfactory_rank_primary,   0);
    rb_define_method(c, "rank_secondary?", rb_gst_elementfactory_rank_secondary, 0);

    rb_define_const(c, "RANK_MARGINAL",  constRankMarginal);
    rb_define_const(c, "RANK_NONE",      constRankNone);
    rb_define_const(c, "RANK_PRIMARY",   constRankPrimary);
    rb_define_const(c, "RANK_SECONDARY", constRankSecondary);
}

