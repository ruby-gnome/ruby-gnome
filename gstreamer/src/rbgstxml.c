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

/* Class: Gst::XML
 * Convert Gst::Element objects into an XML presentation. 
 */

/*
 * Class method: new
 *
 * Creates a new Gst::XML parser object.  
 *
 * Returns: a newly created Gst::XML object.
 */
static VALUE
rb_gst_xml_new (VALUE self)
{
	GstXML *xml = gst_xml_new ();
	if (xml != NULL)
		G_INITIALIZE (self, xml);
	return Qnil;
}

/*
 * Class method: write_file(element, filename)
 * element: a Gst::Element.
 * filename: a file name.
 *
 * Converts the given element into XML and writes the formatted XML 
 * to the specified file name.
 *  
 * Returns: true on success, false on failure.
 */
static VALUE
rb_gst_xml_write_file (VALUE self, VALUE element, VALUE filename)
{
	FILE *file = fopen (RVAL2CSTR (filename), "w");
	gboolean ret = FALSE;
	if (file != NULL) {
		ret = gst_xml_write_file (RGST_ELEMENT (element), file) != -1; 
		fclose (file);
	}
	return CBOOL2RVAL (ret);
}

/*
 * Method: parse_file(filename, rootname=nil)
 * filename: a file name.
 * rootname: a root name (optional).
 *
 * Fills the Gst::XML object with the corresponding elements from 
 * the XML file name.  Optionally it will only build the element 
 * from the element node root (if it is not nil).  
 *
 * This feature is useful if you only want to build a specific 
 * element from an XML file but not the pipeline it is embedded in.
 *
 * Pass "-" as file name to read from stdin.  You can also pass a 
 * URI of any format that libxml supports, including http.
 *  
 * Returns: true on success, false on failure.
 */
static VALUE
rb_gst_xml_parse_file (int argc, VALUE *argv, VALUE self)
{
	VALUE fname, rootname;
	rb_scan_args (argc, argv, "11", &fname, &rootname);	
	return CBOOL2RVAL (gst_xml_parse_file (RGST_XML (self),
					       RVAL2CSTR (fname),
					       NIL_P (rootname) 
					       	   ? NULL 
						   : RVAL2CSTR (rootname)));
}

/*
 * Method: parse_memory(string, rootname=nil)
 * string: an XML description inside a string.
 * rootname: a root name (optional).
 *
 * Basically the same as Gst::XML#parse_file except that this method will
 * take the XML directly from the memory.
 *  
 * Returns: true on success, false on failure.
 */
static VALUE
rb_gst_xml_parse_memory (int argc, VALUE *argv, VALUE self)
{
	VALUE memory, rootname;
	gchar *cstr;
    
	rb_scan_args (argc, argv, "11", &memory, &rootname);	
	cstr = RVAL2CSTR (memory);
	return CBOOL2RVAL (gst_xml_parse_memory (RGST_XML (self),
						 cstr, 
						 strlen (cstr), 
						 NIL_P (rootname) 
				       	   	     ? NULL 
		    			   	     : RVAL2CSTR (rootname)));
}

/*
 * Method: get_element(element_name)
 * element_name: the name of an element.
 *
 * This method is used to get a reference to the Gst::Element 
 * corresponding to name in the pipeline description.  You would use 
 * this if you have to do anything to the element after loading.
 *
 * Returns: a reference to a Gst::Element object if found, otherwise
 * returns nil.
 */
static VALUE
rb_gst_xml_get_element (VALUE self, VALUE element_name)
{
	GstElement *element = gst_xml_get_element (RGST_XML (self),
						   RVAL2CSTR (element_name));
	return element != NULL
		? RGST_ELEMENT_NEW (element)
		: Qnil;
}

/*
 * Method: topelements
 *
 * Retrieves a list of all toplevel elements.
 *
 * Returns: an array of Gst::Element objects.
 */
static VALUE
rb_gst_xml_get_topelements (VALUE self)
{
	GList *list;
	VALUE arr;

	arr = rb_ary_new ();
	for (list = gst_xml_get_topelements (RGST_XML (self));
	     list != NULL;
	     list = g_list_next (list))
		rb_ary_push (arr, RGST_ELEMENT_NEW (list->data)); 
	return arr;
}

/*
 * Method: each_topelement { |element| ... }
 *
 * Calls the block for each topelement, passing a reference to
 * a Gst::Element object as parameter.
 *
 * Returns: always nil.
 */
static VALUE
rb_gst_xml_each_topelement (VALUE self)
{
	return rb_ary_yield (rb_gst_xml_get_topelements (self));
}

void
Init_gst_xml (void)
{
	VALUE c = G_DEF_CLASS (GST_TYPE_XML, "XML", mGst);
	
	rb_define_singleton_method (c, "write_file", rb_gst_xml_write_file, 2);
	
	rb_define_method (c, "initialize", rb_gst_xml_new, 0);
	rb_define_method (c, "parse_file", rb_gst_xml_parse_file, -1);
	rb_define_method (c, "parse_memory", rb_gst_xml_parse_memory, -1);
	rb_define_method (c, "get_element", rb_gst_xml_get_element, 1);
	rb_define_method (c, "topelements", rb_gst_xml_get_topelements, 0);
	rb_define_method (c, "each_topelement", rb_gst_xml_each_topelement, 0);
}
